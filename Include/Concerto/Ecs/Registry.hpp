//
// Created by arthur on 18/05/22.
//

#ifndef CONCERTO_REGISTRY_HPP
#define CONCERTO_REGISTRY_HPP

#include <unordered_map>
#include <vector>
#include <any>
#include <bitset>
#include <exception>
#include <string>
#include <Concerto/Core/SparseVector.hpp>

#include "Concerto/Ecs/Component.hpp"
#include "Concerto/Ecs/Components/Name.hpp"
#include "Concerto/Ecs/Entity.hpp"

namespace Concerto::Ecs
{
	/**
	 * @brief The Registry class is the main class of the ECS.
	 * It is used to create entities and Components.
	 */
	class Registry
	{
	 public:
		using map_element = SparseVector<std::any>;
		using container_type = std::unordered_map<Component::Id, map_element>;
		using iterator = container_type::iterator;
		using const_iterator = container_type::const_iterator;

		Registry() = default;

		Registry(Registry&&) noexcept = default;

		Registry(const Registry&) = delete;

		Registry& operator=(Registry&&) = delete;

		Registry& operator=(const Registry&) = delete;

		~Registry() = default;

		iterator begin() noexcept
		{
			return _components.begin();
		}

		iterator end() noexcept
		{
			return _components.end();
		}

		const_iterator cbegin() const noexcept
		{
			return _components.cbegin();
		}

		const_iterator cend() const noexcept
		{
			return _components.cend();
		}

		/**
		 * @brief Creates a new entity.
		 * @remark The entity will have a default name : "Entity " + std::to_string(id).
		 * @return The new entity.
		 */
		Entity::Id CreateEntity()
		{
			Entity::Id id = _nextId++;
			EmplaceComponent<Name>(id, Name("Entity " + std::to_string(id)));
			return id;
		}

		/**
		 * @brief Creates a new entity with a name.
		 * @param name The name of the entity.
		 * @return The new entity.
		 */
		Entity::Id CreateEntity(const std::string& name)
		{
			Entity::Id id = _nextId++;
			EmplaceComponent<Name>(id, name);
			return id;
		}

		/**
		 * @brief Add a component to an entity
		 * @tparam Comp The component type
		 * @tparam Args The arguments type used to construct the component
		 * @param entity The entity
		 * @param args The arguments to pass to the component constructor
		 * @return A reference to the component
		 */
		template<typename Comp, typename... Args>
		Comp& EmplaceComponent(Entity::Id entity, Args&& ...args)
		{
			Component::Id id = Component::GetId<Comp>();
			auto it = _components.find(id);
			if (it == _components.end())
			{
				auto newCompIt = _components.emplace(id, map_element());
				std::any any = Comp(std::forward<Args>(args)...);
				return std::any_cast<Comp&>(newCompIt.first->second.Emplace(entity, std::move(any)));
			}
			std::any any = Comp(std::forward<Args>(args)...);
			auto& sparseArrayElement = it->second.Emplace(entity, std::move(any));
			return std::any_cast<Comp&>(sparseArrayElement);
		}

		/**
		 * @brief Remove a component from an entity
		 * @tparam Comp The component type
		 * @param entity The entity to remove the component from
		 */
		template<typename Comp>
		void RemoveComponent(Entity::Id entity)
		{
			Component::Id id = Component::GetId<Comp>();
			auto it = _components.find(id);
			if (it != _components.end())
			{
				it->second.Erase(entity);
			}
			else throw std::runtime_error("Component not found");
		}

		/**
		 * @brief Get a component from an entity
		 * @tparam Comp The component type
		 * @param entity The entity to get the component from
		 * @return A reference to the component
		 */
		template<class Comp>
		Comp& GetComponent(Entity::Id entity)
		{
			Component::Id id = Component::GetId<Comp>();
			auto it = _components.find(id);
			if (it == _components.end())
				throw std::runtime_error("Component not found");
			if (!it->second.Has(entity))
				throw std::runtime_error("Component not found");
			return std::any_cast<Comp&>(it->second[entity]);
		}

		/**
		 * @tparam Comp The component type
		 * @param entity The entity to check for the component
		 * @return True if the entity Has the component, false otherwise
		 */
		template<typename Comp>
		[[nodiscard]] bool HasComponent(Entity::Id entity) const
		{
			Component::Id id = Component::GetId<Comp>();
			auto it = _components.find(id);
			return it != _components.end() && it->second.Has(entity);
		}

		/**
		 * @tparam Comp The component type
		 * @param entity The entity to check for the component
		 * @return True if the entity Has the component, false otherwise
		 */
		[[nodiscard]] bool HasComponent(Entity::Id entity, Component::Id id) const
		{
			auto it = _components.find(id);
			return it != _components.end() && it->second.Has(entity);
		}

		/**
		 * @brief Get the number of entities
		 * @return The number of entities
		 */
		[[nodiscard]] Entity::Id GetEntityCount() const
		{
			return _nextId;
		}

	 private:
		std::size_t _nextId = 0;
		container_type _components;
	};
}

#endif //CONCERTO_REGISTRY_HPP
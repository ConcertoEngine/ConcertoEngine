//
// Created by arthur on 20/05/22.
//


#include "Concerto/Engine/Ecs/World.hpp"

namespace Concerto
{
	void World::Update(float deltaTime)
	{
		for (auto& [id, system] : _systems)
		{
			system->Update(deltaTime, _registry);
		}
	}

	void World::StepUpdate(float deltaTime)
	{
		for (auto& [id, system] : _systems)
		{
			system->StepUpdate(deltaTime, _registry);
		}
	}

	Registry& World::GetRegistry()
	{
		return _registry;
	}
}

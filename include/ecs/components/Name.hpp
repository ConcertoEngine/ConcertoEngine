//
// Created by arthur on 13/01/2023.
//

#ifndef CONCERTO_NAME_HPP
#define CONCERTO_NAME_HPP

#include <string>

namespace Concerto::Ecs
{
	struct Name
	{
		Name() = default;
		explicit Name(const std::string& name) : _name(name) {}
		explicit Name(std::string&& name) : _name(std::move(name)) {}
		explicit Name(std::string name) : _name(std::move(name)) {}
		Name(const Name&) = default;
		Name(Name&&) = default;
		Name& operator=(const Name&) = default;
		Name& operator=(Name&&) = default;
		std::string _name;
	};
}
#endif //CONCERTO_NAME_HPP

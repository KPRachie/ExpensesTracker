#pragma once

//std
#include <functional>
#include <string>

//lib
#include "lib/safe_map.hpp"

#include "command.hpp"

class CommandFactory {

public:
	template<typename T>
		requires std::derived_from<T, ICommand>
	void add(const std::string& name) {
		m_commands.set(name, [](TgBot::Bot& bot) {
			return std::make_unique<T>(bot);
		});
	}

	std::function<CommandPtr(TgBot::Bot&)> make(const std::string& name) const {
		return m_commands.get(name);
	}

private:
	lib::SafeMap<std::string, std::function<CommandPtr(TgBot::Bot&)>> m_commands;
};

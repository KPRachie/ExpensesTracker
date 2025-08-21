#pragma once

//TG bot
#include <tgbot/tgbot.h>

//commands
#include "commands/command_factory.hpp"

class Bot : public TgBot::Bot {
public:
	Bot();
	virtual ~Bot();

	//functions

	void run();

	void add_command(const std::string& name);

	void register_commands();

	template<typename T>
	void register_command(const std::string& name) {
		m_factory.add<T>(name);
		add_command(name);
	}

private:
	CommandFactory m_factory;
	std::vector <std::string> m_commands;
};

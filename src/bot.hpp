#pragma once

//STL
#include <unordered_map>
#include <memory>

//TG bot
#include <tgbot/tgbot.h>

//commands
#include "commands/command.hpp"

class Bot : public TgBot::Bot {
public:
	Bot();
	virtual ~Bot();

	//functions

	void run();

	void add_command(std::string name, CommandPtr ptr);

private:
	std::unordered_map<std::string, CommandPtr> commands;
};

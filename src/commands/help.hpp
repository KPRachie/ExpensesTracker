#pragma once

#include "command.hpp"

class Help : public ICommand {
public:
	Help(TgBot::Bot& bot);

	void exec(TgBot::Message::Ptr message) override;

};
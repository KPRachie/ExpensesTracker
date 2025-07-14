#pragma once

#include <memory>

//TG bot
#include <tgbot/tgbot.h>

class ICommand {
public:
	ICommand(TgBot::Bot& bot);
	virtual ~ICommand();

	virtual void exec(TgBot::Message::Ptr& message) = 0;

protected:
	TgBot::Bot& m_bot;
};

using CommandPtr = std::shared_ptr<ICommand>;

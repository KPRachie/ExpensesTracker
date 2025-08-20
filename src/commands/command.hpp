#pragma once

#include <memory>

//TG bot
#include <tgbot/tgbot.h>

//#include "SQLiteCpp/Database.h"

class ICommand {
public:
	explicit ICommand(TgBot::Bot& bot);
	virtual ~ICommand();

	virtual void exec(TgBot::Message::Ptr message) = 0;

protected:
	TgBot::Bot& m_bot;
	//SQLite::Database& m_db;
};

using CommandPtr = std::shared_ptr<ICommand>;

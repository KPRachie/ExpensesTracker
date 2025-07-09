#pragma once

#include "DataAccess.hpp"

namespace expenses_tracker::user_logic {
	void run_bot();

	void command_handler(TgBot::Bot& bot);

	void start(TgBot::Bot& bot, TgBot::Message::Ptr& message);

	void help(TgBot::Bot& bot, TgBot::Message::Ptr& message);

	void get_data(TgBot::Bot& bot, TgBot::Message::Ptr& message);

	void add_new_category(TgBot::Bot& bot, TgBot::Message::Ptr& message);

	void add_new_expense(TgBot::Bot& bot, TgBot::Message::Ptr& message);
}

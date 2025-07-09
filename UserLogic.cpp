#include "UserLogic.hpp"

namespace expenses_tracker::user_logic {
	void run_bot() {
		std::string token = std::getenv("ExpenseTrackerToken");
		TgBot::Bot bot(token);
		TgBot::TgLongPoll longPoll(bot);

		while (true) {
			std::cout << "Long poll started" << '\n';
			longPoll.start();
			command_handler(bot);
		}
	}

	void command_handler(TgBot::Bot& bot) {
		bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
			start(bot, message);
		});
		bot.getEvents().onCommand("help", [&bot](TgBot::Message::Ptr message) {
			help(bot, message);
		});
		bot.getEvents().onCommand("get_data", [&bot](TgBot::Message::Ptr message) {
			get_data(bot, message);
		});
		bot.getEvents().onCommand("add_new_category", [&bot](TgBot::Message::Ptr message) {
			add_new_category(bot, message);
		});
		bot.getEvents().onCommand("add_new_expense", [&bot](TgBot::Message::Ptr message) {
			add_new_expense(bot, message);
		});
	}

	void start(TgBot::Bot& bot, TgBot::Message::Ptr& message) {
		bot.getApi().sendMessage(message->chat->id, "This is an expense management bot. To see available commands, use the /help command.");
		data_access::add_user(message->chat->id, message->chat->firstName);
	}

	void help(TgBot::Bot& bot, TgBot::Message::Ptr& message) {
		bot.getApi().sendMessage(message->chat->id, "To add expenses, use the /add_new_expense command, to add a new expense category, use the /add_new_category command. To view expense statistics, use the /get_data command.");
	}

	void get_data(TgBot::Bot& bot, TgBot::Message::Ptr& message) {}

	void add_new_category(TgBot::Bot& bot, TgBot::Message::Ptr& message) {
		bot.getApi().sendMessage(message->chat->id, "Enter the category name:");
		bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
			data_access::add_category(message->text.c_str());
		});
	}

	void add_new_expense(TgBot::Bot& bot, TgBot::Message::Ptr& message) {}
}



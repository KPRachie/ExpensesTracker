#include "help.hpp"

Help::Help(TgBot::Bot& bot) : ICommand(bot) {}

void Help::exec(TgBot::Message::Ptr message) {
	m_bot.getApi().sendMessage(message->chat->id, "To add expenses, use the /add_new_expense command, to add a new expense category, use the /add_new_category command. To view expense statistics, use the /get_data command.");
}

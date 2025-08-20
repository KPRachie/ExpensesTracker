#include "bot.hpp"

#include "commands/start.hpp"
#include "commands/help.hpp"
#include "commands/get_data.hpp"
#include "commands/add_new_category.hpp"
#include "commands/add_new_expense.hpp"

int main() {
	Bot bot;
	bot.register_command<Start>("start");
	bot.register_command<Help>("help");
	bot.register_command<GetData>("get_data");
	bot.register_command<AddNewCategory>("add_new_category");
	bot.register_command<AddNewExpense>("add_new_expense");
	bot.register_commands();
	bot.run();
    return 0;
}


#include "bot.hpp"

#include "commands/start.hpp"
#include "commands/help.hpp"
#include "commands/get_data.hpp"
#include "commands/add_new_category.hpp"
#include "commands/add_new_expense.hpp"

int main() {
    Bot bot;
    bot.add_command("start", std::make_shared<Start>(bot));
    bot.add_command("help", std::make_shared<Help>(bot));
    bot.add_command("get_data", std::make_shared<GetData>(bot));
    bot.add_command("add_new_category", std::make_shared<AddNewCategory>(bot));
    bot.add_command("add_new_expense", std::make_shared<AddNewExpense>(bot));
    bot.run();
    return 0;
}


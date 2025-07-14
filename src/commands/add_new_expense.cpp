#include "add_new_expense.hpp"

//SQLite
#include <SQLiteCpp/SQLiteCpp.h>

AddNewExpense::AddNewExpense(TgBot::Bot& bot) : ICommand(bot) {}

void AddNewExpense::exec(TgBot::Message::Ptr& message) {
	m_bot.getApi().sendMessage(message->chat->id, "Enter the category name and the price:");
	//m_bot.getEvents().onAnyMessage([this](TgBot::Message::Ptr message) {
	//	add_expense(message->chat->id, message->text.c_str());
	//});
}

void AddNewExpense::add_expense(int64_t tg_id, int64_t cat_id, int64_t cost, std::string comment) {
	SQLite::Database db("databases/expenses.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
	db.exec("CREATE TABLE IF NOT EXISTS expenses (id INTEGER PRIMARY KEY, tg_id INTEGER, cat_id INTEGER, cost INTEGER, comment TEXT)");

	SQLite::Statement query(db, "INSERT INTO expenses (tg_id, cat_id, cost, comment) VALUES (?, ?, ?, ?)");
	query.bind(1, tg_id);
	query.bind(2, cat_id);
	query.bind(3, cost);
	query.bind(4, comment);
	query.exec();
}

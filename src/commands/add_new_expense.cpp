#include "add_new_expense.hpp"

//STL
#include <unordered_map>

//SQLite
#include <SQLiteCpp/SQLiteCpp.h>

//lib
#include "lib/strings.hpp"

//global
#include "global.hpp"

AddNewExpense::AddNewExpense(TgBot::Bot& bot) : ICommand(bot) {}

void AddNewExpense::exec(TgBot::Message::Ptr message) {
	m_bot.getApi().sendMessage(message->chat->id, "Enter the category name and the price:");

	users_states.set(message->chat->id);
	auto& reply = users_states.get(message->chat->id);
	reply.flag.wait(false);
	auto text = std::move(reply.text);
	users_states.erase(message->chat->id);

	add_expense(parse(message->chat->id, text));
}

std::vector<expense_t> AddNewExpense::parse(int64_t tg_id, std::string message) {
	std::unordered_map<std::string, int64_t> cat_ids;
	std::vector<expense_t> expenses;
	expense_t cur_expense;

	cur_expense.tg_id = tg_id;
	for (auto& c : message) {
		std::isupper(c) ? c = std::tolower(c) : true;
	}

	std::vector<std::string> data = lib::split(message);
	for (auto& expense : data) {
		std::string cur_cat = lib::tokenize(expense).first;
		if (cat_ids[cur_cat] == 0) {
			cat_ids[cur_cat] = get_category_id(cur_cat);
		}
		expenses.emplace_back(tg_id, cat_ids[cur_cat], lib::tokenize(expense).second, "");

	}

	return expenses;
}

int64_t AddNewExpense::get_category_id(std::string_view category) {
	SQLite::Database db("databases/categories.db", SQLite::OPEN_READWRITE);
	SQLite::Statement query(db, "SELECT cat_id FROM categories WHERE category = ? LIMIT 1");

	query.bind(1, (std::string) category);
	query.executeStep();

	return query.getColumn(0);
}

void AddNewExpense::add_expense(const std::vector<expense_t>& expenses) {
	SQLite::Database db("databases/expenses.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
	db.exec("CREATE TABLE IF NOT EXISTS expenses (id INTEGER PRIMARY KEY, tg_id INTEGER, cat_id INTEGER, cost INTEGER, comment TEXT)");

	for (auto& expense : expenses) {
		SQLite::Statement query(db, "INSERT INTO expenses (tg_id, cat_id, cost, comment) VALUES (?, ?, ?, ?)");
		query.bind(1, expense.tg_id);
		query.bind(2, expense.cat_id);
		query.bind(3, expense.cost);
		query.bind(4, expense.comment);
		query.exec();
	}
}

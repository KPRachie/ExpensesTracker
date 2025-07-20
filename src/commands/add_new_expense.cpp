#include "add_new_expense.hpp"

//STL
#include <unordered_map>

//SQLite
#include <SQLiteCpp/SQLiteCpp.h>

AddNewExpense::AddNewExpense(TgBot::Bot& bot) : ICommand(bot) {}

void AddNewExpense::exec(TgBot::Message::Ptr& message) {
	bool m_received_first_message = false;
	m_bot.getApi().sendMessage(message->chat->id, "Enter the category name and the price:");
	m_bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
		if (!m_received_first_message) {
			m_received_first_message = true;
			add_expense(parse(message->chat->id, message->text.c_str()));
		}
	});
}

std::vector<expense_t> AddNewExpense::parse(int64_t tg_id, std::string message) {
	std::unordered_map<std::string, int64_t> cat_ids;
	std::vector<expense_t> expenses;
	std::string cur_cat;
	expense_t cur_expense;
	cur_expense.tg_id = tg_id;
	int start = 0;
	message.erase(std::remove_if(message.begin(), message.end(), isspace), message.end());
	for (auto& c : message) {
		std::isupper(c) ? c = std::tolower(c) : true;
	}
	for (int i = 0; i < message.size(); i++) {
		if (std::isdigit(message[i])) {
			cur_cat = std::string(message.begin() + start, message.begin() + i);
			if (!cat_ids[cur_cat]) {
				cat_ids[cur_cat] = get_category_id(cur_cat);
			}
			start = i;
			while (message[i] != ',' && i < message.size()) {
				i++;
			}
			cur_expense.cat_id = cat_ids[cur_cat];
			cur_expense.cost = std::string(message.begin() + start, message.begin() + i);
			start = i + 1;
			expenses.push_back(cur_expense);
		}
	}
	return expenses;
}

int64_t AddNewExpense::get_category_id(std::string category) {
	SQLite::Database db("databases/categories.db", SQLite::OPEN_READWRITE);
	SQLite::Statement query(db, "SELECT cat_id FROM categories WHERE category = ? LIMIT 1");
	query.bind(1, category);
	query.executeStep();
	return query.getColumn(0);
}

void AddNewExpense::add_expense(std::vector<expense_t> expenses) {
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

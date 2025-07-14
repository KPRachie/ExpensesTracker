#include "add_new_category.hpp"

//SQLite
#include <SQLiteCpp/SQLiteCpp.h>

AddNewCategory::AddNewCategory(TgBot::Bot& bot) : ICommand(bot) {}

void AddNewCategory::exec(TgBot::Message::Ptr& message) {
	m_received_first_message = false;
	m_bot.getApi().sendMessage(message->chat->id, "Enter the category name:");
	m_bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
		if (!m_received_first_message) {
			m_received_first_message = true;
			add_category(message->text.c_str());
		}
	});
}

bool AddNewCategory::check_category(std::string category) {
	SQLite::Database db("databases/categories.db", SQLite::OPEN_READWRITE);

	SQLite::Statement query(db, "SELECT 1 FROM categories WHERE category = ? LIMIT 1");
	query.bind(1, category);
	return query.executeStep();
}

void AddNewCategory::add_category(std::string category) {
	SQLite::Database db("databases/categories.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
	db.exec("CREATE TABLE IF NOT EXISTS categories (cat_id INTEGER PRIMARY KEY, category TEXT)");

	if (!check_category(category)) {
		SQLite::Statement query(db, "INSERT INTO categories (category) VALUES (?)");
		query.bind(1, category);
		query.exec();
	}
}

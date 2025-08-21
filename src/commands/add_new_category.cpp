#include "add_new_category.hpp"

//SQLite
#include <SQLiteCpp/SQLiteCpp.h>

//lib
#include <lib/strings.hpp>

//global
#include "global.hpp"

AddNewCategory::AddNewCategory(TgBot::Bot& bot) : ICommand(bot) {}

void AddNewCategory::exec(TgBot::Message::Ptr message) {
	m_bot.getApi().sendMessage(message->chat->id, "Enter the category name(s), separated by commas:");

	users_states.set(message->chat->id);
	auto& reply = users_states.get(message->chat->id);
	reply.flag.wait(false);
	auto text = std::move(reply.text);
	users_states.erase(message->chat->id);

	add_category(parse(text));
}

bool AddNewCategory::check_category(std::string_view category) {
	SQLite::Database db("expenses_tracker.db", SQLite::OPEN_READWRITE);
	SQLite::Statement query(db, "SELECT 1 FROM categories WHERE category = ? LIMIT 1");

	query.bind(1, (std::string) category);

	return query.executeStep();
}

void AddNewCategory::add_category(const std::vector<std::string>& categories) {
	SQLite::Database db("expenses_tracker.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

	db.exec("CREATE TABLE IF NOT EXISTS categories (cat_id INTEGER PRIMARY KEY, category TEXT)");
	for (auto& category : categories) {
		if (!check_category(category)) {
			SQLite::Statement query(db, "INSERT INTO categories (category) VALUES (?)");
			query.bind(1, category);
			query.exec();
		}
	}
}

std::vector<std::string> AddNewCategory::parse(std::string message) {
	for (auto& c : message) {
		std::isupper(c) ? c = std::tolower(c) : true;
	}

	return lib::split(message);
}

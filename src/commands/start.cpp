#include "start.hpp"

//SQLite
#include <SQLiteCpp/SQLiteCpp.h>

Start::Start(TgBot::Bot& bot) : ICommand(bot){}

void Start::exec(TgBot::Message::Ptr& message) {
	m_bot.getApi().sendMessage(message->chat->id, "This is an expense management bot. To see available commands, use the /help command.");
	add_user(message->chat->id, message->chat->firstName);
}

bool Start::check_user(int64_t tg_id) {
	SQLite::Database db("databases/users.db", SQLite::OPEN_READWRITE);

	SQLite::Statement query(db, "SELECT 1 FROM users WHERE tg_id = ? LIMIT 1");
	query.bind(1, tg_id);
	return query.executeStep();
}

void Start::add_user(int64_t tg_id, std::string user_name) {
	SQLite::Database db("databases/users.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
	db.exec("CREATE TABLE IF NOT EXISTS users (tg_id INTEGER PRIMARY KEY, user_name TEXT)");

	if (!check_user(tg_id)) {
		SQLite::Statement query(db, "INSERT INTO users (tg_id, user_name) VALUES (?, ?)");
		query.bind(1, tg_id);
		query.bind(2, user_name);
		query.exec();
	}
}

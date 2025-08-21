#include "get_data.hpp"

//STL
#include <ranges>

//SQLite
#include <SQLiteCpp/SQLiteCpp.h>

//lib
#include "lib/strings.hpp"

//global
#include "global.hpp"

GetData::GetData(TgBot::Bot& bot) : ICommand(bot) {}

void GetData::exec(TgBot::Message::Ptr message) {
	m_bot.getApi().sendMessage(message->chat->id, "Enter the category name(s) or * to see all expenses:");

	users_states.set(message->chat->id);
	auto& reply = users_states.get(message->chat->id);
	reply.flag.wait(false);
	auto text = std::move(reply.text);
	users_states.erase(message->chat->id);

	auto parsed_data = text != "*" ? parse(text) : std::vector<std::string> {};
	auto expenses = process(message->chat->id, parsed_data);
	m_bot.getApi().sendMessage(message->chat->id, prepare_data(expenses));
}

std::vector<std::string> GetData::parse(std::string message) {
	for (auto& c : message) {
		std::isupper(c) ? c = std::tolower(c) : true;
	}

	return lib::split(message);
}

std::unordered_map<int64_t, std::string> GetData::get_cat_names(const std::vector<int64_t>& cat_ids) {
	std::unordered_map<int64_t, std::string> cat_names;

	for (auto& id : cat_ids) {
		if (cat_names[id].empty()) {
			SQLite::Database db("expenses_tracker.db", SQLite::OPEN_READWRITE);
			SQLite::Statement query(db, "SELECT category FROM categories WHERE cat_id = ? LIMIT 1");
			query.bind(1, id);
			query.executeStep();
			cat_names[id] = query.getColumn(0).getString();
		}
	}

	return cat_names;
}

std::unordered_map<std::string, int64_t> GetData::get_cat_ids(const std::vector<std::string>& categories) {
	std::unordered_map<std::string, int64_t> cat_ids;

	for (auto& category : categories) {
		if (cat_ids[category] == 0) {
			SQLite::Database db("expenses_tracker.db", SQLite::OPEN_READWRITE);
			SQLite::Statement query(db, "SELECT cat_id FROM categories WHERE category = ? LIMIT 1");
			query.bind(1, category);
			query.executeStep();
			cat_ids[category] = query.getColumn(0);
		}
	}

	return cat_ids;
}

std::vector<int64_t> GetData::get_all_cat_ids(int64_t tg_id) {
	std::vector<int64_t> categories;

	SQLite::Database db("expenses_tracker.db", SQLite::OPEN_READWRITE);
	SQLite::Statement query(db, "SELECT DISTINCT cat_id FROM expenses WHERE tg_id = ?");
	query.bind(1, tg_id);

	while (query.executeStep()) {
		categories.emplace_back(query.getColumn(0).getInt64());
	}

	return categories;
}

std::unordered_map<int64_t, int64_t> GetData::process(int64_t tg_id, const std::vector<std::string>& categories) {
	std::unordered_map<int64_t, int64_t> expenses;
	int64_t total = 0;
	SQLite::Database db("expenses_tracker.db", SQLite::OPEN_READWRITE);
	SQLite::Statement query(db, "SELECT SUM(cost) FROM expenses WHERE cat_id = ? AND tg_id = ?");

	if (categories.empty()) {
		std::vector<int64_t> cat_ids = get_all_cat_ids(tg_id);

		for (auto& id : cat_ids) {
			query.bind(1, id);
			query.bind(2, tg_id);
			query.executeStep();
			expenses[id] = query.getColumn(0);
			total += expenses[id];
			query.reset();
		}
	} else {
		std::unordered_map<std::string, int64_t> cat_ids = get_cat_ids(categories);

		for (auto& category : categories) {
			query.bind(1, cat_ids[category]);
			query.bind(2, tg_id);
			query.executeStep();
			expenses[cat_ids[category]] = query.getColumn(0);
			total += expenses[cat_ids[category]];
			query.reset();
		}
	}
	expenses[0] = total;

	return expenses;
}

std::string GetData::prepare_data(std::unordered_map<int64_t, int64_t> expenses) {
	std::vector<int64_t> cat_ids;
	std::string data = "Total cost: " + std::to_string(expenses[0]) + '\n' + "Here is your data:\n";

	for (const auto& id : expenses | std::views::keys) {
		if (id != 0) {
			cat_ids.push_back(id);
		}
	}

	std::unordered_map<int64_t, std::string> cat_names = get_cat_names(cat_ids);
	for (auto& [id, sum] : expenses) {
		if (id != 0) {
			data += cat_names[id] + ": " + std::to_string(sum) + " " + std::to_string((int) (((float) sum / expenses[0]) * 100)) + "% " + '\n';
		}
	}

	return data;
}

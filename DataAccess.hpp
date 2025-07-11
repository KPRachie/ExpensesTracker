#pragma once

//STL
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>

//TG bot
#include <tgbot/tgbot.h>

//SQLite
#include <SQLiteCpp/SQLiteCpp.h>


namespace expenses_tracker {
	namespace data_access {

		struct expense_t {
			int64_t id;
			int64_t tg_id;
			int64_t cat_id;
			int64_t cost;
			std::string comment;
		};

		std::vector <expense_t> get_expenses(int64_t tg_id, std::vector<int64_t> cat_ids);

		void add_user(int64_t tg_id, std::string user_name);

		bool check_user(int64_t tg_id);

		void add_category(std::string category);

		bool check_category(std::string category);

		void add_expense(int64_t tg_id, int64_t cat_id, int64_t cost, std::string comment);

		std::map <int64_t, std::string> get_categories(std::vector<int64_t> cat_ids);
	}
}

#include "DataAccess.hpp"

namespace expenses_tracker::data_access {
	std::vector<expense_t> get_expenses(int64_t tg_id, int64_t cat_id) {
		std::vector<expense_t> expenses;

		try {
			SQLite::Database db("databases/expenses.db", SQLite::OPEN_READWRITE);

			SQLite::Statement select(db, "SELECT *  FROM expenses WHERE tg_id = ?, cat_id = ?");
			select.bind(1, tg_id);
			select.bind(2, cat_id);

			while (select.executeStep()) {
				int64_t id = select.getColumn(1);
				int64_t tg_id = select.getColumn(2);
				int64_t cat_id = select.getColumn(3);
				int64_t cost = select.getColumn(4);
				std::string comment = select.getColumn(5);
				expense_t expense;
				expense.id = id;
				expense.tg_id = tg_id;
				expense.cat_id = cat_id;
				expense.cost = cost;
				expense.comment = comment;
				expenses.push_back(expense);

				std::cout << "ID: " << cat_id << ", cost: " << cost << std::endl;
			}

		} catch (const SQLite::Exception& e) {
			std::cerr << "SQLite error: " << e.what() << std::endl;
		}

		return expenses;
	}

	void add_user(int64_t tg_id, std::string user_name) {
		try {
			SQLite::Database db("databases/users.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
			db.exec("CREATE TABLE IF NOT EXISTS users (tg_id INTEGER PRIMARY KEY, user_name TEXT)");

			if (!check_user(tg_id)) {
				SQLite::Statement query(db, "INSERT INTO users (tg_id, user_name) VALUES (?, ?)");
				query.bind(1, tg_id);
				query.bind(2, user_name);
				query.exec();
			}

		} catch (const SQLite::Exception& e) {
			std::cerr << "SQLite error: " << e.what() << std::endl;
		}
	}

	bool check_user(int64_t tg_id) {
		try {
			SQLite::Database db("databases/users.db", SQLite::OPEN_READWRITE);

			SQLite::Statement query(db, "SELECT 1 FROM users WHERE tg_id = ? LIMIT 1");
			query.bind(1, tg_id);
			if (query.executeStep()) {
				return true;
			} else {
				return false;
			}

		} catch (const SQLite::Exception& e) {
			std::cerr << "SQLite error: " << e.what() << std::endl;
		}
	}

	void add_category(std::string category) {
		try {
			SQLite::Database db("databases/categories.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
			db.exec("CREATE TABLE IF NOT EXISTS categories (cat_id INTEGER PRIMARY KEY, category TEXT)");

			if (!check_category(category)) {
				SQLite::Statement query(db, "INSERT INTO categories (category) VALUES (?)");
				query.bind(1, category);
				query.exec();
			}

		} catch (const SQLite::Exception& e) {
			std::cerr << "SQLite error: " << e.what() << std::endl;
		}
	}

	bool check_category(std::string category) {
		try {
			SQLite::Database db("databases/categories.db", SQLite::OPEN_READWRITE);

			SQLite::Statement query(db, "SELECT 1 FROM categories WHERE category = ? LIMIT 1");
			query.bind(1, category);
			return query.executeStep();

		} catch (const SQLite::Exception& e) {
			std::cerr << "SQLite error: " << e.what() << std::endl;
		}
	}

	void add_expense(int64_t id, int64_t tg_id, int64_t cat_id, int64_t cost, std::string comment) {
		try {
			SQLite::Database db("databases/expenses.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
			db.exec("CREATE TABLE IF NOT EXISTS expenses (id INTEGER PRIMARY KEY, tg_id INTEGER, cat_id INTEGER, cost INTEGER, comment TEXT)");

			SQLite::Statement query(db, "INSERT INTO expenses (id, tg_id, cat_id, cost, comment) VALUES (?, ?, ?, ?, ?)");
			query.bind(1, id);
			query.bind(2, tg_id);
			query.bind(3, cat_id);
			query.bind(4, cost);
			query.bind(5, comment);
			query.exec();

		} catch (const SQLite::Exception& e) {
			std::cerr << "SQLite error: " << e.what() << std::endl;
		}
	}

	//TO DO
	std::map<int64_t, std::string> get_categories(std::vector<int64_t> cat_id) {
		return std::map<int64_t, std::string>();
	}
}

#pragma once

#include <string>
#include <vector>

#include "command.hpp"

struct expense_t {
	int64_t tg_id;
	int64_t cat_id;
	std::string cost;
	std::string comment;
};

class AddNewExpense : public ICommand {
public:
	explicit AddNewExpense(TgBot::Bot& bot);

	void exec(TgBot::Message::Ptr message) override;

private:
	void add_expense(const std::vector<expense_t>& expenses);
	std::vector<expense_t> parse(int64_t tg_id, std::string message);
	int64_t get_category_id(std::string_view category);
};


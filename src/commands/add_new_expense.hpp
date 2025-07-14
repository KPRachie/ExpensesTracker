#pragma once

#include <string>

#include "command.hpp"

class AddNewExpense : public ICommand {
public:
	AddNewExpense(TgBot::Bot& bot);

	void exec(TgBot::Message::Ptr& message) override;

private:
	void add_expense(int64_t tg_id, int64_t cat_id, int64_t cost, std::string comment);

	bool m_received_first_message;

};

struct expense_t {
	int64_t id;
	int64_t tg_id;
	int64_t cat_id;
	int64_t cost;
	std::string comment;
};
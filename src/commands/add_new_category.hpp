#pragma once

#include <string>

#include "command.hpp"

class AddNewCategory : public ICommand {
public:
	AddNewCategory(TgBot::Bot& bot);

	void exec(TgBot::Message::Ptr& message) override;

private:
	bool check_category(std::string category);
	void add_category(std::vector<std::string> categories);
	std::vector <std::string> parse(std::string message);

	bool m_received_first_message;
};

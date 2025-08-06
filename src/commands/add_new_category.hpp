#pragma once

#include <string>
#include <vector>

#include "command.hpp"

class AddNewCategory : public ICommand {
public:
	explicit AddNewCategory(TgBot::Bot& bot);

	void exec(TgBot::Message::Ptr message) override;

private:
	bool check_category(std::string_view category);
	void add_category(const std::vector<std::string>& categories);
	std::vector <std::string> parse(std::string message);
};

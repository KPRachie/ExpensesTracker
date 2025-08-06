#pragma once

#include "command.hpp"

#include <string>

class Start : public ICommand {
public:
	Start(TgBot::Bot& bot);

	void exec(TgBot::Message::Ptr message) override;

private:
	bool check_user(int64_t tg_id);
	void add_user(int64_t tg_id, std::string user_name);
};
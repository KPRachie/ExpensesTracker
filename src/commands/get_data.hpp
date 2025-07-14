#pragma once

#include "command.hpp"

class GetData : public ICommand {
public:
	GetData(TgBot::Bot& bot);

	void exec(TgBot::Message::Ptr& message) override;

private:
	void parse();
	void process();
	void send_message();

};
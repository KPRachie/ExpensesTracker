#include "bot.hpp"

#include <string>

Bot::Bot() : TgBot::Bot(std::getenv("ExpensesTrackerToken")) {}

Bot::~Bot() {}

void Bot::run() {
	TgBot::TgLongPoll longPoll(*this);

	for (auto& [name, ptr] : commands) {
		getEvents().onCommand(name, [&](TgBot::Message::Ptr message) {
			ptr->exec(message);
		});
	}

	while (true) {
		std::cout << "Long poll started" << '\n';
		longPoll.start();
	}
}

void Bot::add_command(std::string name, CommandPtr ptr) {
	commands.emplace(name, std::move(ptr));
}

#include "bot.hpp"

//STL
#include <string>

//lib
#include "lib/safe_map.hpp"

//global
#include "global.hpp"

Bot::Bot() : TgBot::Bot(std::getenv("ExpensesTrackerToken")) {}

Bot::~Bot() = default;

void Bot::run() {
	TgBot::TgLongPoll longPoll(*this);

	getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
		if (users_states.contains(message->chat->id)) {
			auto& elem = users_states.get(message->chat->id);
			elem.text = message->text;
			elem.flag.store(true, std::memory_order_release);
			elem.flag.notify_all();
		}
	});

	for (auto& [name, ptr] : commands) {
		getEvents().onCommand(name, [ptr](TgBot::Message::Ptr message) {
			std::thread([ptr, message] {
				ptr->exec(message);
			}).detach();
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

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

	while (true) {
		std::cout << "Long poll started" << '\n';
		longPoll.start();
	}
}

void Bot::add_command(const std::string& name) {
	m_commands.push_back(name);
}

void Bot::register_commands() {
	for (const auto& name : m_commands) {
		getEvents().onCommand(name, [this, name](TgBot::Message::Ptr message) {
			std::thread([message, name, this]() {
				auto command = m_factory.make(name)(*this);
				command->exec(message);
			}).detach();
		});
	}
}

#pragma once

#include "command.hpp"

//STL
#include <string>
#include <unordered_map>
#include <vector>

class GetData : public ICommand {
public:
	explicit GetData(TgBot::Bot& bot);

	void exec(TgBot::Message::Ptr message) override;

private:
	std::vector<std::string> parse(std::string message);
	std::unordered_map<std::string, int64_t> get_cat_ids(const std::vector<std::string>& categories);
	std::unordered_map<int64_t, std::string> get_cat_names(const std::vector<int64_t>& cat_ids);
	std::vector<int64_t> get_all_cat_ids(int64_t tg_id);
	std::unordered_map<int64_t, int64_t> process(int64_t tg_id, const std::vector<std::string>& categories = {});
	std::string prepare_data(std::unordered_map<int64_t, int64_t> expenses);
};

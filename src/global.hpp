#pragma once

//STL
#include <atomic>
#include <string>

//lib
#include "lib/safe_map.hpp"

struct atomic_string {
	mutable std::atomic_bool flag;
	std::string text;

	explicit atomic_string(std::string_view text = "") :
		flag(false),
		text(text) {
	}

	atomic_string(const atomic_string& another) :
		flag(another.flag.load()),
		text(another.text) {
	}

	atomic_string& operator=(const atomic_string& another) {
		flag = another.flag.load();
		text = another.text;
		return *this;
	}

	atomic_string(atomic_string&& another) noexcept :
		flag(another.flag.load()),
		text(std::move(another.text)) {
	}

	atomic_string& operator=(atomic_string&& another) noexcept {
		flag = another.flag.load();
		text = std::move(another.text);
		return *this;
	}
};

extern lib::SafeMap<size_t, atomic_string> users_states;

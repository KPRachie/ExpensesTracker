#pragma once

#include <shared_mutex>
#include <unordered_map>

namespace lib {
	template<typename TKey, typename TVal>
	class SafeMap {
	public:
		SafeMap() = default;

		template<typename... TArgs>
		void set(const TKey& key, const TArgs&... args) {
			std::lock_guard lock(m_mutex);
			m_data[key] = TVal(args...);
		}

		void erase(const TKey& key) {
			std::lock_guard lock(m_mutex);
			m_data.erase(key);
		}

		TVal& get(const TKey& key) {
			std::shared_lock lock(m_mutex);
			return m_data.at(key);
		}

		const TVal& get(const TKey& key) const {
			std::shared_lock lock(m_mutex);
			return m_data.at(key);
		}

		bool contains(const TKey& key) const {
			std::shared_lock lock(m_mutex);
			return m_data.contains(key);
		}

	protected:
		mutable std::shared_mutex m_mutex;
		std::unordered_map<TKey, TVal> m_data;
	};
}

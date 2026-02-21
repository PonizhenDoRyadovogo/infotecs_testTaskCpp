#include <unordered_map>
#include <list>
#include <mutex>
#include <string>
#include <algorithm>

template<
	typename Key, 
	typename Value, 
	typename Hash = std::hash<Key>, 
	typename KeyEqual = std::equal_to<Key>
>
class LRUCache {
public:
	LRUCache(int capacity) {
		if (capacity > 10000) {
			m_capacity = 10000;
			return;
		}

		if (capacity <= 0) {
			m_capacity = 1;
			return;
		}

		m_capacity = capacity;
	}

	bool get(const Key& key, Value& outValue) {
		std::lock_guard<std::mutex> lock(m_mutex);
		auto it = m_cacheMap.find(key);
		if (it == m_cacheMap.end()) {
			return false;
		}
		m_cacheList.splice(m_cacheList.begin(), m_cacheList, it->second);
		outValue = it->second->second;
		return true;
	}

	void put(const Key& key, const Value& value) {
		std::lock_guard<std::mutex> lock(m_mutex);
		auto it = m_cacheMap.find(key);

		if (it != m_cacheMap.end()) {
			m_cacheList.splice(m_cacheList.begin(), m_cacheList, it->second);
			it->second->second = value;
			return;
		}

		if (m_cacheMap.size() >= m_capacity) {
			auto last = m_cacheList.back();
			m_cacheMap.erase(last.first);
			m_cacheList.pop_back();
		}

		m_cacheList.emplace_front(key, value);
		m_cacheMap[key] = m_cacheList.begin();
	}

	int size() const {
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_cacheMap.size();
	}

	void clear() {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_cacheMap.clear();
		m_cacheList.clear();
	}

	bool remove(const Key& key) {
		std::lock_guard<std::mutex> lock(m_mutex);
		auto it = m_cacheMap.find(key);

		if (it == m_cacheMap.end()) {
			return false;
		}
		
		m_cacheList.erase(it->second);
		m_cacheMap.erase(it);
		return true;
	}

private:
	int m_capacity;
	mutable std::mutex m_mutex;
	std::list<std::pair<Key, Value>> m_cacheList;
	std::unordered_map<
		Key, 
		typename std::list<std::pair<Key, Value>>::iterator,
		Hash,
		KeyEqual
	> m_cacheMap;
};
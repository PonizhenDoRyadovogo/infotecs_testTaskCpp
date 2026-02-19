#include <unordered_map>
#include <list>
#include <mutex>
#include <string>

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
		} esle if (capacity <= 0) {
			m_capacity = 1;
		}
		else {
			m_capacity = capacity;
		}
	}
private:
	int m_capacity;
	mutable std::mutex m_mutex;
	std::list<std::pair<Key, Value>> m_cacheList;
	std::unordered_map<
		Key, typename std::list<std::pair<Key, Value>>::iterator,
		Hash,
		KeyEqual
	> m_cacheMap;
};
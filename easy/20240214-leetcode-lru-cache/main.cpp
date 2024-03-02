// https://leetcode.com/problems/lru-cache/description/

#include <cassert>
#include <list>
#include <unordered_map>


class LRUCache {
private:
    // LRU
    std::list<int> lru;                             // list of keys
    int lru_capacity;

    void lru_delete(int key, const std::list<int>::iterator& iterator) {
        assert(*iterator == key);
        lru.erase(iterator);
    }

    void lru_append(int key) {
        lru.push_back(key);
    }

    // Cache
    struct CacheObj {
        int value;
        std::list<int>::iterator lru_iterator;      // pointer to LRU
    };
    std::unordered_map<int, CacheObj> cache;

public:
    static const int NOT_FOUND = -1;

    LRUCache(int capacity) {
        lru_capacity = capacity;
    }

    int get(int key) {
        bool is_exist = cache.find(key) != cache.end();
        if (!is_exist) {
            return NOT_FOUND;
        }
        CacheObj cache_obj = cache[key];
        lru_delete(key, cache_obj.lru_iterator);
        lru_append(key);
        cache[key].lru_iterator = std::prev(lru.end());
        return cache_obj.value;
    }

    void put(int key, int value) {
        // Update the LRU
        bool is_exist = cache.find(key) != cache.end();
        if (is_exist) {
            CacheObj cache_obj = cache[key];
            lru_delete(key, cache_obj.lru_iterator);
        }
        lru_append(key);
        if (lru.size() > lru_capacity) {
            int oldest_key = lru.front(); lru.pop_front();
            cache.erase(oldest_key);
        }
        // Update the Cache
        cache[key] = {value, std::prev(lru.end())};
    }
};


int main() {
    {
        int value;
        LRUCache lRUCache = LRUCache(2);
        lRUCache.put(1, 0);                     // cache is {1=0}
        lRUCache.put(2, 2);                     // cache is {1=0, 2=2}
        value = lRUCache.get(1);                // return 0
        assert(value == 0);
        lRUCache.put(3, 3);                     // LRU key was 2, evicts key 2, cache is {1=0, 3=3}
        value = lRUCache.get(2);                // returns -1 (not found)
        assert(value == LRUCache::NOT_FOUND);
        lRUCache.put(4, 4);                     // LRU key was 1, evicts key 1, cache is {4=4, 3=3}
        value = lRUCache.get(1);                // return -1 (not found)
        assert(value == LRUCache::NOT_FOUND);
        value = lRUCache.get(3);                // return 3
        assert(value == 3);
        value = lRUCache.get(4);                // return 4
        assert (value == 4);
    }
    {
        int value;
        LRUCache lRUCache = LRUCache(2);
        lRUCache.put(2, 1);
        lRUCache.put(2, 2);
        value = lRUCache.get(2);
        assert(value == 2);
        lRUCache.put(1, 1);
        lRUCache.put(4, 1);
        value = lRUCache.get(2);
        assert(value == LRUCache::NOT_FOUND);
    }
    return 0;
}

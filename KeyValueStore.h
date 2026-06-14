#ifndef KEYVALUESTORE_H
#define KEYVALUESTORE_H

#include "Strings.h"

template <typename K, typename V>
class KeyValueStore {
private:
    struct Pair {
        K key;
        V value;
        Pair* next;
        Pair(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };
    
    Pair** buckets;
    int bucketCount;
    int totalPairs;
    
    int hash(const K& key) const;

public:
    KeyValueStore();
    ~KeyValueStore();
    
    void put(const K& key, const V& value);
    V get(const K& key) const;
    bool contains(const K& key) const;
    void remove(const K& key);
    int getSize() const;
    void clear();
};

#include "KeyValueStore.cpp"

#endif
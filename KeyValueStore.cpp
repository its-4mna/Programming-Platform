#ifndef KEYVALUESTORE_CPP
#define KEYVALUESTORE_CPP

#include "KeyValueStore.h"

template <typename K, typename V>
int KeyValueStore<K, V>::hash(const K& key) const {
    const char* str = key.getstr();  // String class ka getstr()
    int h = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        h = (h * 31 + str[i]) % bucketCount;
    }
    return h;
}

template <typename K, typename V>
KeyValueStore<K, V>::KeyValueStore() {
    bucketCount = 16;
    buckets = new Pair*[bucketCount];
    for (int i = 0; i < bucketCount; i++) {
        buckets[i] = nullptr;
    }
    totalPairs = 0;
}

template <typename K, typename V>
KeyValueStore<K, V>::~KeyValueStore() {
    clear();
    delete[] buckets;
}

template <typename K, typename V>
void KeyValueStore<K, V>::put(const K& key, const V& value) {
    int index = hash(key);
    Pair* current = buckets[index];
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value;
            return;
        }
        current = current->next;
    }
    
    Pair* newNode = new Pair(key, value);
    newNode->next = buckets[index];
    buckets[index] = newNode;
    totalPairs++;
}

template <typename K, typename V>
V KeyValueStore<K, V>::get(const K& key) const {
    int index = hash(key);
    Pair* current = buckets[index];
    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return V();
}

template <typename K, typename V>
bool KeyValueStore<K, V>::contains(const K& key) const {
    int index = hash(key);
    Pair* current = buckets[index];
    while (current != nullptr) {
        if (current->key == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename K, typename V>
void KeyValueStore<K, V>::remove(const K& key) {
    int index = hash(key);
    Pair* current = buckets[index];
    Pair* prev = nullptr;
    
    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            totalPairs--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

template <typename K, typename V>
int KeyValueStore<K, V>::getSize() const {
    return totalPairs;
}

template <typename K, typename V>
void KeyValueStore<K, V>::clear() {
    for (int i = 0; i < bucketCount; i++) {
        Pair* current = buckets[i];
        while (current != nullptr) {
            Pair* temp = current;
            current = current->next;
            delete temp;
        }
        buckets[i] = nullptr;
    }
    totalPairs = 0;
}

#endif
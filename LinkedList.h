#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
    };
    
    Node* head;
    Node* tail;
    int size;

public:
    LinkedList();
    ~LinkedList();
    LinkedList(const LinkedList& other);
    LinkedList& operator=(const LinkedList& other);

    void pushFront(const T& value);
    void pushBack(const T& value);
    void popFront();
    void popBack();
    
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    
    void remove(const T& value);
    bool contains(const T& value) const;
    
    int getSize() const;
    bool isEmpty() const;
    void clear();
};

#include "LinkedList.cpp"

#endif
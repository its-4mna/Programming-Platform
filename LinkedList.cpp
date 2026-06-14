#ifndef LINKEDLIST_CPP
#define LINKEDLIST_CPP

#include "LinkedList.h"

template <typename T>
LinkedList<T>::LinkedList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& other) {
    head = nullptr;
    tail = nullptr;
    size = 0;
    
    Node* current = other.head;
    while (current != nullptr) {
        pushBack(current->data);
        current = current->next;
    }
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
    if (this != &other) {
        clear();
        Node* current = other.head;
        while (current != nullptr) {
            pushBack(current->data);
            current = current->next;
        }
    }
    return *this;
}

template <typename T>
void LinkedList<T>::pushFront(const T& value) {
    Node* newNode = new Node(value);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    size++;
}

template <typename T>
void LinkedList<T>::pushBack(const T& value) {
    Node* newNode = new Node(value);
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
}

template <typename T>
void LinkedList<T>::popFront() {
    if (head == nullptr) return;
    
    Node* temp = head;
    head = head->next;
    if (head != nullptr) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
    }
    delete temp;
    size--;
}

template <typename T>
void LinkedList<T>::popBack() {
    if (tail == nullptr) return;
    
    Node* temp = tail;
    tail = tail->prev;
    if (tail != nullptr) {
        tail->next = nullptr;
    } else {
        head = nullptr;
    }
    delete temp;
    size--;
}

template <typename T>
T& LinkedList<T>::front() {
    return head->data;
}

template <typename T>
const T& LinkedList<T>::front() const {
    return head->data;
}

template <typename T>
T& LinkedList<T>::back() {
    return tail->data;
}

template <typename T>
const T& LinkedList<T>::back() const {
    return tail->data;
}

template <typename T>
void LinkedList<T>::remove(const T& value) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data == value) {
            if (current->prev != nullptr) {
                current->prev->next = current->next;
            } else {
                head = current->next;
            }
            if (current->next != nullptr) {
                current->next->prev = current->prev;
            } else {
                tail = current->prev;
            }
            delete current;
            size--;
            return;
        }
        current = current->next;
    }
}

template <typename T>
bool LinkedList<T>::contains(const T& value) const {
    Node* current = head;
    while (current != nullptr) {
        if (current->data == value) return true;
        current = current->next;
    }
    return false;
}

template <typename T>
int LinkedList<T>::getSize() const {
    return size;
}

template <typename T>
bool LinkedList<T>::isEmpty() const {
    return size == 0;
}

template <typename T>
void LinkedList<T>::clear() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    size = 0;
}

#endif
#ifndef DYNAMICARRAY_CPP
#define DYNAMICARRAY_CPP

#include "DynamicArray.h"

template <typename T>
DynamicArray<T>::DynamicArray() {
    currentSize = 0;
    capacity = 4;
    arr = new T[capacity];
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] arr;
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& other) {
    currentSize = other.currentSize;
    capacity = other.capacity;
    arr = new T[capacity];
    for (int i = 0; i < currentSize; i++) {
        arr[i] = other.arr[i];
    }
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& other) {
    if (this != &other) {
        delete[] arr;
        currentSize = other.currentSize;
        capacity = other.capacity;
        arr = new T[capacity];
        for (int i = 0; i < currentSize; i++) {
            arr[i] = other.arr[i];
        }
    }
    return *this;
}

template <typename T>
void DynamicArray<T>::resize() {
    capacity = capacity * 2;
    T* newArr = new T[capacity];
    for (int i = 0; i < currentSize; i++) {
        newArr[i] = arr[i];
    }
    delete[] arr;
    arr = newArr;
}

template <typename T>
void DynamicArray<T>::pushBack(const T& value) {
    if (currentSize == capacity) {
        resize();
    }
    arr[currentSize] = value;
    currentSize++;
}

template <typename T>
void DynamicArray<T>::popBack() {
    if (currentSize > 0) {
        currentSize--;
    }
}

template <typename T>
void DynamicArray<T>::insertAt(int index, const T& value) {
    if (index < 0 || index > currentSize) return;
    
    if (currentSize == capacity) {
        resize();
    }
    
    for (int i = currentSize; i > index; i--) {
        arr[i] = arr[i - 1];
    }
    arr[index] = value;
    currentSize++;
}

template <typename T>
void DynamicArray<T>::removeAt(int index) {
    if (index < 0 || index >= currentSize) return;
    
    for (int i = index; i < currentSize - 1; i++) {
        arr[i] = arr[i + 1];
    }
    currentSize--;
}

template <typename T>
T& DynamicArray<T>::getAt(int index) {
    return arr[index];
}

template <typename T>
const T& DynamicArray<T>::getAt(int index) const {
    return arr[index];
}

template <typename T>
int DynamicArray<T>::getSize() const {
    return currentSize;
}

template <typename T>
bool DynamicArray<T>::isEmpty() const {
    return currentSize == 0;
}

template <typename T>
void DynamicArray<T>::clear() {
    delete[] arr;
    currentSize = 0;
    capacity = 4;
    arr = new T[capacity];
}

template <typename T>
T& DynamicArray<T>::operator[](int index) {
    return arr[index];
}

template <typename T>
const T& DynamicArray<T>::operator[](int index) const {
    return arr[index];
}

#endif
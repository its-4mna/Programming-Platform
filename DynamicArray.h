#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <iostream>

template <typename T>
class DynamicArray {
private:
    T* arr;
    int currentSize;
    int capacity;
    void resize();

public:
    DynamicArray();
    ~DynamicArray();
    DynamicArray(const DynamicArray& other);
    DynamicArray& operator=(const DynamicArray& other);

    void pushBack(const T& value);
    void popBack();
    void insertAt(int index, const T& value);
    void removeAt(int index);
    
    T& getAt(int index);
    const T& getAt(int index) const;
    
    int getSize() const;
    bool isEmpty() const;
    void clear();
    
    T& operator[](int index);
    const T& operator[](int index) const;
};

#include "DynamicArray.cpp"

#endif
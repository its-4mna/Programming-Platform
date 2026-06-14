#ifndef STRINGS_H
#define STRINGS_H

#include <iostream>
using namespace std;

class String 
{
private:
    char* data; //dynamically allocated
    int len;    

    //helper func to allocate and copy src into this->data
    void allocAndCopy(const char* src, int length);

public:
    String();
    String(const char* src);
    String(const String& other);
    ~String();

    //assignment operators
    String& operator=(const String& other);
    String& operator=(const char* src);

    //getters n stuff
    int getlen() const;
    const char* getstr() const;
    bool isEmpty() const;

    //equality operators
    bool operator==(const String& other) const;
    bool operator==(const char* other) const;
    bool operator!=(const String& other) const;
    bool operator!=(const char* other) const;

    //connecting strings together
    String operator+(const String& other) const;
    String operator+(const char* other) const;

    String& operator+=(const String& other);
    String& operator+=(const char* other);

    String& append(const String& other);
    String& append(const char* other);


    //strcmp
    int compare(const String& other) const;

    //to check if the string contains a specific word
    bool contains(const String& pattern) const;
    bool contains(const char* pattern) const;

    //substring extraction 
    String substring(int start, int count) const;

    //to acces individual chars
    char operator[](int index) const;

    //
    static String fromInt(int n);

    //for input n output
    friend std::ostream& operator<<(std::ostream& out, const String& s);
    friend std::istream& operator>>(std::istream& in, String& s);
};

#endif
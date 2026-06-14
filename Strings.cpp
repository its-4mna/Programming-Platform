#include "Strings.h"

void String::allocAndCopy(const char* src, int length) 
{
    len = length;
    data = new char[len + 1];
    for (int i = 0; i < len; ++i)
        data[i] = src[i];
    data[len] = '\0';
}

String::String() 
{
    len = 0;
    data = new char[1];
    data[0] = '\0';
}

String::String(const char* src) 
{
    if (src == nullptr) 
    {
        len = 0;
        data = new char[1];
        data[0] = '\0';
        return;
    }

    int l = 0;
    while (src[l] != '\0') ++l;
    allocAndCopy(src, l);
}

String::String(const String& other) 
{
    allocAndCopy(other.data, other.len);
}

String::~String() 
{
    delete[] data;
}

String& String::operator=(const String& other) 
{
    if (this != &other) 
    {
        delete[] data;
        allocAndCopy(other.data, other.len);
    }
    return *this;
}

String& String::operator=(const char* src) 
{
    delete[] data;
    if (src == nullptr) {
        len = 0;
        data = new char[1];
        data[0] = '\0';
    } else {
        int l = 0;
        while (src[l] != '\0') ++l;
        allocAndCopy(src, l);
    }
    return *this;
}

int String::getlen() const 
{
    return len;
}

const char* String::getstr() const 
{
    return data;
}

bool String::isEmpty() const 
{
    return len == 0;
}

bool String::operator==(const String& other) const 
{
    if (len != other.len) 
        return false;

    for (int i = 0; i < len; ++i)
        if (data[i] != other.data[i]) return false;

    return true;
}

bool String::operator==(const char* other) const 
{
    if (other == nullptr) 
        return (len == 0);

    int l = 0;
    while (other[l] != '\0') 
        l++;

    if (len != l) 
        return false;

    for (int i = 0; i < len; ++i)
        if (data[i] != other[i]) 
            return false;

    return true;
}

bool String::operator!=(const String& other) const 
{
    return !(*this == other);
}

bool String::operator!=(const char* other) const 
{
    return !(*this == other);
}

String String::operator+(const String& other) const 
{
    int newLen = len + other.len;
    char* tmp = new char[newLen + 1];

    for (int i = 0; i < len; ++i)
        tmp[i] = data[i];

    for (int i = 0; i < other.len; ++i)
        tmp[len + i] = other.data[i];

    tmp[newLen] = '\0';
    String result(tmp);
    delete[] tmp;
    return result;
}

String String::operator+(const char* other) const 
{
    String s(other);
    return *this + s;
}

String& String::append(const String& other) 
{
    *this = *this + other;
    return *this;
}

String& String::append(const char* other) 
{
    String s(other);
    return append(s);
}

int String::compare(const String& other) const 
{
    int minLen = (len < other.len) ? len : other.len;

    for (int i = 0; i < minLen; ++i) 
    {
        if (data[i] < other.data[i]) 
            return -1;
        if (data[i] > other.data[i]) 
            return 1;
    }

    if (len < other.len) 
        return -1;
    if (len > other.len) 
        return 1;
    return 0;
}

bool String::contains(const String& pattern) const 
{
    if (pattern.len == 0) 
        return true;

    if (pattern.len > len) 
        return false;

    for (int i = 0; i <= len - pattern.len; ++i) 
    {
        bool match = true;
        for (int j = 0; j < pattern.len; ++j) 
        {
            if (data[i + j] != pattern.data[j]) 
            {
                match = false;
                break;
            }
        }
        if (match) 
            return true;
    }
    return false;
}

bool String::contains(const char* pattern) const 
{
    String s(pattern);
    return contains(s);
}

String String::substring(int start, int count) const 
{
    if (start < 0 || start >= len || count <= 0)
        return String("");
    int actualCount = (start + count > len) ? (len - start) : count;
    char* tmp = new char[actualCount + 1];
    for (int i = 0; i < actualCount; ++i)
        tmp[i] = data[start + i];
    tmp[actualCount] = '\0';
    String result(tmp);
    delete[] tmp;
    return result;
}

char String::operator[](int index) const 
{
    if (index < 0 || index >= len)  
        return '\0';
    return data[index];
}

String String::fromInt(int n) 
{
    if (n == 0) 
        return String("0");

    bool negative = (n < 0);

    if (negative) 
        n = -n;

    char buf[32];
    int pos = 30;
    buf[31] = '\0';

    while (n > 0) 
    {
        buf[pos--] = '0' + (n % 10);
        n /= 10;
    }
    if (negative) 
        buf[pos--] = '-';

    return String(buf + pos + 1);
}

std::ostream& operator<<(std::ostream& out, const String& s) 
{
    out << s.data;
    return out;
}

std::istream& operator>>(std::istream& in, String& s) 
{
    char buffer[4096];
    in >> buffer;
    s = buffer;
    return in;
}

String& String::operator+=(const String& other) 
{
    *this = *this + other;
    return *this;
    
}

String& String::operator+=(const char* other) 
{
    *this = *this + String(other);
    return *this;
}

#ifndef REGFILE_H
#define REGFILE_H

#include "sysEntity.h"

class regFile : public sysEntity
{
private:
    char* content;
    int size;
    int capacity;

    void grow(int newCapacity);

public:
    regFile(const String& n, User* o, const permissions& p);
    regFile(const regFile& other);
    regFile& operator=(const regFile& other);
    virtual ~regFile();

    int read(char* buffer, int bufSize, int offset = 0);
    int write(const char* data, int dataSize, int offset = 0);
    void setContent(const char* data, int dataSize = -1); // -1 = auto strlen

    virtual int getSize() const override;
    virtual sysEntity* clone() const override;
    virtual void print(int depth) const override;
    virtual const char* getType() const override;

    // Inside regFile class, public section:
    String getStoragePath() const;

    void syncFromDisk();   
    // loads content from disk if file exists
    // auto load system 

    const char* getContent() const { return content; }
    virtual String getFullPath() const override;
};

#endif
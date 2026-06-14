#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "sysEntity.h"

class Directory : public sysEntity
{
private:
    sysEntity** children;
    int childCount;
    int childCapacity;

    void expand();

public:
    Directory(const String& n, User* o, const permissions& p);
    Directory(const Directory& other);
    Directory& operator=(const Directory& other);
    virtual ~Directory();

    bool addChild(sysEntity* child);
    bool removeChild(const String& name);
    sysEntity* findChild(const String& name) const;
    int getChildCount() const;
    sysEntity* getChild(int index) const;
    String getFullPath() const;

    // Override virtual functions
    virtual int getSize() const override;   // returns number of children
    virtual sysEntity* clone() const override;
    virtual void print(int depth) const override;
    virtual const char* getType() const override;  // returns "Directory"
};

#endif
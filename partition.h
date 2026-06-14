#ifndef PARTITION_H
#define PARTITION_H

#include "Strings.h"
#include "permissions.h"
#include "user.h"

class storageMedium;
class Directory;   // forward declaration – matches your directory class name

class Partition {
private:
    String name;
    String fsType;
    int totalSizeMB;
    int usedSizeBytes;
    bool mounted;
    User* owner;
    permissions perms;
    storageMedium* parentMedium;
    Directory* rootDir;   // use Directory*

public:
    Partition(const String& n, int sizeMB, const String& fs, User* o, const permissions& p, storageMedium* medium);
    ~Partition();

    String getName() const;
    String getFsType() const;
    int getTotalSizeMB() const;
    int getUsedSizeMB() const;
    int getAvailableSpaceMB() const;
    bool isMounted() const;
    void setMounted(bool m);
    User* getOwner() const;
    const permissions& getPermissions() const;
    storageMedium* getParentMedium() const;
    Directory* getRootDir() const;   // return Directory*

    bool hasCapacity(int bytesNeeded) const;
    bool consumeSpace(int bytes);
    void releaseSpace(int bytes);
    void print() const;
};

#endif
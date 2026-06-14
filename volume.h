#ifndef VOLUME_H
#define VOLUME_H

#include "Strings.h"          // or "Strings.h" – match your exact file
#include "storageMedium.h"
#include "mountPoint.h"
#include "regFile.h"

// Forward declarations – no full definition needed here
class Partition;
class directoy;
class MountPoint;

class Volume {
private:
    String name;
    storageMedium** media;
    int mediaCount;
    int mediaCapacity;
    //User** users;
    //int userCount;
    //int userCapacity;
    Partition* primaryPartition;
    Partition** mountedPartitions;
    int mountedCount;
    int mountedCapacity;

    void expandMedia();
    //void expandUsers();
    void expandMounted();

public:
    Volume(const String& n);
    ~Volume();

    storageMedium* addMedium(int totalCapMB, int type);
    //User* registerUser(const String& uname);
    //User* findUser(const String& uname) const;
    //User* findUserByID(int uid) const;

    void setPrimaryPartition(Partition* p);
    Partition* getPrimaryPartition() const;

    void registerMount(Partition* p);
    bool createMountPoint(const String& mountPath, Partition* p, User* owner, const permissions& perm);

    Directory* getRootDirectory() const;   // fix return type

    void printVolumeState() const;
    const String& getName() const;
};

#endif
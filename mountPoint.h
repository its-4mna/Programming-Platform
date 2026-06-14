#ifndef MOUNTPOINT_H
#define MOUNTPOINT_H

#include "directory.h"

class Partition; // forward declaration

class MountPoint : public Directory {
private:
    Partition* mountedPartition;

public:
    MountPoint(const String& n, User* o, const permissions& p);
    MountPoint(const MountPoint& other);
    MountPoint& operator=(const MountPoint& other);
    virtual ~MountPoint();

    bool mountPartition(Partition* p);
    void unmountPartition();
    Partition* getMountedPartition() const;
    bool isMounted() const;

    virtual const char* getType() const override;
    virtual void print(int depth) const override;
    virtual sysEntity* clone() const override;
};

#endif
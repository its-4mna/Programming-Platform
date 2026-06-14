#ifndef STORAGEMEDIUM_H
#define STORAGEMEDIUM_H

#include "Strings.h"
#include "permissions.h"
#include "user.h"

class Partition; // forward declaration

class storageMedium {
private:
    String label;
    int totalCapacityMB;
    int usedCapacityMB;
    int type;                 // TYPE_SSD, TYPE_HDD, TYPE_OPTICAL
    bool isMounted;
    Partition** partitions;
    int partitionCount;
    int partitionCapacity;

    void expandPartitions();

public:
    static const int TYPE_SSD = 1;
    static const int TYPE_HDD = 2;
    static const int TYPE_OPTICAL = 3;

    storageMedium(const String& lbl, int totalMB, int mediumType);
    ~storageMedium();

    String getLabel() const;
    int getTotalCapacityMB() const;
    int getUsedCapacityMB() const;
    int getAvailableCapacityMB() const;
    int getType() const;
    bool getMounted() const;
    void setMounted(bool mounted);

    Partition* createPartition(const String& name, int sizeMB, const String& fsType, User* owner);
    bool addPartition(Partition* part);
    Partition* getPartition(int index) const;
    int getPartitionCount() const;
    void print() const;
};

#endif
#include "storageMedium.h"
#include "partition.h"
#include <iostream>

void storageMedium::expandPartitions() {
    int newCap = (partitionCapacity == 0) ? 4 : partitionCapacity * 2;
    Partition** newArr = new Partition*[newCap];
    for (int i = 0; i < partitionCount; ++i)
        newArr[i] = partitions[i];
    delete[] partitions;
    partitions = newArr;
    partitionCapacity = newCap;
}

storageMedium::storageMedium(const String& lbl, int totalMB, int mediumType) {
    label = lbl;
    totalCapacityMB = totalMB;
    usedCapacityMB = 0;
    type = mediumType;
    isMounted = false;
    partitions = nullptr;
    partitionCount = 0;
    partitionCapacity = 0;
}

storageMedium::~storageMedium() {
    for (int i = 0; i < partitionCount; ++i)
        delete partitions[i];
    delete[] partitions;
}

String storageMedium::getLabel() const { return label; }
int storageMedium::getTotalCapacityMB() const { return totalCapacityMB; }
int storageMedium::getUsedCapacityMB() const { return usedCapacityMB; }
int storageMedium::getAvailableCapacityMB() const { return totalCapacityMB - usedCapacityMB; }
int storageMedium::getType() const { return type; }
bool storageMedium::getMounted() const { return isMounted; }
void storageMedium::setMounted(bool mounted) { isMounted = mounted; }

Partition* storageMedium::createPartition(const String& name, int sizeMB, const String& fsType, User* owner) {
    if (sizeMB > getAvailableCapacityMB()) {
        std::cout << "  [ERROR] Not enough space on medium '" << label << "' for partition '" << name << "'.\n";
        return nullptr;
    }
    permissions defaultPerm(true, true, true, true, false, false, true, false, false);
    Partition* part = new Partition(name, sizeMB, fsType, owner, defaultPerm, this);
    if (!addPartition(part)) {
        delete part;
        return nullptr;
    }
    usedCapacityMB += sizeMB;
    return part;
}

bool storageMedium::addPartition(Partition* part) {
    if (!part) return false;
    if (partitionCount == partitionCapacity) expandPartitions();
    partitions[partitionCount++] = part;
    return true;
}

Partition* storageMedium::getPartition(int index) const {
    if (index < 0 || index >= partitionCount) return nullptr;
    return partitions[index];
}

int storageMedium::getPartitionCount() const { return partitionCount; }

void storageMedium::print() const {
    std::cout << "Medium: " << label << " | Capacity: " << totalCapacityMB
              << "MB | Used: " << usedCapacityMB << "MB | Type: ";
    if (type == TYPE_SSD) std::cout << "SSD";
    else if (type == TYPE_HDD) std::cout << "HDD";
    else std::cout << "OPTICAL";
    std::cout << (isMounted ? " (mounted)" : "") << "\n";
    for (int i = 0; i < partitionCount; ++i) {
        std::cout << "  Partition " << (i+1) << ": ";
        partitions[i]->print();
    }
}
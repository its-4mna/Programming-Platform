#include "volume.h"
#include "directory.h"         // full definition of directoy
#include "partition.h"        // full definition of Partition
#include <iostream>

void Volume::expandMedia() {
    // same as before (unchanged)
    int newCap = (mediaCapacity == 0) ? 4 : mediaCapacity * 2;
    storageMedium** newArr = new storageMedium*[newCap];

    for (int i = 0; i < mediaCount; ++i) 
        newArr[i] = media[i];

    delete[] media;
    media = newArr;
    mediaCapacity = newCap;
}


void Volume::expandMounted() 
{
    int newCap = (mountedCapacity == 0) ? 4 : mountedCapacity * 2;
    Partition** newArr = new Partition*[newCap];
    for (int i = 0; i < mountedCount; ++i) 
        newArr[i] = mountedPartitions[i];

    delete[] mountedPartitions;
    mountedPartitions = newArr;
    mountedCapacity = newCap;
}

Volume::Volume(const String& n) : name(n), media(nullptr), mediaCount(0), mediaCapacity(0), primaryPartition(nullptr),mountedPartitions(nullptr), mountedCount(0), mountedCapacity(0) {}

Volume::~Volume() 
{
    for (int i = 0; i < mediaCount; ++i) 
        delete media[i];

    

    delete[] media;
    
    delete[] mountedPartitions;
}

storageMedium* Volume::addMedium(int totalCapMB, int type) 
{
    String mediumName = String("Medium") + String::fromInt(mediaCount + 1);

    storageMedium* m = new storageMedium(mediumName, totalCapMB, type);

    if (mediaCount == mediaCapacity) 
        expandMedia();

    media[mediaCount++] = m;
    return m;
}



void Volume::setPrimaryPartition(Partition* p) 
{ 
    
    primaryPartition = p; 
}

Partition* Volume::getPrimaryPartition() const 
{ 
    return primaryPartition;
}

void Volume::registerMount(Partition* p) 
{
    for (int i = 0; i < mountedCount; ++i) 
    {
        if (mountedPartitions[i] == p) 
            return;
    }
    if (mountedCount == mountedCapacity) 
        expandMounted();

    mountedPartitions[mountedCount++] = p;
}

bool Volume::createMountPoint(const String& mountPath, Partition* p, User* owner, const permissions& perm) {
    if (!primaryPartition) 
    {
        std::cout << "  [ERROR] No primary partition set.\n";
        return false;
    }
    Directory* root = primaryPartition->getRootDir();

    if (!root) 
    {
        std::cout << "  [ERROR] Root directory of primary partition is null.\n";
        return false;
    }

    MountPoint* mp = new MountPoint(mountPath, owner, perm);
    if (!root->addChild(mp)) 
    {
        delete mp;
        return false;
    }
    if (!mp->mountPartition(p)) {
        root->removeChild(mountPath);
        delete mp;   
        return false;
    }
    registerMount(p);
    return true;
}

Directory* Volume::getRootDirectory() const 
{
    if (primaryPartition) 
        return primaryPartition->getRootDir();
    return nullptr;
}

void Volume::printVolumeState() const {
    std::cout << "\n=== VOLUME: " << name << " ===\n";
    std::cout << "  Storage Media:\n";
    for (int i = 0; i < mediaCount; ++i) {
        std::cout << "    ";
        media[i]->print();
    }
    if (primaryPartition) {
        std::cout << "\n  Primary Partition:\n    ";
        primaryPartition->print();
    }
}

const String& Volume::getName() const 
{ 
    return name; 
}
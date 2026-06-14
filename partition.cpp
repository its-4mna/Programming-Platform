#include "partition.h"
#include "directory.h"   // include your directory header (not "directoy.h")
#include <iostream>

Partition::Partition(const String& n, int sizeMB, const String& fs, User* o, const permissions& p, storageMedium* medium)
    : name(n), fsType(fs), totalSizeMB(sizeMB), usedSizeBytes(0), mounted(false), owner(o), perms(p), parentMedium(medium), rootDir(nullptr)
{
    permissions defaultPerm(true, true, true, true, false, false, true, false, false);
    rootDir = new Directory(String("/"), o, defaultPerm);   // use Directory
}

Partition::~Partition()
{
    delete rootDir;
}

// Getters
String Partition::getName() const { return name; }
String Partition::getFsType() const { return fsType; }
int Partition::getTotalSizeMB() const { return totalSizeMB; }
int Partition::getUsedSizeMB() const { return usedSizeBytes / (1024 * 1024); }
int Partition::getAvailableSpaceMB() const { return totalSizeMB - (usedSizeBytes / (1024 * 1024)); }
bool Partition::isMounted() const { return mounted; }
void Partition::setMounted(bool m) { mounted = m; }
User* Partition::getOwner() const { return owner; }
const permissions& Partition::getPermissions() const { return perms; }
storageMedium* Partition::getParentMedium() const { return parentMedium; }
Directory* Partition::getRootDir() const { return rootDir; }

// Space management
bool Partition::hasCapacity(int bytesNeeded) const
{
    int neededMB = (bytesNeeded + 1024 * 1024 - 1) / (1024 * 1024);
    return (getAvailableSpaceMB() >= neededMB);
}

bool Partition::consumeSpace(int bytes)
{
    if (!hasCapacity(bytes))
    {
        std::cout << "  [CAPACITY ERROR] Not enough space on partition '" << name << "'.\n";
        return false;
    }
    usedSizeBytes += bytes;
    return true;
}

void Partition::releaseSpace(int bytes)
{
    usedSizeBytes -= bytes;
    if (usedSizeBytes < 0) usedSizeBytes = 0;
}

void Partition::print() const
{
    std::cout << name << " [" << fsType << "] " << totalSizeMB
              << "MB, Used: " << getUsedSizeMB() << "MB, Free: "
              << getAvailableSpaceMB() << "MB" << (mounted ? " (mounted)" : "") << "\n";
}
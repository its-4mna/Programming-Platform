#include "mountPoint.h"
#include "partition.h"

MountPoint::MountPoint(const String& n, User* o, const permissions& p)
    : Directory(n, o, p), mountedPartition(nullptr) {}

MountPoint::MountPoint(const MountPoint& other)
    : Directory(other), mountedPartition(other.mountedPartition) {}

MountPoint& MountPoint::operator=(const MountPoint& other) {
    if (this != &other) {
        Directory::operator=(other);
        mountedPartition = other.mountedPartition;
    }
    return *this;
}

MountPoint::~MountPoint() {}

bool MountPoint::mountPartition(Partition* p) {
    if (!p) {
        std::cout << "  [ERROR] Cannot mount null partition.\n";
        return false;
    }
    if (mountedPartition) {
        std::cout << "  [ERROR] Mount point '" << getName() << "' already has a partition mounted.\n";
        return false;
    }
    if (p->isMounted()) {
        std::cout << "  [ERROR] Partition '" << p->getName() << "' is already mounted elsewhere.\n";
        return false;
    }
    mountedPartition = p;
    p->setMounted(true);
    std::cout << "  [INFO] Partition '" << p->getName() << "' mounted at '" << getName() << "'.\n";
    return true;
}

void MountPoint::unmountPartition() {
    if (!mountedPartition) {
        std::cout << "  [ERROR] No partition mounted at '" << getName() << "'.\n";
        return;
    }
    mountedPartition->setMounted(false);
    mountedPartition = nullptr;
    std::cout << "  [INFO] Partition unmounted from '" << getName() << "'.\n";
}

Partition* MountPoint::getMountedPartition() const { return mountedPartition; }
bool MountPoint::isMounted() const { return mountedPartition != nullptr; }

const char* MountPoint::getType() const { return "MountPoint"; }

void MountPoint::print(int depth) const {
    Directory::print(depth);
    for (int i = 0; i < depth; ++i) std::cout << "  ";
    if (isMounted())
        std::cout << "  (mounted partition: " << mountedPartition->getName() << ")\n";
    else
        std::cout << "  (no partition mounted)\n";
}

sysEntity* MountPoint::clone() const { return new MountPoint(*this); }
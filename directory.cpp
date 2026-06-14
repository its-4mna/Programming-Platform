#include "directory.h"
#include "regFile.h"
#include "PersistentStorage.h"   
#include <iostream>

Directory::Directory(const String& n, User* o, const permissions& p)
    : sysEntity(n, o, p)
{
    children = nullptr;
    childCount = 0;
    childCapacity = 0;
}

Directory::Directory(const Directory& other)
    : sysEntity(other)
{
    children = nullptr;
    childCount = 0;
    childCapacity = 0;
    for (int i = 0; i < other.childCount; ++i) {
        sysEntity* copy = other.children[i]->clone();
        copy->setParent(this);
        if (childCount == childCapacity) expand();
        children[childCount++] = copy;
    }
}

Directory& Directory::operator=(const Directory& other)
{
    if (this != &other) {
        sysEntity::operator=(other);
        // delete existing children
        for (int i = 0; i < childCount; ++i) delete children[i];
        delete[] children;
        children = nullptr;
        childCount = 0;
        childCapacity = 0;
        // copy new children
        for (int i = 0; i < other.childCount; ++i) {
            sysEntity* copy = other.children[i]->clone();
            copy->setParent(this);
            if (childCount == childCapacity) expand();
            children[childCount++] = copy;
        }
    }
    return *this;
}

Directory::~Directory()
{
    for (int i = 0; i < childCount; ++i) delete children[i];
    delete[] children;
}

void Directory::expand()
{
    int newCap = (childCapacity == 0) ? 4 : childCapacity * 2;
    sysEntity** newArr = new sysEntity*[newCap];
    for (int i = 0; i < childCount; ++i) newArr[i] = children[i];
    delete[] children;
    children = newArr;
    childCapacity = newCap;
}

bool Directory::addChild(sysEntity* child)
{
    if (!child) return false;
    // check duplicate name
    if (findChild(child->getName())) return false;
    if (childCount == childCapacity) expand();
    children[childCount++] = child;
    child->setParent(this);

    // auto load system
    // ----- AUTO-LOAD FOR REGULAR FILES -----
    regFile* rf = dynamic_cast<regFile*>(child);
    if (rf) {
        rf->syncFromDisk();
    }
    
    touchModified();

     // ----- PERSISTENCE: if child is a directory, create real folder on disk -----
    // We need to get the full VFS path of this child.
    // Since we don't have a recursive getFullPath() yet, we build it by walking up parents.
    // This is a helper lambda (or you can add a method to sysEntity later)
    auto getFullVfsPath = [](const sysEntity* ent) -> String {
        if (!ent) return String("");
        const sysEntity* p = ent;
        String path;
        while (p) {
            path = String(p->getName()) + String("/") + path;
            p = p->getParent();
        }
        // remove trailing slash
        if (path.getlen() > 0 && path.getstr()[path.getlen()-1] == '/')
            path = path.substring(0, path.getlen() - 1);
        return path;
    };

    Directory* subDir = dynamic_cast<Directory*>(child);
    if (subDir) {
        String fullPath = getFullVfsPath(subDir);
        PersistentStorage::makeDirectory(fullPath);
    }

    return true;
}

bool Directory::removeChild(const String& name)
{
    for (int i = 0; i < childCount; ++i) {
        if (children[i]->getName() == name) {
            delete children[i];
            for (int j = i; j < childCount - 1; ++j)
                children[j] = children[j + 1];
            --childCount;
            touchModified();
            return true;
        }
    }
    return false;
}

sysEntity* Directory::findChild(const String& name) const
{
    for (int i = 0; i < childCount; ++i) {
        if (children[i]->getName() == name) return children[i];
    }
    return nullptr;
}

int Directory::getChildCount() const { return childCount; }

sysEntity* Directory::getChild(int index) const
{
    if (index < 0 || index >= childCount) return nullptr;
    return children[index];
}

int Directory::getSize() const { return childCount; }

sysEntity* Directory::clone() const { return new Directory(*this); }

void Directory::print(int depth) const
{
    printHeader(depth);
    for (int i = 0; i < childCount; ++i) {
        children[i]->print(depth + 1);
    }
}

const char* Directory::getType() const { return "Directory"; }


String Directory::getFullPath() const {
    if (!parent) {
        return name;  // Root directory
    }
    
    Directory* parentDir = dynamic_cast<Directory*>(parent);
    if (parentDir) {
        return parentDir->getFullPath() + String("/") + name;
    }
    
    return name;
}
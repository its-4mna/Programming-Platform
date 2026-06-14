// yeah common properties provide karta hai 
// owner ,  name ,  permission  , parent  , timestamps( kab bana , kab modify hue )
// and vistual functions 


#include "sysEntity.h"

int sysEntity::totalEntitiesCreated = 0;

sysEntity::sysEntity(const String& n, User* o, const permissions& p)
{
    name = n;
    owner = o;
    perms = p;
    parent = 0;
    creationTime = time(0);
    modifiedTime = creationTime;
    totalEntitiesCreated++;
}

sysEntity::sysEntity(const sysEntity& other)
{
    name = other.name;
    creationTime = other.creationTime;
    modifiedTime = other.modifiedTime;
    owner = other.owner;
    perms = other.perms;
    parent = 0;  // parent not copied, set separately
    totalEntitiesCreated++;
}

sysEntity& sysEntity::operator=(const sysEntity& other)
{
    if (this != &other)
    {
        name = other.name;
        creationTime = other.creationTime;
        modifiedTime = other.modifiedTime;
        owner = other.owner;
        perms = other.perms;
        parent = other.parent;
    }
    return *this;
}

sysEntity::~sysEntity()
{
    totalEntitiesCreated--;
}

// Getters
const String& sysEntity::getName() const 
{ 
    return name; 
}

time_t sysEntity::getCreationTime() const 
{ 
    return creationTime; 

}

time_t sysEntity::getModifiedTime() const 
{
    return modifiedTime; 
}

User* sysEntity::getOwner() const 
{ 
    return owner; 
}

const permissions& sysEntity::getPermissions() const 
{ 
    return perms; 
}

permissions& sysEntity::getPermissions() 
{ 
    return perms; 
}

sysEntity* sysEntity::getParent() const 
{ 
    return parent; 
}

// Setters
void sysEntity::setName(const String& n) 
{ 
    name = n; 
}

void sysEntity::setOwner(User* u) 
{ 
    owner = u; 
}

void sysEntity::setParent(sysEntity* p) 
{ 
    parent = p; 
}

void sysEntity::touchModified() 
{ 
    modifiedTime = time(0); 
}

bool sysEntity::checkPermission(const User* user, int operation) const
{
    if (user == 0) 
        return false;
    if (user->isRoot()) 
        return true;

    bool isOwner = (owner->getUsername() == user->getUsername());
    bool sameGroup = (user->getGroupName() == owner->getGroupName());

    if (isOwner)
    {
        if (operation == 0) 
            return perms.canOwnerRead();
        if (operation == 1) 
            return perms.canOwnerWrite();
        if (operation == 2) 
            return perms.canOwnerExecute();
    }
    else if (sameGroup)
    {
        if (operation == 0) 
            return perms.canGroupRead();
        if (operation == 1) 
            return perms.canGroupWrite();
        if (operation == 2) 
            return perms.canGroupExecute();
    }
    else
    {
        if (operation == 0) 
            return perms.canOthersRead();
        if (operation == 1) 
            return perms.canOthersWrite();
        if (operation == 2) 
            return perms.canOthersExecute();
    }
    return false;
}

void sysEntity::printHeader(int depth) const
{
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << "\nType: " << getType() << " " << name;
    std::cout << "\nOwner: " << owner->getUsername();
    std::cout << "\nPerms: ";
    perms.displayPerms();
    std::cout << "\nSize: " << getSize() << std::endl;
}

int sysEntity::getTotalEntitiesCreated()
{
    return totalEntitiesCreated;
}
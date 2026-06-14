#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <iostream>

class permissions 
{
private:
    bool ownerR, ownerW, ownerE;
    bool groupR, groupW, groupE;
    bool othersR, othersW, othersE;

public:
    // Default: owner read/write/execute, group/others read only
    permissions();
    // Full custom constructor (order: owner, group, others)
    permissions(bool oR, bool oW, bool oE,
                bool gR, bool gW, bool gE,
                bool otr, bool otw, bool ote);

    // Display as rwxrwxrwx
    void displayPerms() const;

    // Setters
    void setOwnerRead(bool v);
    void setOwnerWrite(bool v);
    void setOwnerExecute(bool v);
    void setGroupRead(bool v);
    void setGroupWrite(bool v);
    void setGroupExecute(bool v);
    void setOthersRead(bool v);
    void setOthersWrite(bool v);
    void setOthersExecute(bool v);

    // Getters
    bool canOwnerRead() const;
    bool canOwnerWrite() const;
    bool canOwnerExecute() const;
    bool canGroupRead() const;
    bool canGroupWrite() const;
    bool canGroupExecute() const;
    bool canOthersRead() const;
    bool canOthersWrite() const;
    bool canOthersExecute() const;
};

#endif
#include "permissions.h"

// Default constructor
permissions::permissions() {
    ownerR = true; ownerW = true; ownerE = true;
    groupR = true; groupW = false; groupE = false;
    othersR = true; othersW = false; othersE = false;
}

// 9‑parameter constructor
permissions::permissions(bool oR, bool oW, bool oE,
                         bool gR, bool gW, bool gE,
                         bool otr, bool otw, bool ote) {
    ownerR = oR; ownerW = oW; ownerE = oE;
    groupR = gR; groupW = gW; groupE = gE;
    othersR = otr; othersW = otw; othersE = ote;
}

void permissions::displayPerms() const {
    std::cout << (ownerR ? "r" : "-")
              << (ownerW ? "w" : "-")
              << (ownerE ? "x" : "-")
              << (groupR ? "r" : "-")
              << (groupW ? "w" : "-")
              << (groupE ? "x" : "-")
              << (othersR ? "r" : "-")
              << (othersW ? "w" : "-")
              << (othersE ? "x" : "-");
}

// Setters (already in your .cpp, ensure they are present)
void permissions::setOwnerRead(bool v) { ownerR = v; }
void permissions::setOwnerWrite(bool v) { ownerW = v; }
void permissions::setOwnerExecute(bool v) { ownerE = v; }
void permissions::setGroupRead(bool v) { groupR = v; }
void permissions::setGroupWrite(bool v) { groupW = v; }
void permissions::setGroupExecute(bool v) { groupE = v; }
void permissions::setOthersRead(bool v) { othersR = v; }
void permissions::setOthersWrite(bool v) { othersW = v; }
void permissions::setOthersExecute(bool v) { othersE = v; }

// Getters
bool permissions::canOwnerRead() const { return ownerR; }
bool permissions::canOwnerWrite() const { return ownerW; }
bool permissions::canOwnerExecute() const { return ownerE; }
bool permissions::canGroupRead() const { return groupR; }
bool permissions::canGroupWrite() const { return groupW; }
bool permissions::canGroupExecute() const { return groupE; }
bool permissions::canOthersRead() const { return othersR; }
bool permissions::canOthersWrite() const { return othersW; }
bool permissions::canOthersExecute() const { return othersE; }
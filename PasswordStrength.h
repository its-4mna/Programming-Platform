#ifndef PASSWORDSTRENGTH_H
#define PASSWORDSTRENGTH_H

#include "Strings.h"
#include "ContestGroup.h"

class PasswordStrength 
{
public:
    // Returns the appropriate ContestGroup based on password strength
    static ContestGroup* getGroupFromPassword(const String& password);
    
    // Optional: returns a string description of the strength level
    static String getStrengthLevel(const String& password);
};

#endif
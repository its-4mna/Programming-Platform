#include "PasswordStrength.h"

ContestGroup* PasswordStrength::getGroupFromPassword(const String& password) {
    bool hasDigit = false;
    bool hasLetter = false;
    bool hasSpecial = false;
    
    const char* str = password.getstr();
    for (int i = 0; str[i] != '\0'; ++i) {
        char c = str[i];
        if (c >= '0' && c <= '9') {
            hasDigit = true;
        }
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            hasLetter = true;
        }
        else {
            hasSpecial = true;  // any character not digit or letter
        }
    }
    
    if (hasSpecial && hasLetter && hasDigit) {
        return new HardContestGroup();
    }
    else if (hasLetter && hasDigit) {
        return new MediumContestGroup();
    }
    else {
        // Only digits, or only letters, or empty (default to easy)
        return new EasyContestGroup();
    }
}

String PasswordStrength::getStrengthLevel(const String& password) {
    ContestGroup* group = getGroupFromPassword(password);
    String level = group->getName();
    delete group; // avoid memory leak
    return level;
}
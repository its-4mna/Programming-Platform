#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "user.h"
#include "DynamicArray.h"
#include "KeyValueStore.h"
#include "PersistentStorage.h"
#include "userPersistance.h"

class UserManager 
{
private:
    DynamicArray<User> users;
    KeyValueStore<String, int> usernameToIndex;
    KeyValueStore<String, int> emailToIndex;
    KeyValueStore<String, String> verificationTokens;
    
    String generateVerificationToken(const String& username);
    int findUserByUsername(const String& username) const;
    int findUserByEmail(const String& email) const;

    bool createUserVFSDirectory(const String& username);
    bool deleteUserVFSDirectory(const String& username);

    
public:
    UserManager();
    ~UserManager();

    bool registerUser(const String& username, const String& email, const String& password);
    bool verifyUser(const String& token);
    bool loginUser(const String& username, const String& password);
    bool deleteUser(const String& username);
    
    User* getUserByUsername(const String& username);
    const User* getUserByUsername(const String& username) const;
    
    int getUserCount() const;
    //bool deleteUserVFSDirectory(const String& username);

    //void loadAllUsersFromVFS();
    void init();
};

#endif
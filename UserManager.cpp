#include "UserManager.h"
#include "userPersistance.h"
#include "PersistentStorage.h"
#include <iostream>

UserManager::UserManager() {
}

UserManager::~UserManager() {
}

String UserManager::generateVerificationToken(const String& username) {
    String token = username + "_VERIFY_001";
    return token;
}

int UserManager::findUserByUsername(const String& username) const {
    if (usernameToIndex.contains(username)) {
        return usernameToIndex.get(username);
    }
    return -1;
}

int UserManager::findUserByEmail(const String& email) const {
    if (emailToIndex.contains(email)) {
        return emailToIndex.get(email);
    }
    return -1;
}

bool UserManager::registerUser(const String& username, const String& email, const String& password) 
{
    if (findUserByUsername(username) != -1) {
        std::cout << "Username already exists!" << std::endl;
        return false;
    }
    
    if (findUserByEmail(email) != -1) {
        std::cout << "Email already registered!" << std::endl;
        return false;
    }
    
    if (username.getlen() < 3) {
        std::cout << "Username must be at least 3 characters!" << std::endl;
        return false;
    }
    
    if (password.getlen() < 4) {
        std::cout << "Password must be at least 4 characters!" << std::endl;
        return false;
    }

    // Save to disk first
    if (!UserPersistence::saveUser(username, email, password, false, 0, 0)) 
    {
        std::cout << "Failed to save user to disk!" << std::endl;
        return false;
    }
    
    // Add to memory
    User newUser(username, email, password);
    String token = generateVerificationToken(username);
    newUser.setVerificationToken(token);
    
    if(!createUserVFSDirectory(username))
        std::cout << "Could not create vfs directory for user :(\n";

    int newIndex = users.getSize();
    users.pushBack(newUser);
    usernameToIndex.put(username, newIndex);
    emailToIndex.put(email, newIndex);
    verificationTokens.put(token, username);
    
    std::cout << "\n========== REGISTRATION SUCCESS ==========" << std::endl;
    std::cout << "Username: " << username << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "Verification Token: " << token << std::endl;
    std::cout << "==========================================" << std::endl;
    
    return true;
}

bool UserManager::verifyUser(const String& token) {
    if (verificationTokens.contains(token)) 
    {
        String username = verificationTokens.get(token);
        int index = findUserByUsername(username);
        if (index != -1) 
        {
            users[index].setVerified(true);
            UserPersistence::saveUser(username, users[index].getEmail(), 
                                      users[index].getPassword(), true,
                                      users[index].getTotalScore(), 
                                      users[index].getSolvedCount());
            std::cout << "User " << username << " verified successfully!" << std::endl;
            return true;
        }
    }
    std::cout << "Invalid verification token!" << std::endl;
    return false;
}

bool UserManager::loginUser(const String& username, const String& password) {
    int index = findUserByUsername(username);
    if (index == -1) 
    {
        String diskEmail, diskPassword;
        bool diskVerified;
        int diskScore = 0;
        int diskSolved = 0;
        
        if (UserPersistence::loadUser(username, diskEmail, diskPassword, diskVerified, diskScore, diskSolved)) 
        {
            // Found on disk - load into memory
            User loadedUser(username, diskEmail, diskPassword);
            loadedUser.setVerified(diskVerified);
            loadedUser.setTotalScore(diskScore);
            loadedUser.setSolvedCount(diskSolved);
            
            String token = generateVerificationToken(username);
            loadedUser.setVerificationToken(token);
            verificationTokens.put(token, username);

            int newIndex = users.getSize();
            users.pushBack(loadedUser);
            usernameToIndex.put(username, newIndex);
            emailToIndex.put(diskEmail, newIndex);
            
            index = newIndex;
        } 
        else 
        {
            std::cout << "User not found!" << std::endl;
            return false;
        }
    }
    
    User& user = users[index];
    if (!user.getVerified()) 
    {
        std::cout << "Account not verified! Use token: " << user.getVerificationToken() << std::endl;
        return false;
    }
    
    if (user.checkPassword(password)) 
    {
        std::cout << "Login successful! Welcome " << username << "!" << std::endl;
        return true;
    } 
    else 
    {
        std::cout << "Incorrect password!" << std::endl;
        return false;
    }
}

bool UserManager::deleteUser(const String& username) 
{
    int index = findUserByUsername(username);

    // Delete from disk first
    UserPersistence::deleteUser(username);
    deleteUserVFSDirectory(username);

    if (index == -1) {
        std::cout << "User not found in memory but disk files are deleted." << std::endl;
        return true;
    }
    
    User& user = users[index];

    usernameToIndex.remove(user.getUsername());
    emailToIndex.remove(user.getEmail());
    verificationTokens.remove(user.getVerificationToken());
    users.removeAt(index);
        
    for (int i = index; i < users.getSize(); i++) 
    {
        usernameToIndex.put(users[i].getUsername(), i);
        emailToIndex.put(users[i].getEmail(), i);
    }
    
    std::cout << "User " << username << " deleted successfully!" << std::endl;
    return true;
}

User* UserManager::getUserByUsername(const String& username) {
    int index = findUserByUsername(username);
    if (index != -1) {
        return &users[index];
    }
    return nullptr;
}

const User* UserManager::getUserByUsername(const String& username) const {
    int index = findUserByUsername(username);
    if (index != -1) {
        return &users[index];
    }
    return nullptr;
}

int UserManager::getUserCount() const {
    return users.getSize();
}

bool UserManager::createUserVFSDirectory(const String& username) 
{
    String userPath = String("users/") + username;
    bool userDirCreated = PersistentStorage::makeDirectory(userPath);
    
    String contestPath = userPath + String("/contest_saves");
    String savedPath = userPath + String("/saved_contests");
    String normalPath = userPath + String("/saved_solutions");
    
    PersistentStorage::makeDirectory(contestPath);
    PersistentStorage::makeDirectory(savedPath);
    PersistentStorage::makeDirectory(normalPath);

    std::cout << "Created user directory: " << userPath.getstr() << std::endl;
    return userDirCreated;
}

bool UserManager::deleteUserVFSDirectory(const String& username) {
    String userPath = String("users/") + username;
    return PersistentStorage::deleteDirectory(userPath);
}

void UserManager::init() {
    UserPersistence::init();
    std::cout << "[UserManager] Ready" << std::endl;
}
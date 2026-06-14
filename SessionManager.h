#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "LinkedList.h"
#include "Strings.h"

class SessionManager {
private:
    String currentUser;
    LinkedList<String> sessionHistory;
    bool isLoggedIn;

public:
    SessionManager();
    ~SessionManager();

    bool login(const String& username);
    void logout();
    bool isUserLoggedIn() const;
    String getCurrentUser() const;
    
    void addSession(const String& username);
    void removeSession(const String& username);
    bool hasActiveSession(const String& username) const;
    int getActiveSessionCount() const;
};

#endif
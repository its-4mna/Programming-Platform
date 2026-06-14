#include "SessionManager.h"

SessionManager::SessionManager() {
    isLoggedIn = false;
}

SessionManager::~SessionManager() {
}

bool SessionManager::login(const String& username) {
    currentUser = username;
    isLoggedIn = true;
    addSession(username);
    return true;
}

void SessionManager::logout() {
    if (isLoggedIn) {
        removeSession(currentUser);
        isLoggedIn = false;
    }
}

bool SessionManager::isUserLoggedIn() const {
    return isLoggedIn;
}

String SessionManager::getCurrentUser() const {
    return currentUser;
}

void SessionManager::addSession(const String& username) {
    if (!hasActiveSession(username)) {
        sessionHistory.pushBack(username);
    }
}

void SessionManager::removeSession(const String& username) {
    sessionHistory.remove(username);
}

bool SessionManager::hasActiveSession(const String& username) const {
    return sessionHistory.contains(username);
}

int SessionManager::getActiveSessionCount() const {
    return sessionHistory.getSize();
}
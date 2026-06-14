#ifndef USER_H
#define USER_H

#include "Strings.h"

class User {
private:
    String username;
    String email;
    String password;
    String verificationToken;
    bool isVerified;
    String vfsPath;
    //String userID;
    String groupName;
    bool root;

    int totalScore;
    int solvedCount; 

public:
    User();
    User(const String& uname, const String& mail, const String& pass);
    ~User();

    String getUsername() const;
    String getEmail() const;
    //String getUserID() const;
    String getGroupName() const;
    bool isRoot() const;
    void setRoot(bool r);
    String getVFSPath() const;
    bool getVerified() const;
    String getVerificationToken() const;
    String getPassword() const;

    void setVerified(bool verified);
    void setVerificationToken(const String& token);
    void setVFSPath(const String& path);
    void setGroupName(const String& gname);

    bool checkPassword(const String& pass) const;
    void setPassword(const String& pass);

    String serialize() const;      // Convert user to string for saving
    void deserialize(const String& data);  // Load user from string

    int getTotalScore() const { return totalScore; }
    int getSolvedCount() const { return solvedCount; }
    
    void addScore(int points) { totalScore += points; }
    void addSolvedProblem() { solvedCount++; }
    
    void setTotalScore(int score) { totalScore = score; }
    void setSolvedCount(int count) { solvedCount = count; }



};

#endif
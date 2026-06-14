#include "user.h"

User::User() : totalScore(0), solvedCount(0)
{
    isVerified = false;
}

User::User(const String& uname, const String& mail, const String& pass) : totalScore(0), solvedCount(0)
{
    username = uname;
    email = mail;
    password = pass;
    isVerified = false;
    vfsPath = "/users/";
    vfsPath = vfsPath + username;
    root = false;
    groupName = "users";

}
void User::setGroupName(const String& gname)
{
    groupName = gname;
}
User::~User() 
{

}

String User::getUsername() const 
{
    return username;
}

String User::getEmail() const {
    return email;
}

String User::getVFSPath() const {
    return vfsPath;
}

bool User::getVerified() const {
    return isVerified;
}

String User::getVerificationToken() const {
    return verificationToken;
}

void User::setVerified(bool verified) {
    isVerified = verified;
}

void User::setVerificationToken(const String& token) {
    verificationToken = token;
}

void User::setVFSPath(const String& path) {
    vfsPath = path;
}

bool User::checkPassword(const String& pass) const {
    return password == pass;
}

void User::setPassword(const String& pass) {
    password = pass;
}


bool User::isRoot() const
{
    return root;
}

void User::setRoot(bool r)
{
    root = r;
}

String User::getGroupName() const
{
    return groupName;
}

String User::serialize() const {
    //Format: username|email|password|verified|groupName|root
    String result = username + "|" + email + "|" + password + "|";
    result = result + (isVerified ? "1" : "0") + "|";
    result = result + groupName + "|";
    result = result + (root ? "1" : "0");
    return result;
}

void User::deserialize(const String& data) 
{
    // Parse the string back into fields
    const char* str = data.getstr();
    int len = data.getlen();
    int pos = 0;
    
    auto readField = [&]() -> String 
    {
        int start = pos;

        while (pos < len && str[pos] != '|') 
            pos++;
        char* buf = new char[pos - start + 1];
        for (int i = start; i < pos; i++) 
            buf[i - start] = str[i];
        buf[pos - start] = '\0';
        String result(buf);
        delete[] buf;

        if (str[pos] == '|') 
            pos++;
        return result;
    };
    
    username = readField();
    email = readField();
    password = readField();
    
    String verifiedStr = readField();
    isVerified = (verifiedStr == "1");
    
    groupName = readField();
    
    String rootStr = readField();
    root = (rootStr == "1");
    
    vfsPath = String("users/") + username;
}

String User::getPassword() const
{
    return password;
}
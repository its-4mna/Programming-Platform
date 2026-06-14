#ifndef USERPERSISTANCE_H
#define USERPERSISTANCE_H

#include <cstdlib>
#include <cstdio>
#include "Strings.h"

class UserPersistence 
{
private:
    static String buildPath(const String& username) 
    {
        return String("users/") + username + String(".dat");
    }
    
public:
    // Call once at program start
    static void init() 
    {
        system("mkdir -p ./vfs_data/users");
    }
    
    // Save user to disk with score and solvedCount
    static bool saveUser(const String& username, const String& email, 
                         const String& password, bool verified, 
                         int score, int solvedCount) 
    {
        String path = buildPath(username);
        String realPath = String("./vfs_data/") + path;
        
        // Format: username|email|password|verified|score|solvedCount
        String data = username + "|" + email + "|" + password + "|" + 
                      (verified ? "1" : "0") + "|" + 
                      String::fromInt(score) + "|" + 
                      String::fromInt(solvedCount);
        
        FILE* f = fopen(realPath.getstr(), "w");
        if (!f) 
            return false;
        
        fprintf(f, "%s", data.getstr());
        fclose(f);
        return true;
    }
    
    // Load user from disk (returns true if found)
    static bool loadUser(const String& username, String& outEmail, 
                         String& outPassword, bool& outVerified,
                         int& outScore, int& outSolvedCount) 
    {
        String path = buildPath(username);
        String realPath = String("./vfs_data/") + path;
        
        FILE* f = fopen(realPath.getstr(), "r");
        if (!f)     
            return false;
        
        char buffer[1024];
        if (!fgets(buffer, sizeof(buffer), f)) 
        {
            fclose(f);
            return false;
        }
        fclose(f);
        
        // Parse: username|email|password|verified|score|solvedCount
        String content(buffer);
        const char* str = content.getstr();
        int len = content.getlen();
        int pos = 0;
        
        // Helper to read a field between pipes
        auto readField = [&]() -> String {
            int start = pos;
            while (pos < len && str[pos] != '|') 
                pos++;
            char buf[256];
            int fieldLen = pos - start;
            if (fieldLen > 255) fieldLen = 255;
            for (int i = 0; i < fieldLen; i++) 
                buf[i] = str[start + i];
            buf[fieldLen] = '\0';
            if (pos < len && str[pos] == '|') 
                pos++;
            return String(buf);
        };
        
        // Skip username (we already know it)
        readField();
        
        // Read email
        outEmail = readField();
        
        // Read password
        outPassword = readField();
        
        // Read verified
        String verifiedStr = readField();
        outVerified = (verifiedStr.getlen() > 0 && verifiedStr[0] == '1');
        
        // Read score (optional - old files might not have it)
        String scoreStr = readField();
        if (scoreStr.getlen() > 0) {
            // Convert string to int manually
            const char* s = scoreStr.getstr();
            outScore = 0;
            for (int i = 0; s[i] >= '0' && s[i] <= '9'; i++) {
                outScore = outScore * 10 + (s[i] - '0');
            }
        } else {
            outScore = 0;
        }
        
        // Read solvedCount (optional - old files might not have it)
        String solvedStr = readField();
        if (solvedStr.getlen() > 0) {
            const char* s = solvedStr.getstr();
            outSolvedCount = 0;
            for (int i = 0; s[i] >= '0' && s[i] <= '9'; i++) {
                outSolvedCount = outSolvedCount * 10 + (s[i] - '0');
            }
        } else {
            outSolvedCount = 0;
        }
        
        return true;
    }
    
    // Check if user exists
    static bool userExists(const String& username) 
    {
        String path = buildPath(username);
        String realPath = String("./vfs_data/") + path;
        FILE* f = fopen(realPath.getstr(), "r");
        if (f) 
        {
            fclose(f);
            return true;
        }
        return false;
    }
    
    // Delete user
    static bool deleteUser(const String& username) 
    {
        String path = buildPath(username);
        String realPath = String("./vfs_data/") + path;
        
        char cmd[512];
        snprintf(cmd, sizeof(cmd), "rm -f %s", realPath.getstr());
        return system(cmd) == 0;
    }
};

#endif
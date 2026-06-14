#ifndef PERSISTENTSTORAGE_H
#define PERSISTENTSTORAGE_H

#include "Strings.h"

class PersistentStorage {
public:
    static void init();  // creates ./vfs_data/ folder

    // Save data to a file identified by vfsPath (e.g., "root/users/fatima/file.dat")
    static bool saveFile(const String& vfsPath, const char* data, int size);

    // Load data; returns number of bytes read, or -1 if file not found
    static int loadFile(const String& vfsPath, char* buffer, int maxSize);

    // Delete a file
    static bool deleteFile(const String& vfsPath);

    // Check if a file exists
    static bool fileExists(const String& vfsPath);

    //create a directory
    static bool makeDirectory(const String& vfsPath); 

    //delete a directory and all its contents recursively
    static bool deleteDirectory(const String& vfsPath);

};

#endif
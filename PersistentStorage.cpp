#include "PersistentStorage.h"
#include <cstdio>   // fopen, fwrite, fread, fclose, remove

#ifdef _WIN32
#include <direct.h>   // for _mkdir
#else
#include <sys/stat.h> // for mkdir
#endif

static const char* BASE_PATH = "./vfs_data/";

void PersistentStorage::init() {
#ifdef _WIN32
    _mkdir(BASE_PATH);
#else
    mkdir(BASE_PATH, 0777);
#endif
}

static String buildRealPath(const String& vfsPath) {
    return String(BASE_PATH) + vfsPath;
}

bool PersistentStorage::saveFile(const String& vfsPath, const char* data, int size) {
    if (!data || size <= 0) 
        return false;

    String realPath = buildRealPath(vfsPath);
    FILE* f = fopen(realPath.getstr(), "wb");
    if (!f) 
        return false;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == size);
}

int PersistentStorage::loadFile(const String& vfsPath, char* buffer, int maxSize) {
    String realPath = buildRealPath(vfsPath);
    FILE* f = fopen(realPath.getstr(), "rb");
    if (!f) 
        return -1;
    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);
    int toRead = (fileSize < maxSize) ? (int)fileSize : maxSize;
    int bytes = fread(buffer, 1, toRead, f);
    fclose(f);
    return bytes;
}

bool PersistentStorage::deleteFile(const String& vfsPath) {
    String realPath = buildRealPath(vfsPath);
    return remove(realPath.getstr()) == 0;
}

bool PersistentStorage::fileExists(const String& vfsPath) {
    String realPath = buildRealPath(vfsPath);
    FILE* f = fopen(realPath.getstr(), "rb");
    if (f) { fclose(f); return true; }
    return false;
}

bool PersistentStorage::makeDirectory(const String& vfsPath) {
    String realPath = buildRealPath(vfsPath);
#ifdef _WIN32
    return _mkdir(realPath.getstr()) == 0;
#else
    return mkdir(realPath.getstr(), 0777) == 0;
#endif
}

bool PersistentStorage::deleteDirectory(const String& vfsPath) {
    String realPath = buildRealPath(vfsPath);
    
    // Build the command
    String cmd;
#ifdef _WIN32
    cmd = String("rmdir /s /q \"") + realPath + String("\"");
#else
    cmd = String("rm -rf \"") + realPath + String("\"");
#endif
    
    // Execute the command
    int result = system(cmd.getstr());
    
    return (result == 0);
}

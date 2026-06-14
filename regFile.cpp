#include "regFile.h"
#include "PersistentStorage.h"
#include "directory.h"

regFile::regFile(const String& n, User* o, const permissions& p)
    : sysEntity(n, o, p), content(nullptr), size(0), capacity(0) 
    {
        // Try to load from persistent storage
        String path = n; // temporarily, before parent is set. Better deferred.
        // We'll load after parent is set – so do it in a separate init()
    }

regFile::regFile(const regFile& other)
    : sysEntity(other), content(nullptr), size(0), capacity(0)
{
    if (other.size > 0) {
        setContent(other.content, other.size);
    }
}

regFile& regFile::operator=(const regFile& other)
{
    if (this != &other) {
        sysEntity::operator=(other);
        delete[] content;
        content = nullptr;
        size = capacity = 0;
        if (other.size > 0) {
            setContent(other.content, other.size);
        }
    }
    return *this;
}

regFile::~regFile()
{
    delete[] content;
}

void regFile::grow(int newCapacity)
{
    if (newCapacity <= capacity) 
        return;
    char* newBuf = new char[newCapacity];
    for (int i = 0; i < size; ++i) 
        newBuf[i] = content[i];
    delete[] content;
    content = newBuf;
    capacity = newCapacity;
}

int regFile::read(char* buffer, int bufSize, int offset)
{
    if (!buffer || bufSize <= 0 || offset >= size) 
        return 0;
    int toRead = bufSize;
    if (offset + toRead > size) 
        toRead = size - offset;
    for (int i = 0; i < toRead; ++i) 
        buffer[i] = content[offset + i];
    return toRead;
}

int regFile::write(const char* data, int dataSize, int offset)
{
    if (!data || dataSize <= 0) 
        return -1;
    int end = offset + dataSize;
    if (end > capacity) 
        grow(end);
    for (int i = 0; i < dataSize; ++i) 
        content[offset + i] = data[i];
    if (end > size) 
        size = end;
    touchModified();

    String path = getStoragePath();
    PersistentStorage::saveFile(path, content, size);

    return dataSize;
}

void regFile::setContent(const char* data, int dataSize)
{
    delete[] content;
    if (!data || dataSize == 0) {
        content = nullptr;
        size = capacity = 0;
        return;
    }
    if (dataSize == -1) {
        // manual strlen
        dataSize = 0;
        while (data[dataSize] != '\0') ++dataSize;
    }
    content = new char[dataSize];
    for (int i = 0; i < dataSize; ++i) content[i] = data[i];
    size = dataSize;
    capacity = dataSize;
    touchModified();

    String path = getStoragePath();
    PersistentStorage::saveFile(path, content, size);
}

int regFile::getSize() const { return size; }

sysEntity* regFile::clone() const { return new regFile(*this); }

void regFile::print(int depth) const
{
    printHeader(depth);
    std::cout << "Content preview: ";
    int preview = (size < 40) ? size : 40;
    for (int i = 0; i < preview; ++i) 
        std::cout << content[i];
    if (size > 40) 
        std::cout << "...";
    std::cout << std::endl;
}

const char* regFile::getType() const { return "regFile"; }

String regFile::getStoragePath() const 
{
    String path;
    if (!parent) 
        return name;

    Directory* parentDir = dynamic_cast<Directory*>(parent);
    if (parentDir) 
        return parentDir->getFullPath() + String("/") + name;
    
    
    
    return path;
}

void regFile::syncFromDisk() 
{
    String path = getStoragePath();
    if (!PersistentStorage::fileExists(path)) 
    {
        return;  // no file on disk – nothing to load
    }
    // Determine file size
    char buffer[4096];
    int bytes = PersistentStorage::loadFile(path, buffer, sizeof(buffer) - 1);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        setContent(buffer, bytes);
    }
}

String regFile::getFullPath() const {
    return getStoragePath();
}

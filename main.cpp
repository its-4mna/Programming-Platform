#include <SFML/Graphics.hpp>
#include <iostream>

#include "WelcomeScreen.h"
#include "dashboard.h"
#include "ScreenManager.h"
#include "UserManager.h"
#include "SessionManager.h"
#include "ProblemBank.h"

// VFS includes
#include "volume.h"
#include "storageMedium.h"
#include "partition.h"
#include "directory.h"
#include "regFile.h"
#include "permissions.h"
#include "user.h"
#include "userPersistance.h"
#include "ContestData.h"

// Global variables
UserManager usermanager;
SessionManager sessionmanager;
Volume* volume = nullptr;
ProblemBank* problembank = nullptr;

int main()
{
    PersistentStorage::init();
    usermanager.init();
    ContestLibrary::init();
    
    std::cout << "Initializing VFS..." << std::endl;
    
    // Create permissions for root user (full access)
    permissions fullPerm(true, true, true, true, true, true, true, true, true);
    
    // Create root user for VFS (Hania's User class)
    User* rootUser = new User("root", "root@system.com", "rootpass");
    rootUser->setRoot(true);
    rootUser->setGroupName("root");
    
    // Create a storage medium (SSD of 1000 MB)
    storageMedium* medium = new storageMedium("MainSSD", 1000, storageMedium::TYPE_SSD);
    
    // Create a volume
    volume = new Volume("MainVolume");
    
    // Add the medium to volume
    volume->addMedium(1000, storageMedium::TYPE_SSD);
    
    // Create a partition using the medium's method
    Partition* mainPart = medium->createPartition("main", 950, "ext4", rootUser);
    volume->setPrimaryPartition(mainPart);
    volume->registerMount(mainPart);
    
    // Get root directory
    Directory* rootDir = volume->getRootDirectory();
    if (!rootDir) {
        std::cout << "ERROR: Failed to get root directory!" << std::endl;
        return -1;
    }
    
    // Create required directories
    Directory* usersDir = new Directory("users", rootUser, fullPerm);
    rootDir->addChild(usersDir);
    
    /*
    Directory* problemsDir = new Directory("problems", rootUser, fullPerm);
    rootDir->addChild(problemsDir);
    
    // Create difficulty subdirectories inside problems
    for (int diff = 1; diff <= 3; diff++) {
        String diffName = String("difficulty") + String::fromInt(diff);
        Directory* diffDir = new Directory(diffName, rootUser, fullPerm);
        problemsDir->addChild(diffDir);
    }
    */

    Directory* problemsDir = new Directory("problems", rootUser, fullPerm);
    rootDir->addChild(problemsDir);

    // Initialize ProblemBank and seed (seedToVFS will create diff folders)
    problembank = new ProblemBank();

    // Always seed - seedToVFS checks for duplicates internally
    problembank->seedToVFS(problemsDir, rootUser, fullPerm);

    // Then load
    problembank->loadFromVFS(problemsDir);

    // ========== STEP 2: Initialize ProblemBank ==========
    problembank = new ProblemBank();
    
    // Seed problems if directory is empty
    if (problemsDir->getChildCount() == 0) {
        std::cout << "Seeding problems into VFS..." << std::endl;
        problembank->seedToVFS(problemsDir, rootUser, fullPerm);
    }
    
    // Load problems from VFS
    std::cout << "Loading problems from VFS..." << std::endl;
    problembank->loadFromVFS(problemsDir);
    std::cout << "Loaded " << problembank->getProblemCount() << " problems." << std::endl;
    
    // ========== STEP 3: Initialize SFML ==========
    ScreenManager manager;
    
    sf::Font font;
    if (!font.loadFromFile("assets/Goldman-Regular.ttf")) {
        std::cout << "Warning: Could not load font. Continuing anyway." << std::endl;
    }
    
    // ========== STEP 4: Start the application ==========
    WelcomeScreen* welcome = new WelcomeScreen(manager, font, usermanager, sessionmanager, problembank);
    manager.setScreen(welcome);
    
    manager.run();
    
    // ========== STEP 5: Cleanup ==========
    delete problembank;
    delete volume;
    delete rootUser;
    delete medium;
    
    return 0;
}

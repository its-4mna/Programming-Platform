#include "ContestData.h"

Contest ContestLibrary::easyContests[2];
Contest ContestLibrary::midContests[2];
Contest ContestLibrary::hardContests[2];

void ContestLibrary::init() {
    // Easy Contest 1
    easyContests[0].name = "BEGINNER CHALLENGE";
    easyContests[0].durationMinutes = 150;
    easyContests[0].problemCount = 5;
    easyContests[0].problems[0] = {1, 1};
    easyContests[0].problems[1] = {2, 1};
    easyContests[0].problems[2] = {3, 1};
    easyContests[0].problems[3] = {4, 2};
    easyContests[0].problems[4] = {5, 2};
    
    // Easy Contest 2
    easyContests[1].name = "NOVICE CUP";
    easyContests[1].durationMinutes = 150;
    easyContests[1].problemCount = 5;
    easyContests[1].problems[0] = {6, 1};
    easyContests[1].problems[1] = {7, 1};
    easyContests[1].problems[2] = {8, 1};
    easyContests[1].problems[3] = {9, 2};
    easyContests[1].problems[4] = {10, 2};
    
    // Medium Contest 1
    midContests[0].name = "CODER'S LEAGUE";
    midContests[0].durationMinutes = 180;
    midContests[0].problemCount = 5;
    midContests[0].problems[0] = {1, 1};
    midContests[0].problems[1] = {11, 2};
    midContests[0].problems[2] = {12, 2};
    midContests[0].problems[3] = {13, 2};
    midContests[0].problems[4] = {21, 3};
    
    // Medium Contest 2
    midContests[1].name = "ALGORITHM ARENA";
    midContests[1].durationMinutes = 180;
    midContests[1].problemCount = 5;
    midContests[1].problems[0] = {2, 1};
    midContests[1].problems[1] = {14, 2};
    midContests[1].problems[2] = {15, 2};
    midContests[1].problems[3] = {16, 2};
    midContests[1].problems[4] = {22, 3};
    
    // Hard Contest 1
    hardContests[0].name = "MASTER'S CHALLENGE";
    hardContests[0].durationMinutes = 210;
    hardContests[0].problemCount = 5;
    hardContests[0].problems[0] = {11, 2};
    hardContests[0].problems[1] = {12, 2};
    hardContests[0].problems[2] = {21, 3};
    hardContests[0].problems[3] = {22, 3};
    hardContests[0].problems[4] = {23, 3};
    
    // Hard Contest 2
    hardContests[1].name = "GRAND FINALE";
    hardContests[1].durationMinutes = 210;
    hardContests[1].problemCount = 5;
    hardContests[1].problems[0] = {13, 2};
    hardContests[1].problems[1] = {14, 2};
    hardContests[1].problems[2] = {24, 3};
    hardContests[1].problems[3] = {25, 3};
    hardContests[1].problems[4] = {26, 3};
}
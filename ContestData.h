#ifndef CONTESTDATA_H
#define CONTESTDATA_H

#include "Strings.h"

struct ContestProblem {
    int problemId;
    int difficulty;
};

struct Contest {
    String name;
    int durationMinutes;
    ContestProblem problems[5];
    int problemCount;
};

class ContestLibrary {
public:
    static Contest easyContests[2];
    static Contest midContests[2];
    static Contest hardContests[2];
    
    static void init();
};

#endif
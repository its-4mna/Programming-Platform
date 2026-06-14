#ifndef CONTESTMANAGER_H
#define CONTESTMANAGER_H

#include "ProblemBank.h"
#include "ContestGroup.h"
#include "directory.h"
#include "user.h"

struct ContestSession {
    int problemIndices[10];
    int problemCount;
    int attempts[10];
    bool solved[10];
    int submissionTime[10]; // seconds from start when solved (0 = not solved)
    char code[10][4096];
    int startTime;
    int remainingSeconds;
    int totalDurationSeconds;
    int groupId;
};

class ContestManager {
private:
    ProblemBank* bank;
    String getSessionPath(const String& username) const;

public:
    ContestManager(ProblemBank* pb);
    ~ContestManager();

    bool startContest(User* user, ContestGroup* group, Directory* userDir, int durationSeconds = 3600);
    bool submitSolution(User* user, int problemIndex, const char* code, Directory* userDir);
    bool markProblemSolved(User* user, int problemIndex, Directory* userDir, int timeTakenSeconds);

    bool saveContest(User* user, Directory* userDir);
    bool resumeContest(User* user, Directory* userDir);
    int getRemainingSeconds(User* user, Directory* userDir);
    int getCurrentElapsedSeconds(User* user, Directory* userDir);

    int calculateScore(User* user, Directory* userDir);
    void keepCode(User* user, Directory* userDir);
    void discardCode(User* user, Directory* userDir);
    int* getProblemsForContest(int slot, int& outCount);

};

#endif
#include "ContestManager.h"
#include "regFile.h"
#include "PersistentStorage.h"
#include "problem.h"
#include <ctime>

ContestManager::ContestManager(ProblemBank* pb) : bank(pb) {}
ContestManager::~ContestManager() {}

String ContestManager::getSessionPath(const String& username) const {
    return String("users/") + username + String("/contest_saves/current_contest.dat");
}

static int getTimeLimitForDifficulty(int difficulty) {
    if (difficulty == 1) return 1800;   // 30 minutes
    if (difficulty == 2) return 3600;   // 60 minutes
    return 7200;                        // 120 minutes
}

// ---------- Serialization helpers ----------
static bool parseSession(const String& data, ContestSession& session) {
    const char* str = data.getstr();
    int len = data.getlen();
    int pos = 0;
    
    auto readLine = [&]() -> String {
        if (pos >= len) return String("");
        int start = pos;
        while (pos < len && str[pos] != '\n') ++pos;
        int lineLen = pos - start;
        char* buf = new char[lineLen + 1];
        for (int i = 0; i < lineLen; ++i) buf[i] = str[start + i];
        buf[lineLen] = '\0';
        String line(buf);
        delete[] buf;
        if (pos < len && str[pos] == '\n') ++pos;
        return line;
    };
    
    // First line: problemCount
    String line = readLine();
    const char* s = line.getstr();
    int count = 0;
    for (int i = 0; s[i] >= '0' && s[i] <= '9'; ++i) count = count * 10 + (s[i] - '0');
    session.problemCount = count;
    
    for (int i = 0; i < session.problemCount; ++i) {
        String probLine = readLine();
        const char* pl = probLine.getstr();
        int idx = 0, att = 0, solvedFlag = 0, subTime = 0;
        int j = 0;
        // parse problem index
        while (pl[j] >= '0' && pl[j] <= '9') idx = idx * 10 + (pl[j++] - '0');
        while (pl[j] == ' ') ++j;
        // parse attempts
        while (pl[j] >= '0' && pl[j] <= '9') att = att * 10 + (pl[j++] - '0');
        while (pl[j] == ' ') ++j;
        // parse solved flag
        while (pl[j] >= '0' && pl[j] <= '9') solvedFlag = solvedFlag * 10 + (pl[j++] - '0');
        while (pl[j] == ' ') ++j;
        // parse submission time
        while (pl[j] >= '0' && pl[j] <= '9') subTime = subTime * 10 + (pl[j++] - '0');
        while (pl[j] == ' ') ++j;
        // remaining is code
        int codeStart = j;
        int codeLen = 0;
        while (pl[j] != '\0') { ++j; ++codeLen; }
        char* codeBuf = new char[codeLen + 1];
        for (int k = 0; k < codeLen; ++k) codeBuf[k] = pl[codeStart + k];
        codeBuf[codeLen] = '\0';
        
        session.problemIndices[i] = idx;
        session.attempts[i] = att;
        session.solved[i] = (solvedFlag == 1);
        session.submissionTime[i] = subTime;
        int copyLen = (codeLen < 4095) ? codeLen : 4095;
        for (int k = 0; k < copyLen; ++k) session.code[i][k] = codeBuf[k];
        session.code[i][copyLen] = '\0';
        delete[] codeBuf;
    }
    
    // Read startTime, totalDuration, remainingSeconds, groupId
    String startStr = readLine();
    String totalStr = readLine();
    String remainStr = readLine();
    String groupStr = readLine();
    
    auto toInt = [](const String& s) -> int {
        const char* p = s.getstr();
        int res = 0;
        for (int i = 0; p[i] >= '0' && p[i] <= '9'; ++i) res = res * 10 + (p[i] - '0');
        return res;
    };
    
    session.startTime = toInt(startStr);
    session.totalDurationSeconds = toInt(totalStr);
    session.remainingSeconds = toInt(remainStr);
    session.groupId = toInt(groupStr);
    return true;
}

static String serializeSession(const ContestSession& session) {
    String data;
    data = data + String::fromInt(session.problemCount) + "\n";
    for (int i = 0; i < session.problemCount; ++i) {
        data = data + String::fromInt(session.problemIndices[i]) + " " +
               String::fromInt(session.attempts[i]) + " " +
               String::fromInt(session.solved[i] ? 1 : 0) + " " +
               String::fromInt(session.submissionTime[i]) + " " +
               String(session.code[i]) + "\n";
    }
    data = data + String::fromInt(session.startTime) + "\n" +
           String::fromInt(session.totalDurationSeconds) + "\n" +
           String::fromInt(session.remainingSeconds) + "\n" +
           String::fromInt(session.groupId);
    return data;
}

// ---------- Public Methods ----------
bool ContestManager::startContest(User* user, ContestGroup* group, Directory* userDir, int durationSeconds) {
    Directory* savesDir = dynamic_cast<Directory*>(userDir->findChild("contest_saves"));
    if (!savesDir) {
        permissions userPerm(true, true, false, true, false, false, true, false, false);
        savesDir = new Directory("contest_saves", user, userPerm);
        userDir->addChild(savesDir);
    }
    
    int totalProblems = group->getTotalProblems();
    int* indices = bank->selectProblems(group, totalProblems);
    if (!indices) return false;
    
    ContestSession session;
    session.problemCount = totalProblems;
    session.startTime = (int)time(nullptr);
    session.totalDurationSeconds = durationSeconds;
    session.remainingSeconds = durationSeconds;
    for (int i = 0; i < totalProblems; ++i) {
        session.problemIndices[i] = indices[i];
        session.attempts[i] = 0;
        session.solved[i] = false;
        session.submissionTime[i] = -1;
        session.code[i][0] = '\0';
    }
    delete[] indices;
    
    const char* gname = group->getName().getstr();
    if (gname[0] == 'E') session.groupId = 0;
    else if (gname[0] == 'M') session.groupId = 1;
    else session.groupId = 2;
    
    String data = serializeSession(session);
    regFile* sessionFile = new regFile("current_contest.dat", user, savesDir->getPermissions());
    savesDir->addChild(sessionFile);
    sessionFile->setContent(data.getstr(), data.getlen());
    return true;
}

bool ContestManager::submitSolution(User* user, int problemIndex, const char* code, Directory* userDir) {
    Directory* savesDir = dynamic_cast<Directory*>(userDir->findChild("contest_saves"));
    if (!savesDir) return false;
    regFile* sessionFile = dynamic_cast<regFile*>(savesDir->findChild("current_contest.dat"));
    if (!sessionFile) return false;
    
    int size = sessionFile->getSize();
    char* buf = new char[size + 1];
    sessionFile->read(buf, size, 0);
    buf[size] = '\0';
    String data(buf);
    delete[] buf;
    
    ContestSession session;
    if (!parseSession(data, session)) return false;
    if (problemIndex < 0 || problemIndex >= session.problemCount) return false;
    
    session.attempts[problemIndex]++;
    int copyLen = 0;
    while (code[copyLen] != '\0' && copyLen < 4095) {
        session.code[problemIndex][copyLen] = code[copyLen];
        copyLen++;
    }
    session.code[problemIndex][copyLen] = '\0';
    
    String newData = serializeSession(session);
    sessionFile->setContent(newData.getstr(), newData.getlen());
    return true;
}

bool ContestManager::markProblemSolved(User* user, int problemIndex, Directory* userDir, int timeTakenSeconds) {
    Directory* savesDir = dynamic_cast<Directory*>(userDir->findChild("contest_saves"));
    if (!savesDir) return false;
    regFile* sessionFile = dynamic_cast<regFile*>(savesDir->findChild("current_contest.dat"));
    if (!sessionFile) return false;
    
    int size = sessionFile->getSize();
    char* buf = new char[size + 1];
    sessionFile->read(buf, size, 0);
    buf[size] = '\0';
    String data(buf);
    delete[] buf;
    
    ContestSession session;
    if (!parseSession(data, session)) return false;
    if (problemIndex < 0 || problemIndex >= session.problemCount) return false;
    if (session.solved[problemIndex]) return true; // already solved
    
    session.solved[problemIndex] = true;
    session.submissionTime[problemIndex] = timeTakenSeconds;
    
    String newData = serializeSession(session);
    sessionFile->setContent(newData.getstr(), newData.getlen());
    return true;
}

bool ContestManager::saveContest(User* user, Directory* userDir) {
    Directory* savesDir = dynamic_cast<Directory*>(userDir->findChild("contest_saves"));
    if (!savesDir) return false;
    regFile* sessionFile = dynamic_cast<regFile*>(savesDir->findChild("current_contest.dat"));
    if (!sessionFile) return false;
    
    int size = sessionFile->getSize();
    char* buf = new char[size + 1];
    sessionFile->read(buf, size, 0);
    buf[size] = '\0';
    String data(buf);
    delete[] buf;
    
    ContestSession session;
    if (!parseSession(data, session)) return false;
    int now = (int)time(nullptr);
    int elapsed = now - session.startTime;
    int remaining = session.totalDurationSeconds - elapsed;
    if (remaining < 0) remaining = 0;
    session.remainingSeconds = remaining;
    
    String newData = serializeSession(session);
    sessionFile->setContent(newData.getstr(), newData.getlen());
    return true;
}

bool ContestManager::resumeContest(User* user, Directory* userDir) {
    Directory* savesDir = dynamic_cast<Directory*>(userDir->findChild("contest_saves"));
    if (!savesDir) return false;
    regFile* sessionFile = dynamic_cast<regFile*>(savesDir->findChild("current_contest.dat"));
    if (!sessionFile) return false;
    return true;
}

int ContestManager::getRemainingSeconds(User* user, Directory* userDir) {
    Directory* savesDir = dynamic_cast<Directory*>(userDir->findChild("contest_saves"));
    if (!savesDir) return 0;
    regFile* sessionFile = dynamic_cast<regFile*>(savesDir->findChild("current_contest.dat"));
    if (!sessionFile) return 0;
    
    int size = sessionFile->getSize();
    char* buf = new char[size + 1];
    sessionFile->read(buf, size, 0);
    buf[size] = '\0';
    String data(buf);
    delete[] buf;
    
    ContestSession session;
    if (!parseSession(data, session)) return 0;
    int now = (int)time(nullptr);
    int elapsed = now - session.startTime;
    int remaining = session.totalDurationSeconds - elapsed;
    if (remaining < 0) remaining = 0;
    return remaining;
}

int ContestManager::getCurrentElapsedSeconds(User* user, Directory* userDir) {
    Directory* savesDir = dynamic_cast<Directory*>(userDir->findChild("contest_saves"));
    if (!savesDir) return 0;
    regFile* sessionFile = dynamic_cast<regFile*>(savesDir->findChild("current_contest.dat"));
    if (!sessionFile) return 0;
    
    int size = sessionFile->getSize();
    char* buf = new char[size + 1];
    sessionFile->read(buf, size, 0);
    buf[size] = '\0';
    String data(buf);
    delete[] buf;
    
    ContestSession session;
    if (!parseSession(data, session)) return 0;
    int now = (int)time(nullptr);
    int elapsed = now - session.startTime;
    if (elapsed < 0) elapsed = 0;
    return elapsed;
}

int ContestManager::calculateScore(User* user, Directory* userDir) {
    Directory* savesDir = dynamic_cast<Directory*>(userDir->findChild("contest_saves"));
    if (!savesDir) return 0;
    regFile* sessionFile = dynamic_cast<regFile*>(savesDir->findChild("current_contest.dat"));
    if (!sessionFile) return 0;

    int size = sessionFile->getSize();
    char* buf = new char[size + 1];
    sessionFile->read(buf, size, 0);
    buf[size] = '\0';
    String data(buf);
    delete[] buf;

    ContestSession session;
    if (!parseSession(data, session)) return 0;

    // Contest: 3 hrs = 10800 seconds, 5 problems
    // Formula per problem:
    //   basePoints    = difficulty * 100  (diff1=100, diff2=200, diff3=300)
    //   timeBonus     = 1.0  if solved in first half of contest
    //                   0.5  if solved in first 3/4
    //                   0.25 if solved in remaining time
    //   attemptPenalty = 1.0 / attempts  (1st try = full, 2nd = half, etc.)
    //   problemScore  = (int)(basePoints * timeBonus * attemptPenalty)

    int contestDuration = session.totalDurationSeconds;
    if (contestDuration <= 0) contestDuration = 10800;

    int totalScore = 0;
    for (int i = 0; i < session.problemCount; ++i) {
        if (!session.solved[i]) continue;

        Problem* prob = bank->getProblem(session.problemIndices[i]);
        if (!prob) continue;

        int difficulty = prob->getDifficulty();
        int basePoints = difficulty * 100;
        int attempts   = session.attempts[i];
        int timeTaken  = session.submissionTime[i];

        double timeBonus;
        double ratio = (double)timeTaken / contestDuration;
        if      (ratio <= 0.5)  timeBonus = 1.0;
        else if (ratio <= 0.75) timeBonus = 0.5;
        else                    timeBonus = 0.25;

        double attemptPenalty = 1.0 / attempts;

        int problemScore = (int)(basePoints * timeBonus * attemptPenalty);
        totalScore += problemScore;
    }
    return totalScore;
}

void ContestManager::keepCode(User* user, Directory* userDir) {
    Directory* savesDir = dynamic_cast<Directory*>(userDir->findChild("contest_saves"));
    if (!savesDir) return;
    regFile* sessionFile = dynamic_cast<regFile*>(savesDir->findChild("current_contest.dat"));
    if (!sessionFile) return;
    
    int size = sessionFile->getSize();
    char* buf = new char[size + 1];
    sessionFile->read(buf, size, 0);
    buf[size] = '\0';
    String data(buf);
    delete[] buf;
    
    ContestSession session;
    if (!parseSession(data, session)) return;
    
    Directory* savedDir = dynamic_cast<Directory*>(userDir->findChild("saved_contests"));
    if (!savedDir) {
        permissions userPerm(true, true, false, true, false, false, true, false, false);
        savedDir = new Directory("saved_contests", user, userPerm);
        userDir->addChild(savedDir);
    }
    
    for (int i = 0; i < session.problemCount; ++i) {
        String fileName = String("problem_") + String::fromInt(i) + String(".cpp");
        regFile* codeFile = new regFile(fileName, user, savedDir->getPermissions());
        codeFile->setContent(session.code[i], -1);
        savedDir->addChild(codeFile);
    }
}

void ContestManager::discardCode(User* user, Directory* userDir) {
    Directory* savesDir = dynamic_cast<Directory*>(userDir->findChild("contest_saves"));
    if (!savesDir) return;
    savesDir->removeChild("current_contest.dat");
}

int* ContestManager::getProblemsForContest(int slot, int& outCount) {
    // For now, slot determines difficulty
    // slot 0 = Easy, slot 1 = Medium, slot 2 = Hard
    ContestGroup* group = nullptr;
    
    switch(slot) 
    {
        case 0: group = new EasyContestGroup(); break;
        case 1: group = new MediumContestGroup(); break;
        case 2: group = new HardContestGroup(); break;
        default: group = new EasyContestGroup(); break;
    }
    
    outCount = group->getTotalProblems();
    int* indices = bank->selectProblems(group, outCount);
    
    delete group;
    return indices;
}
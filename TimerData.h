#ifndef TIMERDATA_H
#define TIMERDATA_H

#include "Strings.h"
#include <ctime>

struct TimerData {
    int problemId;
    
    // Practice mode
    int timeSpentSeconds;
    int practiceSessionStart;
    
    // Contest mode
    bool isContestMode;
    int contestEndTime;
    int contestStartTime;
    int contestRemainingSeconds;
    
    TimerData()
        : problemId(0)
        , timeSpentSeconds(0)
        , practiceSessionStart(0)
        , isContestMode(false)
        , contestEndTime(0)
        , contestStartTime(0)
        , contestRemainingSeconds(0)
    {}
    
    String serialize() const {
        String result;
        result = result + String::fromInt(problemId) + "|";
        result = result + String::fromInt(timeSpentSeconds) + "|";
        result = result + (isContestMode ? "1" : "0") + "|";
        result = result + String::fromInt(contestEndTime) + "|";
        result = result + String::fromInt(contestStartTime) + "|";
        result = result + String::fromInt(contestRemainingSeconds);
        return result;
    }
    
    void deserialize(const String& data) {
        const char* str = data.getstr();
        int len = data.getlen();
        int pos = 0;
        
        auto readInt = [&]() -> int {
            int val = 0;
            while (pos < len && str[pos] >= '0' && str[pos] <= '9') {
                val = val * 10 + (str[pos] - '0');
                pos++;
            }
            if (pos < len && str[pos] == '|') pos++;
            return val;
        };
        
        problemId = readInt();
        timeSpentSeconds = readInt();
        isContestMode = (readInt() == 1);
        contestEndTime = readInt();
        contestStartTime = readInt();
        contestRemainingSeconds = readInt();
    }
    
    void initContest(int durationSeconds) {
        int now = (int)time(nullptr);
        contestStartTime = now;
        contestEndTime = now + durationSeconds;
        contestRemainingSeconds = durationSeconds;
        isContestMode = true;
    }
    
    int getContestRemaining() const {
        if (contestEndTime == 0) return 0;
        int remaining = contestEndTime - (int)time(nullptr);
        return (remaining < 0) ? 0 : remaining;
    }
    
    int getPracticeTotal() const {
        int total = timeSpentSeconds;
        if (practiceSessionStart != 0) {
            total += (int)time(nullptr) - practiceSessionStart;
        }
        return (total < 0) ? 0 : total;
    }
    
    void beginPracticeSession() {
        if (practiceSessionStart == 0) {
            practiceSessionStart = (int)time(nullptr);
        }
    }
    
    void endPracticeSession() {
        if (practiceSessionStart != 0) {
            int elapsed = (int)time(nullptr) - practiceSessionStart;
            if (elapsed > 0) {
                timeSpentSeconds += elapsed;
            }
            practiceSessionStart = 0;
        }
    }
};

#endif
#ifndef PROBLEMBANK_H
#define PROBLEMBANK_H

#include "problem.h"
#include "ContestGroup.h"
#include "directory.h"

class ProblemBank {
private:
    Problem** problems;
    int problemCount;
    int capacity;
    void expand();

public:
    ProblemBank();
    ~ProblemBank();

    // Load all problems from VFS directory (assumed /problems/difficultyX/ files)
    void loadFromVFS(Directory* problemsDir);

    // Seed initial problems into VFS (call once if folder empty)
    void seedToVFS(Directory* problemsDir, User* owner, const permissions& perm);

    Problem* getProblem(int index) const;
    int getProblemCount() const;

    // Randomly select problems based on group's difficulty distribution
    int* selectProblems(const ContestGroup* group, int count) const;
};

#endif
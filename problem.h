#ifndef PROBLEM_H
#define PROBLEM_H

#include "Strings.h"

class Problem {
private:
    int id;
    int difficulty;   // 1, 2, or 3
    String title;
    String description;
    String constraints;
    String* testInputs;
    String* testOutputs;
    int testCount;
    int testCapacity;

    void expandTests();

public:
    Problem();
    Problem(int _id, int _diff, const String& _title, const String& _desc, const String& _cons);
    Problem(const Problem& other);
    Problem& operator=(const Problem& other);
    ~Problem();

    void addTestCase(const String& input, const String& output);
    int getTestCount() const;
    String getInput(int idx) const;
    String getOutput(int idx) const;

    int getId() const;
    int getDifficulty() const;
    String getTitle() const;
    String getDescription() const;
    String getConstraints() const;

    String serialize() const;
    void deserialize(const String& data);
};

#endif
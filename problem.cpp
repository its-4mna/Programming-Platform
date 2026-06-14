#include "problem.h"

// Manual string to int conversion (no <cstdlib>)
static int stringToInt(const String& s) {
    const char* str = s.getstr();
    int result = 0;
    int sign = 1;
    int i = 0;
    if (str[0] == '-') { sign = -1; i = 1; }
    for (; str[i] != '\0'; ++i) {
        char c = str[i];
        if (c < '0' || c > '9') break;
        result = result * 10 + (c - '0');
    }
    return sign * result;
}

// Expand test cases array
void Problem::expandTests() {
    int newCap = (testCapacity == 0) ? 4 : testCapacity * 2;
    String* newInputs = new String[newCap];
    String* newOutputs = new String[newCap];
    for (int i = 0; i < testCount; ++i) {
        newInputs[i] = testInputs[i];
        newOutputs[i] = testOutputs[i];
    }
    delete[] testInputs;
    delete[] testOutputs;
    testInputs = newInputs;
    testOutputs = newOutputs;
    testCapacity = newCap;
}

// Constructors
Problem::Problem() : id(0), difficulty(1), testCount(0), testCapacity(0), testInputs(nullptr), testOutputs(nullptr) {}

Problem::Problem(int _id, int _diff, const String& _title, const String& _desc, const String& _cons)
    : id(_id), difficulty(_diff), title(_title), description(_desc), constraints(_cons),
      testCount(0), testCapacity(0), testInputs(nullptr), testOutputs(nullptr) {}

Problem::Problem(const Problem& other)
    : id(other.id), difficulty(other.difficulty), title(other.title), description(other.description),
      constraints(other.constraints), testCount(0), testCapacity(0), testInputs(nullptr), testOutputs(nullptr) {
    for (int i = 0; i < other.testCount; ++i) {
        addTestCase(other.testInputs[i], other.testOutputs[i]);
    }
}

Problem& Problem::operator=(const Problem& other) {
    if (this != &other) {
        id = other.id;
        difficulty = other.difficulty;
        title = other.title;
        description = other.description;
        constraints = other.constraints;
        delete[] testInputs;
        delete[] testOutputs;
        testInputs = testOutputs = nullptr;
        testCount = testCapacity = 0;
        for (int i = 0; i < other.testCount; ++i) {
            addTestCase(other.testInputs[i], other.testOutputs[i]);
        }
    }
    return *this;
}

Problem::~Problem() {
    delete[] testInputs;
    delete[] testOutputs;
}

// Test case management
void Problem::addTestCase(const String& input, const String& output) {
    if (testCount == testCapacity) expandTests();
    testInputs[testCount] = input;
    testOutputs[testCount] = output;
    testCount++;
}

int Problem::getTestCount() const { return testCount; }
String Problem::getInput(int idx) const {
    if (idx < 0 || idx >= testCount) return String("");
    return testInputs[idx];
}
String Problem::getOutput(int idx) const {
    if (idx < 0 || idx >= testCount) return String("");
    return testOutputs[idx];
}

// Getters
int Problem::getId() const { return id; }
int Problem::getDifficulty() const { return difficulty; }
String Problem::getTitle() const { return title; }
String Problem::getDescription() const { return description; }
String Problem::getConstraints() const { return constraints; }

// Serialization
String Problem::serialize() const {
    // Use String::fromInt (now available)
    String result = String::fromInt(id) + "|" + String::fromInt(difficulty) + "|" + title + "|" + description + "|" + constraints;
    for (int i = 0; i < testCount; ++i) {
        result = result + "|" + testInputs[i] + "|" + testOutputs[i];
    }
    return result;
}

// Deserialization
void Problem::deserialize(const String& data) {
    // Clear existing data
    delete[] testInputs;
    delete[] testOutputs;
    testInputs = testOutputs = nullptr;
    testCount = testCapacity = 0;

    int len = data.getlen();
    const char* str = data.getstr();
    int pos = 0;

    // Helper lambda to read a field until '|' or end
    auto readField = [&]() -> String {
        if (pos >= len) return String("");
        int start = pos;
        while (pos < len && str[pos] != '|') ++pos;
        int fieldLen = pos - start;
        // Manually copy the field into a char buffer
        char* buf = new char[fieldLen + 1];
        for (int i = 0; i < fieldLen; ++i) buf[i] = str[start + i];
        buf[fieldLen] = '\0';
        String field(buf);
        delete[] buf;
        if (pos < len && str[pos] == '|') ++pos; // skip delimiter
        return field;
    };

    String idStr = readField();
    String diffStr = readField();
    id = stringToInt(idStr);
    difficulty = stringToInt(diffStr);
    title = readField();
    description = readField();
    constraints = readField();

    // Read test cases in pairs
    while (pos < len) {
        String input = readField();
        String output = readField();
        if (input.isEmpty() && output.isEmpty()) break;
        addTestCase(input, output);
    }
}
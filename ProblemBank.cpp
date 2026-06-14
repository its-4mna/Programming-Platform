#include "ProblemBank.h"
#include "regFile.h"
#include "PersistentStorage.h"
#include <ctime>

// ---------- Problem Data ----------
struct ProblemEntry {
    int difficulty;          // 1, 2, or 3
    const char* title;
    const char* description;
    const char* constraints;
    const char* testInputs[3];
    const char* testOutputs[3];
    int testCount;
};

static ProblemEntry problemList[] = 
{
    // Difficulty 1 (Easy) – 10 problems
    {1, "Sum of Two Integers",
     "Given two integers a and b, print their sum.",
     "1 ≤ a, b ≤ 1000",
     {"2 3", "0 0", "100 200"}, {"5", "0", "300"}, 3},
    {1, "Even or Odd",
     "Given an integer n, print 'even' if n is even, else 'odd'.",
     "-10^9 ≤ n ≤ 10^9",
     {"4", "7", "0"}, {"even", "odd", "even"}, 3},
    {1, "Maximum of Three",
     "Given three integers, print the largest.",
     "-10^6 ≤ numbers ≤ 10^6",
     {"3 5 2", "10 10 5", "-1 -5 -3"}, {"5", "10", "-1"}, 3},
    {1, "Factorial",
     "Given n (0 ≤ n ≤ 10), compute n! (factorial).",
     "0 ≤ n ≤ 10",
     {"0", "5", "10"}, {"1", "120", "3628800"}, 3},
    {1, "Reverse a String",
     "Given a string (lowercase letters, no spaces), print its reverse.",
     "1 ≤ length ≤ 100",
     {"hello", "abc", "a"}, {"olleh", "cba", "a"}, 3},
    {1, "Count Vowels",
     "Given a string, count the number of vowels (a,e,i,o,u).",
     "1 ≤ length ≤ 100",
     {"hello", "xyz", "aeiou"}, {"2", "0", "5"}, 3},
    {1, "Simple Interest",
     "Given principal (P), rate (R), time (T) in years, print simple interest = P*R*T/100.",
     "1 ≤ P,R,T ≤ 1000",
     {"100 5 2", "500 10 3", "2000 7 5"}, {"10", "150", "700"}, 3},
    {1, "Area of Circle",
     "Given radius r, print area = 3.14159 * r * r (rounded to 2 decimals).",
     "1 ≤ r ≤ 100",
     {"5", "7", "10"}, {"78.54", "153.94", "314.16"}, 3},
    {1, "Leap Year",
     "Given a year, print 'leap' if it is a leap year, else 'not leap'.",
     "1 ≤ year ≤ 9999",
     {"2000", "1900", "2024"}, {"leap", "not leap", "leap"}, 3},
    {1, "Sum of Digits",
     "Given an integer n (non-negative), print the sum of its digits.",
     "0 ≤ n ≤ 10^6",
     {"123", "5", "1000"}, {"6", "5", "1"}, 3},

    // Difficulty 2 (Medium) – 10 problems
    {2, "Prime Check",
     "Given an integer n (>1), print 'prime' if it is prime, else 'composite'.",
     "2 ≤ n ≤ 10^6",
     {"7", "10", "13"}, {"prime", "composite", "prime"}, 3},
    {2, "Fibonacci Series",
     "Given n, print the first n Fibonacci numbers (space separated).",
     "1 ≤ n ≤ 20",
     {"5", "1", "10"}, {"0 1 1 2 3", "0", "0 1 1 2 3 5 8 13 21 34"}, 3},
    {2, "Palindrome Check",
     "Given a string (lowercase letters), print 'yes' if palindrome, else 'no'.",
     "1 ≤ length ≤ 100",
     {"madam", "hello", "racecar"}, {"yes", "no", "yes"}, 3},
    {2, "GCD of Two Numbers",
     "Given two positive integers a and b, print their greatest common divisor.",
     "1 ≤ a,b ≤ 10^6",
     {"12 18", "7 13", "100 25"}, {"6", "1", "25"}, 3},
    {2, "Linear Search",
     "Given an array of integers and a target, print index (0-based) if found, else -1.",
     "Array size ≤ 10",
     {"3\n1 2 3\n2", "3\n10 20 30\n25", "4\n1 1 1 1\n1"}, {"1", "-1", "0"}, 3},
    {2, "Bubble Sort",
     "Given an array of integers, print them sorted in ascending order.",
     "Size ≤ 10",
     {"4\n4 3 2 1", "3\n5 1 3", "2\n2 1"}, {"1 2 3 4", "1 3 5", "1 2"}, 3},
    {2, "Matrix Addition",
     "Given two 2x2 matrices, print their sum.",
     "Values between -100 and 100",
     {"1 2\n3 4\n5 6\n7 8", "0 0\n0 0\n1 1\n1 1", "1 1\n1 1\n2 2\n2 2"},
     {"6 8\n10 12", "1 1\n1 1", "3 3\n3 3"}, 3},
    {2, "Second Largest",
     "Given three integers, print the second largest.",
     "-10^5 ≤ numbers ≤ 10^5",
     {"5 2 8", "1 1 2", "10 20 15"}, {"5", "1", "15"}, 3},
    {2, "Power Calculation",
     "Given base b and exponent e (non-negative), compute b^e.",
     "1 ≤ b ≤ 10, 0 ≤ e ≤ 10",
     {"2 3", "5 0", "10 2"}, {"8", "1", "100"}, 3},
    {2, "String Length",
     "Given a string (no spaces), print its length.",
     "1 ≤ length ≤ 1000",
     {"hello", "", "abcde"}, {"5", "0", "5"}, 3},

    // Difficulty 3 (Hard) – 10 problems
    {3, "Binary Search",
     "Given a sorted array and a target, return index (0-based) or -1.",
     "Size ≤ 10^5",
     {"5\n1 3 5 7 9\n5", "4\n2 4 6 8\n3", "3\n1 1 1\n1"}, {"2", "-1", "0"}, 3},
    {3, "Merge Two Sorted Arrays",
     "Given two sorted arrays, print merged sorted array.",
     "Size ≤ 10",
     {"3\n1 3 5\n3\n2 4 6", "2\n1 2\n2\n3 4", "1\n5\n1\n6"},
     {"1 2 3 4 5 6", "1 2 3 4", "5 6"}, 3},
    {3, "Longest Common Prefix",
     "Given two strings, print the longest common prefix.",
     "Length ≤ 100",
     {"hello her", "programming progressive", "abc def"},
     {"he", "progr", ""}, 3},
    {3, "Subarray Sum Equals K",
     "Given an array and k, print number of subarrays with sum = k.",
     "Size ≤ 10, values -10 to 10",
     {"4\n1 1 1 1\n2", "3\n1 2 3\n3", "5\n0 0 0 0 0\n0"}, {"3", "2", "15"}, 3},
    {3, "Rotate Array",
     "Given an array and k, rotate right by k steps.",
     "Size ≤ 10",
     {"5\n1 2 3 4 5\n2", "3\n1 2 3\n1", "4\n1 2 3 4\n4"},
     {"4 5 1 2 3", "3 1 2", "1 2 3 4"}, 3},
    {3, "Anagram Check",
     "Given two strings, print 'yes' if they are anagrams, else 'no'.",
     "Length ≤ 100, lowercase",
     {"listen silent", "hello world", "ab ba"},
     {"yes", "no", "yes"}, 3},
    {3, "First Non-Repeating Character",
     "Given a string, print the first character that does not repeat, else '-'.",
     "Length ≤ 100",
     {"swiss", "aabbcc", "abcd"}, {"w", "-", "a"}, 3},
    {3, "Climbing Stairs",
     "Given n stairs, you can climb 1 or 2 steps at a time. Print number of distinct ways to reach the top.",
     "1 ≤ n ≤ 45",
     {"1", "2", "5"}, {"1", "2", "8"}, 3},
    {3, "Maximum Subarray Sum (Kadane)",
     "Given an array of integers (may be negative), print the maximum sum of a contiguous subarray.",
     "Size ≤ 10",
     {"5\n-2 1 -3 4 -1 2 1 -5 4", "3\n-1 -2 -3", "4\n1 2 3 4"},
     {"6", "-1", "10"}, 3},
    {3, "Generate Parentheses",
     "Given n pairs of parentheses, print all valid combinations (space separated).",
     "1 ≤ n ≤ 3",
     {"1", "2", "3"},
     {"()", "()() (())", "((())) (()()) (())() ()(()) ()()()"}, 3}
};

const int TOTAL_PROBLEMS = sizeof(problemList) / sizeof(ProblemEntry);

// ---------- ProblemBank Implementation ----------
void ProblemBank::expand() {
    int newCap = (capacity == 0) ? 16 : capacity * 2;
    Problem** newArr = new Problem*[newCap];
    for (int i = 0; i < problemCount; ++i) newArr[i] = problems[i];
    delete[] problems;
    problems = newArr;
    capacity = newCap;
}

ProblemBank::ProblemBank() : problems(nullptr), problemCount(0), capacity(0) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

ProblemBank::~ProblemBank() {
    for (int i = 0; i < problemCount; ++i) delete problems[i];
    delete[] problems;
}

void ProblemBank::seedToVFS(Directory* problemsDir, User* owner, const permissions& perm) {
    if (!problemsDir) return;

    // Create difficulty subdirectories (1..3)
    Directory* diffDirs[4] = {nullptr, nullptr, nullptr, nullptr};
    for (int diff = 1; diff <= 3; ++diff) {
        String diffName = String("difficulty") + String::fromInt(diff);
        diffDirs[diff] = dynamic_cast<Directory*>(problemsDir->findChild(diffName));
        if (!diffDirs[diff]) {
            diffDirs[diff] = new Directory(diffName, owner, perm);
            problemsDir->addChild(diffDirs[diff]);
        }
    }

    // Iterate through all problems and write them to the correct difficulty folder
    for (int idx = 0; idx < TOTAL_PROBLEMS; ++idx) {
        const ProblemEntry& entry = problemList[idx];
        int diff = entry.difficulty;
        String probName = String("problem_") + String::fromInt(idx + 1);

        // Avoid duplicate if already exists
        if (diffDirs[diff]->findChild(probName)) continue;

        Problem prob(idx + 1, diff,
                    String(entry.title),
                    String(entry.description),
                    String(entry.constraints));
        for (int t = 0; t < entry.testCount; ++t) {
            prob.addTestCase(String(entry.testInputs[t]), String(entry.testOutputs[t]));
        }

        String content = prob.serialize();
        regFile* rf = new regFile(probName, owner, perm);
        rf->setContent(content.getstr(), content.getlen());
        diffDirs[diff]->addChild(rf);
    }
}

void ProblemBank::loadFromVFS(Directory* problemsDir) {
    if (!problemsDir) return;
    for (int diff = 1; diff <= 3; ++diff) {
        String diffName = String("difficulty") + String::fromInt(diff);
        Directory* diffDir = dynamic_cast<Directory*>(problemsDir->findChild(diffName));
        if (!diffDir) continue;
        for (int i = 0; i < diffDir->getChildCount(); ++i) {
            regFile* rf = dynamic_cast<regFile*>(diffDir->getChild(i));
            if (!rf) continue;
            int size = rf->getSize();
            char* buf = new char[size + 1];
            rf->read(buf, size, 0);
            buf[size] = '\0';
            Problem* p = new Problem();
            p->deserialize(String(buf));
            delete[] buf;
            if (problemCount == capacity) expand();
            problems[problemCount++] = p;
        }
    }
}

Problem* ProblemBank::getProblem(int index) const {
    if (index < 0 || index >= problemCount) return nullptr;
    return problems[index];
}

int ProblemBank::getProblemCount() const { return problemCount; }

int* ProblemBank::selectProblems(const ContestGroup* group, int count) const {
    int* dist = group->getDifficultyDistribution(); // array of 3 ints (for diff1,diff2,diff3)
    int* selected = new int[count];
    int idx = 0;

    for (int diff = 1; diff <= 3; ++diff) {
        int required = (count * dist[diff-1]) / 100;
        if (required == 0) continue;
        // Collect indices of problems with this difficulty
        int* candidates = new int[problemCount];
        int candCount = 0;
        for (int i = 0; i < problemCount; ++i) {
            if (problems[i]->getDifficulty() == diff)
                candidates[candCount++] = i;
        }
        int take = (required < candCount) ? required : candCount;
        // Randomly pick 'take' distinct candidates (simple shuffle)
        for (int r = 0; r < take; ++r) {
            int randPos = std::rand() % (candCount - r);
            selected[idx++] = candidates[randPos];
            // swap to avoid reuse
            int temp = candidates[randPos];
            candidates[randPos] = candidates[candCount - r - 1];
            candidates[candCount - r - 1] = temp;
        }
        delete[] candidates;
        if (idx >= count) break;
    }
    // If still not enough (due to rounding), fill with random problems
    while (idx < count) {
        selected[idx++] = std::rand() % problemCount;
    }
    return selected;
}
#ifndef CONTESTGROUP_H
#define CONTESTGROUP_H

#include "Strings.h"

// yehah contest groups ko define karegi means easy , medium hard or total problems per contest batana 


class ContestGroup 

// defines a contrct means har group ka name  , total problems 
// , or get difficulty distribution de ga 

{
    public:
        virtual ~ContestGroup() 
        {
            // bass sirf destructor or koi kaam nahi iss ka 
        }
        virtual int* getDifficultyDistribution() const = 0; // array of 3 ints 
        virtual String getName() const = 0;
        virtual int getTotalProblems() const = 0; // explain karta hai k aik group mein 
        // contest mein kitne problems hon gy 
};

// now we make 3 derive classes for problems distributions
//unki getDifficultyDistribution method array return karti hai 
// return three integers 1,2,3 based on difficulty


class EasyContestGroup : public ContestGroup 
{
    public:
        int* getDifficultyDistribution() const 
        {
            static int dist[] = {60, 30, 10}; // 60% diff1, 30% diff2, 10% diff3
            return dist;
        }
        String getName() const 
        { 
            return String("Easy"); 
        }
        int getTotalProblems() const 
        { 
            return 5; 
        }
};

class MediumContestGroup : public ContestGroup 
{
    public:
        int* getDifficultyDistribution() const 
        {
            static int dist[] = {25, 50, 25};
            return dist;
        }
        String getName() const 
        { 
            return String("Medium");
        }
        int getTotalProblems() const 
        { 
            return 5; 
        }
};

class HardContestGroup : public ContestGroup 
{
    public:
        int* getDifficultyDistribution() const 
        {
            static int dist[] = {10, 30, 60};
            return dist;
        }
        String getName() const 
        { 
            return String("Hard"); 
        }
        int getTotalProblems() const 
        { 
            return 5; 
        }
};

#endif

// iss mein hum sirf hedder file hi banain gy no .cpp needed 
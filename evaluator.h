#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "Strings.h"

class Evaluator {
private:
    String tempDir; //stores path to temporary folder
    
public:
    Evaluator();
    ~Evaluator();
    
    //returns the status, like what the compiler said after looking at the code
    String evaluate(const String& code, const String& input, const String& expectedOutput);
};

#endif
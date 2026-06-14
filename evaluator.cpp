#include "evaluator.h"

#include <fstream>
#include <cstdlib>

Evaluator::Evaluator()
{
    tempDir = "eval/";
    system("mkdir -p eval");    //the p means k if it already exists phir bhi theek hai koi masla nai

}

//im cleaning up everything but this is only for linux
Evaluator::~Evaluator()
{
    system("rm -rf eval");  //dont try this at windows kids
}

String Evaluator::evaluate(const String& code, const String& input, const String& expectedOutput)
{
    //writing users code to a temp file i nthe folder i created uper
    String codePath = tempDir + "solution.cpp"; //ye ho jaye ga eval/solution.cpp
    std::ofstream codeFile(codePath.getstr());
    codeFile << code.getstr();
    codeFile.close();

    //writing the inputs in a file
    String inputPath = tempDir + "input.txt";
    ofstream inputFile(inputPath.getstr());
    inputFile << input.getstr();
    inputFile.close();

    //comipiling w a system call
    //String cmd = "g++ " + codePath + " -o " + tempDir + "solution 2> " + tempDir + "error.txt";
    //int compile = system(cmd.getstr());
    
    //compile ki command bcz idek why uper wala isnt working
    String cmd = "g++ ";
    cmd += codePath;
    cmd+= " -o ";
    cmd += tempDir;
    cmd += "solution 2> ";
    cmd += tempDir;
    cmd += "error.txt";

    int compile = system(cmd.getstr());


    if(compile != 0)
        return "CE";    //compile error

    //if compiled correctly then ab usko run krna hai
    String runCmd = tempDir;
    runCmd = runCmd + "solution < ";
    runCmd = runCmd + inputPath;
    runCmd = runCmd + " > ";
    runCmd = runCmd + tempDir;
    runCmd = runCmd + "output.txt 2> /dev/null";

    int run = system(runCmd.getstr());

    if(run != 0)
        return "RE";    //run time error

    //agar run bhi sai ho gaya, then read the output file and check the code
    String outputPath = tempDir + "output.txt";     //saving the output jo bhi ayi in a file
    std::ifstream outputFile(outputPath.getstr());

    //reading that output and storing it in a string
    String actualOutput;
    char ch;
    while (outputFile.get(ch)) 
    {
        char temp[2] = {ch, '\0'};
        actualOutput = actualOutput + String(temp);
    }
    outputFile.close();    

    if (actualOutput == expectedOutput) 
        return "AC";    //Accepted as in sai hai
    else 
        return "WA";    //Wrong Answer
}
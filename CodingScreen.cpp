#include "CodingScreen.h"
#include "problemScreen.h"
#include "dashboard.h"
#include "Strings.h"
#include "evaluator.h"
#include "PersistentStorage.h"
#include <iostream>
#include "ContestScreen.h"
#include "ContestManager.h"
#include "ResultScreen.h"


codingScreen::codingScreen(ScreenManager& manager, sf::Font& f, String user, String problem, int diff, int index, 
                           UserManager& um, SessionManager& sm, Problem* prob, ProblemBank* pb, bool contestMode) 
    : SM(manager), font(f), userManager(um), sessionManager(sm), currentUser(user), 
      problemTitleStr(problem), difficulty(diff), problemIndex(index), contestSlot(-1), alreadySolved(false),
      currentProblem(prob), practiceStartTime(0), problembank(pb), isContestMode(contestMode), ownerContestScreen(nullptr)
{
    // Initialize popup variables
    showResultPopup = false;
    showExitConfirm = false;
    lastVerdict = "";
    timeElapsed = 0;
    
    if(currentProblem) {
        problemDescription = currentProblem->getDescription();
        constraints = currentProblem->getConstraints();
    }

    // ========== TIMER SETUP ==========
    if (isContestMode) {
        timeRemaining = 4 * 3600;
        timerActive = true;
    } else {
        timeRemaining = 0;
        timerActive = true;
    }
    
    contestClock.restart();

    // ========== UI SETUP ==========
    background.setFillColor(sf::Color(240, 235, 240));
    background.setSize({1280, 720});
    
    headerPanel.setFillColor(sf::Color(170, 70, 120));
    headerPanel.setSize({1280, 70});
    headerPanel.setPosition(0, 0);
    
    leftPanel.setFillColor(sf::Color(200, 100, 150));
    leftPanel.setOutlineColor(sf::Color(28, 30, 45));
    leftPanel.setOutlineThickness(2);
    leftPanel.setSize({380, 630});
    leftPanel.setPosition(20, 80);
    
    rightPanel.setFillColor(sf::Color(200, 100, 150));
    rightPanel.setOutlineColor(sf::Color(28, 30, 45));
    rightPanel.setOutlineThickness(2);
    rightPanel.setSize({850, 630});
    rightPanel.setPosition(410, 80);
    
    backButton = new button(20, 15, 100, 40, "BACK", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    
    problemTitleText.setFont(font);
    problemTitleText.setString((String("PROBLEM: ") + problemTitleStr).getstr());
    problemTitleText.setCharacterSize(20);
    problemTitleText.setFillColor(sf::Color(255, 255, 255));
    problemTitleText.setPosition(140, 22);
    
    modeText.setFont(font);
    modeText.setCharacterSize(18);
    modeText.setFillColor(sf::Color(255, 200, 150));
    modeText.setPosition(730, 24);
    if (isContestMode) {
        modeText.setString("CONTEST");
    } else {
        modeText.setString("PRACTICE");
    }
    
    timerText.setFont(font);
    timerText.setCharacterSize(26);
    timerText.setFillColor(sf::Color(255, 220, 100));
    timerText.setPosition(840, 18);
    timerText.setString("00:00");
    
    submitButton = new button(1130, 12, 90, 46, "SUBMIT", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    clearButton = new button(1030, 12, 90, 46, "CLEAR", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    saveButton = new button(930, 12, 90, 46, "SAVE", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    
    // ========== PROBLEM DESCRIPTION SECTION ==========
    descriptionTitle.setFont(font);
    descriptionTitle.setString("DESCRIPTION");
    descriptionTitle.setCharacterSize(20);
    descriptionTitle.setFillColor(sf::Color(255, 255, 255));
    descriptionTitle.setPosition(35, 100);
    
    descriptionText.setFont(font);
    String shortDesc = problemDescription;
    if (shortDesc.getlen() > 200) {
        shortDesc = shortDesc.substring(0, 200);
    }
    descriptionText.setString(shortDesc.getstr());
    descriptionText.setCharacterSize(14);
    descriptionText.setFillColor(sf::Color(255, 255, 255));
    descriptionText.setPosition(35, 140);
    
    // Input format
    inputTitle.setFont(font);
    inputTitle.setString("INPUT FORMAT");
    inputTitle.setCharacterSize(16);
    inputTitle.setFillColor(sf::Color(255, 255, 255));
    inputTitle.setPosition(35, 280);
    
    inputText.setFont(font);
    inputText.setString("Read input as described in the problem\nstatement above.");
    inputText.setCharacterSize(14);
    inputText.setFillColor(sf::Color(255, 255, 255));
    inputText.setPosition(35, 310);
    
    // Output format
    outputTitle.setFont(font);
    outputTitle.setString("OUTPUT FORMAT");
    outputTitle.setCharacterSize(16);
    outputTitle.setFillColor(sf::Color(255, 255, 255));
    outputTitle.setPosition(35, 390);
    
    outputText.setFont(font);
    outputText.setString("Print output exactly as shown in the example.");
    outputText.setCharacterSize(14);
    outputText.setFillColor(sf::Color(255, 255, 255));
    outputText.setPosition(35, 420);
    
    // Constraints
    constraintsTitle.setFont(font);
    constraintsTitle.setString("CONSTRAINTS");
    constraintsTitle.setCharacterSize(16);
    constraintsTitle.setFillColor(sf::Color(255, 255, 255));
    constraintsTitle.setPosition(35, 500);
    
    constraintsText.setFont(font);
    String shortConstraints = constraints;
    if (shortConstraints.getlen() > 150) {
        shortConstraints = shortConstraints.substring(0, 150);
    }
    constraintsText.setString(shortConstraints.getstr());
    constraintsText.setCharacterSize(14);
    constraintsText.setFillColor(sf::Color(255, 255, 255));
    constraintsText.setPosition(35, 530);
    
    // Test Cases button
    testCasesButton = new button(35, 630, 150, 40, "TEST CASES", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    
    // ========== CODE EDITOR ==========
    String templateCode = "// Write your C++ solution here\n\n#include <iostream>\n#include <vector>\n#include <string>\nusing namespace std;\n\nvoid solve() {\n    // Write your code here\n    int a, b;\n    cin >> a >> b;\n    cout << a + b << endl;\n}\n\nint main() {\n    solve();\n    return 0;\n}";
    
    codeEditor = new CodeEditor(425, 95, 820, 600, templateCode.getstr(), font);
    
    loadTimerState();

    // ========== LOAD SAVED CODE ==========
    String safeTitle = problemTitleStr;
    char* titleStr = const_cast<char*>(safeTitle.getstr());
    for (int i = 0; titleStr[i] != '\0'; i++) {
        if (titleStr[i] == ' ') titleStr[i] = '_';
    }
    
    String loadPath = String("users/") + currentUser + String("/saved_solutions/") + safeTitle + String(".cpp");
    
    if (PersistentStorage::fileExists(loadPath)) {
        char buffer[65536];
        int bytes = PersistentStorage::loadFile(loadPath, buffer, sizeof(buffer) - 1);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            String savedCode(buffer);
            codeEditor->setText(savedCode);
        }
    }
    
    message = new MessageLabel(font, 500, 740, 14);
    
    // ========== RESULT POPUP ==========
    popupBg.setFillColor(sf::Color(0, 0, 0, 200));
    popupBg.setSize({1280, 720});
    
    popupPanel.setFillColor(sf::Color(200, 100, 150));
    popupPanel.setOutlineThickness(2);
    popupPanel.setOutlineColor(sf::Color(50, 60, 100));
    popupPanel.setSize({450, 220});
    popupPanel.setPosition(415, 250);
    
    popupTitle.setFont(font);
    popupTitle.setCharacterSize(32);
    popupTitle.setFillColor(sf::Color(255, 255, 255));
    popupTitle.setPosition(420, 290);
    
    popupMessage.setFont(font);
    popupMessage.setCharacterSize(16);
    popupMessage.setFillColor(sf::Color(240, 230, 245));
    popupMessage.setPosition(430, 340);
    
    popupOkButton = new button(565, 400, 150, 50, "OK", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    
    // ========== EXIT CONFIRMATION POPUP ==========
    confirmBg.setFillColor(sf::Color(0, 0, 0, 200));
    confirmBg.setSize({1280, 720});
    
    confirmPanel.setFillColor(sf::Color(200, 100, 150));
    confirmPanel.setOutlineThickness(2);
    confirmPanel.setOutlineColor(sf::Color(50, 60, 100));
    confirmPanel.setSize({450, 200});
    confirmPanel.setPosition(415, 260);
    
    confirmTitle.setFont(font);
    confirmTitle.setCharacterSize(24);
    confirmTitle.setFillColor(sf::Color(255, 255, 255));
    confirmTitle.setString("UNSAVED CHANGES");
    confirmTitle.setPosition(510, 300);
    
    confirmMessage.setFont(font);
    confirmMessage.setCharacterSize(15);
    confirmMessage.setFillColor(sf::Color(240, 230, 245));
    confirmMessage.setString("Save your code before leaving?");
    confirmMessage.setPosition(490, 350);
    
    confirmYes = new button(490, 400, 150, 46, "SAVE & EXIT", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    confirmNo = new button(660, 400, 150, 46, "EXIT ANYWAY", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
}

void codingScreen::update(float mouseX, float mouseY)
{
    backButton->update(mouseX, mouseY);
    submitButton->update(mouseX, mouseY);
    clearButton->update(mouseX, mouseY);
    saveButton->update(mouseX, mouseY);
    testCasesButton->update(mouseX, mouseY);
    
    if(showResultPopup) {
        popupOkButton->update(mouseX, mouseY);
    }
    
    if(showExitConfirm) {
        confirmYes->update(mouseX, mouseY);
        confirmNo->update(mouseX, mouseY);
    }
    
    codeEditor->update(mouseX, mouseY);
    updateTimerDisplay();
}

void codingScreen::render(sf::RenderWindow& window)
{
    window.draw(background);
    window.draw(headerPanel);
    window.draw(leftPanel);
    window.draw(rightPanel);
    
    backButton->render(window);
    window.draw(problemTitleText);
    window.draw(modeText);
    window.draw(timerText);
    submitButton->render(window);
    clearButton->render(window);
    saveButton->render(window);
    
    window.draw(descriptionTitle);
    window.draw(descriptionText);
    window.draw(inputTitle);
    window.draw(inputText);
    window.draw(outputTitle);
    window.draw(outputText);
    window.draw(constraintsTitle);
    window.draw(constraintsText);
    testCasesButton->render(window);
    
    codeEditor->render(window);
    message->render(window);
    
    if(showResultPopup) {
        window.draw(popupBg);
        window.draw(popupPanel);
        window.draw(popupTitle);
        window.draw(popupMessage);
        popupOkButton->render(window);
    }
    
    if(showExitConfirm) {
        window.draw(confirmBg);
        window.draw(confirmPanel);
        window.draw(confirmTitle);
        window.draw(confirmMessage);
        confirmYes->render(window);
        confirmNo->render(window);
    }
}

void codingScreen::handleEvent(sf::Event& event, sf::RenderWindow& window)
{
    // If result popup is showing, only handle it
    if(showResultPopup) {
        if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            float mx = event.mouseButton.x;
            float my = event.mouseButton.y;
            if(popupOkButton->isClicked(mx, my)) {
                showResultPopup = false;
                
                if(isContestMode) {
                    SM.setScreen(new ContestScreen(SM, font, currentUser, 0, 0, userManager, sessionManager, problembank));
                } else {
                    SM.setScreen(new problemScreen(SM, font, currentUser, difficulty, userManager, sessionManager, problembank));
                }
            }
        }
        return;
    }
    
    // If exit confirmation is showing, only handle it
    if(showExitConfirm) 
    {
        if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            float mx = event.mouseButton.x;
            float my = event.mouseButton.y;
            
            if(confirmYes->isClicked(mx, my)) {
                String userCode = codeEditor->getText();
                String safeTitle = problemTitleStr;
                char* titleStr = const_cast<char*>(safeTitle.getstr());
                for (int i = 0; titleStr[i] != '\0'; i++) {
                    if (titleStr[i] == ' ') titleStr[i] = '_';
                }
                String savePath = String("users/") + currentUser + String("/saved_solutions/") + safeTitle + String(".cpp");
                String dirPath = String("users/") + currentUser + String("/saved_solutions");
                PersistentStorage::makeDirectory(dirPath);
                PersistentStorage::saveFile(savePath, userCode.getstr(), userCode.getlen());
                
                showExitConfirm = false;
                
                if(isContestMode)
                    SM.setScreen(new ContestScreen(SM, font, currentUser, 0, 0, userManager, sessionManager, problembank));
                else
                    SM.setScreen(new problemScreen(SM, font, currentUser, difficulty, userManager, sessionManager, problembank));
                return;
            }
            else if(confirmNo->isClicked(mx, my)) {
                showExitConfirm = false;
                
                if(isContestMode)
                    SM.setScreen(new ContestScreen(SM, font, currentUser, 0, 0, userManager, sessionManager, problembank));
                else
                    SM.setScreen(new problemScreen(SM, font, currentUser, difficulty, userManager, sessionManager, problembank));
                return;
            }
        }
        return;
    }
    
    // Normal event handling
    if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        float mx = event.mouseButton.x;
        float my = event.mouseButton.y;
        
        codeEditor->handleClick(mx, my);
        
        if(backButton->isClicked(mx, my))
        {
            saveTimerState(); 
            if (!codeEditor->isEmpty()) 
            {
                showExitConfirm = true;
            } 
            else 
            {
                if(isContestMode)
                    SM.setScreen(new ContestScreen(SM, font, currentUser, 0, 0, userManager, sessionManager, problembank));
                else
                    SM.setScreen(new problemScreen(SM, font, currentUser, difficulty, userManager, sessionManager, problembank));
            }
            return;
        }
        
        if(clearButton->isClicked(mx, my))
        {
            codeEditor->clear();
            message->showSuccess("Editor cleared!", 1);
            return;
        }
        
        if(saveButton->isClicked(mx, my))
        {
            saveTimerState();

            String userCode = codeEditor->getText();
            
            String safeTitle = problemTitleStr;
            char* titleStr = const_cast<char*>(safeTitle.getstr());
            for (int i = 0; titleStr[i] != '\0'; i++) {
                if (titleStr[i] == ' ') titleStr[i] = '_';
            }
            
            String savePath = String("users/") + currentUser + String("/saved_solutions/") + safeTitle + String(".cpp");
            String dirPath = String("users/") + currentUser + String("/saved_solutions");
            
            PersistentStorage::makeDirectory(dirPath);
            bool saved = PersistentStorage::saveFile(savePath, userCode.getstr(), userCode.getlen());
            
            if (saved) {
                message->showSuccess("Code saved!", 2);
            } else {
                message->showError("Save failed!", 2);
            }
            return;
        }
        
        if(submitButton->isClicked(mx, my))
        {
            // Check if already solved
            if (alreadySolved) {
                message->showError("You already solved this problem!", 2);
                return;
            }
            
            String userCode = codeEditor->getText();

            if(userCode.getlen() < 20)
            {
                message->showError("Write some code first!", 2);
                return;
            }
            
            String fullCode = userCode;

            if(currentProblem)
            {
                Evaluator eval;
                bool allPassed = true;
                String lastError = "";

                for(int i = 0; i < currentProblem->getTestCount(); i++)
                {
                    String input = currentProblem->getInput(i);
                    String expected = currentProblem->getOutput(i);
                    String judgement = eval.evaluate(fullCode, input, expected);

                    if(judgement != "AC")
                    {
                        allPassed = false;
                        lastError = judgement;
                        break;
                    }
                }

                if(allPassed)
                {
                    alreadySolved = true;
                    
                    int points = 0;
                    if (difficulty == 1) points = 100;
                    else if (difficulty == 2) points = 200;
                    else points = 300;
                    
                    User* user = userManager.getUserByUsername(currentUser);
                    if (user) 
                    {
                        user->addScore(points);
                        user->addSolvedProblem();
                        
                        UserPersistence::saveUser(currentUser, user->getEmail(), user->getPassword(), 
                                                  user->getVerified(), user->getTotalScore(), user->getSolvedCount());
                        
                        // Save to solved problems list
                        String solvedPath = String("users/") + currentUser + String("/solved_problems.txt");
                        String problemIdStr = String::fromInt(currentProblem->getId());
                        if (PersistentStorage::fileExists(solvedPath)) {
                            char solvedBuf[4096];
                            int bytes = PersistentStorage::loadFile(solvedPath, solvedBuf, sizeof(solvedBuf) - 1);
                            if (bytes > 0) {
                                solvedBuf[bytes] = '\0';
                                String existing(solvedBuf);
                                if (!existing.contains(problemIdStr)) {
                                    String newData = existing + problemIdStr + "\n";
                                    PersistentStorage::saveFile(solvedPath, newData.getstr(), newData.getlen());
                                }
                            }
                        } else {
                            PersistentStorage::saveFile(solvedPath, (problemIdStr + "\n").getstr(), (problemIdStr + "\n").getlen());
                        }
                    }
                    
                    if (isContestMode && ownerContestScreen) 
                    {
                        ownerContestScreen->markProblemSolved(problemIndex);
                        popupTitle.setString("PROBLEM SOLVED!");
                        popupTitle.setFillColor(sf::Color(100, 200, 100));
                        popupMessage.setString("Returning to contest...");
                        showResultPopup = true;
                    }
                    else
                    {
                        SM.setScreen(new ResultScreen(SM, font, currentUser, userManager, sessionManager, problembank, problemTitleStr, points));
                    }
                    return;
                }
                else
                {
                    if(lastError == "WA") {
                        popupTitle.setString("WRONG ANSWER");
                        popupTitle.setFillColor(sf::Color(255, 255, 255));
                        popupMessage.setString("Your output didn't match\nthe expected output.");
                    }
                    else if(lastError == "CE") {
                        popupTitle.setString("COMPILATION ERROR");
                        popupTitle.setFillColor(sf::Color(255, 255, 255));
                        popupMessage.setString("Check your code for syntax\nerrors and try again.");
                    }
                    else if(lastError == "RE") {
                        popupTitle.setString("RUNTIME ERROR");
                        popupTitle.setFillColor(sf::Color(255, 255, 255));
                        popupMessage.setString("Your program crashed.\nCheck for bugs.");
                    }
                    else {
                        popupTitle.setString("ERROR");
                        popupTitle.setFillColor(sf::Color(255, 255, 255));
                        popupMessage.setString("An unknown error occurred.");
                    }
                    showResultPopup = true;
                }
            }
            else
            {
                message->showError("Problem not found", 2);
            }
            return;
        }
        
        if(testCasesButton->isClicked(mx, my))
        {
            if (currentProblem && currentProblem->getTestCount() > 0) {
                String sampleInput = currentProblem->getInput(0);
                String sampleOutput = currentProblem->getOutput(0);
                String msg = String("Sample Input: ") + sampleInput + String("\nSample Output: ") + sampleOutput;
                message->showSuccess(msg.getstr(), 4);
            } else {
                message->showError("No test cases available", 2);
            }
            return;
        }
    }
    
    // Handle keyboard input for code editor
    if(event.type == sf::Event::TextEntered)
    {
        unsigned int unicode = event.text.unicode;
        
        if(unicode == 8) {
            if(codeEditor->isSelected())
                codeEditor->handleBackspace();
        }
        else if(unicode == 9) {
            if(codeEditor->isSelected())
                codeEditor->handleTab();
        }
        else if(unicode == 13) {
            if(codeEditor->isSelected())
                codeEditor->handleEnter();
        }
        else if(unicode >= 32 && unicode < 127) {
            char c = static_cast<char>(unicode);
            if(codeEditor->isSelected())
                codeEditor->handleTypedChar(c);
        }
    }
    
    // Handle arrow keys
    if(event.type == sf::Event::KeyPressed)
    {
        if(event.key.code == sf::Keyboard::Up) {
            if(codeEditor->isSelected())
                codeEditor->handleArrowUp();
        }
        else if(event.key.code == sf::Keyboard::Down) {
            if(codeEditor->isSelected())
                codeEditor->handleArrowDown();
        }
        else if(event.key.code == sf::Keyboard::Left) {
            if(codeEditor->isSelected())
                codeEditor->handleArrowLeft();
        }
        else if(event.key.code == sf::Keyboard::Right) {
            if(codeEditor->isSelected())
                codeEditor->handleArrowRight();
        }
    }
}

codingScreen::~codingScreen()
{
    delete backButton;
    delete submitButton;
    delete clearButton;
    delete saveButton;
    delete testCasesButton;
    delete codeEditor;
    delete message;
    delete popupOkButton;
    delete confirmYes;
    delete confirmNo;
    saveTimerState();
}

String codingScreen::getTimerSavePath() const {
    String safeTitle = problemTitleStr;
    char* titleStr = const_cast<char*>(safeTitle.getstr());
    for (int i = 0; titleStr[i] != '\0'; i++) {
        if (titleStr[i] == ' ') titleStr[i] = '_';
    }
    return String("users/") + currentUser + String("/timer_") + safeTitle + String(".dat");
}

void codingScreen::saveTimerState() {
    String path = getTimerSavePath();
    
    if (isContestMode) {
        int elapsed = (int)contestClock.getElapsedTime().asSeconds();
        timerData.contestRemainingSeconds = timeRemaining - elapsed;
        if (timerData.contestRemainingSeconds < 0) timerData.contestRemainingSeconds = 0;
        timerData.contestStartTime = time(nullptr) - elapsed;
    } else {
        timerData.timeSpentSeconds = timeElapsed;
    }
    
    String data = timerData.serialize();
    PersistentStorage::saveFile(path, data.getstr(), data.getlen());
}

void codingScreen::loadTimerState() {
    String path = getTimerSavePath();
    
    if (!PersistentStorage::fileExists(path)) {
        return;
    }
    
    char buffer[512];
    int bytes = PersistentStorage::loadFile(path, buffer, sizeof(buffer) - 1);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        timerData.deserialize(String(buffer));
        
        if (isContestMode) {
            int now = (int)time(nullptr);
            int elapsed = now - timerData.contestStartTime;
            timeRemaining = timerData.contestRemainingSeconds - elapsed;
            if (timeRemaining < 0) timeRemaining = 0;
        } else {
            timeElapsed = timerData.timeSpentSeconds;
        }
    }
}

void codingScreen::updateTimerDisplay()
{
    if(!timerActive) return;
    
    if (isContestMode) {
        int now = (int)time(nullptr);
        int elapsed = now - timerData.contestStartTime;
        int remaining = timerData.contestRemainingSeconds - elapsed;
        
        if (remaining <= 0) {
            timerActive = false;
            remaining = 0;
            message->showError("TIME'S UP!", 3);
        }
        
        int minutes = remaining / 60;
        int seconds = remaining % 60;
        
        char timeStr[20];
        snprintf(timeStr, sizeof(timeStr), "%02d:%02d", minutes, seconds);
        timerText.setString(timeStr);
        
        if (remaining < 60)
            timerText.setFillColor(sf::Color(255, 50, 50));
        else if (remaining < 300)
            timerText.setFillColor(sf::Color(255, 150, 50));
        else
            timerText.setFillColor(sf::Color(255, 220, 100));
    } 
    else {
        if (practiceStartTime == 0) {
            practiceStartTime = (int)time(nullptr);
        }
        
        int now = (int)time(nullptr);
        int sessionElapsed = now - practiceStartTime;
        int totalTime = timerData.timeSpentSeconds + sessionElapsed;
        
        int minutes = totalTime / 60;
        int seconds = totalTime % 60;
        
        char timeStr[20];
        snprintf(timeStr, sizeof(timeStr), "%02d:%02d", minutes, seconds);
        timerText.setString(timeStr);
        timerText.setFillColor(sf::Color(150, 150, 180));
    }
}


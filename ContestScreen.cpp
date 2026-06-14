#include "ContestScreen.h"
#include "dashboard.h"
#include "CodingScreen.h"
#include "ContestData.h"
#include "ResultScreen.h"
#include <cstdio>

ContestScreen::ContestScreen(ScreenManager& manager, sf::Font& f, String user, int cType, int cIndex,UserManager& um, SessionManager& sm, ProblemBank* pb)
    : SM(manager), font(f), userManager(um), sessionManager(sm), 
      problembank(pb), currentUser(user), contestType(cType), contestIndex(cIndex),
      problemIndices(nullptr), problemCount(0), solved(nullptr), timerActive(true),
      totalAttempts(0)
{
    // Load contest data
    Contest contestData;
    int durationSeconds = 0;
    
    if (contestType == 0) 
    {
        contestData = ContestLibrary::easyContests[contestIndex];
        durationSeconds = contestData.durationMinutes * 60;
        contestName = contestData.name;
    } 
    else if (contestType == 1) 
    {
        contestData = ContestLibrary::midContests[contestIndex];
        durationSeconds = contestData.durationMinutes * 60;
        contestName = contestData.name;
    } 
    else 
    {
        contestData = ContestLibrary::hardContests[contestIndex];
        durationSeconds = contestData.durationMinutes * 60;
        contestName = contestData.name;
    }
    
    problemCount = contestData.problemCount;
    
    // Store problem IDs
    problemIndices = new int[problemCount];
    for (int i = 0; i < problemCount; i++) {
        problemIndices[i] = contestData.problems[i].problemId;
    }
    
    // Track solved status and attempts
    solved = new bool[problemCount];
    attempts = new int[problemCount];
    for (int i = 0; i < problemCount; i++) {
        solved[i] = false;
        attempts[i] = 0;
    }
    
    // Load timer from disk
    contestEndTime = 0;
    loadContestTimer(durationSeconds);
    timeRemaining = getRemainingSeconds();
    
    for (int i = 0; i < 10; i++) {
        solveButtons[i] = nullptr;
    }
    
    // ========== UI SETUP ==========
    background.setFillColor(sf::Color(240, 235, 240));
    background.setSize({1280, 720});
    
    headerPanel.setFillColor(sf::Color(170, 70, 120));
    headerPanel.setSize({1280, 70});
    headerPanel.setPosition(0, 0);
    
    titleText.setFont(font);
    titleText.setString(contestName.getstr());
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color(255, 255, 255));
    titleText.setPosition(480, 15);
    
    timerText.setFont(font);
    timerText.setCharacterSize(28);
    timerText.setFillColor(sf::Color(255, 220, 100));
    timerText.setPosition(900, 20);
    
    scoreText.setFont(font);
    scoreText.setCharacterSize(18);
    scoreText.setFillColor(sf::Color(255, 200, 150));
    scoreText.setPosition(30, 90);
    
    problemsPanel.setFillColor(sf::Color(200, 100, 150));
    problemsPanel.setOutlineThickness(2);
    problemsPanel.setOutlineColor(sf::Color(50, 60, 100));
    problemsPanel.setSize({800, 450});
    problemsPanel.setPosition(30, 150);
    
    backButton = new button(30, 650, 130, 40, "BACK", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    endContestButton = new button(900, 650, 200, 40, "END CONTEST", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    message = new MessageLabel(font, 400, 660, 14);
    
    for (int i = 0; i < problemCount; i++) {
        problemTexts[i].setFont(font);
        problemTexts[i].setCharacterSize(16);
        problemTexts[i].setFillColor(sf::Color(255, 255, 255));
        problemTexts[i].setPosition(60, 180 + i * 55);
        
        Problem* p = problembank->getProblem(problemIndices[i]);
        if (p) {
            String problemText = String("Problem ") + String::fromInt(i+1) + String(": ") + p->getTitle();
            problemTexts[i].setString(problemText.getstr());
        }
        
        solveButtons[i] = new button(700, 175 + i * 55, 100, 40, "SOLVE", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
        
        solvedTexts[i].setFont(font);
        solvedTexts[i].setCharacterSize(14);
        solvedTexts[i].setFillColor(sf::Color(100, 200, 100));
        solvedTexts[i].setString("✓ SOLVED");
        solvedTexts[i].setPosition(770, 190 + i * 55);
    }
    
    updateScore();
}

ContestScreen::~ContestScreen() {
    saveContestTimer();
    delete[] problemIndices;
    delete[] solved;
    delete[] attempts;
    delete backButton;
    delete endContestButton;
    delete message;
    for (int i = 0; i < problemCount; i++) {
        delete solveButtons[i];
    }
}

String ContestScreen::getContestSavePath() const {
    const char* typeName = "easy";
    if (contestType == 1) typeName = "mid";
    else if (contestType == 2) typeName = "hard";
    
    char idx[4];
    snprintf(idx, sizeof(idx), "%d", contestIndex);
    
    return String("users/") + currentUser + String("/contest_saves/") +
           String(typeName) + String("_") + String(idx) + String(".dat");
}

void ContestScreen::saveContestTimer() {
    if (contestEndTime == 0) return;
    
    String path = getContestSavePath();
    String data = String::fromInt(contestEndTime);
    PersistentStorage::saveFile(path, data.getstr(), data.getlen());
}

void ContestScreen::loadContestTimer(int durationSeconds) {
    String path = getContestSavePath();
    
    if (PersistentStorage::fileExists(path)) {
        char buffer[64];
        int bytes = PersistentStorage::loadFile(path, buffer, sizeof(buffer) - 1);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            int val = 0;
            for (int i = 0; i < bytes && buffer[i] >= '0' && buffer[i] <= '9'; i++) {
                val = val * 10 + (buffer[i] - '0');
            }
            contestEndTime = val;
        }
    }
    
    if (contestEndTime == 0) {
        contestEndTime = (int)time(nullptr) + durationSeconds;
        saveContestTimer();
    }
    
    timeRemaining = getRemainingSeconds();
    if (timeRemaining <= 0) {
        timerActive = false;
        timeRemaining = 0;
    }
}

int ContestScreen::getRemainingSeconds() const {
    if (contestEndTime == 0) return 0;
    int remaining = contestEndTime - (int)time(nullptr);
    return (remaining < 0) ? 0 : remaining;
}

int ContestScreen::calculateTotalScore() const {
    int totalScore = 0;
    const int contestDuration = 3 * 3600; // 3 hours in seconds
    
    for (int i = 0; i < problemCount; i++) {
        if (!solved[i]) continue;
        
        Problem* p = problembank->getProblem(problemIndices[i]);
        if (!p) continue;
        
        int difficulty = p->getDifficulty();
        int basePoints = difficulty * 100;
        int attemptCount = attempts[i];
        
        // Time taken (from timer start to when solved)
        // For now use contest duration as approximation
        int timeTaken = contestDuration - getRemainingSeconds();
        if (timeTaken < 0) timeTaken = contestDuration;
        
        double timeBonus;
        double ratio = (double)timeTaken / contestDuration;
        if (ratio <= 0.5) timeBonus = 1.0;
        else if (ratio <= 0.75) timeBonus = 0.5;
        else timeBonus = 0.25;
        
        double attemptPenalty = 1.0 / attemptCount;
        int problemScore = (int)(basePoints * timeBonus * attemptPenalty);
        totalScore += problemScore;
    }
    return totalScore;
}

void ContestScreen::showEndContestResults() {
    int solvedCountLocal = 0;
    for (int i = 0; i < problemCount; i++) {
        if (solved[i]) solvedCountLocal++;
    }
    
    int totalTimeSeconds = contestEndTime - (int)time(nullptr);
    if (totalTimeSeconds < 0) totalTimeSeconds = 0;
    
    int totalAttemptsCount = 0;
    for (int i = 0; i < problemCount; i++) {
        totalAttemptsCount += attempts[i];
    }
    
    int finalScore = calculateTotalScore();
    
    // Add score to user
    User* user = userManager.getUserByUsername(currentUser);
    if (user) {
        user->addScore(finalScore);
        UserPersistence::saveUser(currentUser, user->getEmail(), user->getPassword(),
                                   user->getVerified(), user->getTotalScore(), user->getSolvedCount());
    }
    
    SM.setScreen(new ResultScreen(SM, font, currentUser, userManager, sessionManager,
                                   problembank, solvedCountLocal, problemCount,
                                   totalTimeSeconds, totalAttemptsCount, finalScore));
}

void ContestScreen::updateScore() {
    int solvedCountLocal = 0;
    for (int i = 0; i < problemCount; i++) {
        if (solved[i]) solvedCountLocal++;
    }
    char buf[64];
    snprintf(buf, sizeof(buf), "Solved: %d / %d", solvedCountLocal, problemCount);
    scoreText.setString(buf);
    
    if (solvedCountLocal == problemCount && problemCount > 0) {
        timerActive = false;
        showEndContestResults();
    }
}

void ContestScreen::updateTimerDisplay() {
    if (!timerActive) return;
    
    int remaining = getRemainingSeconds();
    
    if (remaining <= 0) {
        timerActive = false;
        remaining = 0;
        showEndContestResults();
        return;
    }
    
    timeRemaining = remaining;
    
    int hours = remaining / 3600;
    int minutes = (remaining % 3600) / 60;
    int seconds = remaining % 60;
    
    char timeStr[20];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", hours, minutes, seconds);
    timerText.setString(timeStr);
    
    if (remaining < 60) {
        timerText.setFillColor(sf::Color(255, 50, 50));
    } else if (remaining < 300) {
        timerText.setFillColor(sf::Color(255, 150, 50));
    } else {
        timerText.setFillColor(sf::Color(255, 220, 100));
    }
}

void ContestScreen::update(float mouseX, float mouseY) {
    backButton->update(mouseX, mouseY);
    endContestButton->update(mouseX, mouseY);
    
    for (int i = 0; i < problemCount; i++) {
        if (solveButtons[i] && !solved[i]) {
            solveButtons[i]->update(mouseX, mouseY);
        }
    }
    
    updateTimerDisplay();
}

void ContestScreen::render(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(headerPanel);
    window.draw(titleText);
    window.draw(timerText);
    window.draw(scoreText);
    window.draw(problemsPanel);
    
    for (int i = 0; i < problemCount; i++) {
        window.draw(problemTexts[i]);
        
        if (solved[i]) {
            window.draw(solvedTexts[i]);
        } else {
            solveButtons[i]->render(window);
        }
    }
    
    backButton->render(window);
    endContestButton->render(window);
    message->render(window);
}

void ContestScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        float mx = event.mouseButton.x;
        float my = event.mouseButton.y;
        
        if (backButton->isClicked(mx, my)) {
            saveContestTimer();
            SM.setScreen(new dashboard(SM, font, currentUser, userManager, sessionManager, problembank));
            return;
        }
        
        if (endContestButton->isClicked(mx, my)) {
            saveContestTimer();
            showEndContestResults();
            return;
        }
        
        for (int i = 0; i < problemCount; i++) {
            if (!solved[i] && solveButtons[i] && solveButtons[i]->isClicked(mx, my)) {
                Problem* p = problembank->getProblem(problemIndices[i]);
                if (p) {
                    codingScreen* cs = new codingScreen(SM, font, currentUser, p->getTitle(),
                                                         p->getDifficulty(), i, userManager,
                                                         sessionManager, p, problembank, true);
                    cs->setContestSlot(contestType);
                    cs->setOwnerContestScreen(this);
                    SM.setScreen(cs);
                }
                return;
            }
        }
    }
}

void ContestScreen::markProblemSolved(int problemIndex) {
    if (problemIndex >= 0 && problemIndex < problemCount) {
        solved[problemIndex] = true;
        attempts[problemIndex]++;
        updateScore();
        
        if (solveButtons[problemIndex]) {
            delete solveButtons[problemIndex];
            solveButtons[problemIndex] = new button(750, 175 + problemIndex * 55, 100, 40, "SOLVED", font,
                                                     sf::Color(150, 100, 120), sf::Color(120, 80, 100));
        }
    }
}

void ContestScreen::incrementAttempts(int problemIndex) {
    if (problemIndex >= 0 && problemIndex < problemCount) {
        attempts[problemIndex]++;
    }
}
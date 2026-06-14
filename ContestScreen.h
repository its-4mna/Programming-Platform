#ifndef CONTESTSCREEN_H
#define CONTESTSCREEN_H

#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "ScreenManager.h"
#include "button.h"
#include "MessageLabel.h"
#include "Strings.h"
#include "UserManager.h"
#include "SessionManager.h"
#include "ProblemBank.h"
#include "ContestData.h"
#include "PersistentStorage.h"
#include <ctime>

class ContestScreen : public Screen {
private:
    ScreenManager& SM;
    sf::Font& font;
    UserManager& userManager;
    SessionManager& sessionManager;
    ProblemBank* problembank;
    String currentUser;
    
    int contestType;   // 0=easy, 1=medium, 2=hard
    int contestIndex;  // 0 or 1
    String contestName;
    int* problemIndices;
    int problemCount;
    bool* solved;
    int* attempts;      // Track attempts per problem
    int totalAttempts;
    
    // Timer - uses real Unix timestamp for persistence
    int contestEndTime;    // Authoritative: when contest ends (Unix timestamp)
    int timeRemaining;     // Display snapshot
    bool timerActive;
    
    // UI
    sf::RectangleShape background;
    sf::RectangleShape headerPanel;
    sf::RectangleShape problemsPanel;
    
    sf::Text titleText;
    sf::Text timerText;
    sf::Text scoreText;
    
    button* backButton;
    button* endContestButton;
    button* solveButtons[10];
    sf::Text problemTexts[10];
    sf::Text solvedTexts[10];
    
    MessageLabel* message;
    
    void updateTimerDisplay();
    void updateScore();
    void showEndContestResults();
    int calculateTotalScore() const;
    void incrementAttempts(int problemIndex);
    
    // Timer persistence helpers
    String getContestSavePath() const;
    void saveContestTimer();
    void loadContestTimer(int durationSeconds);
    
public:
    ContestScreen(ScreenManager& manager, sf::Font& f, String user, 
                  int contestType, int contestIndex,
                  UserManager& um, SessionManager& sm, ProblemBank* pb);
    ~ContestScreen();
    
    void update(float mouseX, float mouseY) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    
    void markProblemSolved(int problemIndex);
    void setContestSlot(int slot) {}
    
    // Called by CodingScreen to get remaining time
    int getRemainingSeconds() const;
};

#endif
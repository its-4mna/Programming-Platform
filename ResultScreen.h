#ifndef RESULTSCREEN_H
#define RESULTSCREEN_H

#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "ScreenManager.h"
#include "button.h"
#include "MessageLabel.h"
#include "Strings.h"
#include "UserManager.h"
#include "SessionManager.h"
#include "ProblemBank.h"

class ResultScreen : public Screen {
private:
    ScreenManager& SM;
    sf::Font& font;
    UserManager& userManager;
    SessionManager& sessionManager;
    ProblemBank* problembank;
    String currentUser;
    
    bool isFromContest;  // false = practice, true = contest
    String contestName;
    // For practice mode
    String problemTitle;
    int pointsEarned;
    
    // For contest mode
    int solvedCount;
    int totalProblems;
    int totalTimeSeconds;
    int totalAttempts;
    int totalScore;
    
    // UI
    sf::RectangleShape background;
    sf::RectangleShape headerPanel;
    sf::RectangleShape resultPanel;
    
    sf::Text titleText;
    sf::Text messageText;
    sf::Text statsText;
    
    button* continueButton;
    button* keepButton;
    button* discardButton;
    MessageLabel* message;
    
    void formatTime(int seconds, char* buffer, int bufSize);
    
public:
    // Constructor for practice mode
    ResultScreen(ScreenManager& manager, sf::Font& f, String user, 
                 UserManager& um, SessionManager& sm, ProblemBank* pb,
                 const String& problem, int points);
    
    // Constructor for contest mode
    ResultScreen(ScreenManager& manager, sf::Font& f, String user,
                 UserManager& um, SessionManager& sm, ProblemBank* pb,
                 int solved, int total, int timeSeconds, int attempts, int score);
    
    ~ResultScreen();
    
    void update(float mouseX, float mouseY) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
};

#endif
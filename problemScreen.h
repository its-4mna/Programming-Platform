#ifndef PROBLEMSCREEN_H
#define PROBLEMSCREEN_H

#include "Screen.h"
#include <SFML/Graphics.hpp>
#include "ScreenManager.h"
#include "TextBoxes.h"
#include "button.h"
#include "MessageLabel.h"
#include "Strings.h"
#include "UserManager.h"
#include "SessionManager.h"
#include "ProblemBank.h"

class problemScreen: public Screen
{
private:
    ScreenManager& SM;
    sf::Font& font;
    String currentUser;
    UserManager& userManager;
    SessionManager& sessionManager;
    int currentDifficulty; 

    sf::RectangleShape background;
    sf::RectangleShape header;
    sf::Text title;
    button* easy;
    button* mid;
    button* hard;
    button* back;

    //problem list
    ProblemBank* problembank;
    int problemCount;
    sf::Text problemTitles[10];
    button* solveButtons[10];
    Problem* currentProblems[10];
    
    //which problems to show (based on difficulty)
    //String easyProblems[10];
    //String mediumProblems[10];
    //String hardProblems[10];
    
    MessageLabel* message;
    
    //void loadProblems();
    void showProblemsForDifficulty(int diff);
    void updateDifficultyButtons();

public:
    problemScreen(ScreenManager& manager, sf::Font& f, String user, int diff, UserManager& um, SessionManager& sm, ProblemBank* pb);
    ~problemScreen();
    
    void update(float mouseX, float mouseY) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;




};


#endif
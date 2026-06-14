#ifndef HISTORYSCREEN_H
#define HISTORYSCREEN_H

#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "ScreenManager.h"
#include "button.h"
#include "MessageLabel.h"
#include "Strings.h"
#include "UserManager.h"
#include "SessionManager.h"
#include "ProblemBank.h"

class HistoryScreen : public Screen {
private:
    ScreenManager& SM;
    sf::Font& font;
    UserManager& userManager;
    SessionManager& sessionManager;
    ProblemBank* problembank;
    String currentUser;
    
    // UI
    sf::RectangleShape background;
    sf::RectangleShape headerPanel;
    sf::RectangleShape listPanel;
    
    sf::Text titleText;
    sf::Text emptyText;
    
    button* backButton;
    MessageLabel* message;
    
    // Saved contests data (simple arrays)
    String savedNames[10];
    int savedScores[10];
    int savedSolved[10];
    int savedTotal[10];
    int savedCount;
    
    sf::Text contestTexts[10];
    button* viewButtons[10];
    
public:
    HistoryScreen(ScreenManager& manager, sf::Font& f, String user,UserManager& um, SessionManager& sm, ProblemBank* pb);
    ~HistoryScreen();
    
    void update(float mouseX, float mouseY) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
};

#endif
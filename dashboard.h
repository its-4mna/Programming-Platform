#ifndef DASHBOARD_H
#define DASHBOARD_H

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

class dashboard : public Screen
{
private:
    ScreenManager& SM;
    sf::Font& font;
    UserManager& userManager;
    SessionManager& sessionManager;
    ProblemBank* problembank;
    
    MessageLabel* message;
    sf::RectangleShape background;
    sf::RectangleShape leftPanel;
    sf::RectangleShape headerPanel;

    sf::Text title;
    sf::Text subtitleText1;      // USER INFO
    sf::Text subtitletext2;      // CONTESTS
    sf::Text subtitleText3;      // BROWSE PROBLEMS

    button* easy;
    button* mid;
    button* hard;
    button* logoutButton;
    button* deleteUser;
    String currentUser;

    sf::Text usernameText;
    sf::Text emailText;
    sf::Text solvedText;
    sf::Text scoreText;

    // Confirmation popup
    bool showConfirmPopup;
    sf::RectangleShape popupBg;
    sf::RectangleShape popupPanel;
    sf::Text popupText;
    button* confirmYes;
    button* confirmNo;
    button* viewhistory;

    // Contest section - 6 contests (2 rows of 3)
    struct ContestCard 
    {
        sf::RectangleShape panel;
        sf::Text title;
        sf::Text duration;
        button* startBtn;
        int slot;
        int contestType;
        int contestIndex;
        bool completed;

    };

    ContestCard contestCards[6];
    void setupContestCards();
    void refreshUserData();

public:
    dashboard(ScreenManager& manager, sf::Font& f, String user, UserManager& um, SessionManager& sm, ProblemBank* pb);
    void update(float mouseX, float mouseY) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    ~dashboard();
};

#endif
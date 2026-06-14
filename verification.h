#ifndef VERIFICATION_H
#define VERIFICATION_H

#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "ScreenManager.h"
#include "button.h"
#include "MessageLabel.h"
#include "Strings.h"
#include "UserManager.h"
#include "SessionManager.h"
#include "TextBoxes.h"
#include "ProblemBank.h"

class verification : public Screen {
private:
    ScreenManager& SM;
    sf::Font& font;
    UserManager& userManager;
    SessionManager& sessionManager;
    String currentUser;
    ProblemBank* problembank;
    
    sf::RectangleShape background;
    sf::RectangleShape panel;
    
    sf::Text titleText;
    sf::Text messageText;
    sf::Text instructionText;
    sf::Text tokenLabel;
    
    TextBoxes* tokenBox;
    
    button* verifyButton;
    button* backButton;
    button* resendButton;
    
    MessageLabel* infoMessage;
    
    sf::Texture starTexture;
    sf::Sprite starSprite;
    
    sf::Text makeLabel(const char* text, float x, float y);

public:
    verification(ScreenManager& manager, sf::Font& f, UserManager& um, SessionManager& sm, String user, ProblemBank* pb);
    ~verification();
    
    void update(float mouseX, float mouseY) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
};

#endif
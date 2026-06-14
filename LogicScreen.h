#pragma once
#ifndef LOGICSCREEN_H
#define LOGICSCREEN_H

#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "TextBoxes.h"
#include "button.h"
#include "ScreenManager.h"
#include "MessageLabel.h"
#include "UserManager.h"
#include "SessionManager.h"
#include "ProblemBank.h"

class LogicScreen :public Screen
{
private:
    ScreenManager& SM;
    sf::Font& font;
    UserManager& userManager;      
    SessionManager& sessionManager;
    ProblemBank* problembank;

    MessageLabel* message;

    sf::Text titleText;
    sf::Text subtitleText;
    sf::Text labelUsername;
    sf::Text labelPassword;
    sf::RectangleShape background; //basically the background, will cover the whole screen
    sf::RectangleShape panel;   //a box in the centre for the actual work
    button* LoginButton;
    button* backButton;         
    TextBoxes* usernameBox;     //text field for usrname
    TextBoxes* passwordBox;     //text field for password
    sf::Texture starTexture;
    sf::Sprite starSprite;

public:
    LogicScreen(ScreenManager& manager, sf::Font& f, UserManager& um, SessionManager& sm, ProblemBank* pb);
    ~LogicScreen();
    void update(float mouseX, float mouseY) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;

};

#endif // !LOGICSCREEN_H

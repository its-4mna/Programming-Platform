#pragma once
#ifndef REGISTERSCREEN_H
#define REGISTERSCREEN_H

#include <SFML/Graphics.hpp>
#include "ScreenManager.h"
#include "TextBoxes.h"
#include "button.h"
#include "MessageLabel.h"
#include "Screen.h"
#include "Strings.h"
#include "UserManager.h"
#include "SessionManager.h"
#include "ProblemBank.h"

class RegisterScreen :public Screen
{
private:
    ScreenManager& SM;
    sf::Font& font;
    UserManager& userManager;
    SessionManager& sessionManager;
    ProblemBank* problembank;

    MessageLabel* message;

    sf::RectangleShape background;
    sf::RectangleShape panel;

    sf::Text titleText;
    sf::Text subtitleText;

    TextBoxes* usernameBox;
    TextBoxes* emailBox;
    TextBoxes* passwordBox;
    TextBoxes* confirmBox;
    // Labels above each box
    sf::Text labelUsername;
    sf::Text labelEmail;
    sf::Text labelPassword;
    sf::Text labelConfirm;

    //bas 2 hi buttons chahei na?
    button* registerButton;
    button* backButton;

    sf::Texture starTexture;
    sf::Sprite starSprite;


public:
    RegisterScreen(ScreenManager& manager, sf::Font& f, UserManager& um, SessionManager& sm, ProblemBank* pb);   //im actually gonna kms 
    //RegisterScreen(ScreenManager& SM, sf::Font& f, UserManager& um, SessionManager& sm);
    ~RegisterScreen();
    void update(float mouseX, float mouseY) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;

};


#endif // !REGISTERSCREEN_H

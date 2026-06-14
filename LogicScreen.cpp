#include "LogicScreen.h"
#include "WelcomeScreen.h"
#include "Strings.h"
#include "dashboard.h"
#include "UserManager.h"
#include "SessionManager.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "verification.h"

//ye login screen hai n yahan se user apne account mein aayega

LogicScreen::LogicScreen(ScreenManager& manager, sf::Font& f, UserManager& um, SessionManager& sm, ProblemBank* pb) : SM(manager), font(f), sessionManager(sm), userManager(um), problembank(pb)
{
    background.setFillColor(sf::Color(240, 235, 240));
    background.setSize({1280, 720});

    message = new MessageLabel(font, 430, 460, 14);

    panel.setFillColor(sf::Color(200, 100, 150));
    panel.setOutlineThickness(1);
    panel.setOutlineColor(sf::Color(50, 60, 100));
    panel.setSize({480, 380});
    panel.setPosition(400, 170);

    titleText.setFont(font);
    titleText.setCharacterSize(32);
    titleText.setFillColor(sf::Color(220, 230, 255));
    titleText.setString("Welcome Back");
    titleText.setPosition(430, 198);

    subtitleText.setFont(font);
    subtitleText.setCharacterSize(16);
    subtitleText.setFillColor(sf::Color(220, 230, 255));
    subtitleText.setString("Log in to continue with your account");
    subtitleText.setPosition(430, 242);


    labelUsername = message->makeLabel("username", 430, 280);
    usernameBox = new TextBoxes(430, 302, 420, 46, "Enter your username", false, font);

    labelPassword = message->makeLabel("password", 430, 368);

    passwordBox = new TextBoxes(430, 390, 420, 46, "Enter your password", true, font);

    LoginButton = new button(430, 482, 200, 46, "Log In", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));

    backButton = new button(650, 482, 200, 46, "Back", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));

    //load star image
    if(!starTexture.loadFromFile("assets/star.png"))
    {
        std::cout << "Failed to load star.png" << std::endl;
    }
    starSprite.setTexture(starTexture);
    starSprite.setScale(0.1, 0.1);

}

LogicScreen::~LogicScreen()
{
    delete usernameBox;
    delete passwordBox;
    delete LoginButton;
    delete backButton;
    delete message;
}

void LogicScreen::update(float mouseX, float mouseY)
{
    LoginButton->update(mouseX, mouseY);
    backButton->update(mouseX, mouseY);
}

void LogicScreen::render(sf::RenderWindow& window)
{
    window.draw(background);
    window.draw(panel);

    starSprite.setPosition(80, 100);
    window.draw(starSprite);
    
    starSprite.setPosition(340, 80);
    window.draw(starSprite);
    
    starSprite.setPosition(67, 554);
    window.draw(starSprite);
    
    starSprite.setScale(0.15, 0.15);
    starSprite.setPosition(120, 70);
    window.draw(starSprite);
    
    starSprite.setPosition(900, 600);
    window.draw(starSprite);
        
    starSprite.setScale(0.1, 0.1);
    starSprite.setPosition(290, 350);
    window.draw(starSprite);
    
    starSprite.setPosition(1100, 60);
    window.draw(starSprite);
    
    starSprite.setPosition(826, 580);
    window.draw(starSprite);
    
    starSprite.setScale(0.17, 0.17);
    starSprite.setPosition(1070, 400);
    window.draw(starSprite);
    
    starSprite.setScale(0.1, 0.1);
    starSprite.setPosition(884, 30);
    window.draw(starSprite);

    window.draw(titleText);
    window.draw(subtitleText);
    window.draw(labelUsername);
    usernameBox->render(window);
    window.draw(labelPassword);
    passwordBox->render(window);
    LoginButton->render(window);
    backButton->render(window);
    message->render(window);

}

void LogicScreen::handleEvent(sf::Event& event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        float mx = event.mouseButton.x;
        float my = event.mouseButton.y;

        usernameBox->handleClick(mx, my);
        passwordBox->handleClick(mx, my);

        if (LoginButton->isClicked(mx, my))
        {
            String user = usernameBox->getText();
            String pass = passwordBox->getText();

            //validation - agar kuch bhi empty hai to error dikhao
            if (user.getlen() == 0)
            {
                message->showError("Please enter your username.", 3);
                return;
            }
            if (pass.getlen() == 0)
            {
                message->showError("Please enter your password.", 3);
                return;
            }

            /*
            //sab sahi hai to dashboard pe le jao
            message->showSuccess("Logging in...", 1);
            SM.setScreen(new dashboard(SM, font, user));
            return;
            */
            if (userManager.loginUser(user, pass)) 
            {
                sessionManager.login(user);
                message->showSuccess("Login successful!", 1);
                SM.setScreen(new dashboard(SM, font, user, userManager, sessionManager, problembank));
                return;
            }
            else
            {
                User* userData = userManager.getUserByUsername(user);
                if(userData!=nullptr && !userData->getVerified())
                {
                    message->showError("Email Not Verified", 3);
                    SM.setScreen(new verification(SM, font, userManager, sessionManager, user, problembank));
                    return;

                }
                else
                {
                    message->showError("Invalid username or Password", 3);
                    return;
                }
                
            }
        }

        //back button se wapas welcome screen pe
        if (backButton->isClicked(mx, my))
        {
            SM.setScreen(new WelcomeScreen(SM, font, userManager, sessionManager, problembank));
            return;
        }
    }

    //jo bhi type kar raha hai wo selected textbox mein jaye
    /*
    if (event.type == sf::Event::TextEntered)
    {
        usernameBox->handleTypedChar(event.text.unicode);
        passwordBox->handleTypedChar(event.text.unicode);
    }
    */
    if (event.type == sf::Event::TextEntered)
    {
        unsigned int unicode = event.text.unicode;
        
        if (unicode == 8) //backspace
        {
            if (usernameBox->isSelected()) 
                usernameBox->handleBackspace();
            if (passwordBox->isSelected()) 
                passwordBox->handleBackspace();
        }
        else if (unicode >= 32 && unicode < 127) //printable chars
        {
            char c = static_cast<char>(unicode);
            if (usernameBox->isSelected()) 
                usernameBox->handleTypedChar(c);
            if (passwordBox->isSelected()) 
                passwordBox->handleTypedChar(c);
        }
    }
}
#include "verification.h"
#include "dashboard.h"
#include "WelcomeScreen.h"
#include <iostream>

verification::verification(ScreenManager& manager, sf::Font& f, UserManager& um, SessionManager& sm, String user, ProblemBank* pb) : SM(manager), font(f), userManager(um), sessionManager(sm), currentUser(user), problembank(pb)
{
    background.setFillColor(sf::Color(240, 235, 240));
    background.setSize({1280, 720});
    
    panel.setFillColor(sf::Color(200, 100, 150));
    panel.setOutlineThickness(1);
    panel.setOutlineColor(sf::Color(50, 60, 100));
    panel.setSize({520, 450});
    panel.setPosition(380, 135);
    
    titleText.setFont(font);
    titleText.setCharacterSize(32);
    titleText.setFillColor(sf::Color(220, 230, 255));
    titleText.setString("VERIFY YOUR ACCOUNT");
    titleText.setPosition(435, 163);
    
    messageText.setFont(font);
    messageText.setCharacterSize(16);
    messageText.setFillColor(sf::Color(220, 230, 255));
    messageText.setString("A verification token has been sent to your email.");
    messageText.setPosition(410, 220);
    
    instructionText.setFont(font);
    instructionText.setCharacterSize(16);
    instructionText.setFillColor(sf::Color(200, 210, 240));
    instructionText.setString("Please enter the verification token below to activate\nyour account.");
    instructionText.setPosition(410, 260);
    
    tokenLabel = makeLabel("Verification Token", 410, 310);
    tokenBox = new TextBoxes(410, 332, 460, 46, "Enter verification token", false, font);
    
    verifyButton = new button(730, 440, 130, 46, "verify", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    resendButton = new button(570, 440, 130, 46, "resend", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    backButton = new button(410, 440, 130, 46, "back", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    
    infoMessage = new MessageLabel(font, 410, 550, 14);
    
    if(!starTexture.loadFromFile("assets/star.png"))
    {
        std::cout << "Failed to load star.png" << std::endl;
    }
    starSprite.setTexture(starTexture);
    starSprite.setScale(0.1, 0.1);
}

verification::~verification()
{
    delete tokenBox;
    delete verifyButton;
    delete resendButton;
    delete backButton;
    delete infoMessage;
}

sf::Text verification::makeLabel(const char* text, float x, float y)
{
    sf::Text t;
    t.setPosition(x, y);
    t.setFont(font);
    t.setString(text);
    t.setFillColor(sf::Color(220, 230, 255));
    t.setCharacterSize(14);
    return t;
}

void verification::update(float mouseX, float mouseY)
{
    verifyButton->update(mouseX, mouseY);
    resendButton->update(mouseX, mouseY);
    backButton->update(mouseX, mouseY);
    //tokenBox->update(mouseX, mouseY);
}

void verification::render(sf::RenderWindow& window)
{
    window.draw(background);
    
    starSprite.setPosition(80, 100);
    window.draw(starSprite);
    starSprite.setPosition(1010, 290);
    window.draw(starSprite);
    starSprite.setPosition(400, 70);
    window.draw(starSprite);
    starSprite.setPosition(1150, 550);
    window.draw(starSprite);
    starSprite.setPosition(300, 630);
    window.draw(starSprite);
    starSprite.setPosition(960, 180);
    window.draw(starSprite);
    starSprite.setPosition(200, 350);
    window.draw(starSprite);
    starSprite.setPosition(1100, 50);
    window.draw(starSprite);
    
    window.draw(panel);
    window.draw(titleText);
    window.draw(messageText);
    window.draw(instructionText);
    window.draw(tokenLabel);
    tokenBox->render(window);
    verifyButton->render(window);
    resendButton->render(window);
    backButton->render(window);
    infoMessage->render(window);
}

void verification::handleEvent(sf::Event& event, sf::RenderWindow& window)
{
    if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        float mx = event.mouseButton.x;
        float my = event.mouseButton.y;
        
        tokenBox->handleClick(mx, my);
        
        if(backButton->isClicked(mx, my))
        {
            SM.setScreen(new WelcomeScreen(SM, font, userManager, sessionManager, problembank));
            return;
        }
        
        if(resendButton->isClicked(mx, my))
        {
            //mock resend - in real project, would generate new token
            infoMessage->showSuccess("New verification token sent to your email!", 3);
            return;
        }
        
        if(verifyButton->isClicked(mx, my))
        {
            String token = tokenBox->getText();
            
            if(token.getlen() == 0)
            {
                infoMessage->showError("Please enter verification token", 3);
                return;
            }
            
            if(userManager.verifyUser(token))
            {
                infoMessage->showSuccess("Account verified successfully!", 2);
                sessionManager.login(currentUser);
                SM.setScreen(new dashboard(SM, font, currentUser, userManager, sessionManager, problembank));
                return;
            }
            
            
            //auto login after verification
            //sessionManager.login(currentUser);
            //SM.setScreen(new dashboard(SM, font, currentUser, userManager, sessionManager));
            //return;
        }
    }
    
    if(event.type == sf::Event::TextEntered)
    {
        unsigned int unicode = event.text.unicode;
        
        if(unicode == 8) //backspace
        {
            if(tokenBox->isSelected())
                tokenBox->handleBackspace();
        }
        else if(unicode >= 32 && unicode < 127)
        {
            char c = static_cast<char>(unicode);
            if(tokenBox->isSelected())
                tokenBox->handleTypedChar(c);
        }
    }
}
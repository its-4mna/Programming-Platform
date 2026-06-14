#include "MessageLabel.h"


MessageLabel::MessageLabel(sf::Font& font, float x, float y, unsigned int size)
{
    this->font = font;
    visible = false;
    autoHideSeconds = 0;
    text.setFont(font);
    text.setCharacterSize(size);
    text.setPosition(x, y);
    text.setFillColor(sf::Color(255, 200, 220));
}

void MessageLabel::showError(const char* msg, float autoHideSec)
{
    text.setFillColor(sf::Color(28, 31, 45));
    text.setString(msg);
    visible = true;
    autoHideSeconds= autoHideSec;
    hideTimer.restart();
}

void MessageLabel::showSuccess(const char* msg, float autoHideSec)
{
    text.setFillColor(sf::Color(80, 200, 120));
    text.setString(msg);
    visible= true;
    autoHideSeconds = autoHideSec;
    hideTimer.restart();
}

void MessageLabel::hide() 
{ 
    visible = false; 

}

void MessageLabel::render(sf::RenderWindow& window)
{
    if (!visible) 
        return;
        
    if (autoHideSeconds > 0 && hideTimer.getElapsedTime().asSeconds() > autoHideSeconds)
    {
        visible = false;
        return;
    }
    window.draw(text);
}

sf::Text MessageLabel::makeLabel(const char* text, float x, float y)//helper func for text labels 
{
    sf::Text t;
    t.setPosition(x, y);
    t.setFont(font);
    t.setString(text);
    t.setFillColor(sf::Color(220, 230, 255));
    t.setCharacterSize(14);
    return t;

}
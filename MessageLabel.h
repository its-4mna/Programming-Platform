#ifndef MESSAGELABEL_H
#define MESSAGELABEL_H
#include "SFML/Graphics.hpp"

class MessageLabel
{
private:
    sf::Text    text;
    bool        visible;
    sf::Clock   hideTimer;
    float       autoHideSeconds;   //0 = never auto-hide
    sf::Font font;

public:
    MessageLabel(sf::Font& font, float x, float y, unsigned int size = 16);

    sf::Text makeLabel(const char* text, float x, float y); //helper func for text labels 

    void showError(const char* msg, float autoHideSec = 0.f);
    void showSuccess(const char* msg, float autoHideSec = 0.f);
    void hide();

    void render(sf::RenderWindow& window);
    
};


#endif
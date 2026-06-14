#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>
#include "Strings.h"

class button
{
private:
	sf::RectangleShape shape;
	sf::Text label;
	sf::Color colorNormal;
	sf::Color colorHover;	//ofc we're going all out bcz aesthetics
	sf::Color colorPressed;	//we def need ts too duh
	bool hovered;

public:
	//constructor needs size, label text, font. colors default bhi chalyn gy
	button(float x, float y, float w, float h, String text, sf::Font& font,sf::Color bgColor = sf::Color(70, 130, 230),sf::Color hoverColor = sf::Color(100, 160, 255));
	
	//checks whether the mouse is on the button and updates hover status 
	void update(float mouseX, float mouseY);

	//just in case i want to put the button anywhere else :>
	//void setPosition(float x, float y); nvm its getting complicated i dont have time for ts

	bool isClicked(float mouseX, float mouseY);

	//actually draws the button
	void render(sf::RenderWindow& window) const;

	void setOutlineThickness(float thickness);
	void setOutlineColor(sf::Color color);
	//do we need destructor here? no girl 

};

#endif // !BUTTON_H

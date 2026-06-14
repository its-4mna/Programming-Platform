#ifndef TEXTBOXES_H
#define TEXTBOXES_H

#include <SFML/Graphics.hpp>
#include "Strings.h"

class TextBoxes
{
private:
	String text;
	bool isPassword;	//cuz then usko * krna hoga bcz aesthetics ofc
	bool selected;	//like is this textBox currently selected or no

	sf::RectangleShape box;
	sf::Text displayText;
	sf::Text placeHolderText;
	sf::Clock cursorClock;	//FOR BLINKING THE CURSOR BCZ AESTHETICS

	sf::Color colorNormal;
	sf::Color colorFocused;
	sf::Color colorText;
	sf::Color colorPlaceholder;
	sf::Color outlineColor;

public:
	//constructor, sara size dena hai sath
	TextBoxes(float x, float y, float w, float h, const char* placeholder, bool password, sf::Font& font);
	
	//to handle text entered
	void handleTypedChar(char c);
	void handleBackspace();	

	void unselect();	//to unselect the text box
	bool isSelected() const;	//check if the text box is currently selected or not

	//delete all text in the box
	void clear();

	const char* getText() const;
	const String& getString() const;

	void render(sf::RenderWindow& window);

	//to check if the mouse coordinates fall in the textbox or not and then it selects that textbox
	void handleClick(float mouseX, float mouseY);

};

#endif // !TEXTBOXES_H


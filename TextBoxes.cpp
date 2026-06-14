#include "TextBoxes.h"

#include <SFML/Graphics.hpp>

TextBoxes::TextBoxes(float x, float y, float w, float h, const char* placeholder, bool password, sf::Font& font)
{
	isPassword = password;
	selected = false;

	//yaar ye change krny hyn abhi pink theme rakhni or red

	colorNormal = sf::Color(255, 200, 220);     // Very light pinkish white
	colorFocused = sf::Color(28, 31, 45);    // Pastel pink when focused
	colorText = sf::Color(80, 70, 80);          
	colorPlaceholder = sf::Color(180, 160, 170);
	outlineColor = sf::Color(255, 180, 200);

	//the actual text box shape ig
	box.setSize({ w, h });
	box.setPosition( x, y);
	box.setFillColor(colorNormal);
	box.setOutlineColor(outlineColor);
	box.setOutlineThickness(2.f);

	//to display what user is typing or typed idk
	displayText.setFont(font);
	displayText.setCharacterSize(18);
	displayText.setFillColor(colorText);
	displayText.setPosition(x + 14.f, y + h / 2.f - 12.f);

	//placeholder text like what the user sees hint k liye on what to write in the text box
	placeHolderText.setFont(font);
	placeHolderText.setCharacterSize(18);
	placeHolderText.setString(placeholder);
	placeHolderText.setFillColor(colorPlaceholder);
	placeHolderText.setPosition(x + 14.f, y + h / 2.f - 12.f);

}

//to unselect a textbox like when switching screens or smth
void TextBoxes::unselect()
{
	selected = false;
	box.setOutlineColor(outlineColor);
}

//this ones pretty obv ig
bool TextBoxes::isSelected() const
{
	return selected;
}

//assigns a new empty string obj to text matlab basically clear the textBox
void TextBoxes::clear()
{
	text = String();
}

const char* TextBoxes::getText() const
{
	return text.getstr();
}

const String& TextBoxes::getString() const
{
	return text;
}

void TextBoxes::handleClick(float mouseX, float mouseY)
{
	//chheck if mouse click is inside the box
	if (mouseX >= box.getPosition().x && mouseX <= box.getPosition().x + box.getSize().x && mouseY >= box.getPosition().y && mouseY <= box.getPosition().y + box.getSize().y)
	{
		selected = true;
		box.setOutlineColor(colorFocused);
	}
	else
	{
		selected = false;
		box.setOutlineColor(outlineColor);
	}
}


void TextBoxes::render(sf::RenderWindow& window)
{
	window.draw(box);

	if (text.isEmpty())
	{
		window.draw(placeHolderText);
		return;
	}

	String display;
	if (isPassword)
	{
		int len = text.getlen();
		char* stars = new char[len + 1];

		for (int i = 0; i < len; i++)
			stars[i] = '*';
		stars[len] = '\0';

		display = String(stars);
		delete[] stars;
	}
	else
		display = text;

	displayText.setString(display.getstr());
	window.draw(displayText);

	//blinking cursor wala part baad me krun gi idk kese krna (AAMNA PLS DONT FORGET)
}

void TextBoxes::handleTypedChar(char c)
{
	if (!selected) return;
    //acccept newline as well
    if (c == 10 || c == 13)  // 10 = LF, 13 = CR 
    {
        //trwat as newline (store \n)
        char arr[2] = {'\n', '\0'};
        String charStr(arr);
        text = text + charStr;
    }
    else if (c >= 32 && c < 127)  //baki printable chars
    {
        char arr[2] = {c, '\0'};
        String charStr(arr);
        text = text + charStr;
    }

}

//deleting text
void TextBoxes::handleBackspace()
{
	if (!selected) 
		return;

	int len = text.getlen();
	if (len > 0)	//check if the text is even there or not
	{
		text = text.substring(0, len - 1);	//creates a new str that contains everything except the last char and assigns it to text
	}
}
#include "Strings.h"
#include "button.h"

button::button(float x, float y, float w, float h, String text, sf::Font& font, sf::Color bgColor, sf::Color hoverColor)
{
	hovered = false;

	colorNormal = bgColor;
	colorHover = hoverColor;
	colorPressed = sf::Color(50, 100, 200);

	shape.setPosition(x, y);
	shape.setSize({w, h});
	shape.setFillColor(colorNormal);
	shape.setOutlineThickness(0.f);

	label.setFont(font);
	label.setString(text.getstr());
	label.setCharacterSize(19);
	label.setFillColor(sf::Color(80, 70, 80));

	sf::FloatRect textBounds = label.getLocalBounds();
	float textX = x + (w - textBounds.width) / 2.f;
	float textY = y + (h - textBounds.height) / 2.f;
	label.setPosition(textX, textY);

}

//the render fnction that draws the button
void button::render(sf::RenderWindow& window) const
{
	window.draw(shape);
	window.draw(label);
}

/*
bool button::isClicked(float mouseX, float mouseY) 
{
	float left = shape.getPosition().x;
	float top = shape.getPosition().y;
	float right = left + shape.getSize().x;
	float bottom = top + shape.getSize().y;

	return (mouseX >= left && mouseX <= right && mouseY >= top && mouseY <= bottom);
}
*/

bool button::isClicked(float mouseX, float mouseY)
{
	return shape.getGlobalBounds().contains(mouseX, mouseY);
}


void button::update(float mouseX, float mouseY)
{
	hovered = shape.getGlobalBounds().contains(mouseX, mouseY);
	shape.setFillColor(hovered ? colorHover : colorNormal);
}

void button::setOutlineThickness(float thickness)
{
    shape.setOutlineThickness(thickness);
}

void button::setOutlineColor(sf::Color color)
{
    shape.setOutlineColor(color);
}
#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Graphics.hpp>
//using namespace sf;

class MainGameLoop;

class Screen
{
private:

public:
	Screen();

	//called once per frame to update logic (timers, animations, etc.)
	virtual void update(float mouseX, float mouseY) = 0;

	//caled once per frame to draw everything
	virtual void render(sf::RenderWindow& window) = 0;

	//called for every SFML event (clicks, key presses, text input)
	virtual void handleEvent(sf::Event& ev, sf::RenderWindow& window) = 0;

	virtual ~Screen();

};
#endif // !SCREEN_H


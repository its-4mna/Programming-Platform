#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Screen.h"
#include <SFML/Graphics.hpp>
//using namespace sf;

class ScreenManager
{ 
private:
	Screen* currentScreen;
	sf::RenderWindow window;

public:
	ScreenManager();
	~ScreenManager();
	void setScreen(Screen* newScreen);
	void run();
	sf::RenderWindow& getWindow();

};

#endif // !SCREENMANAGER_H
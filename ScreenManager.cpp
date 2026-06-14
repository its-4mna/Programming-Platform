#include "ScreenManager.h"

#include <SFML/Graphics.hpp>
//using namespace sf;

ScreenManager::ScreenManager() : window(sf::VideoMode({ 1280, 720 }), "VFS JUDGE", sf::Style::Default)
{
	currentScreen = nullptr;
	window.setFramerateLimit(60);

}

//basic window getter
sf::RenderWindow& ScreenManager::getWindow()
{
	return window;
}

ScreenManager::~ScreenManager()
{
	delete currentScreen;
}

void ScreenManager::setScreen(Screen* newScreen)
{
	//deleting the old screen and putting new one
	delete currentScreen;
	currentScreen = newScreen;
}

void ScreenManager::run()
{
	sf::Event ev;
	//main game loop basically
	while (window.isOpen())
	{
		
		while (window.pollEvent(ev))
		{
			//can close the screen by pressing the x or by pressing Esc
			if (ev.type == sf::Event::Closed)
				window.close();
			else if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
				window.close();
			else if(ev.type == sf::Event::Resized)
			{
				sf::FloatRect viewRect(0.f, 0.f, ev.size.width, ev.size.height);
				window.setView(sf::View(viewRect));
			}

			if (currentScreen)
				currentScreen->handleEvent(ev, window);
		}
		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
		if (currentScreen)
			currentScreen->update((float)mousePosition.x, (float)mousePosition.y);

		window.clear();
		if (currentScreen)
			currentScreen->render(window);
		window.display();
	}
	


}
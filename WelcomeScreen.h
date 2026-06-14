#pragma once
#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "ScreenManager.h"
#include "button.h"
#include "TextBoxes.h"
#include "UserManager.h"
#include "SessionManager.h"
#include "ProblemBank.h"
//using namespace sf;

class WelcomeScreen : public Screen
{
private:
	ScreenManager& SM;
	sf::Font& font;
	UserManager& userManager;
	SessionManager& sessionManager;
	ProblemBank* problembank;

	sf::Texture bgTexture;
	sf::Sprite bgSprite;
	bool isbgLoaded;

	sf::RectangleShape forText;

	sf::Text title;
	sf::Text subtitle;
	sf::Text moretext;

	button* login;
	button* registr;

public:
	WelcomeScreen(ScreenManager& manager, sf::Font& f, UserManager& um, SessionManager& sm, ProblemBank* pb);
	~WelcomeScreen();

	void update(float mouseX, float mouseY) override;

	void render(sf::RenderWindow& window) override;
	void handleEvent(sf::Event& ev, sf::RenderWindow& window)override;
};

#endif // !WELCOMESCREEN_H

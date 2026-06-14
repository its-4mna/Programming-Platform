#include "WelcomeScreen.h"
#include <iostream>
#include "LogicScreen.h"
#include "RegisterScreen.h"
#include "dashboard.h"

//ye welcome screen hai jahan se user start karega

WelcomeScreen::WelcomeScreen(ScreenManager& manager, sf::Font& f, UserManager& um, SessionManager& sm, ProblemBank* pb) : SM(manager), font(f), sessionManager(sm), userManager(um), problembank(pb)
{
	//isbgLoaded = bgTexture.loadFromFile("assets/welcomebg.jpg");
	login = nullptr;
	registr = nullptr;

	//ye background image wala part hai abhi commented hai kyunk i cudnt find anything n maybe aesy bhi acha hi lag raha hai
    //I JUST WANT A PICTURE JISPR COPYRIGHT NA AYE CUZ USSE AURA LOSS HOGA BUT I CANT FINT ANYTHING
    //aesay hi acha hai yaar
	//baad mein uncomment kar dena
	/*
	if (isbgLoaded)
	{
		bgSprite.setTexture(bgTexture);
		sf::Vector2u texSize = bgTexture.getSize();
		bgSprite.setScale(1280 / texSize.x, 720 / texSize.y);
	}
	else
		std::cout << "coudnt load bg img";
	*/
	
	//ye poori screen cover karega background color ke liye
	forText.setSize(sf::Vector2f(1280, 720));
	forText.setFillColor(sf::Color(240, 235, 240));

	//bada sa title upar left mein
	title.setFont(font);
	title.setCharacterSize(72);
	title.setFillColor(sf::Color(200, 100, 150));
	title.setString("WELCOME");
	title.setPosition(80, 180);
	title.setLetterSpacing(1.3);

	//subtitle thoda neechay
	subtitle.setFont(font);
	subtitle.setCharacterSize(32);
	subtitle.setFillColor(sf::Color(180, 130, 160));
	subtitle.setString("ONLINE JUDGE");
	subtitle.setLetterSpacing(1.5);
	subtitle.setPosition(80, 320);

	//extra text for project info
	moretext.setFont(font);
	moretext.setString("\nOOP PROJECT");
	moretext.setCharacterSize(18);
	moretext.setFillColor(sf::Color(180, 130, 160));
	moretext.setLetterSpacing(2);
	moretext.setPosition(80, 380);

	//buttons ke positions set ki hai right side pe
	float btnX = 780;
	float btnW = 260;
	float btnH = 56;
	float btnY1 = 300;
	float btnY2 = btnY1 + btnH + 18;

	login = new button(btnX, btnY1, btnW, btnH, "LogIn", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
	registr = new button(btnX, btnY2, btnW, btnH, "Register", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
}

WelcomeScreen::~WelcomeScreen()
{
	delete login;
	delete registr;
}

//mouse movement detect karta hai buttons ke hover effect ke liye
void WelcomeScreen::update(float mouseX, float mouseY) 
{
	if (login) 
		login->update(mouseX, mouseY);
	if (registr) 
		registr->update(mouseX, mouseY);
}

//ye sab kuch screen pe draw karega
void WelcomeScreen::render(sf::RenderWindow& window) 
{
	//background pehle draw karo
	if (isbgLoaded)
		window.draw(bgSprite);
	else
	{
		//background image nahi hai to color daaldo
		sf::RectangleShape bg(sf::Vector2f(1280, 720));
		bg.setFillColor(sf::Color(12, 14, 26));
		window.draw(bg);

		
	}

	window.draw(forText);
	window.draw(title);

	//ye saari decorative lines hain hania ne kaha tha "make more lines" to kar diya
	sf::RectangleShape Line1(sf::Vector2f(120, 3));
	Line1.setFillColor(sf::Color(200, 100, 150));
	Line1.setPosition(80, 280);
	window.draw(Line1);
    
	sf::RectangleShape Line2(sf::Vector2f(80, 1.5));
	Line2.setFillColor(sf::Color(200, 100, 150));
	Line2.setPosition(80, 290);
	window.draw(Line2);
    
	sf::RectangleShape Line3(sf::Vector2f(2.5, 180));
	Line3.setFillColor(sf::Color(200, 100, 150));
	Line3.setPosition(50, 200);
	window.draw(Line3);
    
	sf::RectangleShape Line4(sf::Vector2f(20, 2.5));
	Line4.setFillColor(sf::Color(200, 100, 150));
	Line4.setPosition(50, 200);
	window.draw(Line4);
    
	sf::RectangleShape Line5(sf::Vector2f(20, 2.5));
	Line5.setFillColor(sf::Color(200, 100, 150));
	Line5.setPosition(50, 378);
	window.draw(Line5);
    
	sf::RectangleShape Line6(sf::Vector2f(35, 2));
	Line6.setFillColor(sf::Color(200, 100, 150));
	Line6.setPosition(680, 140);
	window.draw(Line6);
    
	sf::RectangleShape Line7(sf::Vector2f(2, 35));
	Line7.setFillColor(sf::Color(200, 100, 150));
	Line7.setPosition(680, 140);
	window.draw(Line7);
    
	sf::RectangleShape Line8(sf::Vector2f(35, 2));
	Line8.setFillColor(sf::Color(200, 100, 150));
	Line8.setPosition(1165, 540);
	window.draw(Line8);
    
	sf::RectangleShape Line9(sf::Vector2f(2, 35));
	Line9.setFillColor(sf::Color(200, 100, 150));
	Line9.setPosition(1198, 507);
	window.draw(Line9);
    
	sf::RectangleShape Line10(sf::Vector2f(280, 1.5));
	Line10.setFillColor(sf::Color(200, 100, 150));
	Line10.setPosition(760, 270);
	window.draw(Line10);
    
	sf::RectangleShape Line11(sf::Vector2f(2, 60));
	Line11.setFillColor(sf::Color(200, 100, 150));
	Line11.setPosition(1180, 200);
	window.draw(Line11);
    
	sf::RectangleShape Line12(sf::Vector2f(25, 2));
	Line12.setFillColor(sf::Color(200, 100, 150));
	Line12.setPosition(1157, 200);
	window.draw(Line12);
    
	sf::RectangleShape Line13(sf::Vector2f(40, 1.5));
	Line13.setFillColor(sf::Color(200, 100, 150));
	Line13.setPosition(1140, 160);
	Line13.setRotation(45);
	window.draw(Line13);
    
	sf::RectangleShape Line14(sf::Vector2f(40, 1.5));
	Line14.setFillColor(sf::Color(200, 100, 150));
	Line14.setPosition(1140, 195);
	Line14.setRotation(-45);
	window.draw(Line14);
    
	sf::RectangleShape Line15(sf::Vector2f(450, 1));
	Line15.setFillColor(sf::Color(200, 100, 150));
	Line15.setPosition(750, 560);
	window.draw(Line15);
    
	sf::RectangleShape Line16(sf::Vector2f(150, 2));
	Line16.setFillColor(sf::Color(200, 100, 150));
	Line16.setPosition(810, 480);
	window.draw(Line16);
    
	sf::RectangleShape Line17(sf::Vector2f(1.5, 25));
	Line17.setFillColor(sf::Color(200, 100, 150));
	Line17.setPosition(885, 457);
	window.draw(Line17);
    
	sf::RectangleShape Line18(sf::Vector2f(30, 1.5));
	Line18.setFillColor(sf::Color(200, 100, 150));
	Line18.setPosition(720, 400);
	window.draw(Line18);
    
	sf::RectangleShape Line19(sf::Vector2f(1.5, 30));
	Line19.setFillColor(sf::Color(200, 100, 150));
	Line19.setPosition(734, 385);
	window.draw(Line19);
    
	sf::RectangleShape Line20(sf::Vector2f(30, 1));
	Line20.setFillColor(sf::Color(200, 100, 150, 80));
	Line20.setPosition(910, 328);
	Line20.setRotation(90);
	window.draw(Line20);

	window.draw(subtitle);
	window.draw(moretext);

	//buttons ko draw karo agar exist karte hain
	if (login) 
		login->render(window);
	if (registr) 
		registr->render(window);
}

//mouse clicks handle karta hai
void WelcomeScreen::handleEvent(sf::Event& ev, sf::RenderWindow& window)
{
	if(ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Num1)
	{
		SM.setScreen(new dashboard(SM, font, "test", userManager, sessionManager, problembank));
		return;
	}


	if (ev.type == sf::Event::MouseButtonPressed)
	{
		float mx = ev.mouseButton.x;
		float my = ev.mouseButton.y;

		//login button dabaya to LogicScreen pe le jao
		if (login->isClicked(mx, my))
		{
			//std::cout << "Login button clicked" << std::endl;
    		//std::cout << "problembank pointer: " << problembank << std::endl;
    		//if(problembank) 
        	//	std::cout << "problembank has " << problembank->getProblemCount() << " problems" << std::endl;
    
    		SM.setScreen(new LogicScreen(SM, font, userManager, sessionManager, problembank));
    		//std::cout << "LogicScreen created" << std::endl;
    		return;
			//SM.setScreen(new LogicScreen(SM, font, userManager, sessionManager, problembank));
			//std::cout << "Login"; //debug ke liye console pe print
		}

		//register button dabaya to RegisterScreen pe le jao
		if (registr->isClicked(mx, my))
		{
			SM.setScreen(new RegisterScreen(SM, font, userManager, sessionManager, problembank));
			std::cout << "Register";
			return;
		}
	}
}
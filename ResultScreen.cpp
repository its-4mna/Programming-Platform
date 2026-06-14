#include "ResultScreen.h"
#include "dashboard.h"
#include "problemScreen.h"

ResultScreen::ResultScreen(ScreenManager& manager, sf::Font& f, String user,UserManager& um, SessionManager& sm, ProblemBank* pb,const String& problem, int points)
    : SM(manager), font(f), userManager(um), sessionManager(sm), 
      problembank(pb), currentUser(user), isFromContest(false),
      problemTitle(problem), pointsEarned(points)
{
    // Practice mode UI
    background.setFillColor(sf::Color(240, 235, 240));
    background.setSize({1280, 720});
    
    headerPanel.setFillColor(sf::Color(170, 70, 120));
    headerPanel.setSize({1280, 70});
    headerPanel.setPosition(0, 0);
    
    resultPanel.setFillColor(sf::Color(200, 100, 150));
    resultPanel.setOutlineThickness(2);
    resultPanel.setOutlineColor(sf::Color(50, 60, 100));
    resultPanel.setSize({500, 350});
    resultPanel.setPosition(390, 150);
    
    titleText.setFont(font);
    titleText.setString("PROBLEM SOLVED!");
    titleText.setCharacterSize(32);
    titleText.setFillColor(sf::Color(100, 200, 100));
    titleText.setPosition(480, 190);
    
    messageText.setFont(font);
    messageText.setCharacterSize(18);
    messageText.setFillColor(sf::Color(255, 255, 255));
    String msg = String("You solved: ") + problemTitle;
    messageText.setString(msg.getstr());
    messageText.setPosition(420, 260);
    
    char statsBuf[128];
    snprintf(statsBuf, sizeof(statsBuf), "Points Earned: %d\n\nTotal Score: %d", pointsEarned, userManager.getUserByUsername(currentUser)->getTotalScore());
    statsText.setFont(font);
    statsText.setCharacterSize(16);
    statsText.setFillColor(sf::Color(240, 230, 245));
    statsText.setString(statsBuf);
    statsText.setPosition(450, 310);
    
    continueButton = new button(540, 430, 200, 50, "CONTINUE", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    
    keepButton = nullptr;
    discardButton = nullptr;
    message = new MessageLabel(font, 400, 550, 14);
}

ResultScreen::ResultScreen(ScreenManager& manager, sf::Font& f, String user,UserManager& um, SessionManager& sm, ProblemBank* pb,int solved, int total, int timeSeconds, int attempts, int score)
    : SM(manager), font(f), userManager(um), sessionManager(sm),
      problembank(pb), currentUser(user), isFromContest(true),
      solvedCount(solved), totalProblems(total), totalTimeSeconds(timeSeconds),
      totalAttempts(attempts), totalScore(score)
{
    // Contest mode UI
    background.setFillColor(sf::Color(240, 235, 240));
    background.setSize({1280, 720});
    
    headerPanel.setFillColor(sf::Color(170, 70, 120));
    headerPanel.setSize({1280, 70});
    headerPanel.setPosition(0, 0);
    
    resultPanel.setFillColor(sf::Color(200, 100, 150));
    resultPanel.setOutlineThickness(2);
    resultPanel.setOutlineColor(sf::Color(50, 60, 100));
    resultPanel.setSize({550, 450});
    resultPanel.setPosition(365, 120);
    
    char titleBuf[64];
    if (solvedCount == totalProblems) {
        snprintf(titleBuf, sizeof(titleBuf), "CONTEST COMPLETED!");
        titleText.setFillColor(sf::Color(100, 200, 100));
    } else {
        snprintf(titleBuf, sizeof(titleBuf), "CONTEST ENDED");
        titleText.setFillColor(sf::Color(255, 200, 100));
    }
    titleText.setFont(font);
    titleText.setString(titleBuf);
    titleText.setCharacterSize(32);
    titleText.setPosition(480, 160);
    
    char timeBuf[32];
    int hours = totalTimeSeconds / 3600;
    int minutes = (totalTimeSeconds % 3600) / 60;
    int secs = totalTimeSeconds % 60;
    snprintf(timeBuf, sizeof(timeBuf), "%02d:%02d:%02d", hours, minutes, secs);
    
    char statsBuf[256];
    snprintf(statsBuf, sizeof(statsBuf),
             "Problems Solved: %d / %d\n\n"
             "Total Time: %s\n\n"
             "Total Attempts: %d\n\n"
             "Final Score: %d\n\n",
             solvedCount, totalProblems, timeBuf, totalAttempts, totalScore);
    
    statsText.setFont(font);
    statsText.setCharacterSize(18);
    statsText.setFillColor(sf::Color(255, 255, 255));
    statsText.setString(statsBuf);
    statsText.setPosition(420, 210);
    
    keepButton = new button(420, 470, 200, 50, "KEEP CODE", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    discardButton = new button(660, 470, 200, 50, "DISCARD CODE", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    
    continueButton = nullptr;
    message = new MessageLabel(font, 400, 550, 14);
}

ResultScreen::~ResultScreen() 
{
    delete continueButton;
    delete keepButton;
    delete discardButton;
    delete message;
}

void ResultScreen::update(float mouseX, float mouseY) 
{
    if (continueButton) 
        continueButton->update(mouseX, mouseY);
    if (keepButton) 
        keepButton->update(mouseX, mouseY);
    if (discardButton) 
        discardButton->update(mouseX, mouseY);
}

void ResultScreen::render(sf::RenderWindow& window) 
{
    window.draw(background);
    window.draw(headerPanel);
    window.draw(resultPanel);
    window.draw(titleText);
    window.draw(messageText);
    window.draw(statsText);
    
    if (continueButton) 
        continueButton->render(window);

    if (keepButton) 
        keepButton->render(window);

    if (discardButton) 
        discardButton->render(window);
    
    message->render(window);
}

void ResultScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) 
{
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) 
    {
        float mx = event.mouseButton.x;
        float my = event.mouseButton.y;
        
        if (continueButton && continueButton->isClicked(mx, my)) 
        {
            SM.setScreen(new problemScreen(SM, font, currentUser, 0, userManager, sessionManager, problembank));
            return;
        }
        
        if (keepButton && keepButton->isClicked(mx, my)) 
        {
            String savedDir = String("users/") + currentUser + String("/saved_contests/");
            PersistentStorage::makeDirectory(savedDir);
    
            // Create data string
            String data = contestName + String("|") + String::fromInt(totalScore) + String("|") +  String::fromInt(solvedCount) + String("|") + String::fromInt(totalProblems);
    
            // Find next available slot
            int slot = 0;
            char fileName[64];
            String filePath;
    
            do 
            {
                snprintf(fileName, sizeof(fileName), "contest_%d.dat", slot);
                filePath = savedDir + String(fileName);
                slot++;
            } 
            while (PersistentStorage::fileExists(filePath) && slot < 100);
    
            PersistentStorage::saveFile(filePath, data.getstr(), data.getlen());


            SM.setScreen(new dashboard(SM, font, currentUser, userManager, sessionManager, problembank));
            return;
        }
        
        //basically kuch bhi na karo
        if (discardButton && discardButton->isClicked(mx, my)) 
        {
            SM.setScreen(new dashboard(SM, font, currentUser, userManager, sessionManager, problembank));
            return;
        }
    }
}
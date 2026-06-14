#include "problemScreen.h"
#include "dashboard.h"
#include "WelcomeScreen.h"
#include "CodingScreen.h"
#include "ProblemBank.h"

problemScreen::problemScreen(ScreenManager& manager, sf::Font& f, String user, int diff, UserManager& um, SessionManager& sm, ProblemBank* pb): SM(manager), font(f), userManager(um), sessionManager(sm), problembank(pb)
{
    currentUser = user;
    currentDifficulty = diff;
    problemCount = 0;

    //initialize all problems to ntohing
    for(int i = 0; i < 10; i++)
    {
        currentProblems[i] = nullptr;
    }

    //init buttons to nullptr
    for(int i = 0; i < 10; i++)
    {
        solveButtons[i] = nullptr;
    }
    
    message = new MessageLabel(font, 400, 600, 14);
    
    //background
    background.setFillColor(sf::Color(240, 235, 240));
    background.setSize({1280, 720});
    
    //header panel (dark pink)
    header.setFillColor(sf::Color(170, 70, 120));
    header.setSize({1280, 70});
    header.setPosition(0, 0);
    
    //title "PROBLEMS" in center
    title.setFont(font);
    title.setString("PROBLEMS");
    title.setCharacterSize(32);
    title.setFillColor(sf::Color(220, 230, 255));
    title.setPosition(540, 15);
    
    //back button (left side)
    back = new button(20, 12, 100, 45, "BACK", font,  sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    
    //difficulty buttons (right side)
    easy = new button(880, 12, 100, 45, "EASY", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    mid = new button(990, 12, 110, 45, "MEDIUM", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    hard = new button(1110, 12, 100, 45, "HARD", font,  sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    
    //load problem data
    //loadProblems();
    
    //show problems for current difficulty
    showProblemsForDifficulty(currentDifficulty);
    updateDifficultyButtons();
}

void problemScreen::updateDifficultyButtons()
{
    //reset all to normal
    easy->setOutlineThickness(0);
    mid->setOutlineThickness(0);
    hard->setOutlineThickness(0);
    
    //bold border for selected difficulty
    if(currentDifficulty == 0)
    {
        easy->setOutlineThickness(3);
        easy->setOutlineColor(sf::Color(255, 255, 255));
    }
    else if(currentDifficulty == 1)
    {
        mid->setOutlineThickness(3);
        mid->setOutlineColor(sf::Color(255, 255, 255));
    }
    else if(currentDifficulty == 2)
    {
        hard->setOutlineThickness(3);
        hard->setOutlineColor(sf::Color(255, 255, 255));
    }
}

void problemScreen::showProblemsForDifficulty(int diff)
{
    //delete old solve buttons
    for(int i = 0; i < problemCount; i++)
    {
        if(solveButtons[i])
        {
            delete solveButtons[i];
            solveButtons[i] = nullptr;
        }
        currentProblems[i] = nullptr;
    }
    
    if(!problembank)
        return;

    //get problems from the bank based on difficulty
    int problemIndex = 0;
    int totalProbs = problembank->getProblemCount();

    for(int i=0; i<totalProbs && problemIndex < 10; i++)
    {
        Problem* p = problembank->getProblem(i);
        
        if(p && p->getDifficulty() == diff +1) //diff: 0=easy(1), 1=medium(2), 2=hard(3)
        {
            currentProblems[problemIndex] = p;

            float yPos = 100 + (problemIndex*55);

            //Display problem title
            problemTitles[problemIndex].setFont(font);
            String numberedTitle = String::fromInt(problemIndex + 1) + ". " + p->getTitle();
            problemTitles[problemIndex].setString(numberedTitle.getstr());
            //problemTitles[problemIndex].setString(p->getTitle().getstr());
            problemTitles[problemIndex].setCharacterSize(18);
            problemTitles[problemIndex].setFillColor(sf::Color(170, 70, 120));
            problemTitles[problemIndex].setPosition(100, yPos + 10);


            solveButtons[problemIndex] = new button(950, yPos, 120, 40, "SOLVE", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
            problemIndex++;
        }
    }
    problemCount = problemIndex;

}

problemScreen::~problemScreen()
{
    delete back;
    delete easy;
    delete mid;
    delete hard;
    delete message;
    
    for(int i = 0; i < problemCount; i++)
    {
        if(solveButtons[i]) 
            delete solveButtons[i];
    }
}

void problemScreen::update(float mouseX, float mouseY)
{
    back->update(mouseX, mouseY);
    easy->update(mouseX, mouseY);
    mid->update(mouseX, mouseY);
    hard->update(mouseX, mouseY);
    
    for(int i = 0; i < problemCount; i++)
    {
        if(solveButtons[i]) 
            solveButtons[i]->update(mouseX, mouseY);
    }
}

void problemScreen::render(sf::RenderWindow& window)
{
    window.draw(background);
    window.draw(header);
    window.draw(title);
    
    back->render(window);
    easy->render(window);
    mid->render(window);
    hard->render(window);
    
    for(int i = 0; i < problemCount; i++)
    {
        window.draw(problemTitles[i]);
        if(solveButtons[i]) solveButtons[i]->render(window);
    }
    
    message->render(window);
}

void problemScreen::handleEvent(sf::Event& event, sf::RenderWindow& window)
{
    if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        float mx = event.mouseButton.x;
        float my = event.mouseButton.y;
        
        if(back->isClicked(mx, my))
        {
            SM.setScreen(new dashboard(SM, font, currentUser, userManager, sessionManager, problembank));
            return;
        }
        
        if(easy->isClicked(mx, my))
        {
            currentDifficulty = 0;
            showProblemsForDifficulty(0);
            updateDifficultyButtons();
            return;
        }
        if(mid->isClicked(mx, my))
        {
            currentDifficulty = 1;
            showProblemsForDifficulty(1);
            updateDifficultyButtons();
            return;
        }
        if(hard->isClicked(mx, my))
        {
            currentDifficulty = 2;
            showProblemsForDifficulty(2);
            updateDifficultyButtons();
            return;
        }
        
        for(int i = 0; i < problemCount; i++)
        {
            if(solveButtons[i] && solveButtons[i]->isClicked(mx, my))
            {
                
                String problemTitle = currentProblems[i]->getTitle();
                message->showSuccess("openeing...", 1);

                SM.setScreen(new codingScreen(SM, font, currentUser, problemTitle, currentDifficulty, i, userManager, sessionManager, currentProblems[i], problembank));
                return;
                
            }
        }
    }
}
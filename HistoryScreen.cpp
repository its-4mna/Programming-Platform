#include "HistoryScreen.h"
#include "dashboard.h"
#include "PersistentStorage.h"
#include <cstdio>

HistoryScreen::HistoryScreen(ScreenManager& manager, sf::Font& f, String user,
                              UserManager& um, SessionManager& sm, ProblemBank* pb)
    : SM(manager), font(f), userManager(um), sessionManager(sm),
      problembank(pb), currentUser(user), savedCount(0)
{
    for (int i = 0; i < 10; i++) {
        viewButtons[i] = nullptr;
    }
    
    // Background
    background.setFillColor(sf::Color(240, 235, 240));
    background.setSize({1280, 720});
    
    // Header
    headerPanel.setFillColor(sf::Color(170, 70, 120));
    headerPanel.setSize({1280, 70});
    headerPanel.setPosition(0, 0);
    
    // Title
    titleText.setFont(font);
    titleText.setString("SAVED CONTESTS");
    titleText.setCharacterSize(32);
    titleText.setFillColor(sf::Color(255, 255, 255));
    titleText.setPosition(520, 15);
    
    // List panel
    listPanel.setFillColor(sf::Color(200, 100, 150));
    listPanel.setOutlineThickness(2);
    listPanel.setOutlineColor(sf::Color(50, 60, 100));
    listPanel.setSize({800, 500});
    listPanel.setPosition(240, 100);
    
    // Back button
    backButton = new button(30, 650, 130, 40, "BACK", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    
    message = new MessageLabel(font, 400, 620, 14);
    
    // Empty text
    emptyText.setFont(font);
    emptyText.setString("No saved contests yet.");
    emptyText.setCharacterSize(18);
    emptyText.setFillColor(sf::Color(255, 255, 255));
    emptyText.setPosition(510, 300);
    
    // Load saved contests
    String savedDir = String("users/") + currentUser + String("/saved_contests/");
    
    // Try to load up to 10 saved contests
    for (int i = 0; i < 10; i++) {
        char fileName[64];
        snprintf(fileName, sizeof(fileName), "contest_%d.dat", i);
        String filePath = savedDir + String(fileName);
        
        if (PersistentStorage::fileExists(filePath)) {
            char buffer[512];
            int bytes = PersistentStorage::loadFile(filePath, buffer, sizeof(buffer) - 1);
            if (bytes > 0) {
                buffer[bytes] = '\0';
                String data(buffer);
                
                // Parse: name|score
                const char* str = data.getstr();
                int len = data.getlen();
                int pos = 0;
                
                // Read name
                int nameStart = pos;
                while (pos < len && str[pos] != '|') pos++;
                char nameBuf[128];
                for (int j = nameStart; j < pos; j++) nameBuf[j - nameStart] = str[j];
                nameBuf[pos - nameStart] = '\0';
                savedNames[savedCount] = String(nameBuf);
                pos++;
                
                // Read score
                int scoreStart = pos;
                while (pos < len && str[pos] != '|') pos++;
                char scoreBuf[32];
                for (int j = scoreStart; j < pos; j++) scoreBuf[j - scoreStart] = str[j];
                scoreBuf[pos - scoreStart] = '\0';
                savedScores[savedCount] = 0;
                for (int j = 0; scoreBuf[j] >= '0' && scoreBuf[j] <= '9'; j++) {
                    savedScores[savedCount] = savedScores[savedCount] * 10 + (scoreBuf[j] - '0');
                }
                pos++;
                
                // Read solved
                int solvedStart = pos;
                while (pos < len && str[pos] != '|') pos++;
                char solvedBuf[32];
                for (int j = solvedStart; j < pos; j++) solvedBuf[j - solvedStart] = str[j];
                solvedBuf[pos - solvedStart] = '\0';
                savedSolved[savedCount] = 0;
                for (int j = 0; solvedBuf[j] >= '0' && solvedBuf[j] <= '9'; j++) {
                    savedSolved[savedCount] = savedSolved[savedCount] * 10 + (solvedBuf[j] - '0');
                }
                pos++;
                
                // Read total
                int totalStart = pos;
                while (pos < len && str[pos] != '\0') pos++;
                char totalBuf[32];
                for (int j = totalStart; j < pos; j++) totalBuf[j - totalStart] = str[j];
                totalBuf[pos - totalStart] = '\0';
                savedTotal[savedCount] = 0;
                for (int j = 0; totalBuf[j] >= '0' && totalBuf[j] <= '9'; j++) {
                    savedTotal[savedCount] = savedTotal[savedCount] * 10 + (totalBuf[j] - '0');
                }
                
                savedCount++;
            }
        }
    }
    
    // Create display texts and buttons
    for (int i = 0; i < savedCount && i < 10; i++) {
        char line[256];
        snprintf(line, sizeof(line), "%s  |  Score: %d  |  Solved: %d/%d",
                 savedNames[i].getstr(), savedScores[i], savedSolved[i], savedTotal[i]);
        
        contestTexts[i].setFont(font);
        contestTexts[i].setString(line);
        contestTexts[i].setCharacterSize(14);
        contestTexts[i].setFillColor(sf::Color(255, 255, 255));
        contestTexts[i].setPosition(270, 130 + i * 50);
        
        viewButtons[i] = new button(900, 125 + i * 50, 80, 35, "VIEW", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    }
}

HistoryScreen::~HistoryScreen() 
{
    delete backButton;
    delete message;
    for (int i = 0; i < savedCount; i++) 
        delete viewButtons[i];
    
}

void HistoryScreen::update(float mouseX, float mouseY) 
{
    backButton->update(mouseX, mouseY);
    for (int i = 0; i < savedCount; i++) 
    {
        if (viewButtons[i]) 
            viewButtons[i]->update(mouseX, mouseY);
    }
}

void HistoryScreen::render(sf::RenderWindow& window) 
{
    window.draw(background);
    window.draw(headerPanel);
    window.draw(titleText);
    window.draw(listPanel);
    
    if (savedCount == 0) 
        window.draw(emptyText);
    else 
    {
        for (int i = 0; i < savedCount; i++) 
        {
            window.draw(contestTexts[i]);
            if (viewButtons[i]) 
                viewButtons[i]->render(window);
            
        }
    }
    
    backButton->render(window);
    message->render(window);
}

void HistoryScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) 
{
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) 
    {
        float mx = event.mouseButton.x;
        float my = event.mouseButton.y;
        
        if (backButton->isClicked(mx, my)) {
            SM.setScreen(new dashboard(SM, font, currentUser, userManager, sessionManager, problembank));
            return;
        }
        
        for (int i = 0; i < savedCount; i++) {
            if (viewButtons[i] && viewButtons[i]->isClicked(mx, my)) {
                char msg[128];
                snprintf(msg, sizeof(msg), "Contest: %s\nScore: %d", 
                         savedNames[i].getstr(), savedScores[i]);
                message->showSuccess(msg, 3);
                return;
            }
        }
    }
}
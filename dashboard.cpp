#include "dashboard.h"
#include "WelcomeScreen.h"
#include "problemScreen.h"
#include "ContestScreen.h"
#include "PasswordStrength.h"
#include "ContestGroup.h"
#include <cstdio>
#include "PersistentStorage.h"
#include "HistoryScreen.h"

dashboard::dashboard(ScreenManager& manager, sf::Font& f, String user, UserManager& um, SessionManager& sm, ProblemBank* pb) 
    : SM(manager), font(f), userManager(um), sessionManager(sm), problembank(pb)
{
    currentUser = user;

    background.setFillColor(sf::Color(240, 235, 240));
    background.setSize({1280, 720});

    leftPanel.setFillColor(sf::Color(200, 100, 150));
    leftPanel.setSize({340, 680});
    leftPanel.setPosition(0,60);

    headerPanel.setFillColor(sf::Color(170, 70, 120));
    headerPanel.setSize({1280, 60});
    headerPanel.setPosition(0,0);

    title.setFont(font);
    title.setString("-D-A-S-H-B-O-A-R-D-");
    title.setCharacterSize(36);
    title.setPosition(440, 3);
    title.setFillColor(sf::Color(220, 230, 255));

    subtitleText1.setFont(font);
    subtitleText1.setFillColor(sf::Color(255, 200, 220));
    subtitleText1.setString("USER INFO");
    subtitleText1.setPosition(30, 75);
    subtitleText1.setCharacterSize(28);

    subtitletext2.setFont(font);
    subtitletext2.setCharacterSize(28);
    subtitletext2.setFillColor(sf::Color(200, 100, 150));
    subtitletext2.setString("CONTEST STATUS");
    subtitletext2.setPosition(370, 75);

    subtitleText3.setFont(font);
    subtitleText3.setFillColor(sf::Color(200, 100, 150));
    subtitleText3.setString("BROWSE PROBLEMS");
    subtitleText3.setPosition(370, 350);
    subtitleText3.setCharacterSize(28);

    User* userData = userManager.getUserByUsername(currentUser);

    // Set user info text (will be updated in refreshUserData)
    usernameText.setFont(font);
    usernameText.setCharacterSize(18);
    usernameText.setFillColor(sf::Color(255, 240, 245));
    usernameText.setPosition(30, 130);

    emailText.setFont(font);
    emailText.setCharacterSize(18);
    emailText.setFillColor(sf::Color(255, 240, 245));
    emailText.setPosition(30, 165);

    scoreText.setFont(font);
    scoreText.setCharacterSize(18);
    scoreText.setFillColor(sf::Color(255, 240, 245));
    scoreText.setPosition(30, 200);

    solvedText.setFont(font);
    solvedText.setCharacterSize(18);
    solvedText.setFillColor(sf::Color(255, 240, 245));
    solvedText.setPosition(30, 235);

    // No rank text - removed as requested

    logoutButton = new button(20, 640, 145, 60, "Log Out", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    deleteUser = new button(175, 640, 145, 60, " Delete\nAccount", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    easy = new button(370, 400, 180, 50, "e a s y", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    mid = new button(565, 400, 180, 50, "m e d i u m", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    hard = new button(760, 400, 180, 50, "h a r d", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    viewhistory = new button(1000, 80, 200, 30, "view history", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));


    // Refresh user data to display actual score and solved count
    refreshUserData();

    // Initialize popup (hidden by default)
    showConfirmPopup = false;

    // Dark overlay (covers whole screen)
    popupBg.setFillColor(sf::Color(0, 0, 0, 150));
    popupBg.setSize({1280, 720});

    // Panel in center
    popupPanel.setFillColor(sf::Color(200, 100, 150));
    popupPanel.setOutlineThickness(2);
    popupPanel.setOutlineColor(sf::Color(50, 60, 100));
    popupPanel.setSize({400, 180});
    popupPanel.setPosition(440, 270);

    // Text
    popupText.setFont(font);
    popupText.setString("Are You Absolutely Sure you\nWant to Delete Your Account?");
    popupText.setCharacterSize(20);
    popupText.setFillColor(sf::Color(255, 255, 255));
    popupText.setPosition(485, 300);

    // Yes button (left)
    confirmYes = new button(490, 370, 120, 45, "YES", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));

    // No button (right)
    confirmNo = new button(660, 370, 120, 45, "NO", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));

    setupContestCards();
}

void dashboard::refreshUserData() {
    User* userData = userManager.getUserByUsername(currentUser);
    
    if (userData) {
        // Username
        usernameText.setString((String("Name: ") + userData->getUsername()).getstr());
        
        // Email
        emailText.setString((String("Email: ") + userData->getEmail()).getstr());
        
        // Score
        char scoreBuf[64];
        snprintf(scoreBuf, sizeof(scoreBuf), "Score: %d", userData->getTotalScore());
        scoreText.setString(scoreBuf);
        
        // Solved Problems Count
        char solvedBuf[64];
        snprintf(solvedBuf, sizeof(solvedBuf), "Solved: %d problems", userData->getSolvedCount());
        solvedText.setString(solvedBuf);
    } else {
        // Fallback if user not found
        usernameText.setString((String("Name: ") + currentUser).getstr());
        emailText.setString((String("Email: ") + currentUser + String("@nu.edu.pk")).getstr());
        scoreText.setString("Score: 0");
        solvedText.setString("Solved: 0 problems");
    }
}

void dashboard::render(sf::RenderWindow& window)
{
    refreshUserData(); 
    window.draw(background);
    window.draw(leftPanel);
    window.draw(headerPanel);
    window.draw(title);
    window.draw(subtitleText1);
    window.draw(usernameText);
    window.draw(emailText);
    window.draw(scoreText);
    window.draw(solvedText);
    window.draw(subtitletext2);
    window.draw(subtitleText3);
    
    logoutButton->render(window);
    deleteUser->render(window);
    easy->render(window);
    mid->render(window);
    hard->render(window);
    viewhistory->render(window);

    // Render contest cards
    for (int i = 0; i < 3; i++) 
    {
        window.draw(contestCards[i].panel);
        window.draw(contestCards[i].title);
        window.draw(contestCards[i].duration);
        contestCards[i].startBtn->render(window);
    }

    if (showConfirmPopup) 
    {
        window.draw(popupBg);
        window.draw(popupPanel);
        window.draw(popupText);
        confirmYes->render(window);
        confirmNo->render(window);
    }
}

void dashboard::update(float mouseX, float mouseY) 
{
    logoutButton->update(mouseX, mouseY);
    easy->update(mouseX, mouseY);
    mid->update(mouseX, mouseY);
    hard->update(mouseX, mouseY);
    deleteUser->update(mouseX, mouseY);
    viewhistory->update(mouseX, mouseY);

    // Update popup buttons if showing
    if (showConfirmPopup) 
    {
        confirmYes->update(mouseX, mouseY);
        confirmNo->update(mouseX, mouseY);
    }

    for (int i = 0; i < 3; i++)
        contestCards[i].startBtn->update(mouseX, mouseY);
}

dashboard::~dashboard()
{
    delete logoutButton;
    delete easy;
    delete mid;
    delete hard;
    delete deleteUser;
    delete confirmYes;
    delete confirmNo;
    delete viewhistory;
    for (int i = 0; i < 3; i++) 
        delete contestCards[i].startBtn;
}

void dashboard::handleEvent(sf::Event& event, sf::RenderWindow& window)
{
    float mx, my;
    if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        mx = event.mouseButton.x;
        my = event.mouseButton.y;

        // If popup is showing, only handle popup buttons
        if (showConfirmPopup) 
        {
            if (confirmYes->isClicked(mx, my)) 
            {
                showConfirmPopup = false;
                sessionManager.logout();
                if (userManager.deleteUser(currentUser)) 
                    SM.setScreen(new WelcomeScreen(SM, font, userManager, sessionManager, problembank));
            }
            else if (confirmNo->isClicked(mx, my)) 
                showConfirmPopup = false;
            
            return;
        }
        
        if(logoutButton->isClicked(mx, my))
            SM.setScreen(new WelcomeScreen(SM, font, userManager, sessionManager, problembank));

        if(easy->isClicked(mx, my))
        {
            SM.setScreen(new problemScreen(SM, font, currentUser, 0, userManager, sessionManager, problembank));
        }
        
        if(mid->isClicked(mx, my))
        {
            SM.setScreen(new problemScreen(SM, font, currentUser, 1, userManager, sessionManager, problembank));
        }
        
        if(hard->isClicked(mx, my))
        {
            SM.setScreen(new problemScreen(SM, font, currentUser, 2, userManager, sessionManager, problembank));
        }

        if(viewhistory->isClicked(mx, my))
        {
            SM.setScreen(new HistoryScreen(SM, font, currentUser, userManager, sessionManager, problembank));
        }

        // Contest card buttons
        for (int i = 0; i < 3; i++) 
        {
            if (contestCards[i].startBtn->isClicked(mx, my) && !contestCards[i].completed) 
            {
                // Get user's group from password
                User* user = userManager.getUserByUsername(currentUser);
                int userGroup = 1;
                
                if (user) 
                {
                    ContestGroup* group = PasswordStrength::getGroupFromPassword(user->getPassword());
                    String groupName = group->getName();
                    delete group;
                    
                    if (groupName == "Easy") userGroup = 0;
                    else if (groupName == "Medium") userGroup = 1;
                    else userGroup = 2;
                }
                                   
                SM.setScreen(new ContestScreen(SM, font, currentUser, contestCards[i].contestType, contestCards[i].contestIndex, userManager, sessionManager, problembank));
                
                return;
            }
        }

        if(deleteUser->isClicked(mx, my))
        {
            showConfirmPopup = true;
        }
    }
}

void dashboard::setupContestCards() 
{
    User* userData = userManager.getUserByUsername(currentUser);
    int userGroup = 1; // default medium
    
    if (userData) {
        ContestGroup* group = PasswordStrength::getGroupFromPassword(userData->getPassword());
        String groupName = group->getName();
        delete group;
        
        if (groupName == "Easy") userGroup = 0;
        else if (groupName == "Medium") userGroup = 1;
        else userGroup = 2;
    }
    
    float startX = 370;
    float startY = 120;
    float cardW = 260;
    float cardH = 140;
    float gap = 20;
    
    // Contest data based on user group
    struct ContestInfo {
        const char* name;
        int duration;
        int contestType; // 0=easy, 1=mid, 2=hard
        int contestIndex; // 0 or 1
    };
    
    ContestInfo contests[3];
    
    if (userGroup == 0) { // Easy group: 2 easy, 1 medium
        contests[0] = {"BEGINNER CHALLENGE", 150, 0, 0};
        contests[1] = {"NOVICE CUP", 150, 0, 1};
        contests[2] = {"CODER'S LEAGUE", 180, 1, 0};
    } 
    else if (userGroup == 1) { // Medium group: 1 easy, 1 medium, 1 hard
        contests[0] = {"BEGINNER CHALLENGE", 150, 0, 0};
        contests[1] = {"ALGORITHM ARENA", 180, 1, 1};
        contests[2] = {"MASTER'S CHALLENGE", 210, 2, 0};
    } 
    else { // Hard group: 2 medium, 1 hard
        contests[0] = {"CODER'S LEAGUE", 180, 1, 0};
        contests[1] = {"GRAND FINALE", 210, 2, 1};
        contests[2] = {"MASTER'S CHALLENGE", 210, 2, 0};
    }
    
    for (int i = 0; i < 3; i++) {
        contestCards[i].slot = i;
        contestCards[i].contestType = contests[i].contestType;
        contestCards[i].contestIndex = contests[i].contestIndex;
        contestCards[i].completed = false;
        
        // Load completion status from file
        char path[256];
        snprintf(path, sizeof(path), "users/%s/contest_%d_completed.dat", currentUser.getstr(), i);
        if (PersistentStorage::fileExists(path)) {
            contestCards[i].completed = true;
        }
        
        contestCards[i].panel.setSize({cardW, cardH});
        contestCards[i].panel.setPosition(startX + i * (cardW + gap), startY);
        contestCards[i].panel.setFillColor(contestCards[i].completed ? sf::Color(150, 100, 120) : sf::Color(200, 100, 150));
        contestCards[i].panel.setOutlineThickness(1);
        contestCards[i].panel.setOutlineColor(sf::Color(255, 200, 220));
        
        contestCards[i].title.setFont(font);
        contestCards[i].title.setString(contests[i].name);
        contestCards[i].title.setCharacterSize(16);
        contestCards[i].title.setFillColor(sf::Color(255, 255, 255));
        contestCards[i].title.setPosition(startX + i * (cardW + gap) + 15, startY + 15);
        
        char durBuf[32];
        snprintf(durBuf, sizeof(durBuf), "%d min", contests[i].duration);
        contestCards[i].duration.setFont(font);
        contestCards[i].duration.setString(durBuf);
        contestCards[i].duration.setCharacterSize(14);
        contestCards[i].duration.setFillColor(sf::Color(255, 200, 150));
        contestCards[i].duration.setPosition(startX + i * (cardW + gap) + 15, startY + 50);
        
        contestCards[i].startBtn = new button(startX + i * (cardW + gap) + 15, startY + 85, 100, 35, 
                                               contestCards[i].completed ? "VIEW" : "START", 
                                               font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    }
}
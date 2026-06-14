#ifndef CODINGSCREEN_H
#define CODINGSCREEN_H

#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "ScreenManager.h"
#include "button.h"
#include "CodeEditor.h"
#include "MessageLabel.h"
#include "Strings.h"
#include "UserManager.h"
#include "SessionManager.h"
#include "problem.h"
#include "ProblemBank.h"
#include "TimerData.h"
#include "ContestManager.h"

class ContestScreen;  // Forward declaration

class codingScreen : public Screen {
private:
    ScreenManager& SM;
    sf::Font& font;
    UserManager& userManager;
    SessionManager& sessionManager;
    String currentUser;
    String problemTitleStr;
    int difficulty;
    int problemIndex;
    ProblemBank* problembank;
    
    Problem* currentProblem;
    String problemDescription, inputFormat, outputFormat, constraints;

    TimerData timerData;
    String getTimerSavePath() const;
    void saveTimerState();
    void loadTimerState();
    int practiceStartTime; 

    // Result popup
    bool showResultPopup;
    sf::RectangleShape popupBg;
    sf::RectangleShape popupPanel;
    sf::Text popupTitle;
    sf::Text popupMessage;
    button* popupOkButton;
    String lastVerdict;
    
    // Exit confirmation popup
    bool showExitConfirm;
    sf::RectangleShape confirmBg;
    sf::RectangleShape confirmPanel;
    sf::Text confirmTitle;
    sf::Text confirmMessage;
    button* confirmYes;
    button* confirmNo;
    
    // UI Panels
    sf::RectangleShape background;
    sf::RectangleShape headerPanel;
    sf::RectangleShape leftPanel;
    sf::RectangleShape rightPanel;
    
    sf::Text problemTitleText;
    button* backButton;
    button* submitButton;
    button* clearButton;
    button* saveButton;
    sf::Text timerText;
    sf::Text modeText;
    
    // Problem description elements
    sf::Text descriptionTitle;
    sf::Text descriptionText;
    sf::Text inputTitle;
    sf::Text inputText;
    sf::Text outputTitle;
    sf::Text outputText;
    sf::Text constraintsTitle;
    sf::Text constraintsText;
    button* testCasesButton;
    int contestSlot;
    
    // Code editor
    CodeEditor* codeEditor;
    ContestScreen* ownerContestScreen;

    MessageLabel* message;
    bool alreadySolved;
    // Timer
    sf::Clock contestClock;
    int timeRemaining;
    int timeElapsed;
    bool timerActive;
    bool isContestMode;
    
    void updateTimerDisplay();

public:
    codingScreen(ScreenManager& manager, sf::Font& f, String user, String problem, int diff, int index, UserManager& um, SessionManager& sm, Problem* prob, ProblemBank* pb, bool contestMode = false);
    ~codingScreen();
    void setOwnerContestScreen(ContestScreen* owner) { ownerContestScreen = owner; }
    void setContestSlot(int slot) { contestSlot = slot; }
    
    void update(float mouseX, float mouseY) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
};

#endif
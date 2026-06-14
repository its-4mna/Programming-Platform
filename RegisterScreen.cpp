#include "RegisterScreen.h"
#include "WelcomeScreen.h"
#include "dashboard.h"
#include "verification.h"

//register screen yahan se naya account banega

RegisterScreen::RegisterScreen(ScreenManager& manager, sf::Font& f, UserManager& um, SessionManager& sm, ProblemBank* pb) :SM(manager), font(f), userManager(um), sessionManager(sm), problembank(pb)
{
    background.setFillColor(sf::Color(240, 235, 240));
    background.setSize({1280, 720});
    
    panel.setFillColor(sf::Color(200, 100, 150));
    panel.setOutlineThickness(1);
    panel.setOutlineColor(sf::Color(50, 60, 100));
    panel.setSize({520, 550});
    panel.setPosition(380, 85);   //pure vibes pr set kr rahi hun inki jagah

    titleText.setFont(font);
    titleText.setCharacterSize(32);
    titleText.setFillColor(sf::Color(220, 230, 255));
    titleText.setString("Welcome !");
    titleText.setPosition(410, 113);   

    subtitleText.setFont(font);
    subtitleText.setCharacterSize(16);
    subtitleText.setFillColor(sf::Color(220, 230, 255));
    subtitleText.setString("Please Register to Continue");
    subtitleText.setPosition(410, 157);  

    //message label neechay buttons ke upar
    message = new MessageLabel(font, 410, 535, 14);  

    //username - pehla field
    labelUsername = message->makeLabel("Username", 410, 195);  
    usernameBox = new TextBoxes(410, 217, 460, 46, "e.g. musa_coder", false, font);

    //email - doosra field
    labelEmail = message->makeLabel("Email", 410, 283); 
    emailBox = new TextBoxes(410, 305, 460, 46, "example@nu.edu.pk", false, font);

    //password - teesra field
    labelPassword = message->makeLabel("Password", 410, 371);  
    passwordBox = new TextBoxes(410, 393, 460, 46, "Min 8 characters", true, font);

    //confirm password - chautha field
    labelConfirm = message->makeLabel("Confirm Password", 410, 459); 
    confirmBox = new TextBoxes(410, 481, 460, 46, "Re-enter password", true, font);

    //register button left side
    registerButton = new button(410, 567, 220, 46, "Register", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));
    
    //back button right side
    backButton = new button(650, 567, 220, 46, "Back", font, sf::Color(255, 200, 220), sf::Color(255, 180, 200));

    if(!starTexture.loadFromFile("assets/star.png"))
    {
        std::cout << "Failed to load star.png" << std::endl;
    }
    starSprite.setTexture(starTexture);
    starSprite.setScale(0.1, 0.1);

}

RegisterScreen::~RegisterScreen()
{
    delete message;
    delete registerButton;
    delete backButton;
    delete usernameBox;
    delete emailBox;
    delete passwordBox;
    delete confirmBox;
}

void RegisterScreen::update(float mouseX, float mouseY)
{
    registerButton->update(mouseX, mouseY);
    backButton->update(mouseX, mouseY);
}

void RegisterScreen::render(sf::RenderWindow& window)
{
    //pehle background phir panel phir baqi sab
    window.draw(background);
    window.draw(panel);

    starSprite.setPosition(80, 100);
    window.draw(starSprite);
    
    starSprite.setPosition(400, 10);
    window.draw(starSprite);
    
    starSprite.setPosition(67, 554);
    window.draw(starSprite);
    
    starSprite.setScale(0.15, 0.15);
    starSprite.setPosition(120, 70);
    window.draw(starSprite);
    
    starSprite.setPosition(900, 600);
    window.draw(starSprite);
        
    starSprite.setScale(0.1, 0.1);
    starSprite.setPosition(290, 350);
    window.draw(starSprite);
    
    starSprite.setPosition(1100, 60);
    window.draw(starSprite);
    
    //starSprite.setPosition(826, 580);
    //window.draw(starSprite);
    
    starSprite.setScale(0.17, 0.17);
    starSprite.setPosition(1070, 400);
    window.draw(starSprite);
    
    starSprite.setScale(0.1, 0.1);
    starSprite.setPosition(884, 30);
    window.draw(starSprite);


    window.draw(titleText);
    window.draw(subtitleText);
    window.draw(labelUsername);
    usernameBox->render(window);
    window.draw(labelEmail);
    emailBox->render(window);
    window.draw(labelPassword);
    passwordBox->render(window);
    window.draw(labelConfirm);
    confirmBox->render(window);
    registerButton->render(window);
    backButton->render(window);
    message->render(window);
}

void RegisterScreen::handleEvent(sf::Event& event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        float mx = event.mouseButton.x;
        float my = event.mouseButton.y;

        //focus management matlab click jis field pe ho woh select ho jaye
        usernameBox->handleClick(mx, my);
        emailBox->handleClick(mx, my);
        passwordBox->handleClick(mx, my);
        confirmBox->handleClick(mx, my);

        //register button daba to validation check karo
        if (registerButton->isClicked(mx, my))
        {
            String user = usernameBox->getText();
            String email = emailBox->getText();
            String pass = passwordBox->getText();
            String confirm = confirmBox->getText();

            //sari fields check kar rahe hain
            if (user.getlen() < 3)
            {
                message->showError("Username must be at least 3 characters.", 3);
                return;
            }
            if (email.getlen() < 5)
            {
                message->showError("Please enter a valid email.", 3);
                return;
            }
            if (pass.getlen() < 8)
            {
                message->showError("Password must be at least 8 characters.", 3);
                return;
            }
            if (pass != confirm)
            {
                message->showError("Passwords do not match.", 3);
                return;
            }

            if (userManager.registerUser(user, email, pass))
            {
                message->showSuccess("Account created!", 2);
                //clear fields after successful registration
                usernameBox->clear();
                emailBox->clear();
                passwordBox->clear();
                confirmBox->clear();
                
                //sessionManager.login(user);
                SM.setScreen(new verification(SM, font, userManager, sessionManager, user, problembank));
                return;
            }
            else
            {
                message->showError("Username or email already exists!", 3);
                return;
            }

            //sab sahi hai to dashboard pe le jao
            //eashal ne kaha tha simple rakho, verification baad mein karenge
            //message->showSuccess("Account created! Welcome!", 2);
            
            //abhi seedha dashboard pe le chalte hain
            //SM.setScreen(new dashboard(SM, font, user));
            //return;
        }

        //back button daba to welcome screen pe wapas
        if (backButton->isClicked(mx, my))
        {
            SM.setScreen(new WelcomeScreen(SM, font, userManager, sessionManager, problembank));
            return;
        }
    }

    //typing handle kar rahe hain - jo field selected hai usme character add hoga
    if (event.type == sf::Event::TextEntered)
    {
        unsigned int unicode = event.text.unicode;
        
        if (unicode == 8) //backspace
        {
            if (usernameBox->isSelected()) 
                usernameBox->handleBackspace();
            if (passwordBox->isSelected()) 
                passwordBox->handleBackspace();
            if(emailBox->isSelected())
                emailBox->handleBackspace();
            if(confirmBox->isSelected())
                confirmBox->handleBackspace();
        }
        else if (unicode >= 32 && unicode < 127) //printable chars
        {
            char c = static_cast<char>(unicode);
            if (usernameBox->isSelected()) 
                usernameBox->handleTypedChar(c);
            if (passwordBox->isSelected()) 
                passwordBox->handleTypedChar(c);
            if(emailBox->isSelected())
                emailBox->handleTypedChar(c);
            if(confirmBox->isSelected())
                confirmBox->handleTypedChar(c);
        }
    }
}
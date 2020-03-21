//Including Libraries
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

//Constants
#define screenWidth 1000
#define screenHeight 650

//Background
sf::Texture backgroundTexture;
sf::Sprite background;

//variable to know which window to render and handle
char session='m';

struct SinglePlayer
{
    // Textures declaration
    sf::Texture p1, p2, ballT, g1, g2;

    // Sprites declaration
    sf::Sprite player1, player2, ball, goal1, goal2;

    void Load()
    {
        // Textures
        p1.loadFromFile("Data/Images/Head1.png");
        p2.loadFromFile("Data/Images/Head2.png");
        g1.loadFromFile("Data/Images/Goal1.png");
        g2.loadFromFile("Data/Images/Goal2.png");
        ballT.loadFromFile("Data/Images/ball.png");

        // Sprites 
        player1.setTexture(p1);
        player2.setTexture(p2);
        ball.setTexture(ballT);
        goal1.setTexture(g1);
        goal2.setTexture(g2);

        //Set Origin
        ball.setOrigin(sf::Vector2f(25, 25));
        player1.setOrigin(sf::Vector2f(35, 35));
        player2.setOrigin(sf::Vector2f(35, 35));
        goal1.setOrigin(sf::Vector2f(50, 90));
        goal2.setOrigin(sf::Vector2f(50, 90));

        //Set Position
        ball.setPosition(sf::Vector2f(500, 100));
        player1.setPosition(sf::Vector2f(120, 550));
        player2.setPosition(sf::Vector2f(880, 550));
        goal1.setPosition(sf::Vector2f(50, 500));
        goal2.setPosition(sf::Vector2f(950, 500));

    }   

    void Logic()
    {
        // Keyboard Movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            player1.move(-0.9f, 0.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            player1.move(0.9f, 0.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            player1.move(0.0f, -0.9f);
    }
    
    void render(sf::RenderWindow &window)
    {
        window.draw(background);
        window.draw(ball);
        window.draw(player1);
        window.draw(player2);
        window.draw(goal1);
        window.draw(goal2);
    }
};

struct Button
{
    sf::Texture frameTexture;
    sf::Vector2u size;
    sf::Font BtnFont;
    sf::Sprite frame;
    sf::Text title;

    void lock()
    {
        frame.setTextureRect(sf::IntRect(size.x * 2 + 30,0,size.x - 28,size.y));
    }
    void clicked()
    {
        frame.setTextureRect(sf::IntRect(0,0,size.x - 27,size.y));
    }
    void notClicked()
    {
         frame.setTextureRect(sf::IntRect(size.x + 12,0,size.x - 27,size.y));
    }

    void create(sf::Vector2f pos,std::string x,float shift = 0)
    {
        //Button Style
        frameTexture.loadFromFile("Data/Images/RecButton.png");
        size = frameTexture.getSize();
        size.x /= 3;
        frame.setTexture(frameTexture);
        notClicked();
        frame.setOrigin(sf::Vector2f(size.x / 2, size.y / 2));
        frame.setPosition(pos);
        frame.setScale(1,0.5);

        ////Text inside button
        BtnFont.loadFromFile("Data/Fonts/fontBtn.ttf");
        title.setFont(BtnFont);
        title.setCharacterSize(50);
        title.setPosition(frame.getPosition().x - (size.x / 6) - shift, frame.getPosition().y - (size.y / 5));
        title.setString(x);
        
    }

    void render(sf::RenderWindow &window)
    {
        window.draw(frame);
        window.draw(title);
    }
};

struct MainMenu
{   
    //Music
    sf::Music menuMusic, btnHover, btnClick;

    //Buttons
    Button newGame, coninue, multi, info, credits;


    //Cursor
    sf::Texture cursorTexture;
    sf::Sprite cursor;

    //Play Music
    void playMusic() 
    {
        menuMusic.play();
    }

    //Creating Objects
    void create()
    {        
        //Create Cursor
        cursorTexture.loadFromFile("Data/Images/cursor.png");
        cursor.setTexture(cursorTexture);
        cursor.setOrigin(cursorTexture.getSize().x / 7, 4);
        cursor.setScale(0.08,0.08);

        //Create Buttons : Divided spaces in screen into 8 Xs and 7 Ys to put buttons in order
        newGame.create(sf::Vector2f(screenWidth / 8 * 4, screenHeight /7 * 2), "Single Player", 40);
        multi.create(sf::Vector2f(screenWidth /8 * 4, screenHeight /7 * 3), "Multiplayer", 40);
        info.create(sf::Vector2f(screenWidth /8 * 4, screenHeight /7 * 4), "Instructions", 50);
        credits.create(sf::Vector2f(screenWidth /8 * 4, screenHeight /7 * 5), "Credits", 7);    
        //coninue.create(sf::Vector2f(screenWidth /8 * 4, screenHeight /7 * 6), "Continue", 26);

        //Load Music
        menuMusic.openFromFile("Data/Sounds/MainMenu.wav");
        btnHover.openFromFile("Data/Sounds/btnHover.wav");
        btnClick.openFromFile("Data/Sounds/btnClick.wav");

    }

    //Logic

    //When mouse hovers over buttons
    void Logic(sf::RenderWindow &window)
    {   
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

        if(newGame.frame.getGlobalBounds().contains(mousePos))
        {
            newGame.clicked();
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                session = 's';
        }
        else
            newGame.notClicked();

        /*if(coninue.frame.getGlobalBounds().contains(mousePos))
           coninue.clicked(); 
        else
            coninue.notClicked(); */
        
        if(multi.frame.getGlobalBounds().contains(mousePos))
        {
            multi.clicked();
        }
        else
            multi.notClicked();
        
        if(info.frame.getGlobalBounds().contains(mousePos))
        {
            info.clicked();
        }
        else
            info.notClicked();

        if(credits.frame.getGlobalBounds().contains(mousePos))
        {
            credits.clicked();  
        }
        else
            credits.notClicked();        
    }


    //Moving Cursor with mouse position
    void moveCursor(sf::RenderWindow &window)
    {
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

        cursor.setPosition(mousePos);
    }

    //Render Cursor
    void renderCursor(sf::RenderWindow &window)
    {
        window.draw(cursor);
    }

    //Rendering
    void render(sf::RenderWindow &window)
    {
        window.draw(background);
        newGame.render(window);
        multi.render(window);
        info.render(window);
        credits.render(window);
        // coninue.render(window);
    }
};

int main()
{
    //Creating window
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Head Soccer", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    
    //Create Background
    backgroundTexture.loadFromFile("Data/Images/Background1.jpg");
    background.setTexture(backgroundTexture);
    
    
    //Main Menu
    MainMenu menu;
    menu.create();

    //Single Player Session
    SinglePlayer NewGame;
    NewGame.Load();

    //Game Loop
    while (window.isOpen())
    {   
        //Events
        sf::Event e;
        while (window.pollEvent(e))
        {
            switch (e.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }
        //Logic
        switch (session)
        {
        case 'm':
            menu.Logic(window);
            break;
        case 's':
            NewGame.Logic();
            break;
        }

        menu.moveCursor(window);

        //Rendering
        window.clear();
        
        switch (session)
        {
        case 'm':
            menu.render(window);
            break;
        case 's':
            NewGame.render(window);
            break;
        }
        
        menu.renderCursor(window);
        window.display();
    }

    return 0;
}

#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;

//Constants
#define screenWidth 1000
#define screenHeight 650

struct SinglePlayer
{
    // Textures declaration
    sf::Texture back, p1, p2, ballT, g1, g2;

    // Sprites declaration
    sf::Sprite player1, player2, background, ball, goal1, goal2;

    void Load()
    {
        // Textures
        back.loadFromFile("Data/Images/Background1.jpg");
        p1.loadFromFile("Data/Images/Head1.png");
        p2.loadFromFile("Data/Images/Head2.png");
        g1.loadFromFile("Data/Images/Goal1.png");
        g2.loadFromFile("Data/Images/Goal2.png");
        ballT.loadFromFile("Data/Images/ball.png");

        // Sprites 
        sf::Sprite player1(p1), player2(p2), background(back), ball(ballT), goal1(g1), goal2(g2);
    }

    void setOrigin()
    {
        ball.setOrigin(Vector2f(25, 25));
        player1.setOrigin(Vector2f(35, 35));
        player2.setOrigin(Vector2f(35, 35));
        goal1.setOrigin(Vector2f(50, 90));
        goal2.setOrigin(Vector2f(50, 90));
    }

    void Position() {
        ball.setPosition(Vector2f(500, 100));
        player1.setPosition(Vector2f(120, 550));
        player2.setPosition(Vector2f(880, 550));
        goal1.setPosition(Vector2f(50, 500));
        goal2.setPosition(Vector2f(950, 500));
    }

    void Logic()
    {
        // Keyboard Movement
        if (Keyboard::isKeyPressed(Keyboard::Key::Left))
            player1.move(-0.1f, 0.0f);
        if (Keyboard::isKeyPressed(Keyboard::Key::Right))
            player1.move(0.1f, 0.0f);
        if (Keyboard::isKeyPressed(Keyboard::Key::Up))
            player1.move(0.0f, -0.1f);
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
    //Buttons
    Button newGame, coninue, multi, info, credits;

    //Background
    sf::Texture backgroundTexture;
    sf::Sprite background;

    //Cursor
    sf::Texture cursorTexture;
    sf::Sprite cursor;

    //Creating Objects
    void create()
    {
        //Create Background
        backgroundTexture.loadFromFile("Data/Images/Background1.jpg");
        background.setTexture(backgroundTexture);
        
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
    }

    //Logic

    //When mouse hovers over buttons
    void blink(sf::RenderWindow &window)
    {   
        Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

        if(newGame.frame.getGlobalBounds().contains(mousePos))
            newGame.clicked(); 
        else
            newGame.notClicked();

        /*if(coninue.frame.getGlobalBounds().contains(mousePos))
           coninue.clicked(); 
        else
            coninue.notClicked(); */
        
        if(multi.frame.getGlobalBounds().contains(mousePos))
            multi.clicked(); 
        else
            multi.notClicked();
        
        if(info.frame.getGlobalBounds().contains(mousePos))
            info.clicked(); 
        else
            info.notClicked();
        
        if(credits.frame.getGlobalBounds().contains(mousePos))
            credits.clicked(); 
        else
            credits.notClicked();        
    }

    //Moving Cursor with mouse position
    void moveCursor(sf::RenderWindow &window)
    {
        Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

        cursor.setPosition(mousePos);
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
        window.draw(cursor);
    }
};

int main()
{
    
    //Creating window
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Head Soccer", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    //Main Menu
    MainMenu menu;
    menu.create();

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
        menu.blink(window);
        menu.moveCursor(window);

        //Rendering
        window.clear();
        menu.render(window);
        window.display();
    }

    return 0;
}

#include <SFML/Graphics.hpp>
#include <string>

//Constants
#define screenWidth 1000
#define screenHeight 650

struct Button
{
    sf::Texture frameTexture;
    sf::Vector2u size;
    sf::Font BtnFont;
    sf::Sprite frame;
    sf::Text title;


    void lock()
    {
        frame.setTextureRect(sf::IntRect(size.x * 2,0,size.x,size.y));
    }
    void clicked()
    {
        frame.setTextureRect(sf::IntRect(0,0,size.x,size.y));
    }
    void notClicked()
    {
         frame.setTextureRect(sf::IntRect(size.x + 12,0,size.x - 20,size.y - 10));
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
    Button newGame,coninue,multi,info,credits;
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
    void blink(sf::Event &e)
    {   
        if(newGame.frame.getGlobalBounds().contains(e.mouseMove.x, e.mouseMove.y))
            newGame.clicked(); 
        else
            newGame.notClicked();

        /*if(coninue.frame.getGlobalBounds().contains(e.mouseMove.x, e.mouseMove.y))
           coninue.clicked(); 
        else
            coninue.notClicked(); */
        
        if(multi.frame.getGlobalBounds().contains(e.mouseMove.x, e.mouseMove.y))
            multi.clicked(); 
        else
            multi.notClicked();
        
        if(info.frame.getGlobalBounds().contains(e.mouseMove.x, e.mouseMove.y))
            info.clicked(); 
        else
            info.notClicked();
        
        if(credits.frame.getGlobalBounds().contains(e.mouseMove.x, e.mouseMove.y))
            credits.clicked(); 
        else
            credits.notClicked();        
    }

    //Moving Cursor with mouse position
    void moveCursor(sf::Event &e)
    {
        cursor.setPosition(e.mouseMove.x, e.mouseMove.y);
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
    sf::RenderWindow window(sf::VideoMode(1000, 650), "Head Soccer", sf::Style::Close | sf::Style::Titlebar);
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
        menu.blink(e);
        menu.moveCursor(e);

        //Rendering
        window.clear();
        menu.render(window);    
        window.display();
    }

    return 0;
}

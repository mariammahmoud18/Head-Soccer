//Including Libraries
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

//Constants
#define screenWidth 1000
#define screenHeight 650
#define groundTop 590

struct Gravity
{	
	bool inAir = 0;
	float dv; //Delta V the acceleration of Gravity
	
	void activate(sf::Sprite& body,sf::Vector2f& bodyV)
	{
		inAir = body.getPosition().y + body.getGlobalBounds().height / 2 < groundTop; //If object is above Air
		
		if(inAir)
		{
			bodyV.y += dv;
			body.move(bodyV);
		}
	}
};

struct Player
{
    //VARIABLES

    bool up=0,down=0,right=0,left=0; //Movement Booleans
    
    //Character
    sf::Texture texture;
    sf::Sprite character;

    sf::Vector2f velocity; //Character current Velocity

    //FUNCTIONS

    void create(std::string path,sf::Vector2f pos)
    {
        texture.loadFromFile(path);
        character.setTexture(texture);
        character.setPosition(pos);
        character.setOrigin(character.getGlobalBounds().width / 2,character.getGlobalBounds().height / 2);
    }

    void move()
    {
        velocity = sf::Vector2f();
        float maxY =410.0f;
        float currentTopPos = character.getGlobalBounds().top;
        float currentBottomPos = currentTopPos + character.getGlobalBounds().height;
        float currentLeftPos = character.getGlobalBounds().left; //character.getPosition().y - character.getGlobalBounds().width / 2;
        float currentRightPos = currentLeftPos + character.getGlobalBounds().width;

        //Player1 COntrols
        if(up && currentTopPos > maxY)
			velocity.y -= 5.0f;
		else up =0;

        if(down && currentBottomPos < groundTop) velocity.y = 5.0f;
        if(right && currentRightPos < screenWidth) velocity.x = 5.0f;
        if(left && currentLeftPos > 0) velocity.x = -5.0f;

        if( !(right || left) ) velocity.x = 0;

        //Movement Action
        character.move(velocity);
    }

    bool stopCollision(sf::Sprite& body)
    {
        //Collisions
        if(character.getGlobalBounds().intersects(body.getGlobalBounds())) //Collision with body
        {
            character.move(-velocity); //Stop Player
            return true;
        }
        return false;
    }
    
    //pressed button
    void upPressed()
    {
        if(character.getPosition().y + character.getGlobalBounds().height /2 >= groundTop)
            up=1;
    }

    void downPressed()
    {
        down=1;
    }

    void rightPressed()
    {
        right=1;
    }

    void leftPressed()
    {
        left=1;
    }

    //Released button

    void upRealesed(sf::RectangleShape& ground)
    {
        up=0;
    }

    void downRealesed()
    {
        down=0;
    }

    void rightRealesed()
    {
        right=0;
    }

    void leftRealesed()
    {
        left=0;
    }
};

struct Button
{
    sf::Texture frameTexture;
    sf::Vector2f size;
    sf::Font BtnFont;
    sf::Sprite frame;
    sf::Text title;

    bool inside=0;

    void lock()
    {
        frame.setTextureRect(sf::IntRect(static_cast<int>(size.x) * 2 + 30,0, static_cast<int>(size.x) - 28, static_cast<int>(size.y)));
    }
    void notClicked()
    {
        frame.setTextureRect(sf::IntRect(0,0, static_cast<int>(size.x) - 27, static_cast<int>(size.y)));
    }
    void clicked()
    {
         frame.setTextureRect(sf::IntRect(static_cast<int>(size.x) + 12,0, static_cast<int>(size.x) - 27, static_cast<int>(size.y)));
    }

    void create(sf::Vector2f pos,std::string x)
    {
        //Button Style
        frameTexture.loadFromFile("Data/Images/RecButton.png");
        size = sf::Vector2f(static_cast<float>(frameTexture.getSize().x), static_cast<float>(frameTexture.getSize().y));
        size.x /= 3;
        frame.setTexture(frameTexture);
        notClicked();
        frame.setScale(1,0.5);
        frame.setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
        frame.setPosition(pos);

        ////Text inside button
        BtnFont.loadFromFile("Data/Fonts/fontBtn.ttf");
        title.setFont(BtnFont);
        title.setCharacterSize(50);
        title.setString(x);
        title.setOrigin(title.getLocalBounds().width /2, title.getGlobalBounds().height / 2);
        title.setPosition(frame.getPosition().x, frame.getPosition().y - 15);
    }

    void render(sf::RenderWindow& window)
    {
        window.draw(frame);
        window.draw(title);
    }
};

struct MainMenu
{   
    /////////////////VARIABLES
    
    //Music
    sf::Music menuMusic;

    //Sounds
    sf::SoundBuffer btnHoverbufr, btnClickbufr;
    sf::Sound btnHover, btnClick;

    //Buttons
    static const int noOfBtns=4;
    Button btn[noOfBtns];
    std::string btnTitle[noOfBtns] ={"Single Player", "Multiplayer", "Instructions", "Credits"};
    char btnSession[noOfBtns] = {'s', 'm', 'i', 'c'};
    //Cursor
    sf::Texture cursorTexture;
    sf::Sprite cursor;

    
    /////////////////FUNCTIONS

    //Creating Objects
    void create()
    {        
        //Create Cursor
        cursorTexture.loadFromFile("Data/Images/cursor.png");
        cursor.setTexture(cursorTexture);
        cursor.setOrigin(cursorTexture.getSize().x / 7.0f, 4.0f);
        cursor.setScale(0.08f,0.08f);

        //Create Buttons : Divided spaces in screen into 8 Xs and 7 Ys to put buttons in order
        for (char i = 0; i < 4; i++)
        {
            btn[i].create(sf::Vector2f(screenWidth /8 * 4, screenHeight /6.5 * (i+3) ), btnTitle[i]);
        }

        //Load and Play Music
        menuMusic.openFromFile("Data/Sounds/MainMenu.wav");
        menuMusic.setLoop(true);
        menuMusic.play();

        //Load Sounds
        btnHoverbufr.loadFromFile("Data/Sounds/btnHover.wav");
        btnHover.setBuffer(btnHoverbufr);
        btnClickbufr.loadFromFile("Data/Sounds/btnClick.wav");
        btnClick.setBuffer(btnClickbufr);
    }

    //Logic

    //When mouse hovers over buttons
    void Logic(sf::RenderWindow& window, char& session,sf::Vector2f& mousePos)
    {   
        //Buttons Hovered or Clicked Actions

        for (int i = 0; i < noOfBtns; i++)
        {
            if(btn[i].frame.getGlobalBounds().contains(mousePos))
            {
                if (!btn[i].inside)
                {
                    btn[i].clicked();
                    btnHover.play();
                    btn[i].inside = 1;
                }
                
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    btnClick.play();
                    session = btnSession[i];
                    menuMusic.stop();
                }
                
            }
            else
            {
                if (btn[i].inside)
                    btn[i].notClicked();
                btn[i].inside = 0;
                
            }
            
        }
    }

    //Moving Cursor with mouse position
    void moveCursor(sf::Vector2f& mousePos)
    {
        cursor.setPosition(mousePos);
    }

    //Render Cursor
    void renderCursor(sf::RenderWindow& window)
    {
        window.draw(cursor);
    }

    //Rendering
    void render(sf::RenderWindow& window)
    {
        for (int i = 0; i < noOfBtns; i++)
        {
            btn[i].render(window);
        }
    }
};

struct Match
{
    ////VARIABLES

    // Textures declaration
    sf::Texture g1,g2,ballT;

    // Bodies declaration
    sf::Sprite ball, goal1, goal2;
    sf::RectangleShape ground;
    Player player1,player2;
    
    // gravity
    Gravity playersG,ballG;

    //Sounds
    sf::SoundBuffer touchBallbuff;
    sf::Sound touchBall;

    ////FUNCTIONS

    void create()
    {
        //Ground
        ground.setSize(sf::Vector2f(screenWidth,1));
        ground.setOrigin(0,0.5);
        ground.setPosition(0,groundTop);
        ground.setFillColor(sf::Color::Transparent);
        
        // Players
        player1.create("Data/Images/Head1.png", sf::Vector2f(120, 550));
        player2.create("Data/Images/Head2.png", sf::Vector2f(880, 550));
        
        playersG.dv = 5.0f;
        
        //Ball
        ballT.loadFromFile("Data/Images/ball.png");
        ball.setTexture(ballT);
        ball.setOrigin(sf::Vector2f(25, 25));
        ball.setPosition(sf::Vector2f(500, 100));
        ballG.dv = 10.0f;
        
        //Goals
        g1.loadFromFile("Data/Images/Goal1.png");
        goal1.setTexture(g1);
        goal1.setOrigin(sf::Vector2f(50, 90));
        goal1.setPosition(sf::Vector2f(50, 500));
        
        g2.loadFromFile("Data/Images/Goal2.png");
        goal2.setTexture(g2);
        goal2.setOrigin(sf::Vector2f(50, 90));
        goal2.setPosition(sf::Vector2f(950, 500));

        //Sounds
        touchBallbuff.loadFromFile("Data/Sounds/Kick.wav");
        touchBall.setBuffer(touchBallbuff);

    }   
    
    //Logic

    void SingleLogic()
    {
        sf::Vector2f ballV;

        //Movement Control
        player1.move();
        player2.move();

        //Gravity
        playersG.activate(player1.character,player1.velocity);
        playersG.activate(player2.character,player2.velocity);
        ballG.activate(ball,ballV);
        
        //Collisions
        if(player1.stopCollision(ball) || player2.stopCollision(ball))
            touchBall.play();

        player1.stopCollision(player2.character);
    }
    
    //Rendering
    void render(sf::RenderWindow& window)
    {
        window.draw(ground);
        window.draw(ball);
        window.draw(player1.character);
        window.draw(player2.character);
        window.draw(goal1);
        window.draw(goal2);
    }
};

void loadScreen(sf::RenderWindow& window)
{
    sf::Text title;
    sf::Font titlefnt;
    titlefnt.loadFromFile("Data/Fonts/fontBtn.ttf");
    title.setFont(titlefnt);
    title.setString("Loading");
    title.setCharacterSize(100);
    title.setOrigin(title.getGlobalBounds().width / 2,title.getGlobalBounds().height / 2); 
    title.setPosition(screenWidth/2,screenHeight/2);
    window.clear(sf::Color::Black);
    window.draw(title);
    window.display();
}

int main()
{
    //variables
    char session='d'; // to know which screen to render and handle
    sf::Vector2f mousePos; //to save current mouse position

    //Creating window
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Head Soccer", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    
    //Loading Screen till Resources Loads
    loadScreen(window);

    //Background
    sf::Texture backgroundTexture;
    sf::Sprite background;
    
    //Create Background
    backgroundTexture.loadFromFile("Data/Images/Background1.jpg");
    background.setTexture(backgroundTexture);
    
    //Main Menu
    MainMenu menu;
    menu.create();

    //Single Player Session
    Match Game;
    Game.create();

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
                case sf::Event::MouseMoved:
                    mousePos = sf::Vector2f(static_cast<float>(e.mouseMove.x), static_cast<float>(e.mouseMove.y));
                    break;
                case sf::Event::KeyPressed:
                    switch (e.key.code)
                    {
                    case sf::Keyboard::Up:
                        Game.player1.upPressed();
                        break;
                    case sf::Keyboard::Down:
                        Game.player1.downPressed();
                        break;
                    case sf::Keyboard::Right:
                        Game.player1.rightPressed();
                        break;
                    case sf::Keyboard::Left:
                        Game.player1.leftPressed();
                        break;
                    }
                    break;
                case sf::Event::KeyReleased:
                    switch (e.key.code)
                    {
                     case sf::Keyboard::Down:
                        Game.player1.downRealesed();
                        break;
                    case sf::Keyboard::Right:
                        Game.player1.rightRealesed();
                        break;
                    case sf::Keyboard::Left:
                        Game.player1.leftRealesed();
                        break;
                    }
                    break;
            }
        }

        //Logic
        switch (session)
        {
        case 'd':
            menu.Logic(window, session, mousePos);
            break;
        case 's':
            Game.SingleLogic();
            break;
        }

        menu.moveCursor(mousePos); //Move Cursor with the mouse

        //Rendering
        window.clear();
        window.draw(background);
        
        switch (session)
        {
        case 'd': //Default which is main menu
            menu.render(window);
            break;
        case 's': //Single Player
            Game.render(window);
            break;
        case 'm': //Multiplayer
            break;
        case 'i': //Instructions
            break;
        case 'c': //Credits
            break;
        }
        
        menu.renderCursor(window);
        window.display();
    }

    return 0;
}
//Including Libraries
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

//Constants
#define screenWidth 1000
#define screenHeight 650

struct Gravity
{
	sf::RectangleShape ground;
	
	bool inAir = 0;
	float maxV,dv;
	
	void activate(sf::Sprite& body,sf::Vector2f& bodyV)
	{
		inAir = body.getPosition().y + body.getGlobalBounds().height / 2 <= ground.getPosition().y - ground.getGlobalBounds().height /2;
		
		if(inAir)
		{
			if(bodyV.y < maxV) bodyV.y += dv;
			body.move(bodyV);
		}
	}

	void setGround(sf::RectangleShape& body,float deltaV=1.5, float maxVelocity=10)
	{
		ground = body;
        maxV = maxVelocity;
        dv = deltaV;

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
        //menuMusic.play();

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
        //Single Player Button Hovered or Clicked Actions

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
    void render(sf::RenderWindow& window,sf::Sprite& background)
    {
        window.draw(background);
        for (int i = 0; i < noOfBtns; i++)
        {
            btn[i].render(window);
        }
        
        // coninue.render(window);
    }
};

struct Match
{
    ////VARIABLES
    
    //Movement Booleans
    bool up=0,down=0,right=0,left=0;

    // Textures declaration
    sf::Texture p1, p2, ballT, g1, g2;

    // Bodies declaration
    sf::Sprite player1, player2, ball, goal1, goal2;
    sf::RectangleShape ground;

    //Bodies Velcoity
    sf::Vector2f player1V, player2V, ballV;
    
    //Bodies gravity
    Gravity playersG,ballG;

    ////FUNCTIONS

    void Load()
    {
        //Ground
        ground.setSize(sf::Vector2f(screenWidth,1));
        ground.setOrigin(0,0.5);
        ground.setPosition(0,590);
        ground.setFillColor(sf::Color::Transparent);
        
        // Players
        p1.loadFromFile("Data/Images/Head1.png");
        player1.setTexture(p1);
        player1.setOrigin(sf::Vector2f(35, 35));
        player1.setPosition(sf::Vector2f(120, 100));
         
        p2.loadFromFile("Data/Images/Head2.png");
        player2.setTexture(p2);
        player2.setOrigin(sf::Vector2f(35, 35));
        player2.setPosition(sf::Vector2f(880, 550));
        
        playersG.setGround(ground, 5.0f);
        
        //Ball
        ballT.loadFromFile("Data/Images/ball.png");
        ball.setTexture(ballT);
        ball.setOrigin(sf::Vector2f(25, 25));
        ball.setPosition(sf::Vector2f(500, 100));
        ballG.setGround(ground, 10, 100);
        
        //Goals
        g1.loadFromFile("Data/Images/Goal1.png");
        goal1.setTexture(g1);
        goal1.setOrigin(sf::Vector2f(50, 90));
        goal1.setPosition(sf::Vector2f(50, 500));
        
        g2.loadFromFile("Data/Images/Goal2.png");
        goal2.setTexture(g2);
        goal2.setOrigin(sf::Vector2f(50, 90));
        goal2.setPosition(sf::Vector2f(950, 500));

    }   
    
    //Logic
    
    void jump()
    {
        if(player1.getPosition().y + player1.getGlobalBounds().height /2 >= ground.getPosition().y - ground.getGlobalBounds().height /2)
            up=1;
    }

    void SingleLogic()
    {
        player1V =sf::Vector2f(0,0);
        player2V =sf::Vector2f(0,0);
        ballV =sf::Vector2f(0,0);

        //Movement Control

        //Player1 COntrols
        if(up && player1.getPosition().y - player1.getGlobalBounds().height / 2 > 410)
			player1V.y -= 5.0f;
		else up =0;
        if(down) player1V.y = 5.0f;
        if(right) player1V.x = 5.0f;
        if(left) player1V.x = -5.0f;
        
        if( !(right || left) ) player1V.x = 0;

        //Movement Action
        player1.move(player1V);

        //Gravity
        playersG.activate(player1,player1V);
        playersG.activate(player2,player2V);
        ballG.activate(ball,ballV);
        
        //Collisions

        if(player1.getGlobalBounds().intersects(ball.getGlobalBounds())) //Collision with Ball
        {
            player1.move(-player1V); //Stop Player
        }
    }
    
    //Rendering
    void render(sf::RenderWindow& window, sf::Sprite& background)
    {
        window.draw(background);
        window.draw(ground);
        window.draw(ball);
        window.draw(player1);
        window.draw(player2);
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
    char session='d'; // to know which window to render and handle
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
    Game.Load();

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
                        Game.jump();
                        break;
                    case sf::Keyboard::Down:
                        Game.down=1;
                        break;
                    case sf::Keyboard::Right:
                        Game.right=1;
                        break;
                    case sf::Keyboard::Left:
                        Game.left=1;
                        break;
                    }
                    break;
                case sf::Event::KeyReleased:
                    switch (e.key.code)
                    {
                     case sf::Keyboard::Down:
                        Game.down=0;
                        break;
                    case sf::Keyboard::Right:
                        Game.right=0;
                        break;
                    case sf::Keyboard::Left:
                        Game.left=0;
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
        //Move Cursor with the mouse
        menu.moveCursor(mousePos);

        //Rendering
        window.clear();
        
        switch (session)
        {
        case 'd': //Default which is main menu
            menu.render(window, background);
            break;
        case 's': //Single Player
            Game.render(window, background);
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
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

struct Button
{
    private:

    sf::Texture shape;
    sf::Vector2u size;
    sf::Font BtnFont;

    public:
    sf::Sprite _shape;
    sf::Text Title;

    void load()
    {
        shape.loadFromFile("Data/Images/RecButton.png");
        BtnFont.loadFromFile("Data/Fonts/fontBtn.ttf");
        size = shape.getSize();
        size.x /= 3;
        _shape.setTexture(shape);
        _shape.setTextureRect(sf::IntRect(size.x,0,size.x,size.y));
        _shape.setOrigin(sf::Vector2f(size.x / 2, size.y / 2));
        Title.setFont(BtnFont);
        // Title.setColor(sf::Color::Black);
        Title.setCharacterSize(40);
        Title.setPosition(_shape.getPosition().x, _shape.getPosition().y);
    }
    
};

int main()
{
    //Creating window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Head Soccer", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    
    //Buttons
    Button FirstButton;
    FirstButton._shape.setPosition(400,300);
    FirstButton.load();
    FirstButton.Title.setString("Hello");
    //Game Loop
    while (window.isOpen())
    {   
        //Events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //Logic
        
        //Rendering
        window.clear();
        window.draw(FirstButton._shape);
        window.draw(FirstButton.Title);
        window.display();
    }

    return 0;
}
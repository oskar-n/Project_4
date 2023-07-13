
#include <SFML/Graphics.hpp>
#include <iostream>

constexpr int SCREEN_WIDTH   =1500;
constexpr int SCREEN_HEIGHT  = 800;
constexpr int BUTTON_WIDTH   =50;
constexpr int ELEVATOR_WIDTH =200;
constexpr int ELEVATOR_HIGHT =100;

class Elevator
{
    private:
    int m_x=0;
    int m_y=0;
    int m_width=ELEVATOR_WIDTH;
    int m_hight=ELEVATOR_HIGHT;
    sf::Color m_color;
    sf::RectangleShape m_rectangle;
    public:
    Elevator(int x, int y, sf::Color color)
    {
        m_x=x;
        m_y=y;
        m_color=color;
        m_rectangle.setSize(sf::Vector2f(m_width, m_hight));
        m_rectangle.setPosition(m_x, m_y);
    }
    
    void moving ()
    {   
    sf::sleep(sf::milliseconds(2)); 
    if (m_rectangle.getPosition().y>m_y)
    {
        m_rectangle.move(0,-1);
    }
    if (m_rectangle.getPosition().y<m_y)
    {
        m_rectangle.move(0,1);
    }
    
    }
    void set_pos(int x, int y)
    {   
        m_x=x;
        m_y=y;
    }

    void draw(sf::RenderWindow &window)
     {
        m_rectangle.setSize(sf::Vector2f(m_width, m_width));
        m_rectangle.setFillColor(m_color);
        m_rectangle.setOutlineColor(sf::Color::Yellow);
        m_rectangle.setOutlineThickness(5);
        window.draw(m_rectangle);
    }

};

class Button{
    private: 
    int m_pos_x;
    int m_pos_y;
    int m_width=BUTTON_WIDTH;
    sf::Color m_color;
    sf::RectangleShape m_rectangle;
    sf::Text m_text;
    sf::Font m_font;
    public:

    Button(int pos_x, int pos_y, sf::Color color,  char num)
    {
        m_pos_x=pos_x;
        m_pos_y=pos_y;
        m_color=color;
        m_rectangle.setSize(sf::Vector2f(m_width, m_width));
        if (!m_font.loadFromFile("digital-7.ttf"))
        {
            std::cout<<"Error loading font"<<std::endl;
        }
        m_text.setFont(m_font);
        m_text.setCharacterSize(50);
        m_text.setFillColor(sf::Color::White);
        m_text.setString(num);
    }

    void draw(sf::RenderWindow &window)
    {
        m_rectangle.setFillColor(m_color);
        m_rectangle.setPosition(m_pos_x, m_pos_y);
        m_text.setPosition(m_pos_x+m_width/2, m_pos_y);
        window.draw(m_rectangle);
        window.draw(m_text);
    }
    
    void is_pressed(sf::RenderWindow &window, Elevator &m_elevator, int post_y)
    {
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
        if (mouse_pos.x>m_pos_x && mouse_pos.x<m_pos_x+m_width && mouse_pos.y>m_pos_y && mouse_pos.y<m_pos_y+m_width)
        {
            m_color=sf::Color::Green;
            m_elevator.set_pos(SCREEN_WIDTH/2-ELEVATOR_WIDTH, post_y);
        }
        
    }
};


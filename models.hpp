
#include <SFML/Graphics.hpp>
#include <iostream>
#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 800
#define BUTTON_WIDTH 50
#define ELEVATOR_WIDTH 200
#define ELEVATOR_HIGHT 100


class elevator
{
    private:
    int x_=0;
    int y_=0;
    int m_width=ELEVATOR_WIDTH;
    int m_hight=ELEVATOR_HIGHT;
    sf::Color m_color;
    sf::RectangleShape m_rectangle;
    public:
    elevator(int x, int y, sf::Color color)
    {
        x_=x;
        y_=y;
        m_color=color;
        m_rectangle.setSize(sf::Vector2f(m_width, m_hight));
        m_rectangle.setPosition(x_, y_);
    }
    
    void moving ()
    {   
    sf::sleep(sf::milliseconds(2)); 
    if (m_rectangle.getPosition().y>y_)
    {
        m_rectangle.move(0,-1);
    }
    if (m_rectangle.getPosition().y<y_)
    {
        m_rectangle.move(0,1);
    }
    
    }
    void set_pos(int x, int y)
    {   
        x_=x;
        y_=y;
    }

    void draw_elevator(sf::RenderWindow &window)
     {
        m_rectangle.setSize(sf::Vector2f(m_width, m_width));
        m_rectangle.setFillColor(m_color);
        m_rectangle.setOutlineColor(sf::Color::Yellow);
        m_rectangle.setOutlineThickness(5);
        window.draw(m_rectangle);
    }

};

class button{
    private: 
    int m_pos_x;
    int m_pos_y;
    int m_width=BUTTON_WIDTH;
    sf::Color m_color;
    sf::RectangleShape m_rectangle;
    sf::Text m_text;
    sf::Font m_font;
    public:
    button(int pos_x, int pos_y, sf::Color color,  char num)
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

    void draw_button(sf::RenderWindow &window)
    {
        m_rectangle.setFillColor(m_color);
        m_rectangle.setPosition(m_pos_x, m_pos_y);
        m_text.setPosition(m_pos_x+m_width/2, m_pos_y);
        window.draw(m_rectangle);
        window.draw(m_text);
    }
    
    void is_pressed(sf::RenderWindow &window, elevator &m_elevator, int post_y)
    {
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
        if (mouse_pos.x>m_pos_x && mouse_pos.x<m_pos_x+m_width && mouse_pos.y>m_pos_y && mouse_pos.y<m_pos_y+m_width)
        {
            m_color=sf::Color::Green;
            m_elevator.set_pos(SCREEN_WIDTH/2-ELEVATOR_WIDTH, post_y);
        }
        
    }
};


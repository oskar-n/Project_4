
#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>

constexpr int SCREEN_WIDTH   =1500;
constexpr int SCREEN_HEIGHT  =800;
constexpr int BUTTON_WIDTH   =50;
constexpr int ELEVATOR_WIDTH =200;
constexpr int ELEVATOR_HIGHT =100;

template <typename... Args>
void draw(sf::RenderWindow &win, Args... args){
    (args.draw(win), ...);
}

class Elevator
{
    private:
        int m_x=0;
        int m_y=0;
        int m_width=ELEVATOR_WIDTH;
        int m_hight=ELEVATOR_HIGHT;
        bool m_is_moving=false;
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
            if(m_rectangle.getPosition().y == m_y)
            {
                m_is_moving=false;
                return;
            }
            m_is_moving=true;
        }

        bool is_moving()
        {
            return m_is_moving;
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
        int m_goal;
        Button(int pos_x, int pos_y,int goal, sf::Color color,  char num)
        {
            m_pos_x=pos_x;
            m_pos_y=pos_y;
            m_color=color;
            goal=goal;
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

        void make_red(){
            m_color=sf::Color::Red;
        }

        void draw(sf::RenderWindow &window)
        {
            m_rectangle.setFillColor(m_color);
            m_rectangle.setPosition(m_pos_x, m_pos_y);
            m_text.setPosition(m_pos_x+m_width/2, m_pos_y);
            window.draw(m_rectangle);
            window.draw(m_text);
        }

        bool is_pressed(sf::RenderWindow &window)
        {
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            if (mouse_pos.x>m_pos_x && mouse_pos.x<m_pos_x+m_width && mouse_pos.y>m_pos_y && mouse_pos.y<m_pos_y+m_width)
            {
                m_color=sf::Color::Green;
                return true;
            }
            return false;
        }
};

struct move{
    int x;
    int y;
    Button &b;
};

class ObjectManager{
    Elevator m_elevator{SCREEN_WIDTH/2-ELEVATOR_WIDTH,
                        SCREEN_HEIGHT-ELEVATOR_HIGHT*2-5, sf::Color::Blue};
    Button m_button1{50,50,70,sf::Color::Red, '1'};
    Button m_button2{50,150,320,sf::Color::Red, '2'};
    sf::RenderWindow m_window{sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Elevator"};
    std::queue<move> m_orders{};
    public:

    void loop(){
        while (m_window.isOpen())
        {
            sf::Event event;
            while (m_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    m_window.close();
                if (event.type==sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    if (m_button1.is_pressed(m_window))  m_orders.push({0,m_button1.m_goal, m_button1});
                    if (m_button2.is_pressed(m_window))  m_orders.push({0,m_button2.m_goal, m_button2}); 
                }
            }
            if(!m_orders.empty())
            {
                m_elevator.set_pos(m_orders.front().x, m_orders.front().y);
            }
            m_elevator.moving();
            if (!m_elevator.is_moving() && !m_orders.empty())
            {
                m_orders.front().b.make_red();
                m_orders.pop();
            }
            m_window.clear(sf::Color(255, 255, 255));
            draw(m_window,m_elevator, m_button1, m_button2);
            m_window.display();
        }
    }

};


#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include <array>
#include <type_traits>

constexpr int SCREEN_WIDTH   =1500;
constexpr int SCREEN_HEIGHT  =1200;
constexpr int BUTTON_WIDTH   =50;
constexpr int ELEVATOR_WIDTH =290;
constexpr int ELEVATOR_HIGHT =190;
constexpr int FLOOR_HIGHT =40;

template <typename T>
void draw(T &Object, sf::RenderWindow &window){
    Object.draw(window);
}

template <typename T>
void draw(std::array<T,5> Object_cont, sf::RenderWindow &window){
    for (auto &i: Object_cont)
    {
        draw(i,window);
    }
}

template <typename... Args>
void draw(sf::RenderWindow &win, Args... args){
    (draw(args, win), ...);
}

enum FLOORS{
    FIRST=950,
    SECOND=750,
    THIRD=550,
    FOURTH=350,
    FIFTH=150,
};

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
        bool is_reached_beg=false;
        bool is_reached_goal=false;
        Elevator(int x, int y, sf::Color color)
        {
            m_x=x;
            m_y=y;
            m_color=color;
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
            m_rectangle.setSize(sf::Vector2f(m_width, m_hight));
            m_rectangle.setFillColor(m_color);
            m_rectangle.setOutlineColor(sf::Color::Yellow);
            m_rectangle.setOutlineThickness(5);
            window.draw(m_rectangle);
        }

        void reach_check()
        {
            
            if (m_rectangle.getPosition().y==m_y && is_reached_beg==true)
            {
                is_reached_goal=true;
                
            }
            if (m_rectangle.getPosition().y==m_y)
            {
                is_reached_beg=true;
                sf::sleep(sf::milliseconds(700));
            }
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
        int m_dest;
        Button(int pos_x, int pos_y, int goal, sf::Color color,  char num)
        {
            m_pos_x=pos_x;
            m_pos_y=pos_y;
            m_color=color;
            m_goal=goal;
            m_dest=pos_y;
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
            m_text.setPosition(m_pos_x+m_width/2, m_pos_y-10);
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
    int dest_floor;
    int goal_floor;
    Button &b;
};


using Buttongroup = std::array<Button, 5>; 

inline Buttongroup make_buttons(int pos_x, int pos_y, sf::Color color){
    return Buttongroup{
        Button(pos_x, pos_y, FLOORS::FIRST, color, '1'),
        Button(pos_x + BUTTON_WIDTH, pos_y, FLOORS::SECOND, color, '2'),
        Button(pos_x + BUTTON_WIDTH * 2, pos_y, FLOORS::THIRD, color, '3'),
        Button(pos_x + BUTTON_WIDTH * 3, pos_y, FLOORS::FOURTH, color, '4'),
        Button(pos_x + BUTTON_WIDTH * 4, pos_y, FLOORS::FIFTH, color, '5')
    };
}

inline void draw_buttons(Buttongroup &buttons, sf::RenderWindow &window){
    for (auto &b : buttons)
    {
        b.draw(window);
    }
}

inline void buttongr_pressed(Buttongroup &bg, sf::RenderWindow &window, std::queue<move> &orders){
    for (auto &b : bg)
    {
        if (b.is_pressed(window))
        {
            orders.push({b.m_goal, b.m_dest, b});
        }
    }    
}

class Floors{
    private: 
    int m_pos_x;
    int m_width=(SCREEN_WIDTH-ELEVATOR_WIDTH-10)/2;
    int m_hight=FLOOR_HIGHT;
    sf::Color m_color;
    sf::RectangleShape m_rectangle_1;
    sf::RectangleShape m_rectangle_2;
    sf::RectangleShape m_rectangle_3;
    sf::RectangleShape m_rectangle_4;
    sf::RectangleShape m_rectangle_5;
    public:
    Floors(int pos_x, sf::Color color): m_pos_x(pos_x), m_color(color){}

    void draw_one_floor(sf::RenderWindow &window, sf::RectangleShape &rectangle, bool is_left, int pos_y)
    {
        rectangle.setSize(sf::Vector2f(m_width, m_hight));
        rectangle.setFillColor(m_color);
        if(is_left)rectangle.setPosition(m_pos_x, pos_y+ELEVATOR_HIGHT);
        else rectangle.setPosition(SCREEN_WIDTH-m_pos_x-m_width, pos_y+ELEVATOR_HIGHT);
        window.draw(rectangle);
    }
    void draw(sf::RenderWindow &window)
    {
        draw_one_floor(window, m_rectangle_1, true, FLOORS::FIRST);
        draw_one_floor(window, m_rectangle_2, false, FLOORS::SECOND);
        draw_one_floor(window, m_rectangle_3, true, FLOORS::THIRD);
        draw_one_floor(window, m_rectangle_4, false, FLOORS::FOURTH);
        draw_one_floor(window, m_rectangle_5, true, FLOORS::FIFTH);
    }


};

class ObjectManager{
    Elevator m_elevator{SCREEN_WIDTH/2-ELEVATOR_WIDTH/2,
                        FLOORS::FIRST, sf::Color::Blue};
    std::array<Buttongroup, 5> m_buttongroups = {
        make_buttons(50, FLOORS::FIRST,sf::Color::Red),
        make_buttons(SCREEN_WIDTH-50-BUTTON_WIDTH*5, FLOORS::SECOND,sf::Color::Red),
        make_buttons(50, FLOORS::THIRD,sf::Color::Red),
        make_buttons(SCREEN_WIDTH-50-BUTTON_WIDTH*5, FLOORS::FOURTH,sf::Color::Red),
        make_buttons(50, FLOORS::FIFTH,sf::Color::Red)
    };
    Floors m_floors{0, sf::Color::Blue};
    sf::RenderWindow m_window{sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Elevator"};
    std::queue<move> m_orders{};
    public:

    void loop(){
        while (m_window.isOpen())
        {
            auto cursor = sf::Mouse::getPosition(m_window);
            std::cout << cursor.x << " | "<< cursor.y << '\n';
            sf::Event event;
            while (m_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    m_window.close();
                if (event.type==sf::Event::KeyPressed && event.key.scancode == sf::Keyboard::Scan::Escape )
                    m_window.close();
                if (event.type==sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    for(auto i : m_buttongroups){
                        buttongr_pressed(i,m_window, m_orders);
                    }
                }

            }
            if(!m_orders.empty())
            {
                
                if(m_elevator.is_reached_beg==false){
                m_elevator.set_pos(0, m_orders.front().dest_floor);
                }
                else {
                    m_elevator.set_pos(0, m_orders.front().goal_floor);
                }
                m_elevator.reach_check(); 
            }
            m_elevator.moving();
            std::cout<<"             "<<m_elevator.is_reached_beg<<"   "<<m_elevator.is_reached_goal<<std::endl;
            if (!m_elevator.is_moving() && !m_orders.empty() && m_elevator.is_reached_beg==true)
            {
                m_orders.front().b.make_red();
            }
            if (!m_elevator.is_moving() && !m_orders.empty() && m_elevator.is_reached_goal==true)
            {
                m_elevator.is_reached_beg=false;
                m_elevator.is_reached_goal=false;
                m_orders.pop();
            }
            m_window.clear(sf::Color(255, 255, 255));
            draw(m_window,m_elevator, m_buttongroups, m_floors);
            m_window.display();
        }
    }

};


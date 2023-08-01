#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include <array>

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
void draw(std::array<T,5> &Object_cont, sf::RenderWindow &window){
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
         bool m_should_move=true;
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
            if(m_should_move)
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
            };

        }

        bool is_moving()
        {
            return m_is_moving;
        }

        bool shoud_move()
        {
            return m_should_move;
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
            }
        }

        void elevator_pause(){
            static int i=1;
            if(i%500==0)
            {
                i=0;
                m_should_move=true;
            }
            else 
            {
               m_should_move=false;
            }
            i++;
            
            
           std::cout<<i<<"||"<<m_should_move<<std::endl;
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
        int m_beg;
        int m_goal;
        Button(int pos_x, int pos_y, int goal, sf::Color color,  char num)
        {
            m_pos_x=pos_x;
            m_pos_y=pos_y;
            m_color=color;
            m_goal=goal;
            m_beg=pos_y;
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
    int beg_floor;
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
            orders.push({b.m_beg, b.m_goal, b});
        }
    }    
}

struct Floor {
    int m_width = (SCREEN_WIDTH-ELEVATOR_WIDTH-10)/2;
    int m_hight = FLOOR_HIGHT;
    bool is_left;
    int m_pos_x;
    FLOORS m_pos_y;
    sf::RectangleShape rectangle;
    sf::Color color;
    Floor(int pos_x, FLOORS pos_y, sf::Color color, bool is_left) : m_pos_x(pos_x), m_pos_y(pos_y), color(color), is_left(is_left) {
        rectangle.setSize(sf::Vector2f(m_width, m_hight));
        rectangle.setFillColor(color);
        rectangle.setOutlineColor(sf::Color::Yellow);
        rectangle.setOutlineThickness(5);
        if(is_left)rectangle.setPosition(m_pos_x, m_pos_y+ELEVATOR_HIGHT);
        else rectangle.setPosition(SCREEN_WIDTH-m_pos_x-m_width, m_pos_y+ELEVATOR_HIGHT);
    }

    void draw(sf::RenderWindow &window){
        window.draw(rectangle);
    }
};

using Floors = std::array<Floor, 5>;

inline Floors make_floors(int pos_x, sf::Color color){
    return {
        Floor(pos_x, FLOORS::FIRST, color, true),
        Floor(pos_x, FLOORS::SECOND, color, false),
        Floor(pos_x, FLOORS::THIRD, color, true),
        Floor(pos_x, FLOORS::FOURTH, color, false),
        Floor(pos_x, FLOORS::FIFTH, color, true)
    };
}

class Human{
    private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    int m_x=2000;
    int m_y=2000;
    public:
    Human(){
        if (!m_texture.loadFromFile("human.png"))
        {
            std::cout<<"Error loading image"<<std::endl;
        }
        m_sprite.setTexture(m_texture);
    }

    void draw(sf::RenderWindow &window){
        m_sprite.setScale(0.1,0.1);
        window.draw(m_sprite);
    }

    void human_set_pos(int y){
        if(FLOORS::FIRST==y || FLOORS::THIRD==y || FLOORS::FIFTH==y)
            m_x=50;
        else
            m_x=SCREEN_WIDTH-50-m_sprite.getGlobalBounds().width*0.1;
        m_y=y+m_sprite.getGlobalBounds().height*0.1;

        m_sprite.setPosition(m_x,m_y);
    }


    void animation(){
       {
                sf::sleep(sf::milliseconds(2)); 
            if (m_sprite.getPosition().x>m_x)
            {
                m_sprite.move(0,-1);
            }
            if (m_sprite.getPosition().y<m_y)
            {
                m_sprite.move(0,1);
            }
            if(m_sprite.getPosition().y == m_y)
            {
                return;
            }
            };
    }
   
};

class ObjectManager{
    Elevator m_elevator{SCREEN_WIDTH/2-ELEVATOR_WIDTH/2,
                        FLOORS::FIRST, sf::Color::Blue};
    std::array<Buttongroup, 5> m_buttongroups = {
        make_buttons(50, FLOORS::FIRST,sf::Color::Red  ),
        make_buttons(SCREEN_WIDTH-50-BUTTON_WIDTH*5, FLOORS::SECOND,sf::Color::Red),
        make_buttons(50, FLOORS::THIRD,sf::Color::Red),
        make_buttons(SCREEN_WIDTH-50-BUTTON_WIDTH*5, FLOORS::FOURTH,sf::Color::Red),
        make_buttons(50, FLOORS::FIFTH,sf::Color::Red)
    };
    Floors m_floors = make_floors(0, sf::Color::Green);
    Human m_human;
    sf::RenderWindow m_window{sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Elevator"};
    std::queue<move> m_orders{};
    public:
    bool human_is=false;
    void loop(){
       
        while (m_window.isOpen())
        {
            auto cursor = sf::Mouse::getPosition(m_window);
            // std::cout << cursor.x << " | "<< cursor.y << '\n';
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
                m_elevator.set_pos(0, m_orders.front().beg_floor);
                }
                else {
                    m_elevator.set_pos(0, m_orders.front().goal_floor);
                    human_is=true;
                    m_human.human_set_pos(m_orders.front().beg_floor);
                }
                m_elevator.reach_check(); 
            }

            m_elevator.moving();
            m_human.animation();
            if (!m_elevator.is_moving() && !m_orders.empty() && m_elevator.is_reached_beg==true)
            m_elevator.elevator_pause();
            if (!m_elevator.is_moving() && !m_orders.empty() && m_elevator.is_reached_goal==true) //operation to keep elevator at floor for the animation to happen
            {
                if(!m_elevator.shoud_move()) 
                m_elevator.elevator_pause();
               else {
                m_elevator.is_reached_beg=false;
                m_elevator.is_reached_goal=false;
                m_orders.front().b.make_red();
                m_orders.pop();
                human_is=false;
                }
            }
            m_window.clear(sf::Color(255, 255, 255));

            if(human_is) draw(m_window, m_human);
            draw(m_window,m_elevator, m_buttongroups, m_floors);
            m_window.display();
        }
    }
};


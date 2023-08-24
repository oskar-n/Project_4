#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include <array>
#include <list>
#include <ranges>
#include <map>
#include "utils.hpp"
#include <unordered_map>
#include <utility>
#include <memory>

constexpr int SCREEN_WIDTH   =1500;
constexpr int SCREEN_HEIGHT  =1200;
constexpr int BUTTON_WIDTH   =50;
constexpr int ELEVATOR_WIDTH =290;
constexpr int ELEVATOR_HIGHT =190;
constexpr int FLOOR_HIGHT =40;
constexpr int COUNTER_WIDTH = 200;
constexpr int COUNTER_HIGHT = 50;
constexpr int HUMAN_WEIGHT = 70;

template <typename T>
concept Container = requires(T t) {
    {t.begin()} -> std::same_as<typename T::iterator>;
    {t.end()} -> std::same_as<typename T::iterator>;
};

template <typename T>
void draw(T &Object, sf::RenderWindow &window){
    Object.draw(window);
}

template <typename Key, typename Val>
void draw(std::unordered_map<Key,Val> &Object_cont, sf::RenderWindow &window){
    for (auto &[key,value]: Object_cont)
    {
        draw(value, window);
    }
}

template <typename T>
void draw(T* &Object, sf::RenderWindow &window){
    Object->draw(window);
}

template <Container C>
void draw(C &Object_cont, sf::RenderWindow &window){
    for (auto &i: Object_cont)
    {
        draw(i,window);
    }
}

template <typename... Args>
void draw(sf::RenderWindow &win, Args &...args){
    (draw(args, win), ...);
}

enum FLOORS{
    FIRST=950,
    SECOND=750,
    THIRD=550,
    FOURTH=350,
    FIFTH=150,
};

class Human{
    static sf::Texture m_texture;
    sf::Sprite m_sprite;
    int m_speed = 10;
    bool m_startedmoving = false;
    bool m_should_appear = true;

    void rotate(double dt){
        static int angle = 30;
        if(!m_startedmoving){
            m_sprite.rotate(angle*dt);
        }
        else{
            m_sprite.rotate(2*angle*dt);
        }
        angle = -angle;
        m_startedmoving = true;
    }

    public:
    bool finished_moving = false;
    int m_goal;
    int m_beg;
    Human(int y, int goal) : m_goal(goal), m_beg(y){
        m_sprite.setTexture(m_texture);
        m_sprite.setScale(0.1,0.1);
        set_pos(y);
    }

    int get_x() const {
        return m_sprite.getPosition().x;
    }

    void set_pos(int y,int x=50){
        int m_x;
        if(FLOORS::FIRST==y || FLOORS::THIRD==y || FLOORS::FIFTH==y){
            m_x=x;
        }
        else{
            x == 50 ? m_x=SCREEN_WIDTH-50-m_sprite.getGlobalBounds().width : m_x=x;
        }
        int m_y=y+m_sprite.getGlobalBounds().height;
        m_sprite.setPosition(m_x,m_y);
    }

    static void load_texture(){
        if (!m_texture.loadFromFile("human.png"))
        {
            std::cout<<"Error loading texture"<<std::endl;
        }
    }
    bool move(int goal_x, double dt){
        const auto human_x = m_sprite.getPosition().x;
        if(human_x == goal_x){
            finished_moving = true;
            return false;
        }
        if(human_x < goal_x)
            m_sprite.move(m_speed*dt,0);
        else if (human_x > goal_x)
            m_sprite.move(-m_speed*dt,0);
        rotate(dt);
        // std::cout << "human_x: " << human_x << " goal: " << goal_x << std::endl;
        return true;
    }

    void hide() {
        m_should_appear = false;
    }


    void show(){
        m_should_appear = true;
    }

    void draw(sf::RenderWindow &window) const {
        if(m_should_appear)
            window.draw(m_sprite);
    }
};
using HumanPtr = Human*;

void Human_cleaner(Container auto &cont){
    if(cont.empty()) return;
    for(auto &i : cont){
        if(i == nullptr) continue;
        delete i;
    }
}

class Elevator {
    private:
        const int m_x=SCREEN_WIDTH/2 - ELEVATOR_WIDTH/2;
        int m_y=0;
        int m_width=ELEVATOR_WIDTH;
        int m_hight=ELEVATOR_HIGHT;
        bool m_is_moving=false;
        bool m_should_move=true;
        sf::Color m_color;
        sf::RectangleShape m_rectangle;
        UniqueQueue<FLOORS> m_path;
    public:
        bool is_reached_beg=false;
        std::deque<HumanPtr> m_humans;
        bool is_reached_goal=false;
        Elevator(int y, sf::Color color)
        {
            m_y=y;
            m_color=color;
            m_rectangle.setPosition(m_x, m_y);
            m_rectangle.setSize(sf::Vector2f(m_width, m_hight));
            m_rectangle.setFillColor(m_color);
            m_rectangle.setOutlineColor(sf::Color::Yellow);
            m_rectangle.setOutlineThickness(5);
        }

        bool move_human_to(auto &h) {
            if(m_humans.size() < 10){
                m_humans.emplace_back(h);
                h = nullptr;
                return true;
            }
            return false;
        }

        int get_x() const {
            return m_x;
        }

        void move_next(){
            if(m_path.size() > 0){
                m_y = m_path[0];
                m_path.pop_front();
            }
        }

        void add_to_path(FLOORS floor){
            if(m_path.add(floor))
            {
                std::cout << "added to path" << std::endl;
            }
            //Debug
            for(auto i : m_path){
                std::cout << i << std::endl;
            }
            std::sort(m_path.begin(), m_path.end());
        }

        int get_y() const
        {
            return m_rectangle.getPosition().y;
        }

        bool moving ()
        {   
            if(m_should_move)
            {
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
                    return false;
                }
                m_is_moving=true;
                return true;
            }
            return false;
        }

        bool is_moving()
        {
            return m_is_moving;
        }

        bool should_move()
        {
            return m_should_move;
        }

        void set_pos(int y)
        {   
            m_y=y;
        }

        void draw(sf::RenderWindow &window)
        {
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

        void pause(){
            m_should_move=false;
        }
        void start(){
            m_should_move=true;
        }
        ~Elevator(){
            std::cerr <<"Elevator destructor called"<<std::endl;
            Human_cleaner(m_humans);
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
        int m_beg = 0;
        int m_goal = 0;
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
            m_rectangle.setFillColor(m_color);
            m_rectangle.setPosition(m_pos_x, m_pos_y);
            m_text.setPosition(m_pos_x+m_width/2, m_pos_y-10);
        }

        void make_red(){
            m_color=sf::Color::Red;
        }

        void draw(sf::RenderWindow &window)
        {
            window.draw(m_rectangle);
            window.draw(m_text);
        }

        bool is_pressed(sf::Vector2i const &mouse_pos)
        {
            if (mouse_pos.x>m_pos_x && mouse_pos.x<m_pos_x+m_width && mouse_pos.y>m_pos_y && mouse_pos.y<m_pos_y+m_width)
            {
                m_color=sf::Color::Green;
                return true;
            }
            return false;
        }
};

struct Move{
    int beg_floor;
    int goal_floor;
    Button &b;
};


using Buttongroup = std::array<Button, 5>; 

inline Buttongroup make_buttons(int pos_x, FLOORS floor, sf::Color color){

    return Buttongroup{
        Button(pos_x, floor, FLOORS::FIRST, color, '1'),
        Button(pos_x + BUTTON_WIDTH, floor, FLOORS::SECOND, color, '2'),
        Button(pos_x + BUTTON_WIDTH * 2, floor, FLOORS::THIRD, color, '3'),
        Button(pos_x + BUTTON_WIDTH * 3, floor, FLOORS::FOURTH, color, '4'),
        Button(pos_x + BUTTON_WIDTH * 4, floor, FLOORS::FIFTH, color, '5')
    };
}

struct Floor {
    int m_width = (SCREEN_WIDTH-ELEVATOR_WIDTH-10)/2;
    int m_hight = FLOOR_HIGHT;
    bool is_left;
    int m_pos_x;
    FLOORS m_pos_y;
    sf::RectangleShape rectangle;
    sf::Color color;
    std::deque<HumanPtr> m_humans{};
    bool is_human_exiting = false;
    Floor(int pos_x, FLOORS pos_y, sf::Color color, bool is_left) : m_pos_x(pos_x), m_pos_y(pos_y), color(color), is_left(is_left) {
        rectangle.setSize(sf::Vector2f(m_width, m_hight));
        rectangle.setFillColor(color);
        rectangle.setOutlineColor(sf::Color::Yellow);
        rectangle.setOutlineThickness(5);
        if(is_left)rectangle.setPosition(m_pos_x, m_pos_y+ELEVATOR_HIGHT);
        else rectangle.setPosition(SCREEN_WIDTH-m_pos_x-m_width, m_pos_y+ELEVATOR_HIGHT);
    }

    void draw(sf::RenderWindow &window){
        for(auto &human : m_humans){
            if(human == nullptr) continue;
            human->draw(window);
        }
        window.draw(rectangle);
    }

    ~Floor(){
        std::cerr <<"Floor destructor called"<<std::endl;
        Human_cleaner(m_humans);
    }
};

using Floors = std::unordered_map<FLOORS, Floor>;

inline Floors make_floors(int pos_x, sf::Color color){
    return Floors{
        {FLOORS::FIRST, Floor(pos_x, FLOORS::FIRST, color, true)},
        {FLOORS::SECOND, Floor(pos_x, FLOORS::SECOND, color, false)},
        {FLOORS::THIRD, Floor(pos_x, FLOORS::THIRD, color, true)},
        {FLOORS::FOURTH, Floor(pos_x, FLOORS::FOURTH, color, false)},
        {FLOORS::FIFTH, Floor(pos_x, FLOORS::FIFTH, color, true)}
    };
}

inline int get_boundry(FLOORS floor){
    if(FLOORS::FIRST==floor || FLOORS::THIRD==floor || FLOORS::FIFTH==floor){
        return 0;
    }
    else{
        return SCREEN_WIDTH;
    }
}

inline int find_border(int y){
    switch(y) {
        case FLOORS::FIRST: return 0;
        case FLOORS::SECOND: return SCREEN_WIDTH;
        case FLOORS::THIRD: return 0;
        case FLOORS::FOURTH: return SCREEN_WIDTH;
        case FLOORS::FIFTH: return 0;
        default: return 0;
    }
}

class ObjectManager{
    Elevator m_elevator{FLOORS::FIRST, sf::Color::Blue};
    std::array<Buttongroup, 5> m_buttongroups = {
        make_buttons(50, FLOORS::FIRST,sf::Color::Red),
        make_buttons(SCREEN_WIDTH-50-BUTTON_WIDTH*5, FLOORS::SECOND,sf::Color::Red),
        make_buttons(50, FLOORS::THIRD,sf::Color::Red),
        make_buttons(SCREEN_WIDTH-50-BUTTON_WIDTH*5, FLOORS::FOURTH,sf::Color::Red),
        make_buttons(50, FLOORS::FIFTH,sf::Color::Red)
    };
    Floors m_floors = make_floors(0, sf::Color::Green);
    sf::RenderWindow m_window{sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Elevator"};
    const double dt = 0.05;
    bool human_entered = false;
    std::deque<HumanPtr> m_LeftOvers;
    
    void spawn_human(FLOORS Beg, FLOORS goal){
        m_elevator.add_to_path(Beg);
        m_floors.at(Beg).m_humans.emplace_back(new Human(Beg, goal));
    }

    void buttongr_pressed(Buttongroup &bg){
        for (auto &b : bg)
        {
            if (b.is_pressed(sf::Mouse::getPosition(m_window)))
            {
                std::cout<<"human created"<<std::endl;
                    spawn_human((FLOORS)b.m_beg, (FLOORS)b.m_goal);
            }
        }    
    }

    /* Left overs as in humans, no logger taking part in the elevator path decision.
     * are to be moved from the scene, so all that is needed is to use the move_to_leftovers
     * and the object manager will move everyhuman off the screen*/
    void move_to_leftovers(HumanPtr &h){
        m_LeftOvers.push_back(h);
        h=nullptr;
    }

    void move_leftovers(){
        if(m_LeftOvers.empty()) return;
        for(auto & i : m_LeftOvers){
            if(i == nullptr) continue;
            if(!i->move(find_border(i->get_x()),dt)){
                m_LeftOvers.pop_front();
                delete i;
                i = nullptr;
            }
        }
    }

    public:
    void handle_events(){
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type==sf::Event::KeyPressed && event.key.scancode == sf::Keyboard::Scan::Escape )
                m_window.close();
            if (event.type==sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                for(auto i : m_buttongroups){
                    buttongr_pressed(i);
                }
            }
        }
    }

    void drop_off(FLOORS Goal){
        auto &humans = m_elevator.m_humans;
        if(humans.empty()) return;
        for(auto &i : humans){
            if(i == nullptr) continue;
            if(i->m_goal != Goal) continue; 
            i->set_pos(Goal, m_elevator.get_x());
            move_to_leftovers(i);
            //i = nullptr; at this point the human is moved to the left overs
        }
    }

    //should return true, if all the humans, that are supposed to be in the elevator, are in the elevator
    //and false otherwise
    bool pick_up(FLOORS floor){
        auto &humans = m_floors.at(floor).m_humans;
        if(humans.empty()) return true;
        auto human = &humans.front();
        if(!(*human)->move(m_elevator.get_x(),dt))
        {
            m_elevator.add_to_path((FLOORS)(*human)->m_goal);
            if(!m_elevator.move_human_to(*human)){
                move_to_leftovers(*human);
                return true;
            }
            //Debug:
            human++;
            humans.pop_front();
        }
        return false;
    }

    //will be called once at the beginning
    void start(){
        Human::load_texture();
    }

    //will be called once per frame
    void loop(){
        while (m_window.isOpen())
        {
            handle_events();
            if(!m_elevator.moving()){
                drop_off((FLOORS)m_elevator.get_y());
                if(pick_up((FLOORS)m_elevator.get_y()))
                { 
                    m_elevator.move_next();
                }
            }
            move_leftovers();
            m_window.clear(sf::Color::White);
            draw(m_window,m_elevator,m_buttongroups, m_floors, m_LeftOvers);
            m_window.display();
        }
    }
    ~ObjectManager(){
        std::cerr <<"ObjectManager destructor called"<<std::endl;
        Human_cleaner(m_LeftOvers);
    }
};


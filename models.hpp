#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include <array>
#include <list>
#include <set>

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

template <Container C>
void draw(C &Object_cont, sf::RenderWindow &window){
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

enum class DIRECTION{
    UP,
    DOWN,
    NONE
};

class Elevator
{
    private:
        const int m_x=SCREEN_WIDTH/2 - ELEVATOR_WIDTH/2;
        int m_destination=0;
        int m_width=ELEVATOR_WIDTH;
        int m_hight=ELEVATOR_HIGHT;
        bool m_is_moving=false;
        bool m_should_move=true;
        sf::Color m_color;
        sf::RectangleShape m_rectangle;
    public:
        bool is_reached_beg=false;
        bool is_reached_goal=false;
        Elevator(int y, sf::Color color)
        {
            m_destination=y;
            m_color=color;
            m_rectangle.setPosition(m_x, m_destination);
            m_rectangle.setSize(sf::Vector2f(m_width, m_hight));
            m_rectangle.setFillColor(m_color);
            m_rectangle.setOutlineColor(sf::Color::Yellow);
            m_rectangle.setOutlineThickness(5);
        }

        int get_x() const {
            return m_x;
        }

        int get_y() const
        {
            return m_rectangle.getPosition().y;
        }

        DIRECTION move ()
        {   
            if(m_should_move)
            {
                if (m_rectangle.getPosition().y>m_destination)
                {
                    m_rectangle.move(0,-1);
                    return DIRECTION::UP;
                }
                if (m_rectangle.getPosition().y<m_destination)
                {
                    m_rectangle.move(0,1);
                    return DIRECTION::DOWN;
                }
                if(m_rectangle.getPosition().y == m_destination)
                {
                    m_is_moving=false;
                    return DIRECTION::NONE;
                }
            }
            return DIRECTION::NONE;
        }

        bool is_moving()
        {
            return m_is_moving;
        }

        bool should_move()
        {
            return m_should_move;
        }

        void set_destination(int y)
        {   
            m_destination=y;
        }

        void draw(sf::RenderWindow &window)
        {
            window.draw(m_rectangle);
        }

        void pause(){
            m_should_move=false;
        }
        void start(){
            m_should_move=true;
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

    Move& operator = (Move const &other){
        beg_floor = other.beg_floor;
        goal_floor = other.goal_floor;
        b = other.b;
        return *this;
    }

    const Move& operator=(Move const &other) const {
        if (this != &other) {
            const_cast<Move*>(this)->beg_floor = other.beg_floor;
            const_cast<Move*>(this)->goal_floor = other.goal_floor;
            const_cast<Move*>(this)->b = other.b;
        }
        return *this;
    }
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

    Human(int y){
        m_sprite.setTexture(m_texture);
        m_sprite.setScale(0.1,0.1);
        set_pos(y);
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
        std::cout << "human_x: " << human_x << " goal: " << goal_x << std::endl;
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

class Counter{
    private:
    int m_pos_x;
    int m_pos_y;
    int m_width=COUNTER_WIDTH;
    int m_hight=COUNTER_HIGHT;
    sf::Color m_color;
    sf::RectangleShape m_rectangle;
    sf::Text m_text;
    sf::Font m_font;
    int m_count = 0;

    public:
    Counter(int pos_x, int pos_y, sf::Color color)
        {
            m_pos_x=pos_x;
            m_pos_y=pos_y;
            m_color=color;
            m_rectangle.setSize(sf::Vector2f(m_width, m_hight));
            if (!m_font.loadFromFile("digital-7.ttf"))
            {
                std::cout<<"Error loading font"<<std::endl;
            }
            m_text.setFont(m_font);
            m_text.setCharacterSize(50);
            m_text.setFillColor(sf::Color::White);
        } 
    
    void draw(sf::RenderWindow &window)
    {
        m_text.setString(std::to_string(m_count));
        m_rectangle.setFillColor(m_color);
        m_rectangle.setPosition(m_pos_x, m_pos_y);
        m_text.setPosition(m_pos_x+m_width/2, m_pos_y-10);
        window.draw(m_rectangle);
        window.draw(m_text);
    }
    void increment(){
        m_count=m_count+HUMAN_WEIGHT;
    }
    void decrement(){
        m_count=m_count-HUMAN_WEIGHT;
    }
    int get_count(){
        return m_count;
    }
};

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

//TODO: Remove
template<typename cont>
void print(cont const &C){
    for(auto i : C){
        std::cerr << i.beg_floor << " ";
    }
    std::cerr << std::endl;
}

struct UniqueQueue{
    std::deque <Move> m_path{};
    std::set<int> m_visited_floors{};
    void add_move(Move const &move){
        auto [iter, inserted] = m_visited_floors.insert(move.beg_floor);
        if(inserted){
            m_path.push_back(move);
        }
    }
    void remove_move(Move const &move){
        m_visited_floors.erase(move.beg_floor);
        m_path.pop_front();
    }

    auto begin() const{
        return m_path.begin();
    }

    auto end()const{
        return m_path.end();
    }
};

struct ElevatorLogic{
  Elevator &elevator;  
  std::deque<Move> &orders;
  std::vector<Human> &humans;
  UniqueQueue m_path;
  
  void pick_up(){
    //TODO
  }
  
   void make_path(DIRECTION dir){
        if(orders.empty()){
            return;
        }
        auto &current_order = orders.front();
        for(auto order : orders){
            switch(dir){
                case DIRECTION::UP:
                    if(order.beg_floor <= current_order.beg_floor){
                        m_path.add_move(order);
                    }
                    break;
                case DIRECTION::DOWN:
                    if(order.beg_floor >= current_order.beg_floor){
                        m_path.add_move(order);
                    }
                    break;
                case DIRECTION::NONE:
                    break;
            }
        }
        std::sort(m_path.begin(), m_path.end(), [](Move const &a, Move const &b){
            return a.beg_floor < b.beg_floor;
        });
        //Debug:
        std::cerr << "Path: ";
        print(m_path);
  }
};

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
    Counter m_counter{SCREEN_WIDTH-COUNTER_WIDTH, 0, sf::Color::Blue};
    sf::RenderWindow m_window{sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Elevator"};
    std::deque<Move> m_orders{};
    std::vector<Human> m_humans{};
    ElevatorLogic m_elevLogic{m_elevator, m_orders, m_humans};
    const double dt = 0.1;
    bool human_entered = false;
    DIRECTION dir = DIRECTION::NONE;
    void buttongr_pressed(Buttongroup &bg){
        for (auto &b : bg)
        {
            if (b.is_pressed(sf::Mouse::getPosition(m_window)))
            {
                m_orders.push_back({b.m_beg, b.m_goal, b});
                m_humans.emplace_back(Human(b.m_beg));
                std::cerr<<"Orders";
                print(m_orders);
                m_elevLogic.make_path(DIRECTION::UP);
                std::cout<<"human created"<<std::endl;
            }
        }    
    }

    public:
    void handle_events(){
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
                    buttongr_pressed(i);
                }
            }
        }
    }

    //will be called once at the beginning
    void start(){
        Human::load_texture();
    }

    //will be called once per frame
    void loop(){
        bool human_droped_off = false;
        dir = m_elevator.move();
        while (m_window.isOpen())
        {
            handle_events();

            m_window.clear(sf::Color::White);
            draw(m_window,m_elevator, m_buttongroups, m_floors, m_counter, m_humans);
            m_window.display();
        }
    }
};

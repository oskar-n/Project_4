
#include "models.hpp"

template <typename... Args>
void draw(sf::RenderWindow &win, Args... args){
    (args.draw(win), ...);
}

int main()
{
    int elevator_pos_x=SCREEN_WIDTH/2-ELEVATOR_WIDTH;
    int elevator_pos_y=SCREEN_HEIGHT-ELEVATOR_HIGHT*2-5;
    sf::RenderWindow window(sf::VideoMode(1500, 800), "Elevator");
    Elevator elevator(elevator_pos_x,elevator_pos_y, sf::Color::Blue);
    Button b1(50, 50, sf::Color::Red, '1');
    Button b2(50, 150, sf::Color::Red, '2');
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type==sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                b1.is_pressed(window, elevator, 100);
                b2.is_pressed(window, elevator, 300);
            }

        }
        elevator.moving();

        window.clear();
        draw(window,elevator, b1, b2);
        window.display();
    }
    return 0;
}

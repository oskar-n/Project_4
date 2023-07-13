
#include "models.hpp"

int main()
{
int elevator_pos_x=SCREEN_WIDTH/2-ELEVATOR_WIDTH;
int elevator_pos_y=SCREEN_HEIGHT-ELEVATOR_HIGHT*2-5;
sf::RenderWindow window(sf::VideoMode(1500, 800), "Elevator");
elevator m_elevator(elevator_pos_x,elevator_pos_y, sf::Color::Blue);
button b1(50, 50, sf::Color::Red, '1');
button b2(50, 150, sf::Color::Red, '2');
while (window.isOpen())
{
sf::Event event;
while (window.pollEvent(event))
{
if (event.type == sf::Event::Closed)
    window.close();
if (event.type==sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        b1.is_pressed(window, m_elevator, 100);
        b2.is_pressed(window, m_elevator, 300);
    }
        
}
m_elevator.moving();

window.clear();

b1.draw_button(window);
b2.draw_button(window);
m_elevator.draw_elevator(window);
window.display();
}
return 0;
}

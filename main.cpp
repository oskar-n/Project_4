#include <SFML/Graphics.hpp>


struct elevator
{
    int x_=0;
    int y_=0;
void moving (sf::CircleShape &circulo)
{   
   sf::sleep(sf::milliseconds(2));
   if (circulo.getPosition().y>y_)
   {
    circulo.move(0,-1);
   }
   if (circulo.getPosition().y<y_)
   {
    circulo.move(0,1);
   }
   
}
void set_pos(int x, int y)
{   
     x_=x;
     y_=y;
}

};

int main()
{
sf::RenderWindow ventana(sf::VideoMode(800, 800), "Elevator");
sf::CircleShape circulo(200,200);
elevator elevador;

circulo.setOrigin(100, 5);
circulo.setFillColor(sf::Color::Cyan);
circulo.setPosition(400, 400);
elevador.set_pos(circulo.getPosition().x, circulo.getPosition().y);
while (ventana.isOpen())
{
sf::Event event;
while (ventana.pollEvent(event))
{
if (event.type == sf::Event::Closed)
ventana.close();
if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
    elevador.set_pos(200, 200);
if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
    elevador.set_pos(500, 500);
}
elevador.moving(circulo);
ventana.clear();
ventana.draw(circulo);
ventana.display();
}
return 0;
}

#include "models.hpp"

inline sf::Texture Human::m_texture;
int main()
{
    ObjectManager manager;
    manager.start();
    manager.loop();
    return 0;
}

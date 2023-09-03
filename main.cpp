#include "models.hpp"

sf::Texture Human::m_texture;
int main()
{
  try {
    ObjectManager manager;
    manager.start();
    manager.loop();
    return 0;
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

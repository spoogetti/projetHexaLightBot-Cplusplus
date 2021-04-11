#ifndef FLATHEXA_H
#define FLATHEXA_H
#include "SFML/Graphics.hpp"
#include "Tools.h"

class FlatHexa
{
private:
  sf::CircleShape m_hexa;
  sf::Vector3i m_position;
  sf::Font m_font;
  sf::Text m_text;
  int m_type;
  static int m_radius;
  void update();
  void init(sf::Vector3i v, int type);

public:
  FlatHexa(sf::Vector3i pos, int type);
  static void setRadius(int radius);
  static float getRadius();
  static sf::Vector2f Origin();


  void setHeight(int height);
  void addHeight();
  int getHeight();

  void setType(int type);
  int getType();

  void setPos(int x, int y);
  void setPos(int x, int y, int z);
  sf::Vector3i getPos();

  void setCenter(int x, int y);
  sf::Vector2f getCenter();

  sf::Vector2i getCoord();

  void draw_on(sf::RenderWindow &w);

};

#endif // FLATHEXA_H

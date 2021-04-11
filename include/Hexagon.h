#ifndef HEXAGON_H
#define HEXAGON_H

#include <SFML/Graphics.hpp>

class Hexagon
{
public :
  Hexagon(int x, int y, int z, sf::Color color, int state);
    Hexagon() = default;
    Hexagon* clone();

    void draw_on(sf::RenderWindow & window);

    static void loadFromFile();

    void setTexture();
    void setSpritePosition(sf::Vector2f);

    sf::Color getColor();
    void setFillColor(sf::Color);

    int getState();
    void setState(int);

    int getHeight();
    void setHeight(int);

    int getHexaDepth();
    int getHexaWidth();
    int getHexaHeight();

    sf::Vector2f getCenter();


private :
    sf::Color m_color;
    int m_height;
    int m_state;
    sf::Vector2f m_sprite_position;
    static sf::Texture m_texture;
    sf::Sprite m_sprite;

};

#endif // HEXAGON_H

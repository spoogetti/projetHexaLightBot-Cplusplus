#include "../include/Level.h"
#include "../include/Hexagon.h"
#include <stdlib.h>

sf::Texture Hexagon::m_texture;

Hexagon::Hexagon(int x, int y, int z, sf::Color color, int state)
    :m_color { color },
     m_height { z },
     m_state { state },
     m_sprite_position { sf::Vector2f( xORIGIN + HEXAGON_WIDTH * x + y%2 * RADIUS,
                         yORIGIN + HEXAGON_DEPTH / 2 * y - HEXAGON_HEIGHT * z ) }
{
    m_sprite.setTextureRect(sf::IntRect(0, 0, INSTR_SIZE_SCALED, INSTR_SIZE_SCALED));
    m_sprite.setTexture(m_texture);
    m_sprite.setPosition({ m_sprite_position });
}

Hexagon* Hexagon::clone()
{
    auto d = new Hexagon();
    d->setFillColor(m_color);
    d->setHeight(m_height);
    d->setState(m_state);
    d->setSpritePosition(m_sprite_position);
    d->setTexture();
    return d;
}

void Hexagon::draw_on(sf::RenderWindow &window)
{
    window.draw(m_sprite);
}

void Hexagon::loadFromFile()
{
  // Chargement de la texture des hexagones
  if (!m_texture.loadFromFile("ressources/hexagon.png")){exit(EXIT_FAILURE);}
}

void Hexagon::setTexture()
{
    m_texture.setSmooth(true);
    m_sprite.setTexture(m_texture);
    m_sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
    m_sprite.setPosition(m_sprite_position);
    m_sprite.setColor(m_color);
}

void Hexagon::setSpritePosition(sf::Vector2f position)
{
    m_sprite_position = position;
}

sf::Color Hexagon::getColor()
{
    return m_color;
}

void Hexagon::setFillColor(sf::Color color)
{
    m_color = color;
    m_sprite.setColor(color);
}

int Hexagon::getState()
{
    return m_state;
}

void Hexagon::setState(int state)
{
    m_state = state;
}

int Hexagon::getHeight()
{
    return m_height;
}

void Hexagon::setHeight(int height)
{
    m_height = height;
}

int Hexagon::getHexaDepth() //Hauteur de la partie "Plateau" de l'hexagon
{
    return m_sprite.getGlobalBounds().height * (87./100.);
}

int Hexagon::getHexaWidth() //Largeur de l'hexagone
{
    return m_sprite.getGlobalBounds().width;
}

int Hexagon::getHexaHeight() //Hauteur de la bordure en pixel
{
  return m_sprite.getGlobalBounds().height * (29./100.);
}

sf::Vector2f Hexagon::getCenter()
{
  return sf::Vector2f (m_sprite.getGlobalBounds().width / 2, getHexaHeight() / 2);
}


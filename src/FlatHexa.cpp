#include "../include/FlatHexa.h"
#include <iostream>
#include <cmath>

FlatHexa::FlatHexa(sf::Vector3i pos, int type)
{
    init(pos, type);
    if (!m_font.loadFromFile(POLICE)) throw "Police "+POLICE+" manquante";
    m_text.setFont(m_font);
    m_text.setCharacterSize(40);
    m_text.setString(std::to_string(pos.z));

    update();
}

void FlatHexa::init(sf::Vector3i cubicPos, int type)
{
    m_hexa.setRadius(m_radius);
    m_type = type;
    m_position = cubicPos;
    m_hexa.setOutlineThickness(1);
    m_hexa.setPointCount(6);
    switch (type) {
    case GHOST: // Cas selection
        m_hexa.setFillColor(sf::Color(255,0,0,100));
        m_hexa.setOutlineColor(sf::Color::Red);
        break;
    case PLAYER: // Cas joueur
        m_hexa.setFillColor(sf::Color::Transparent);
        m_hexa.setOutlineThickness(2);
        m_hexa.setOutlineColor(sf::Color::Green);
        break;
    case SIMPLE_HEXA: // Cas hexagonne simple
        m_hexa.setFillColor(sf::Color::Black);
        m_hexa.setOutlineColor(sf::Color::White);
        m_text.setFillColor(sf::Color::White);
        break;
    case LIT_HEXA: // Cas hexagonne lumineux
        m_hexa.setFillColor(sf::Color::Cyan);
        m_hexa.setOutlineColor(sf::Color::White);
        m_text.setFillColor(sf::Color::Black);
        break;
    }
}

int FlatHexa::m_radius;

void FlatHexa::setRadius(int radius)
{
    m_radius = radius;
}

float FlatHexa::getRadius()
{
    return m_radius;
}

sf::Vector2f Origin()
{
    return { xORIGIN, yORIGIN };
}

void FlatHexa::update()
{
    m_text.setString(std::to_string(m_position.z));
    m_hexa.setPosition( MARGIN_TOP_EDITOR / 2 + m_radius * sqrt( 3 ) * ( m_position.x + ( (m_position.y ) * 0.5) ),
                        MARGIN_TOP_EDITOR + m_radius * 3 / 2 * m_position.y );

    sf::Vector2f v;
    v.x = m_hexa.getPosition().x + m_hexa.getGlobalBounds().width / 2 - m_text.getGlobalBounds().width / 2 ;
    v.y = m_hexa.getPosition().y + m_hexa.getGlobalBounds().height / 2 - m_text.getGlobalBounds().height / 2 + m_text.getPosition().y - m_text.getGlobalBounds().top ;

    m_text.setPosition(v);
}

void FlatHexa::setHeight(int height)
{
    m_position.z = height;
}

void FlatHexa::addHeight()
{
    if (m_position.z == 2)
        m_position.z = 0;
    else
        m_position.z ++;
}

int FlatHexa::getHeight()
{
    return m_position.z;
}

void FlatHexa::setType(int type)
{
    m_type = type;
}

int FlatHexa::getType()
{
    return m_type;
}

void FlatHexa::setPos(int x, int y)
{
    m_position.x = x;
    m_position.y = y;
}

void FlatHexa::setPos(int x, int y, int z)
{
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
}

sf::Vector3i FlatHexa::getPos()
{
    return m_position;
}

void FlatHexa::setCenter(int x, int y)
{
    m_position.x = x - m_hexa.getGlobalBounds().width / 2;
    m_position.y = y - m_hexa.getGlobalBounds().height / 2;
}

sf::Vector2f FlatHexa::getCenter()
{
    return { m_hexa.getGlobalBounds().left + m_hexa.getGlobalBounds().width / 2,
                m_hexa.getGlobalBounds().top + m_hexa.getGlobalBounds().height / 2 };
}

sf::Vector2i FlatHexa::getCoord()
{
    return {m_position.x, m_position.y};
}

void FlatHexa::draw_on(sf::RenderWindow& window)
{
    update();
    if (m_type == GHOST)
    {
        //Zone de la grille du level Editor
        if (m_position.x + m_position.y / 2 >= 0 && m_position.y >= 0 && m_position.x + m_position.y / 2 <= MAX_HEXA && m_position.y <= MAX_HEXA)
            window.draw(m_hexa);
    }
    else if (m_type == PLAYER)
        window.draw(m_hexa);
    else
    {
        window.draw(m_hexa);
        window.draw(m_text);
    }
}

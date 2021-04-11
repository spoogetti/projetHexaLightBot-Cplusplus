#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include "Tools.h"

enum DIRECTION { RIGHT = 0, UP_RIGHT = 1, UP_LEFT = 2, LEFT = 3, DOWN_LEFT = 4, DOWN_RIGHT = 5};

class Player
{
public:
    Player(sf::Vector3i position, int direction);
    Player() = default;
    Player* clone();

    static void loadFromFile();

    void draw_on(sf::RenderWindow & window);

    void move(int newHeight);
    void move();

    void turnRight();
    void turnLeft();

    sf::Vector3i getPosition();
    void setPosition(sf::Vector3i);
    void setHeight(int);

    int getDirection();
    void setDirection(int direction);

    void refresh();

    bool isMoving();

    void changePos();
    sf::Vector3i changePos(sf::Vector3i pos, int direction);

    int getFrame();

private :
    sf::Vector3i m_position;
    sf::Vector3i m_start_position;
    sf::Vector3i m_end_position;
    int m_direction;
    int m_frame = 0;
    sf::Clock m_clock; // Permet de mesurer le temps pour l'affichage
    bool m_is_moving = false;
    static sf::Texture m_texture_jump;
    static sf::Texture m_texture_idle;
    sf::Sprite m_sprite;
};

#endif // PLAYER_H

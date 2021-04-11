#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <string>
#include "Tools.h"


class Instruction
{
public:
    Instruction(int getType, sf::Texture, Position);
    void draw_on(sf::RenderWindow &window);
    bool contains(Position position);
    int getType();
    void setPosition(Position position);
    ~Instruction() = default;
private:
    int m_type;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
};

#endif // INSTRUCTION_H

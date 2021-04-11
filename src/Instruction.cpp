#include "../include/Instruction.h"

Instruction::Instruction(int type, sf::Texture texture, Position position)
    : m_type { type },
      m_texture { texture }
{
    m_sprite.setTexture(m_texture);

    switch(type)
    {
    case FORWARD:
        m_sprite.setTextureRect(sf::IntRect(0, 0, INSTR_SIZE, INSTR_SIZE));
        m_sprite.setPosition(position);
        break;
    case LIGHT_UP:
        m_sprite.setTextureRect(sf::IntRect(INSTR_SIZE, 0, INSTR_SIZE, INSTR_SIZE));
        m_sprite.setPosition(position);
        break;
    case ROTATE_LEFT:
        m_sprite.setTextureRect(sf::IntRect(INSTR_SIZE*2, 0, INSTR_SIZE, INSTR_SIZE));
        m_sprite.setPosition(position);
        break;
    case ROTATE_RIGHT:
        m_sprite.setTextureRect(sf::IntRect(INSTR_SIZE*3, 0, INSTR_SIZE, INSTR_SIZE));
        m_sprite.setPosition(position);
        break;
    case JUMP:
        m_sprite.setTextureRect(sf::IntRect(INSTR_SIZE*4, 0, INSTR_SIZE, INSTR_SIZE));
        m_sprite.setPosition(position);
        break;
    case SP1:
        m_sprite.setTextureRect(sf::IntRect(INSTR_SIZE*5, 0, INSTR_SIZE, INSTR_SIZE));
        m_sprite.setPosition(position);
        break;
    case SP2:
        m_sprite.setTextureRect(sf::IntRect(INSTR_SIZE*6, 0, INSTR_SIZE, INSTR_SIZE));
        m_sprite.setPosition(position);
        break;
    }
}

void Instruction::draw_on(sf::RenderWindow &window)
{
    window.draw(m_sprite);
}

bool Instruction::contains(Position position)
{
    auto d = position - m_sprite.getPosition();
    if(0 <= d.x and d.x <= INSTR_SIZE and 0 <= d.y and d.y <= INSTR_SIZE)
        return true;
    return false;
}

int Instruction::getType()
{
    return m_type;
}

void Instruction::setPosition(Position position)
{
    m_sprite.setPosition(position);
}

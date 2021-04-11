#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include "Instruction.h"

class Program
{
public:
    Program(unsigned short nb_instr, float marginTop, std::string tag);
    ~Program();
    // retourne le set d'instructions
    // le set d'instruction est plus adapté car on ne recherchera jamais un élément particulier
    // et le set n'aura jamais une grande taille de plus l'insertion d'élément est plus simple
    void setInstrTexture(sf::Texture);

    std::vector<Instruction *> instructions();

    std::string type();
    void draw_on(sf::RenderWindow &window);
    void addInstruction(int type);
    void refreshPositions();
    void insertInstruction(int pos, int instrType);
    bool contains(sf::Vector2f position);
    void clear();
    void refreshInstructions(unsigned short pos);
    int isBetweenInstructions(sf::Vector2f);
    void setFillColor(sf::Color);


    std::string getTag();

    // Méthodes de parcour de programme
    void reset_pos();
    Instruction* read_next();
    Instruction* read();
    bool end();
private:

    // Description du programme
    sf::Font m_font;
    sf::Text m_tag;

    // Fond de couleur indiquant la "hitbox" du programme
    sf::RectangleShape m_rect;
    float m_marginTop;

    // Texture des instructions
    sf::Texture m_textInst;
    // Instructions et nombre maximum d'instructions
    std::vector<Instruction*> m_instructions;
    unsigned short m_nbMaxInstr;

    // Itérateur pour parcourir le programme
    std::vector<Instruction*>::iterator m_iterator;
};

#endif // PROGRAM_H

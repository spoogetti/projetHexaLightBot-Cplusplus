#include "../include/Program.h"

Program::Program(unsigned short nb_instr, float marginTop, std::string tag)
    : m_marginTop { marginTop },
      m_nbMaxInstr { nb_instr }

{
    // Initialisation du fond de la "program box"
    m_rect.setPosition({MARGIN_LEFT_PROG, marginTop});
    m_rect.setSize({INSTR_SIZE*4, (nb_instr/4) * (float)INSTR_SIZE});
    m_rect.setFillColor(BRIGHT_BACKGROUND);

    // Initialisation du texte descriptif
    m_tag.setPosition(MARGIN_LEFT_PROG, marginTop - SMALL_MARGIN/2);
    if (!m_font.loadFromFile(POLICE)) throw "Police "+POLICE+" manquante";
    m_tag.setFont(m_font);
    m_tag.setFillColor(sf::Color::Black);
    m_tag.setCharacterSize(10);
    m_tag.setString(tag);

    m_iterator = m_instructions.begin();
}

Program::~Program()
{
    for (auto i : m_instructions)
        delete i;
    m_instructions.clear();
}

void Program::setInstrTexture(sf::Texture textInstr)
{
    m_textInst = textInstr;
}

std::vector<Instruction*> Program::instructions()
{
    return m_instructions;
}

void Program::draw_on(sf::RenderWindow &window)
{
    window.draw(m_rect);
    window.draw(m_tag);
    for(auto sprite : m_instructions)
        sprite->draw_on(window);

}

void Program::refreshPositions()
{
    int i = 0;
    for(auto instr : m_instructions)
    {
        instr->setPosition({ MARGIN_LEFT_PROG + INSTR_SIZE * (i%4),
                             m_marginTop + INSTR_SIZE * (i/4)} );
        i++;
    }
}

void Program::insertInstruction(int pos, int instrType)
{
    Instruction* i = new Instruction ( instrType, m_textInst, { MARGIN_LEFT_PROG + INSTR_SIZE * (m_instructions.size()%4), m_marginTop + INSTR_SIZE * (m_instructions.size()/4)} );
    if (m_instructions.size() < m_nbMaxInstr)
        m_instructions.insert(m_instructions.begin() + pos, i);
    refreshPositions();
    m_iterator = m_instructions.begin();
}

bool Program::contains(sf::Vector2f position)
{
    return m_rect.getGlobalBounds().contains(position);
}

void Program::clear()
{
    for (auto i : m_instructions)
        delete i;
    m_instructions.clear();
}


// supprime l'instruction à l'emplacement pos
void Program::refreshInstructions(unsigned short pos)
{
    std::vector<Instruction*> tempList;
    for(unsigned short i=0; i < m_instructions.size(); i++)
    {
        if (i != pos)
            tempList.push_back(m_instructions[i]);
        else delete m_instructions[i];
    }
    m_instructions.clear();
    m_instructions = tempList;
    refreshPositions();
    m_iterator = m_instructions.begin();
}

// si la souris est entre deux instructions du programme, retourne la position de l'instruction "courante"
int Program::isBetweenInstructions(sf::Vector2f position)
{
    int i = 0;
    // retourne la position de fin de tableau si la position n'est entre aucune instructions
    int pos = m_instructions.size();
    for (auto instr : m_instructions)
    {
        if(instr->contains(position))
            pos = i;
        i++;
    }
    return pos;
}

void Program::setFillColor(sf::Color color)
{
    m_rect.setFillColor(color);
}

std::string Program::getTag()
{
    return m_tag.getString();
}

void Program::reset_pos()
{
    m_iterator = m_instructions.begin();
}

Instruction* Program::read_next() //Incremente l'iterateur si possible et le retourne
{
    if (m_iterator != m_instructions.end())
    {
        m_iterator++;
        if (m_iterator != m_instructions.end())
          return *m_iterator;
    }
    return nullptr;
}

Instruction* Program::read() // retourne l'iterateur si pas a la fin du vecteur d'instructions
{
    if (m_iterator != m_instructions.end())
        return *m_iterator;
    return nullptr;
}

bool Program::end()
{
    return m_iterator == m_instructions.end();
}

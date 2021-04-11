#include "../include/Level_Editor.h"
#include <fstream>
#include <cmath>

Level_Editor::Level_Editor(const std::string& name)
    : m_level_name { name }
{
  if(!m_text_direction.loadFromFile("ressources/direction.png")){exit(EXIT_FAILURE);}
  m_sprite_direction.setTexture(m_text_direction);
  m_sprite_direction.setOrigin(m_sprite_direction.getGlobalBounds().width/2,m_sprite_direction.getGlobalBounds().height/2);
}

Level_Editor::~Level_Editor()
{
    for (auto hx : m_grid)
        for (auto hy : hx.second)
            delete hy.second;
    m_grid.clear();
}

sf::Vector2i Level_Editor::mouseToHex(sf::Vector2f mousePos)
{
    float axialX = mousePos.x - MARGIN_TOP_EDITOR / 2 - FlatHexa::getRadius();
    float axialY = mousePos.y - MARGIN_TOP_EDITOR - FlatHexa::getRadius();

    float cubicX =  ( ( axialX * ( sqrt(3) / 3.) ) - axialY / 3. ) /  FlatHexa::getRadius();
    float cubicZ = axialY * (2. / 3.) / FlatHexa::getRadius();
    float cubicY = - ( cubicX + cubicZ );

    int roundedX = std::round(cubicX);  // coordonnées cubiques arrondies
    int roundedY = std::round(cubicY);
    int roundedZ = std::round(cubicZ);

    float decimalX = std::abs(roundedX - cubicX); // écarts par rapport aux arrondis
    float decimalY = std::abs(roundedY - cubicY);
    float decimalZ = std::abs(roundedZ - cubicZ);

    if ( decimalX > decimalY  && decimalX > decimalZ )
        roundedX = - ( roundedY + roundedZ );       // on rectifie xd si il a le plus grand écart
    else if ( decimalY > decimalZ )
        roundedY = - ( roundedX + roundedZ);        // ou zd si plus grand écart que yd
    else
        roundedZ = - ( roundedX + roundedY );

    return {roundedX, roundedZ};
}

void Level_Editor::addHex(sf::Vector3i cubicPos, int type)
{
    if ((cubicPos.x + cubicPos.y / 2) >= 0 && cubicPos.y >= 0 && (cubicPos.x + cubicPos.y / 2) <= MAX_HEXA && cubicPos.y <= MAX_HEXA)
    {
        if (!hexagonAtPos({cubicPos.x, cubicPos.y}))
        {
            if (cubicPos.x > m_max_x)
                m_max_x = cubicPos.x;
            if (cubicPos.y > m_max_y)
                m_max_y = cubicPos.y;

            m_grid[cubicPos.x][cubicPos.y] = new FlatHexa ({ cubicPos.x, cubicPos.y, cubicPos.z}, type);
        }
        else
            m_grid[cubicPos.x][cubicPos.y]->addHeight();
    }
}

void Level_Editor::removeHex(sf::Vector2i axialPos)
{
    // Si le joueur etait à cette position, le détruit
    if (axialPos == m_player)
        m_player = {-1, -1};

    if (hexagonAtPos(axialPos))
    {
        delete m_grid[axialPos.x][axialPos.y];
        m_grid[axialPos.x].erase(axialPos.y);
        // Supprime la clée en y si il n'y a plus de y correspondant
        if (m_grid[axialPos.x].size() == 0)
            m_grid.erase(axialPos.x);

        if (axialPos.x == m_max_x|| axialPos.y == m_max_y)
        {
            int axialX = 0;
            int axialY = 0;
            for (auto hx : m_grid)
            {
                if (hx.first > axialX)
                    axialX = hx.first;
                for (auto hy : hx.second)
                    if (hy.first > axialY)
                        axialY = hy.first;
            }
            m_max_x = axialX;
            m_max_y = axialY;

        }
    }
}

bool Level_Editor::hexagonAtPos(sf::Vector2i axialPos)
{
    bool hexagonExist = false;
    auto xIterator = m_grid.find(axialPos.x);
    if(xIterator != m_grid.end())
    {
        auto yIterator = m_grid[axialPos.x].find(axialPos.y);
        if (yIterator != m_grid[axialPos.x].end())
            hexagonExist = true;
    }
    return hexagonExist;
}

void Level_Editor::setPlayer(sf::Vector2i axialPos)
{
  if (m_player == axialPos)
    changeDirection();
  else if (hexagonAtPos(axialPos))
      m_player = axialPos;
}

void Level_Editor::changeDirection()
{
  m_direction = (m_direction + 1) % 6;
  m_sprite_direction.setRotation(-360/6*m_direction);
}

void Level_Editor::refreshDirection()
{
  m_direction = (m_direction) % 6;
  m_sprite_direction.setRotation(-360/6*m_direction);
}

void Level_Editor::initDirection(int dir)
{
  m_direction = (dir) % 6;
  m_sprite_direction.setRotation(-360/6*m_direction);
}

std::string Level_Editor::getName()
{
    return m_level_name;
}

void Level_Editor::draw_on(sf::RenderWindow& window)
{
    for (auto hx : m_grid)
        for (auto hy : hx.second)
            hy.second->draw_on(window);

    if (!(m_player.x == -1 && m_player.y == -1))
    {
        FlatHexa player {{m_player.x, m_player.y, 0}, PLAYER};
        player.draw_on(window);
        m_sprite_direction.setPosition(player.getCenter());
        refreshDirection();
        window.draw(m_sprite_direction);
    }
}

void Level_Editor::saveInFile(std::string name, int creatorScore)
{
    std::fstream f;
    std::string filePath = "ressources/Levels/Editor/" + name;
    f.open(filePath.c_str(), std::ios::out);
    if( f.fail() )
    {
        std::cerr << "ouverture en lecture impossible (fichier niveau)" << std::endl;
        exit(EXIT_FAILURE);
    }
    f << "creatorScore " + std::to_string(creatorScore);
    f << " userScore 999 ";
    for (auto hx : m_grid)
        for (auto hy : hx.second)
        {
            FlatHexa* h = hy.second;
            f << "case" << std::endl;
            f << h->getCoord().x + h->getCoord().y/2 << " " << h->getCoord().y << " " << h->getHeight() << std::endl;
            f << h->getType() << std::endl;
        }
    if(!(m_player.x == -1 && m_player.y == -1))
    {
        f << "player" << std::endl;
        f << m_player.x + m_player.y / 2 << " " << m_player.y << " " << m_grid[m_player.x][m_player.y]->getHeight() << " " << m_direction << std::endl;
    }
    f.close();
}

Level_Editor* Level_Editor::loadFromFile(const std::string& name)
{
    std::fstream f;
    std::string line;
    Level_Editor* level = new Level_Editor(name);
    int coordX = 0;
    int coordY = 0;
    int coordZ = 0;
    int direction = 0;
    int type = 0;
    std::string filePath;
    filePath = "ressources/Levels/Editor/" + name;
    f.open(filePath.c_str(), std::ios::in);
    if( f.fail() )
    {
        std::cerr << "ouverture en lecture impossible (fichier niveau)" << std::endl;
        exit(EXIT_FAILURE);
    }
    f >> line;
    while( !f.eof() )
    {
        if(line == "case")
        {
            f >> coordX;
            f >> coordY;
            f >> coordZ;
            f >> type;

            level->addHex({coordX - coordY/2, coordY, coordZ}, type);
        }
        else if(line == "player")
        {
            f >> coordX;
            f >> coordY;
            f >> coordZ;
            f >> direction;

            level->setPlayer({coordX - coordY/2, coordY});
            level->initDirection(direction);
        }
        f >> line;
    }

    f.close();
    return level;
}

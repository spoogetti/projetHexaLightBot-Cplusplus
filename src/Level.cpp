#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "../include/Level.h"

Level::Level(std::string  name)
    : m_player { nullptr },
      m_name {std::move( name )}
{
}

Level::~Level()
{
    delete m_player;
    for( const auto& hexagonY : m_hexagons )
        for( auto hexagonX : hexagonY.second)
            delete hexagonX.second;
    m_hexagons.clear();
}

Level* Level::clone() const
{
    Level* d = new Level(m_name);
    for( auto hexagonY : m_hexagons )
        for( auto hexagonX : hexagonY.second)
            d->addHexagon(hexagonX.first, hexagonY.first, hexagonX.second->clone());
    if (m_player != nullptr)
        d->setPlayer(m_player->clone());
    return d;
}

bool Level::isCompleted()
{
    bool isCompleted = true;
    for( auto hexagonY : m_hexagons )
        for( auto hexagonX : hexagonY.second)
            if(hexagonX.second->getState() == LIT_HEXA)
                isCompleted = false;
    return isCompleted;
}

unsigned short Level::getNumber(const std::vector<std::pair<std::string, int> >& levels)
{
  //Retourne la position du niveau dans le vector
    unsigned short i = 0;
    while ( i < levels.size() && "ressources/Levels/Game/" + levels[i].first != m_name)
        i++;
    return i;
}

void Level::draw_on(sf::RenderWindow &window)
{
    Hexagon* GhostHexa;
    // dessin des hexagones
    for( auto hexagonY : m_hexagons )
        for( auto hexagonX : hexagonY.second)
        {
            for(int i=0; i<hexagonX.second->getHeight(); i++) // affiche les hexagonnes en dessous
            {
                GhostHexa = new Hexagon { hexagonX.first, hexagonY.first, i, hexagonX.second->getColor(), 0};
                GhostHexa->draw_on(window);
                delete GhostHexa;
            }

            hexagonX.second->draw_on(window);

            // dessin du sprite du personnage uniquement aux bonnes coordonnés
            if (m_player != nullptr)
                if (hexagonX.first == m_player->getPosition().x && hexagonY.first == m_player->getPosition().y)
                    m_player->draw_on(window);

        }
}

Player* Level::player()
{
    return m_player;
}

Player* Level::newPlayer()
{
  //Place un nouveau et detruit l'ancien si besoin
    if (m_player != nullptr)
        delete m_player;
    m_player = new Player();
    return m_player;
}

void Level::setPlayer(Player* player)
{
    if (m_player != nullptr)
        delete m_player;
    m_player = player;
}

void Level::moveIfPossible() //on tente de se deplace un clone, et on regarde si a l'arrivé il y a une case plus basse ou au meme niveau, si oui on bouge
{
    if (m_player != nullptr)
    {
        bool canMove = false;
        int heightDelta = -1;

        Player* p = m_player->clone();

        p->changePos();
        sf::Vector3i posPlayer = m_player->getPosition();
        sf::Vector3i newPosPlayer = p->getPosition();

        if(hexagonAtPos(newPosPlayer))
        {
            heightDelta = m_hexagons[posPlayer.y][posPlayer.x]->getHeight() - m_hexagons[newPosPlayer.y][newPosPlayer.x]->getHeight();
            if(heightDelta >= 0 && heightDelta < 3)
                canMove = true;
        }
        delete p;
        if (canMove)
            m_player->move(m_hexagons[newPosPlayer.y][newPosPlayer.x]->getHeight());
    }
}

void Level::jumpIfPossible() //on tente de se deplace un clone, et on regarde si a l'arrivé il y a une case 1 plus haut, si oui on bouge
{
    if (m_player != nullptr)
    {
        bool canJump = false;
        int heightDelta = -1;
        Player* p = m_player->clone();
        p->changePos();
        sf::Vector3i posPlayer = m_player->getPosition();
        sf::Vector3i newPosPlayer = p->getPosition();
        if(hexagonAtPos(newPosPlayer))
        {
            heightDelta = m_hexagons[posPlayer.y][posPlayer.x]->getHeight() - m_hexagons[newPosPlayer.y][newPosPlayer.x]->getHeight();
            if(heightDelta == -1)
                canJump = true;
        }
        delete p;
        if (canJump)
            m_player->move(m_hexagons[newPosPlayer.y][newPosPlayer.x]->getHeight());
    }
}

void Level::refreshPlayerHeight()
{
    if (m_player != nullptr)
    {
        m_player->setHeight(m_hexagons[m_player->getPosition().y][m_player->getPosition().x]->getHeight());
        m_player->refresh();
    }
}

std::map<int, std::map<int, Hexagon*> > Level::hexagons()
{
    return m_hexagons;
}

void Level::addHexagon(int x, int y, Hexagon* hexagon)
{
    m_hexagons[y][x] = hexagon;
}

void Level::lightOffIfLit(sf::Vector3i position)
{
    if(m_hexagons[position.y][position.x]->getState() == LIT_HEXA)
    {
        m_hexagons[position.y][position.x]->setFillColor(sf::Color::White);
        m_hexagons[position.y][position.x]->setState(SIMPLE_HEXA);
    }
}

bool Level::hexagonAtPos(sf::Vector3i position)
{
    bool hexagonExist = false;
    auto iteratorY = m_hexagons.find(position.y);
    if(iteratorY != m_hexagons.end())
    {
        auto iteratorX = m_hexagons[position.y].find(position.x);
        if (iteratorX != m_hexagons[position.y].end())
            hexagonExist = true;
    }
    return hexagonExist;
}

Level* Level::loadLevelFromFile(const std::string &path)
{
    std::fstream f;
    std::string line;
    Level* level= new Level(path);
    int coordX = 0;
    int coordY = 0;
    int coordZ = 0;
    int direction = 0;
    int type = 0;
    sf::Color color;
    f.open(path.c_str(), std::ios::in);
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
            if(type == LIT_HEXA)
                color = LIT;
            else
                color = STATIC;
            Hexagon* h = new Hexagon (coordX, coordY, coordZ, color, type );
            level->addHexagon(coordX, coordY, h);
        }
        else if(line == "player")
        {
            level->newPlayer();
            f >> coordX;
            f >> coordY;
            f >> coordZ;
            f >> direction;
            level->player()->setPosition({coordX, coordY, coordZ});
            level->player()->setDirection(direction);
            level->player()->refresh();
        }
        f >> line;
    }

    f.close();
    return level;
}

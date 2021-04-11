#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include "Hexagon.h"
#include "Player.h"

class Level
{
public:
    Level(std::string name);
    ~Level();
    // méthodes liées au niveau
    Level* clone() const;
    bool isCompleted();
    unsigned short getNumber(const std::vector<std::pair<std::string, int> > &levels);
    void draw_on(sf::RenderWindow &window);

    // méthodes liées au joueur
    Player* player();
    Player* newPlayer();
    void setPlayer(Player*);
    void moveIfPossible();
    void jumpIfPossible();
    void refreshPlayerHeight();

    // méthodes liées aux hexagones
    std::map<int, std::map<int, Hexagon*> > hexagons();
    void addHexagon(int x, int y, Hexagon*);
    void lightOffIfLit(sf::Vector3i);
    bool hexagonAtPos(sf::Vector3i);

    static Level* loadLevelFromFile(const std::string &path);

private :
    Player* m_player;
    // Les clés des map sont respectivement Y puis X pour correspondre à un affichage axial ( suite à une conversion de coordonées cubique )
    std::map<int, std::map<int, Hexagon*> > m_hexagons;
    std::string m_name;
};

#endif // GRID_H

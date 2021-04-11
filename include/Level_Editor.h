#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#include <iostream>

#include "FlatHexa.h"
#include "vector"
#include "Button.h"

const int MAX_RADIUS = 100;
const int MIN_RADIUS = 20;

class Level_Editor
{

public:
  Level_Editor(const std::string &name);
  ~Level_Editor();

  // Methodes de manipulation d'hexagonnes
  sf::Vector2i mouseToHex(sf::Vector2f mousePos);
  void addHex(sf::Vector3i cubicPos, int type);
  void removeHex(sf::Vector2i axialPos);
  bool hexagonAtPos(sf::Vector2i axialPos);
  void setPlayer(sf::Vector2i axialPos);
  void changeDirection();
  void refreshDirection();
  void initDirection(int dir);
  std::string getName();

  void draw_on(sf::RenderWindow &window);

  // Methodes de manipulation de fichiers
  void saveInFile(std::string name, int creatorScore);
  static Level_Editor* loadFromFile(const std::string &name);

private:
  std::string m_level_name;
  std::map <int, std::map <int, FlatHexa*> > m_grid;
  int m_max_x = 0; // nombre max d'hexagonnes en x
  int m_max_y = 0; // nombre max d'hexagonnes en y
  sf::Vector2i m_player = {-1, -1}; // Emplacement du joueur (-1, -1) par defaut
  int m_direction = 2;
  sf::Texture m_text_direction;
  sf::Sprite m_sprite_direction;
};

#endif // LEVEL_EDITOR_H

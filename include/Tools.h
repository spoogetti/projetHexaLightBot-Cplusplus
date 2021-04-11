#ifndef TOOLS_H
#define TOOLS_H
#include <SFML/Graphics.hpp>
#include <string>
#include <set>

///Instruction.h
using Position = sf::Vector2f;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int INSTR_SIZE = 50; // taille de l'incone des instructions
const int MARGIN = (7./100)*SCREEN_HEIGHT; // marge pour une meilleure lisibilité
const int SMALL_MARGIN = (3./100)*SCREEN_HEIGHT;

const float MARGIN_LEFT_PROG = SCREEN_WIDTH - INSTR_SIZE*4 - MARGIN;
const float MARGIN_TOP_SP1 = SMALL_MARGIN*2 + INSTR_SIZE*3;
const float MARGIN_TOP_SP2 = SMALL_MARGIN*3 + INSTR_SIZE*5;


///Appli.h
// Couleur des élément du jeu
const sf::Color BACKGROUND = sf::Color(159,	214, 235);         // bleu
const sf::Color BRIGHT_BACKGROUND = sf::Color(139, 194, 215);  // bleu moins bleu
const sf::Color COMPLEMENTARY_BACKGROUND = sf::Color(201, 132, 215, 75); // violet complémentaire

const sf::Color START_BUTTON = sf::Color::Green;
const sf::Color STOP_BUTTON = sf::Color::Red;

const std::string POLICE = "ressources/FreeSans.ttf"; // police des textes
const int FPS = 120;                                  // nombre d'images par secondes

// Variables instructions
const float INSTR_SIZE_SCALED = 100; // taille de l'icone des intructions après agrandissement
const int NB_DIFF_INSTRUCTIONS = 7;

// Variables d'animation
const float ANIMATION_TIME = 100;  // temps en milisecondes entre chaque actions
const float FRAME_TIME = 0.04;     // temps en secondes entre chaque frame


/// Tailles des boutons
const sf::Vector2f LEVEL_BUTTON_SIZE      = { (18./100)* SCREEN_WIDTH, (10./100)* SCREEN_HEIGHT};
const sf::Vector2f MENU_BUTTON_SIZE       = { (27./100)* SCREEN_WIDTH, (18./100)* SCREEN_HEIGHT};
const sf::Vector2f STARTSTOP_BUTTON_SIZE  = { (16./100)* SCREEN_WIDTH, (7./100)* SCREEN_HEIGHT};
const sf::Vector2f HEXA_TYPE_BUTTON_SIZE  = { ( 8./100)* SCREEN_WIDTH, (14./100)* SCREEN_HEIGHT};

// Textures des boutons
const int TILE_SCALE = 2; // échelle de taille des boutons
const int TILE_SIZE = 16; // taille en pixels d'un des 9 eléments constitutifs d'un bouton

///Level.h
const int MAX_GRID_LENGTH = 9;     // Longeur de la grille
const int MAX_GRID_WIDTH = 7;      // Largeur de la grille
const float xORIGIN = MARGIN;      // origine en y
const float yORIGIN = MARGIN;      // origine en x

// Pour le placement en fonction des hexagonnes
const float HEXAGON_HEIGHT = 20;         // Hauteur de l'Hexagone
const float HEXAGON_WIDTH = 100;         // Largeur de l'Hexagone
const float HEXAGON_DEPTH = 87;          // Hauteur de la "plateforme" de l'Hexagone
const int RADIUS = HEXAGON_WIDTH/2;      // rayon des hexagones
const int MARGIN_TOP_EDITOR = 160;
const int MAX_HEXA = 8;                  // Nombre maximum d'hexagon par ligne et colonne - 1

const sf::Color LIT = BRIGHT_BACKGROUND;
const sf::Color STATIC = sf::Color(255, 247, 247);

// Nombre de niveaux par pages dans l'ecran de selction
const unsigned  LEVELS_BY_PAGE = 16;

// Type des hexagonnes pour le traitement dans l'editeur
enum HEXA_TYPE { GHOST = 0, PLAYER = 1, SIMPLE_HEXA = 2, LIT_HEXA = 3};
// Types d'instructions
enum INSTR_TYPE {FORWARD = 0, LIGHT_UP = 1, ROTATE_LEFT = 2, ROTATE_RIGHT = 3, JUMP = 4, SP1 = 5, SP2 = 6, NONE = 7};

#endif // TOOLS_H

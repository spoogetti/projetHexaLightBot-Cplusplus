#ifndef APPLI_H
#define APPLI_H

#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <stack>
#include <string>

#include "Tools.h"
#include "Level.h"
#include "Level_Editor.h"
#include "Player.h"
#include "Program.h"
#include "Button.h"

class Appli
{
public:
    /// Appli.cpp
    Appli();
    void run();

private:
    // Etats généraux
    enum class State { MENU, EDITOR_LEVEL_SELECT, GAME_LEVEL_SELECT, EDITOR, TEST_LEVEL, GAME } m_generalState = State::MENU;
    // Instruction à ajouter
    enum class InstrAdd { FORWARD, LIGHT_UP, ROTATE_LEFT, ROTATE_RIGHT, JUMP, SP1, SP2, NONE } m_instrAdd = InstrAdd::NONE;
    // Etats du mode jeu
    enum class Animation { START, STOP, WIN } m_animation = Animation::STOP;
    // Programme sélectionné
    enum class Selected_program { MAIN, SP1, SP2 } m_selected_program = Selected_program::MAIN;

    sf::RenderWindow m_window;
    bool             m_appli_running;

    Program* m_main; // Programme principal
    Program* m_sp1;  // Sous-programme 1
    Program* m_sp2;  // Sous-Programme 2

    // attributs liés aux niveaux
    std::vector<std::pair<std::string, int> > m_levels; // nom des niveaux et score

    Level* m_base_level;           // niveau à copier à chaque reset
    Level* m_level_selected;       // niveau modifié
    Level_Editor* m_edited_level;  // niveau edité

    sf::Vector2f m_mouse;          // position de la souris
    sf::Mouse::Button m_button;    // évènement souris appuyé/relâché

    sf::Font m_police;

    // textures des hexagones et instructions
    sf::Texture m_text_hexagons;
    sf::Texture m_text_instructions;

    // sprites des hexagonnes et instructions
    sf::Sprite m_sprite_instructions;
    sf::Sprite m_sprite_addInstr;

    // texture des scores
    sf::Texture m_text_scores;

    // sprite des scores
    sf::Sprite m_sprite_scores;

    /// Boutons :
    /// Partout

    Button m_button_back { {(91./100)*SCREEN_WIDTH , (84./100)*SCREEN_HEIGHT},  {(7./100)*SCREEN_WIDTH, (7./100)*SCREEN_WIDTH}, sf::Color::White, "Back", 20 };

    /// Menu
    Button m_button_game   { {( 3./100)*SCREEN_WIDTH, SMALL_MARGIN}, MENU_BUTTON_SIZE, sf::Color::Yellow, "GAME", 75 };
    Button m_button_editor { {(37./100)*SCREEN_WIDTH, SMALL_MARGIN}, MENU_BUTTON_SIZE, sf::Color::Green, "EDITOR", 75 };
    Button m_button_quit   { {(72./100)*SCREEN_WIDTH, SMALL_MARGIN}, MENU_BUTTON_SIZE, sf::Color::Red, "QUIT", 75 };

    /// Level select
    // - ajout d'un niveau
    Button m_button_editor_add  { {(91./100)*SCREEN_WIDTH, (28./100)*SCREEN_HEIGHT}, {(7./100)*SCREEN_WIDTH, (7./100)*SCREEN_WIDTH}, sf::Color::White, "Add", 20 };

    // - incrémenter la page de niveau ( LEVELS_BY_PAGE niveau par pages)
    Button m_button_prev { {(91./100)*SCREEN_WIDTH, ( 45./100)*SCREEN_HEIGHT}, {(7./100)*SCREEN_WIDTH, (7./100)*SCREEN_WIDTH}, sf::Color::White, "Prev", 20 };
    Button m_button_next { {(91./100)*SCREEN_WIDTH, (61.5/100)*SCREEN_HEIGHT}, {(7./100)*SCREEN_WIDTH, (7./100)*SCREEN_WIDTH}, sf::Color::White, "Next", 20 };
    // Nombre de fois que le boutton "next" a été "préssé" dans l'ecran de selection de niveau
    int m_next_pressed = 0;

    /// Jeu
    // - lancer ou arrêter en l'animation
    Button m_button_startStop { {(70./100)*SCREEN_WIDTH , (84./100)*SCREEN_HEIGHT}, STARTSTOP_BUTTON_SIZE, START_BUTTON, "Start", 30 };

    /// Editeur

    // - sauvegarder sans tester le niveau dans l'editeur
    Button m_button_editor_test   { {(61./100)*SCREEN_WIDTH, MARGIN}, {(10./100)*SCREEN_WIDTH, MARGIN}, sf::Color::Yellow, "Test", 30 };
    // - lancer le niveau dans l'editeur
    Button m_button_editor_save   { {(74./100)*SCREEN_WIDTH, MARGIN}, {(10./100)*SCREEN_WIDTH, MARGIN}, sf::Color::Green, "Save", 30 };
    // - supprime le niveau dans l'editeur
    Button m_button_editor_delete { {(87./100)*SCREEN_WIDTH, MARGIN}, {(10./100)*SCREEN_WIDTH, MARGIN}, sf::Color::Red, "Delete", 30 };
    // - dans l'editeur (un par type de cases)
    std::map<int, Button*> m_types_button;

    // Nom du niveau chargé
    std::string m_level_name;

    // Animation sur le menu principal
    std::vector<std::pair<Player,bool>> m_menu_animation;

    // id du type actuellement selectionné
    int m_selected_type;

    // Crédits : HexalightBot - IUT de Bordeaux - Roubaix Romain Dumartin Lucas
    sf::Text m_credit;

    /// Appli.cpp
    void draw();
    void process_events();
    void draw_editor();
    void draw_level_select();
    void draw_game();
    void draw_mascot();

    /// AppliEvent.cpp
    // Evenements clavier
    void keyPressed(sf::Keyboard::Key code);
    // Evenements souris
    void mousePressed();
    void mouseReleased();
    void mouseMoved();
    void scrollBarEvents(sf::Event& event);
    // Methodes editeur
    void selectEditor(Position);
    void EditorEvents();

    /// AppliTools.cpp
    // Methodes d'initialisations
    void init_hexaType_buttons(); // initialise les boutons pour choisir le type de case que l'on veux ajouter dans l'editeur "manuel"
    void initState(Appli::State state);
    // Methodes pour l'ajout d'instructions
    void selectProgram(Program *program);
    void selectInProgram(Program *program);
    void selectInstruction();
    // Methodes pour l'animation
    void selectStart();
    void selectStop();
    void runAnimation();
    void tempo();
    // Methodes pour manipuler les niveaux et fichiers
    void loadFileNamesAndScores(std::string filePath);
    void resetLevel();
    int getScore(Program *program);
    void loadNextLevel();
    void refreshUserScore(int score);
    void chooseLevelName();
};

#endif // APPLI_H

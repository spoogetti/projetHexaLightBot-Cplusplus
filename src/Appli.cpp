#include <fstream>
#include <cstdlib>
#include <dirent.h>

#include <string>
#include <array>
#include <vector>
#include <iostream>

#include <time.h>
#include <stdio.h>

#include "../include/Appli.h"
#include "../include/Level.h"
#include "../include/Player.h"

Appli::Appli()
{
    if (! m_police.loadFromFile(POLICE)) throw "Police "+POLICE+" manquante";

    // Chargement des textures d'hexagonnes
    if (!m_text_hexagons.loadFromFile("ressources/hexagon.png")){exit(EXIT_FAILURE);}
    m_text_hexagons.setSmooth(true);

    // Chargement des textures d'instructions
    if (!m_text_instructions.loadFromFile("ressources/Instructions.png")){exit(EXIT_FAILURE);}

    // Chargement des sprites d'instructions
    m_sprite_addInstr.setTexture(m_text_instructions);
    m_sprite_instructions.setTexture(m_text_instructions);
    m_sprite_instructions.setTextureRect(sf::IntRect(0, 0, INSTR_SIZE, INSTR_SIZE));
    m_sprite_instructions.setScale(2, 2);

    // Chargement des textures de scores
    if (!m_text_scores.loadFromFile("ressources/Stars.png")){exit(EXIT_FAILURE);}
    m_text_scores.setSmooth(true);

    // Chargement des sprites de scores
    m_sprite_scores.setTexture(m_text_scores);
    m_sprite_scores.setScale(.4, .4);

    m_selected_type = 0;

    m_main = new Program { 12, SMALL_MARGIN, "MAIN" };
    // Le main est le programme sélectionné par défaut
    m_main->setFillColor(COMPLEMENTARY_BACKGROUND);
    m_sp1 = new Program  { 8, MARGIN_TOP_SP1, "P1" };
    m_sp2 = new Program  { 8, MARGIN_TOP_SP2, "P2" };

    // Chargement unique des textures pour les classes PLayer, Button et Hexagon
    Player::loadFromFile();
    Hexagon::loadFromFile();
    Button::loadFromFile();

    // Rayon des hexagons du level_Editor
    FlatHexa::setRadius(35);

  for (int i = 0; i< 1; i++)
    m_menu_animation.push_back({Player(sf::Vector3i(5,8,0), rand()%6), false});

  // Affichage des crédits
  m_credit.setFont(m_police);
  m_credit.setString(" HexalightBot - IUT de Bordeaux - Roubaix Romain Dumartin Lucas ");
  m_credit.setCharacterSize(20);
  m_credit.setFillColor(sf::Color::Black);
  m_credit.setPosition(SCREEN_WIDTH - m_credit.getGlobalBounds().width, SCREEN_HEIGHT - m_credit.getGlobalBounds().height - SMALL_MARGIN/2);
}

/**
 * Appli::run()
 * ouverture de la fenêtre et boucle d'interaction
 */
void Appli::run()
{
    m_window.create(sf::VideoMode {SCREEN_WIDTH, SCREEN_HEIGHT, 32},
                    "HexaLightBot",
                    sf::Style::Close);
    m_window.setFramerateLimit(FPS);

    m_appli_running = true;

    while (m_appli_running) {
        draw();
        process_events();
    }

    delete m_main;
    delete m_sp1;
    delete m_sp2;
}

/**
 * dessin des objets à l'écran
 */
void Appli::draw() {
    srand(time(NULL));
    m_window.clear(BACKGROUND);

    if(m_generalState == State::MENU || m_generalState == State::EDITOR_LEVEL_SELECT || m_generalState == State::GAME_LEVEL_SELECT)
    {
        m_button_editor.draw_on(m_window, m_mouse);
        m_button_game.draw_on(m_window, m_mouse);
        m_button_quit.draw_on(m_window, m_mouse);
        if(m_generalState == State::EDITOR_LEVEL_SELECT || m_generalState == State::GAME_LEVEL_SELECT)
            draw_level_select();
        else
          {
            draw_mascot();
            m_window.draw(m_credit);
          }
    }
    else if(m_generalState == State::TEST_LEVEL || m_generalState == State::GAME)
        draw_game();
    else if(m_generalState == State::EDITOR)
        draw_editor();

    m_window.display();
}

/**
 * Appli::process_events
 * Convertit les évènements en appels à keyPressed, etc.
 */
void Appli::process_events()
{
    sf::Event event;
    while (m_window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed :
            m_appli_running = false;
            break;

        case  sf::Event::KeyPressed :
            keyPressed(event.key.code);
            break;

        case sf::Event::MouseButtonPressed :
            m_button = event.mouseButton.button;
            mousePressed();
            break;

        case sf::Event::MouseButtonReleased :
            m_button = event.mouseButton.button;
            mouseReleased();
            break;

        case sf::Event::MouseMoved :
            // conversion de coordonnées
            // important si la fenetre est redimensionnée
            m_mouse = m_window.mapPixelToCoords( {event.mouseMove.x, event.mouseMove.y });
            break;
        default:
            // autre événement
            break;
        }
    }
}

/**
 * @brief Appli::draw_editor
 */
void Appli::draw_editor()
{
    // Coté artistique ( pour l'instant design terne pour mettre en valeur la mise en forme )
    sf::RectangleShape fancyTop;
    fancyTop.setPosition(0, 0);
    fancyTop.setSize({SCREEN_WIDTH, (21./100)*SCREEN_HEIGHT});
    fancyTop.setFillColor(BRIGHT_BACKGROUND);

    m_edited_level->draw_on(m_window);

    sf::Vector2i axialPos = m_edited_level->mouseToHex(m_mouse);
    FlatHexa* ghost = new FlatHexa ({ axialPos.x, axialPos.y, -1}, GHOST);
    ghost->draw_on(m_window);
    delete ghost;

    m_window.draw(fancyTop);

    // gestion de niveau (sauvegarde, destruction, test)
    m_button_editor_delete.draw_on(m_window, m_mouse);
    m_button_editor_test.draw_on(m_window, m_mouse);
    m_button_editor_save.draw_on(m_window, m_mouse);

    // type de cases
    for (auto b : m_types_button)
        b.second->draw_on(m_window, m_mouse);

    // retour à l'ecran précédent
    m_button_back.draw_on(m_window, m_mouse);
}

/**
 * @brief Appli::draw_level_select
 */
void Appli::draw_level_select()
{
    // Button qui servira pour afficher les numéros des niveaux dans des cases pour les selectioner
    Button levelButton;

    // Variables poue le nombre de niveaux à afficher par pages
    int nbPages = (m_levels.size() / LEVELS_BY_PAGE);
    int toDisplay = 0;

    if(m_next_pressed == nbPages)
        toDisplay = m_levels.size()%LEVELS_BY_PAGE;
    else
        toDisplay = LEVELS_BY_PAGE;

    // Affichage des boutons de sélection de niveau
    for(unsigned short i=0; i < toDisplay ; i++)
    {
        // Afichage du bouton du niveau ainsi que son texte
        levelButton.setPosition({MARGIN + (MARGIN + LEVEL_BUTTON_SIZE.x) * (i%4),
                                 4 * MARGIN + (MARGIN + LEVEL_BUTTON_SIZE.y) * (i/4)});
        levelButton.setSize(LEVEL_BUTTON_SIZE);
        levelButton.setFillColor(sf::Color::White);
        levelButton.setString(m_levels[i + m_next_pressed * LEVELS_BY_PAGE].first);
        levelButton.setCharacterSize(40);
        levelButton.setTextColor(sf::Color::Black);
        levelButton.draw_on(m_window, m_mouse);

        // Affichage du score correspondant au niveau
        if (m_generalState != State::EDITOR_LEVEL_SELECT)
        {
            m_sprite_scores.setTextureRect({(int)INSTR_SIZE_SCALED * (int)m_levels[i].second, 0, (int)INSTR_SIZE_SCALED, (int)INSTR_SIZE_SCALED});
            m_sprite_scores.setPosition(levelButton.getPosition().x + SMALL_MARGIN/2, levelButton.getPosition().y + SMALL_MARGIN/2);
            m_window.draw(m_sprite_scores);
        }
    }

    // Afichage du bouton "prev"
    if (m_next_pressed > 0)
        m_button_prev.draw_on(m_window, m_mouse);

    // Afichage du bouton "next"
    if (m_levels.size() > ((m_next_pressed+1) * LEVELS_BY_PAGE))
        m_button_next.draw_on(m_window, m_mouse);

    // Affichage du bouton pour ajouter un niveau
    if (m_generalState == State::EDITOR_LEVEL_SELECT)
        m_button_editor_add.draw_on(m_window, m_mouse);

    // Affichage du bouton pour revenir au  menu principal
    m_button_back.draw_on(m_window, m_mouse);
}

/**
 * @brief Appli::draw_game
 */
void Appli::draw_game()
{
    // Coté artistique
    sf::RectangleShape fancyBottom ({SCREEN_WIDTH, (25./100)*SCREEN_HEIGHT});
    fancyBottom.setPosition(0, (75./100)*SCREEN_HEIGHT );
    fancyBottom.setFillColor(BRIGHT_BACKGROUND);

    // dessin d'une ligne en bas pour rendre plus jolie l'interface
    m_window.draw(fancyBottom);

    // dessin de la ligne des instructions
    for(int i = 0; i < NB_DIFF_INSTRUCTIONS; i++)
    {
        m_sprite_instructions.setTextureRect({i*MARGIN, 0, INSTR_SIZE, INSTR_SIZE});
        m_sprite_instructions.setPosition(MARGIN + INSTR_SIZE_SCALED*i, (81.4/100)*SCREEN_HEIGHT);
        m_window.draw(m_sprite_instructions);
    }

    // dessin du niveau selectionné (joueur et hexagones)
    m_level_selected->draw_on(m_window);

    // dessin des programmes (arrière plan et instructions)
    m_main->draw_on(m_window);
    m_sp1->draw_on(m_window);
    m_sp2->draw_on(m_window);

    // dessin du boutton pour lancer ou arrêter le programme
    m_button_startStop.draw_on(m_window, m_mouse);

    // Affichage du bouton pour revenir au  menu principal, seulement si l'animation est à l'arrêt
    if(m_animation == Animation::STOP)
        m_button_back.draw_on(m_window, m_mouse);

    // dessin de l'instruction à ajouter
    if(m_instrAdd != InstrAdd::NONE)
    {
        switch(m_instrAdd)
        {
        case InstrAdd::FORWARD:
            m_sprite_addInstr.setTextureRect(sf::IntRect(0, 0, INSTR_SIZE, INSTR_SIZE));
            break;
        case InstrAdd::LIGHT_UP:
            m_sprite_addInstr.setTextureRect(sf::IntRect(INSTR_SIZE, 0, INSTR_SIZE, INSTR_SIZE));
            break;
        case InstrAdd::ROTATE_LEFT:
            m_sprite_addInstr.setTextureRect(sf::IntRect(INSTR_SIZE*2, 0, INSTR_SIZE, INSTR_SIZE));
            break;
        case InstrAdd::ROTATE_RIGHT:
            m_sprite_addInstr.setTextureRect(sf::IntRect(INSTR_SIZE*3, 0, INSTR_SIZE, INSTR_SIZE));
            break;
        case InstrAdd::JUMP:
            m_sprite_addInstr.setTextureRect(sf::IntRect(INSTR_SIZE*4, 0, INSTR_SIZE, INSTR_SIZE));
            break;
        case InstrAdd::SP1:
            m_sprite_addInstr.setTextureRect(sf::IntRect(INSTR_SIZE*5, 0, INSTR_SIZE, INSTR_SIZE));
            break;
        case InstrAdd::SP2:
            m_sprite_addInstr.setTextureRect(sf::IntRect(INSTR_SIZE*6, 0, INSTR_SIZE, INSTR_SIZE));
            break;
        default:
            break;
        }
        m_sprite_addInstr.setPosition(m_mouse - Position {INSTR_SIZE/2, INSTR_SIZE/2});
        m_window.draw(m_sprite_addInstr);
    }
}

void Appli::draw_mascot()
{
    for (std::pair<Player,bool>& o : m_menu_animation) // Partie de l'animation du personnage sur l'ecran du menu
      {
        Player& p = o.first;
        if (!p.isMoving()) // on interragi avec le personnage uniquement si il n'est pas en mouvement
          {
            if (!o.second && rand()%3 == 1)
              {
                p.turnRight();
                o.second = true;
              }
            else if (!o.second && rand()%3 == 2)
              {
                p.turnLeft();
                o.second = true;
              }

            if (p.getPosition().x <= -1) // Debut de la gestion du depacement d'ecran
              p.setDirection(0);
            if (p.getPosition().x >= 11)
              p.setDirection(3);
            if (p.getPosition().y <= 5)
              {
                if (p.getPosition().x < 5)
                  p.setDirection(5);
                if (p.getPosition().x >= 5)
                  p.setDirection(4);
              }
            if (p.getPosition().y >= 14)
              {
                if (p.getPosition().x < 5)
                  p.setDirection(1);
                if (p.getPosition().x >= 5)
                  p.setDirection(2);
              }                         //Fin

            if (rand()%5 == 0)
              {
                p.move();
                o.second = false;
              }
          }
        p.draw_on(m_window);
      }
}

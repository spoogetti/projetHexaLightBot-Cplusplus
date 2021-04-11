#include <fstream>
#include <cstdlib>
#include <dirent.h>

#include <string>
#include <array>
#include <vector>
#include <iostream>
#include <stdio.h>

#include <time.h>
#include <stdio.h>

#include "../include/Appli.h"
#include "../include/Level.h"
#include "../include/Player.h"

/**
 * Que faire quand une touche est appuyée ?
 */
void Appli::keyPressed(sf::Keyboard::Key code)
{
    if (code == sf::Keyboard::Escape ) // navigation via la touche esc
    {
        switch (m_generalState)
        {
        case State::MENU :
            m_appli_running = false;
            break;
        case State::GAME_LEVEL_SELECT :
            initState(State::MENU);
            break;
        case State::GAME :
            initState(State::GAME_LEVEL_SELECT);
            break;
        case State::EDITOR :
            initState(State::EDITOR_LEVEL_SELECT);
            break;
        case State::EDITOR_LEVEL_SELECT :
            initState( State::MENU );
            break;
        case State::TEST_LEVEL :
            initState(State::EDITOR);
            break;
        };
    }
}

/**
 * bouton de la souris appuyé ?
 */
void Appli::mousePressed()
{
    // capture de la position de la souris
    sf::Vector2f d;

    if(m_generalState == State::MENU || m_generalState == State::EDITOR_LEVEL_SELECT || m_generalState == State::GAME_LEVEL_SELECT)
    {
        // Evenements boutons du menu
        if (m_button_game.contains(m_mouse))
        {
            // Récupération des noms et scores des niveau (pour les boutons)
            loadFileNamesAndScores("ressources/Levels/Game/");
            initState(State::GAME_LEVEL_SELECT);
        }
        else if (m_button_editor.contains(m_mouse))
        {
            // Récupération des noms et scores des niveau (pour les boutons)
            loadFileNamesAndScores("ressources/Levels/Editor/");
            initState(State::EDITOR_LEVEL_SELECT);
        }
        else if (m_button_quit.contains(m_mouse))
            m_appli_running = false;


        // Evenements boutons de choix de niveau
        if(m_generalState == State::EDITOR_LEVEL_SELECT || m_generalState == State::GAME_LEVEL_SELECT)
        {
            // Clic sur le bouton de niveaux
            for(unsigned short i=0; i<LEVELS_BY_PAGE; i++)
            {

                d = m_mouse - sf::Vector2f(MARGIN + (MARGIN + LEVEL_BUTTON_SIZE.x) * (i%4),
                                           4 * MARGIN + (MARGIN + LEVEL_BUTTON_SIZE.y) * (i/4) );
                if (0 <= d.x and d.x <= LEVEL_BUTTON_SIZE.x and 0 <= d.y and d.y <= LEVEL_BUTTON_SIZE.y)
                {
                    m_level_name = m_levels[i + m_next_pressed * LEVELS_BY_PAGE].first;
                    if (m_generalState == State::GAME_LEVEL_SELECT)
                        initState(State::GAME);
                    else if (m_generalState == State::EDITOR_LEVEL_SELECT)
                        initState(State::EDITOR);
                }
            }

            // si le bouton "prev" existe
            if (m_next_pressed > 0)
            {
                // Position relative du bouton next
                d = m_mouse - sf::Vector2f((91./100)*SCREEN_WIDTH, (45./100)*SCREEN_HEIGHT);
                // Clic sur le bouton "next"
                if(0 <= d.x and d.x <= (7./100)*SCREEN_WIDTH and 0 <= d.y and d.y <= (7./100)*SCREEN_WIDTH)
                    m_next_pressed--;
            }

            // si le bouton "next" existe
            if (m_levels.size() > ((m_next_pressed+1) * LEVELS_BY_PAGE))
            {
                // Position relative du bouton prev
                d = m_mouse - sf::Vector2f((91./100)*SCREEN_WIDTH, (61.5/100)*SCREEN_HEIGHT);
                // Clic sur le bouton "prev"
                if(0 <= d.x and d.x <= (7./100)*SCREEN_WIDTH and 0 <= d.y and d.y <= (7./100)*SCREEN_WIDTH)
                    m_next_pressed++;
            }

            // Clic sur le bouton d'ajout de niveau
            if(m_generalState == State::EDITOR_LEVEL_SELECT)
                if (m_button_editor_add.contains(m_mouse))
                    chooseLevelName();
        }
    }

    // Evenements en mode jeu
    else if(m_generalState == State::TEST_LEVEL || m_generalState == State::GAME)
    {
        if(m_animation == Animation::STOP) // si l'animation est arrétée
        {
            // Selection du programme pour l'ajout "rapide"
            if(m_main->contains(m_mouse))
                selectProgram(m_main);
            else if(m_sp1->contains(m_mouse))
                selectProgram(m_sp1);
            else if(m_sp2->contains(m_mouse))
                selectProgram(m_sp2);

            // Selection des instructions pour l'ajout "lent"
            selectInstruction();
            selectInProgram(m_main);
            selectInProgram(m_sp1);
            selectInProgram(m_sp2);
            selectStart();
        }
        else if(m_animation == Animation::START) // si l'animation est lancé
            selectStop();
    }
    // Evenements en mode editeur
    else if(m_generalState == State::EDITOR)
        EditorEvents();

    // Clic sur le bouton back, présent dans tous les états sauf menu
    if(m_generalState != State::MENU && m_animation != Animation::START)
        if(m_button_back.contains(m_mouse))
            keyPressed(sf::Keyboard::Escape); // Exploitation de la méthode keypressed
}
/**
 * bouton de la souris relâché ?
 */
void Appli::mouseReleased()
{
    if (m_generalState == State::GAME || m_generalState == State::TEST_LEVEL)
    {
        int instrToAdd;
        // capture de l'instruction à ajouter
        switch(m_instrAdd)
        {
        case InstrAdd::FORWARD:
            instrToAdd = FORWARD;
            break;
        case InstrAdd::LIGHT_UP:
            instrToAdd =  LIGHT_UP;
            break;
        case InstrAdd::ROTATE_LEFT:
            instrToAdd =  ROTATE_LEFT;
            break;
        case InstrAdd::ROTATE_RIGHT:
            instrToAdd =  ROTATE_RIGHT;
            break;
        case InstrAdd::JUMP:
            instrToAdd = JUMP;
            break;
        case InstrAdd::SP1:
            instrToAdd = SP1;
            break;
        case InstrAdd::SP2:
            instrToAdd = SP2;
            break;
        default :
            instrToAdd = NONE;
            break;
        }

        // capture de la position de la souris
        sf::Vector2f d;

        // si il y à une instruction à ajouter
        if (instrToAdd != NONE)
        {
            if(m_main->contains(m_mouse))
                m_main->insertInstruction(m_main->isBetweenInstructions(m_mouse), instrToAdd);
            // gestion du cas particulier où l'instruction est celle résumant un programme
            else if (/*instrToAdd != SP1 &&*/ m_sp1->contains(m_mouse))
                    m_sp1->insertInstruction(m_sp1->isBetweenInstructions(m_mouse), instrToAdd);
            else if (/*instrToAdd != SP2 &&*/ m_sp2->contains(m_mouse))
                    m_sp2->insertInstruction(m_sp2->isBetweenInstructions(m_mouse), instrToAdd);
            // si on clique sur une instruction sans qu'elle ne soit dans un programme, l'ajoute au programme sélectionné
            else
            {
                // dessin de la ligne des instructions
                for(int i = 0; i < NB_DIFF_INSTRUCTIONS; i++)
                {
                    d = m_mouse - sf::Vector2f(MARGIN + INSTR_SIZE_SCALED*i, (83./100)*SCREEN_HEIGHT);
                    if(0 <= d.x and d.x <= INSTR_SIZE_SCALED and 0 <= d.y and d.y <= INSTR_SIZE_SCALED)
                    {
                        // Ajoute l'instruction au programme sélectionné
                        switch (m_selected_program) {
                        case Selected_program::MAIN:
                            m_main->insertInstruction(m_main->isBetweenInstructions(m_mouse), instrToAdd);
                            break;
                        case Selected_program::SP1:
                                m_sp1->insertInstruction(m_sp1->isBetweenInstructions(m_mouse), instrToAdd);
                            break;
                        case Selected_program::SP2:
                                m_sp2->insertInstruction(m_sp2->isBetweenInstructions(m_mouse), instrToAdd);
                            break;
                        }
                        break; // quitte la boucle une fois l'instruction trouvée
                    }
                }
            }
            // dans tous les cas quand on relache la souris, l'instruction disparait
            m_instrAdd = InstrAdd::NONE;
        }
    }
}

/**
 * souris déplacée ?
 */
void Appli::mouseMoved()
{
}

/**
 * @brief Appli::manualEditorEvents
 */
void Appli::EditorEvents()
{
  //Recuperation de la position de l'hexagon sous la souris
    sf::Vector2i Pos = m_edited_level->mouseToHex(m_mouse);
    FlatHexa* ghostHexa = new FlatHexa ({ Pos.x, Pos.y, -1}, GHOST);

    if (m_button == sf::Mouse::Left)
    {
        //Placer une nouvelle case
        if (m_selected_type != PLAYER)
            m_edited_level->addHex({Pos.x, Pos.y, 0}, m_selected_type);
        //Placer le joueur sur une case
        else
          m_edited_level->setPlayer({Pos.x, Pos.y});

      }
    //Suppresion d'un hexagon de la grille si present sous la souris
    else if (m_button == sf::Mouse::Right)
        m_edited_level->removeHex({Pos.x, Pos.y});

    delete ghostHexa;

    // si on clique sur le bouton test
    if (m_button_editor_test.contains(m_mouse))
    {
        m_edited_level->saveInFile(m_level_name, 0);
        m_level_name = m_edited_level->getName();
        initState(State::TEST_LEVEL);
    }
    // si on clique sur le bouton save
    if (m_button_editor_save.contains(m_mouse))
        m_edited_level->saveInFile(m_level_name, 0);
    // si on clique sur le bouton delete
    if (m_button_editor_delete.contains(m_mouse))
    {
        // Chemin du fichier à supprimer
        std::string filePath = "ressources/Levels/Editor/" + m_level_name;
        // Suppression du fichier (support physique)
        std::remove(filePath.c_str());
        // Suppression du fichier (support logique)
        std::vector<std::pair<std::string, int> > tempLevels;

        for(auto pair : m_levels)
            if (pair.first != m_level_name)
                tempLevels.push_back(pair);
        m_levels.clear();
        for (auto newPair : tempLevels)
            m_levels.push_back(newPair);
        // Retour au menu de choix du niveaux
        initState(State::EDITOR_LEVEL_SELECT);
    }
    //Pour chaque bouton de type
    for (auto button : m_types_button)
        if (button.second->contains(m_mouse))
        {
            m_types_button[m_selected_type]->setFillColor(sf::Color::Transparent);
            m_selected_type = button.second->getType();
            m_types_button[m_selected_type]->setFillColor(sf::Color::White);
        }
}



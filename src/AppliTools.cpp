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

/**
 * @brief Appli::init_hexaType_buttons
 */
void Appli::init_hexaType_buttons()
{
  // Bouton pour placer le joueur
  sf::Vector2f pos = { MARGIN, MARGIN/2 };
  Button* player = new Button (pos, HEXA_TYPE_BUTTON_SIZE);
  player->setType(PLAYER);
  player->setCharacterSize(20);
  player->setString("Player");
  player->setFillColor(sf::Color::Transparent);
  // /!\ l'index dans le vecteur des bouton est aussi le numéro caractérisant le type
  m_types_button[PLAYER] = player;

  // Bouton pour placer les hexagonnes simples
  pos = { 2 * MARGIN + HEXA_TYPE_BUTTON_SIZE.x, MARGIN/2 };
  Button* hexagon = new Button (pos, HEXA_TYPE_BUTTON_SIZE);
  hexagon->setType(SIMPLE_HEXA);
  hexagon->setCharacterSize(20);
  hexagon->setString("Hexagon");

  // Bouton choisi par défaut
  hexagon->setFillColor(sf::Color::White); // highlight le type de case séléctionné
  m_selected_type = SIMPLE_HEXA;

  m_types_button[SIMPLE_HEXA] = hexagon;


  // Bouton pour placer les hexagonnes lumineux
  pos = { 3 * MARGIN + HEXA_TYPE_BUTTON_SIZE.x * 2, MARGIN/2 };
  Button* light = new Button (pos, HEXA_TYPE_BUTTON_SIZE);
  light->setType(LIT_HEXA);
  light->setCharacterSize(20);
  light->setString("Light");
  light->setFillColor(sf::Color::Transparent);
  m_types_button[LIT_HEXA] = light;
}

/**
 * @brief Appli::initState
 * @param state
 */
void Appli::initState(Appli::State state)
{
  //On commence par "nétoyer" l'etat actuel
  switch (m_generalState)
    {
    case State::TEST_LEVEL:
    case State::GAME :
      // change la couleur du boutton startStop
      m_button_startStop.setFillColor(START_BUTTON);
      m_button_startStop.setString("Start");
      m_animation = Animation::STOP;
      // Supprime les niveaux
      delete m_base_level;
      delete m_level_selected;
      // Vide les programmes
      m_main->clear();
      m_sp1->clear();
      m_sp2->clear();
      // Remet le main comme programme sélectionné
      selectProgram(m_main);
      break;
    case State::EDITOR :
      if ( state != State::TEST_LEVEL) // supprime uniquement si on sort du test de niveaux
        {
          for (auto button : m_types_button)
            delete button.second;
          m_types_button.clear();
          delete m_edited_level;
        }
      break;
    default:
      // Pour eviter de charger la nième page de l'autre selection de niveau
      m_next_pressed = 0;
      break;
    }
  // Puis on charge l'etat désiré
  std::string filePath;

  switch (state) {
    case State::EDITOR :
      if (m_generalState != State::TEST_LEVEL)
        {
          m_edited_level = Level_Editor::loadFromFile(m_level_name);
          init_hexaType_buttons();
        }
      m_button_startStop.setFillColor(START_BUTTON);
      m_button_startStop.setString("Start");
      m_animation = Animation::STOP;
      m_generalState = state;
      break;
    case State::TEST_LEVEL :
    case State::GAME :
      // Chargement de la texture des instructions
      m_main->setInstrTexture(m_text_instructions);
      m_sp1->setInstrTexture(m_text_instructions);
      m_sp2->setInstrTexture(m_text_instructions);

      m_generalState = state;

      // Chargement du niveau
      if (m_generalState == State::GAME)
        filePath = "ressources/Levels/Game/" + m_level_name;
      else
        filePath = "ressources/Levels/Editor/" + m_level_name;

      m_base_level = Level::loadLevelFromFile(filePath);
      m_level_selected = m_base_level->clone();
      break;
    default:
      m_generalState = state;
      break;
    }
}

/**
 * @brief Appli::selectProgram
 */
void Appli::selectProgram(Program* program)
{
  std::string programTag = program->getTag();
  if(programTag == "MAIN") {
      m_selected_program = Selected_program::MAIN;
      // Change la couleur pour savoir quel programme est séléctionné
      m_main->setFillColor(COMPLEMENTARY_BACKGROUND);
      m_sp1->setFillColor(BRIGHT_BACKGROUND);
      m_sp2->setFillColor(BRIGHT_BACKGROUND);
    }
  else if (programTag == "P1") {
      m_selected_program = Selected_program::SP1;
      m_main->setFillColor(BRIGHT_BACKGROUND);
      m_sp1->setFillColor(COMPLEMENTARY_BACKGROUND);
      m_sp2->setFillColor(BRIGHT_BACKGROUND);

    }
  else if (programTag == "P2") {
      m_selected_program = Selected_program::SP2;
      m_main->setFillColor(BRIGHT_BACKGROUND);
      m_sp1->setFillColor(BRIGHT_BACKGROUND);
      m_sp2->setFillColor(COMPLEMENTARY_BACKGROUND);

    }
}

/**
 * @brief Appli::selectInProgram
 * @param program
 * si on clique sur une instruction dans le programme passé en paramètre => change l'etat d'ajout
 */
void Appli::selectInProgram(Program* program)
{
  // i est l'indice de l'instruction à supprimer dans le tableau d'instruction parcouru (si il y à suppression)
  unsigned short i = 0;
  if(program->contains(m_mouse))
    for(auto instr : program->instructions())
      {
        if (instr->contains(m_mouse) && m_instrAdd == InstrAdd::NONE)
          {
            switch(instr->getType())
              {
              case FORWARD:
                m_instrAdd = InstrAdd::FORWARD;
                break;
              case LIGHT_UP:
                m_instrAdd = InstrAdd::LIGHT_UP;
                break;
              case ROTATE_LEFT:
                m_instrAdd = InstrAdd::ROTATE_LEFT;
                break;
              case ROTATE_RIGHT:
                m_instrAdd = InstrAdd::ROTATE_RIGHT;
                break;
              case JUMP:
                m_instrAdd = InstrAdd::JUMP;
                break;
              case SP1:
                m_instrAdd = InstrAdd::SP1;
                break;
              case SP2:
                m_instrAdd = InstrAdd::SP2;
                break;
              }
            program->refreshInstructions(i);
          }
        i++;
      }
}

/**
 * @brief Appli::selectInstruction
 * si on clique sur les icones d'instructions => change l'état d'ajout
 */
void Appli::selectInstruction()
{
  int i = 0;
  // capture de la postion de la souris ( position des instruction en jeu )
  Position InstrButtonPos = {MARGIN + INSTR_SIZE_SCALED, SCREEN_HEIGHT - MARGIN};
  Position d = InstrButtonPos - m_mouse;
  // Si on clique sur une instruction
  while(i < NB_DIFF_INSTRUCTIONS && m_instrAdd == InstrAdd::NONE)
    {
      if(0 <= d.x and d.x <= INSTR_SIZE_SCALED and 0 <= d.y and d.y <= INSTR_SIZE_SCALED)
        {
          switch(i)
            {
            case FORWARD:
              m_instrAdd = InstrAdd::FORWARD;
              break;
            case LIGHT_UP:
              m_instrAdd = InstrAdd::LIGHT_UP;
              break;
            case ROTATE_LEFT:
              m_instrAdd = InstrAdd::ROTATE_LEFT;
              break;
            case ROTATE_RIGHT:
              m_instrAdd = InstrAdd::ROTATE_RIGHT;
              break;
            case JUMP:
              m_instrAdd = InstrAdd::JUMP;
              break;
            case SP1:
              m_instrAdd = InstrAdd::SP1;
              break;
            case SP2:
              m_instrAdd = InstrAdd::SP2;
              break;
            default:
              m_instrAdd = InstrAdd::NONE;
            }
        }
      InstrButtonPos = { MARGIN + (INSTR_SIZE_SCALED*(i+1)),
                         (81.4/100)*SCREEN_HEIGHT };
      d = m_mouse - InstrButtonPos;
      i++;
    }
}

/**
 * @brief Appli::selectStart
 * Gère le clic sur le bouton start
 */
void Appli::selectStart()
{
  // si on clique sur le bouton pour lancer l'animation
  if(m_button_startStop.contains(m_mouse))
    {
      // Change l'état d'animation
      m_animation = Animation::START;
      // Change la couleur du bouton pour passer en mode "stop"
      m_button_startStop.setFillColor(STOP_BUTTON);
      m_button_startStop.setString("Stop");
      // Lance l'animation
      runAnimation();
      // Si le niveau est gagné
      if(m_animation == Animation::WIN)
        {
          if (m_generalState == State::GAME)
            loadNextLevel();
          else if(m_generalState == State::TEST_LEVEL)
            initState(State::EDITOR);
          m_animation = Animation::STOP;
        }
      // Note: Il n'y à pas d'effet si le niveau est perdu, l'utilisateur doit cliquer sur le bouton stop
    }
}

/**
 * @brief Appli::selectStop
 * Gère le clic sur le bouton stop
 */
void Appli::selectStop()
{
  //Reset du bouton Start/Stop en position pres a redemarer une sequence d'instruction
  if(m_button_startStop.contains(m_mouse))
    {
      resetLevel();
      m_button_startStop.setFillColor(START_BUTTON);
      m_button_startStop.setString("Start");
      m_animation = Animation::STOP;
    }
}

/**
 * @brief Appli::runAnimation
 * @param program
 */
void Appli::runAnimation()
{
  // variable pour la capture de la position du joueur
  sf::Vector3i newPosPlayer;
  Player* player = m_level_selected->player();
  if (player != nullptr)
    {
      m_main->reset_pos();
      m_sp1->reset_pos();
      m_sp2->reset_pos();

      std::stack<Program*> history;
      history.push(m_main);
      Program* currentProgram = m_main;

      int score = 0; // nombre d'instruction exécutées avant de gagner
      while ( !history.empty() && ( m_animation == Animation::START ) )
        {
          newPosPlayer = m_level_selected->player()->getPosition();
          if (currentProgram->end())
            {
              history.pop();
              if (!history.empty())
                {
                  currentProgram = history.top();
                  currentProgram->read_next();
                }
            }
          else //if (!currentProgram->end())
            switch(currentProgram->read()->getType())
              {
              case(FORWARD):
                m_level_selected->moveIfPossible();
                currentProgram->read_next();
                score++;
                break;
              case(LIGHT_UP):
                m_level_selected->lightOffIfLit(newPosPlayer); //Si la case est une lampe eteinte on l'allume
                score++;
                if(m_level_selected->isCompleted())
                {
                  m_animation = Animation::WIN;
                  // calcul et mise à jour du score
                  if(m_generalState != State::TEST_LEVEL)
                      refreshUserScore(score);
                }
                currentProgram->read_next();
                break;
              case(ROTATE_LEFT):
                tempo();
                m_level_selected->player()->turnLeft();
                currentProgram->read_next();
                score++;
                break;
              case(ROTATE_RIGHT):
                tempo();
                m_level_selected->player()->turnRight();
                currentProgram->read_next();
                score++;
                break;
              case(JUMP):
                m_level_selected->jumpIfPossible();
                currentProgram->read_next();
                score++;
                break;
              case(SP1):
                if (history.size() < 3) //signifie qu'il y a forcement une boucle infinie entre P1 et P2, donc plus besoin d'avoir d'historique
                  history.push(m_sp1);
                m_sp1->reset_pos();
                currentProgram = m_sp1;
                break;
              case(SP2):
                if (history.size() < 3)
                  history.push(m_sp2);
                m_sp2->reset_pos();
                currentProgram = m_sp2;
                break;
              }
          do {
              draw();
              process_events();
            } while ( m_animation != Animation::STOP && m_level_selected->player()->isMoving() );
        }
    }
}

void Appli::tempo()
{
  //fonction qui permet de garder le controle
  sf::Clock time;
  while(time.getElapsedTime().asMilliseconds() < ANIMATION_TIME)
    {
      draw();
      process_events();
    }
}

/**
 * @brief Appli::loadFileNamesAndScores
 * @param filePath
 */
void Appli::loadFileNamesAndScores( std::string filePath )
{
  // variables pour calculer le score du joueur
  int creatorScore = 0;
  int userScore = 0;
  bool isCalculated = false;
  // variable pour la lecture de fichiers
  std::fstream f;
  std::string line;
  // paire contenant le nom du niveau et le score du joueur
  std::pair<std::string, int> levelNameScore;

  m_levels.clear();
  // ouvre le repertoire "ressources/Levels/Game" ou "ressources/Levels/Editor"
  DIR* rep = opendir(filePath.c_str());
  if (rep != NULL)
    {
      struct dirent* ent = readdir(rep);
      while (ent != NULL)
        {
          // Capture du nom des fichiers (un par un)
          levelNameScore.first = ent->d_name;
          // Pour eviter la capture des répertoires courants et parents
          if (levelNameScore.first != "." && levelNameScore.first != "..")
            {
              // ouvre le fichier pour obtenir les scores
              f.open(filePath + levelNameScore.first.c_str(), std::ios::in);
              if( f.fail() )
                {
                  std::cerr << "ouverture en lecture impossible (fichier niveau)" << std::endl;
                  exit(EXIT_FAILURE);
                }
              f >> line;
              while( !f.eof() && !isCalculated)
                {
                  if(line == "creatorScore")
                    f >> creatorScore;
                  else if(line == "userScore")
                    {
                      f >> userScore;
                      int score = userScore - creatorScore;
                      if(score <= 0 )
                        levelNameScore.second = 0;
                      else if(score > 0 && score < 3)
                        levelNameScore.second = 1;
                      else if(score > 2 && score < 5)
                        levelNameScore.second = 2;
                      else
                        levelNameScore.second = 3;
                      isCalculated = true;
                    }
                  f >> line;
                }
              f.close();
              m_levels.push_back(levelNameScore);
              isCalculated = false;
            }
          ent = readdir(rep);
        }
      // tri par ordre lexicographique standard --cf. M.Merry
      std::sort(m_levels.begin(),m_levels.end());
      closedir(rep);
    }
}

/**
 * @brief Appli::resetLevel
 */
void Appli::resetLevel()
{
  delete m_level_selected;
  m_level_selected = m_base_level->clone();
}


/**
 * @brief Appli::loadNextLevel
 * Charge le niveau suivant :
 */
void Appli::loadNextLevel()
{
  // mise à jour du bouton pour lancer l'animation
  m_button_startStop.setFillColor(START_BUTTON);
  m_button_startStop.setString("Start");

  if ( m_base_level->getNumber(m_levels) < m_levels.size()-1) // si il y à un niveau suivant
    {
      std::string filePath = "ressources/Levels/Game/" + m_levels[m_base_level->getNumber(m_levels)+1].first;
      m_base_level = Level::loadLevelFromFile(filePath);
      m_level_selected = m_base_level->clone();
      m_main->clear();
      m_sp1->clear();
      m_sp2->clear();
    }
  else // si il n'y à plus de niveaux après
    {
      initState(State::MENU);
      m_main->clear();
      m_sp1->clear();
      m_sp2->clear();
    }
}

/**
 * @brief Appli::refreshUserScore
 * @param score
 */
void Appli::refreshUserScore(int score)
{
  /// Mise à jour dans l'application
  std::fstream f;
  std::string line;

  unsigned short levelNumber = m_level_selected->getNumber(m_levels); // capture du numéro du niveau
  std::string levelName = m_levels[levelNumber].first; // capture du nom du niveau
  std::string filePath = "ressources/Levels/Game/" + levelName; // accède au premier élément de la paire definissant un niveau <=> son nom (a.k.a son filePath relatif)

  bool isCalculated = false;
  bool betterScore = false;
  int creatorScore = 0;
  // Ouvre le fichier pour obtenir le score du créateur
  f.open(filePath.c_str(), std::ios::in);
  if( f.fail() )
    {
      std::cerr << "ouverture en lecture impossible (fichier niveau)" << std::endl;
      exit(EXIT_FAILURE);
    }
  f >> line;
  while( !f.eof() && !isCalculated)
    {
      if(line == "creatorScore")
        {
          f >> creatorScore; // récupération du score du créateur
          int newScore = score - creatorScore; // calcul du nouveau score
          betterScore = newScore < m_levels[m_level_selected->getNumber(m_levels)].second;
          if (betterScore) // Si le nouveau score est mieux que l'ancien
            {
              if(newScore <= 0 ) // met à jour le score :
                m_levels[m_level_selected->getNumber(m_levels)].second = 0;
              else if(newScore > 0 && newScore < 3)
                m_levels[m_level_selected->getNumber(m_levels)].second = 1;
              else if(newScore > 2 && newScore < 5)
                m_levels[m_level_selected->getNumber(m_levels)].second =  2;
              else
                m_levels[m_level_selected->getNumber(m_levels)].second =  3;
            }
          isCalculated = true;
        }
      f >> line;
    }
  f.close();

  // Si le nouveau score est mieux que l'ancien
  if (betterScore)
    {
      /// Mise à jour dans le fichier
      std::vector<std::string> newFile;

      f.open(filePath.c_str(), std::ios::in);
      if( f.fail() )
        {
          std::cerr << "ouverture en lecture impossible (fichier niveau)" << std::endl;
          exit(EXIT_FAILURE);
        }
      f >> line;
      while( !f.eof() )
        {
          newFile.push_back(line);
          if(line == "userScore")
            {
              f >> line;
              newFile.push_back(std::to_string(score));
            }
          f >> line;
        }
      f.close();

      f.open(filePath.c_str(), std::ios::out | std::ios::trunc); // combinaison pour mettre à jour (effacer puis réécrire)
      if( f.fail() )
        {
          std::cerr << "ouverture en écriture impossible (fichier niveau)" << std::endl;
          exit(EXIT_FAILURE);
        }
      for(size_t i=0; i<newFile.size(); i++)
        f << newFile[i] << std::endl;

      f.close();

    }
}

void Appli::chooseLevelName()
{
  // Reinitialise le texte de la boite de texte
  m_level_name = "";
  // Boite de texte
  sf::RectangleShape textBox({SCREEN_WIDTH - 2*MARGIN, MARGIN*2});
  textBox.setPosition(MARGIN, SCREEN_HEIGHT/2 - MARGIN/2);
  textBox.setFillColor(BRIGHT_BACKGROUND);
  // Texte de la boite
  sf::Text levelName;
  levelName.setFont(m_police);
  levelName.setCharacterSize(70);
  levelName.setPosition(MARGIN + SMALL_MARGIN, SCREEN_HEIGHT/2 - MARGIN/2);
  levelName.setFillColor(sf::Color::Black);

  bool levelNameChose = false;
  bool abort = false;
  sf::Event event;
  while(!levelNameChose && !abort)
    while (m_window.pollEvent(event) )
      {
        if(event.type == sf::Event::KeyPressed)
          {
            if (event.key.code == sf::Keyboard::BackSpace)
              {
                if (m_level_name.size() > 0) {
                    std::string tmp = m_level_name;
                    tmp.pop_back();
                    m_level_name = tmp;
                  }
              }
            else if(event.key.code == sf::Keyboard::Return)
              if(m_level_name.size() > 0) // si le nom du niveau n'est pas null
                levelNameChose = true;  // si on clique sur entrer, quitte la boucle
              else
                abort = true;
            else if(event.key.code == sf::Keyboard::Escape)
              abort = true;
          }
        if (event.type == sf::Event::TextEntered)
          if (event.text.unicode < 128) //Test if in ACSII Range
            {
              char inputChar = static_cast<char>(event.text.unicode);
              if ((inputChar >= 48 && inputChar <= 57) || // Test numbers
                  (inputChar >= 65 && inputChar <= 90) || // Test UpperCase characters
                  (inputChar >= 97 && inputChar <= 122) ) // Test LowerCase characters
                {
                  if (m_level_name.size() < 9)
                    m_level_name += inputChar;
                }
            }
        //Affichage du nouveau nom au fur et a mesure de son edition
        levelName.setString(m_level_name);
        m_window.draw(textBox);
        m_window.draw(levelName);
        m_window.display();
      }
  if(!abort)
    {
      std::ofstream f("ressources/Levels/Editor/" + m_level_name, std::ios::out);
      f.close();
      std::pair<std::string, int> newLevel(m_level_name, 0);
      m_levels.push_back(newLevel);
      initState(State::EDITOR);
    }
  else initState(State::EDITOR_LEVEL_SELECT);
}

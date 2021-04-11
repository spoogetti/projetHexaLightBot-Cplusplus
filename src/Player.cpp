#include "../include/Player.h"
#include "../include/Level.h"
#include <cmath>

sf::Texture Player::m_texture_idle;
sf::Texture Player::m_texture_jump;

Player::Player(sf::Vector3i position,int direction)
    : m_position { position },
      m_direction { direction }
{
    // créé le sprite
    m_sprite.setTexture(m_texture_idle);

    m_sprite.setTextureRect(sf::IntRect(0, 0, 300, 300));

    // xORIGIN et yORIGIN pour décaler l'origine
    m_sprite.setPosition( xORIGIN + HEXAGON_WIDTH * m_position.x + m_position.y%2 * RADIUS,
                          yORIGIN + HEXAGON_DEPTH / 2 * (m_position.y - 1) - HEXAGON_HEIGHT * (m_position.z + 1) );
    m_sprite.setScale(sf::Vector2f(1/3., 1/3.)); // pour passer de 300px a 100px
}

void Player::loadFromFile()
{
  // charge la texture
  if (!m_texture_idle.loadFromFile("ressources/Idle.png")){exit(EXIT_FAILURE);}
  if (! m_texture_jump.loadFromFile("ressources/Jump.png")){exit(EXIT_FAILURE);}
}

Player* Player::clone()
{
  return new Player(m_position, m_direction);
}

void Player::changePos()
{
  m_position = changePos(m_position, m_direction);
}

sf::Vector3i Player::changePos(sf::Vector3i pos, int direction) //Fonction locale utilisée uniquement dans Player::move()
{
  switch (direction) {  // sens anti-horraire en partant de la droite
    case RIGHT:     // ( +1;  0)
      pos.x++;
      break;
    case UP_RIGHT: // haut-droite  cas impair vers pair : ( +1; -1) cas pair vers impair : (  0; -1)
      if (pos.y%2 == 1 || pos.y == 0){
          pos.x++;
          pos.y--;
        }
      else{
          pos.y--;
        }
      break;
    case UP_LEFT: // haut-gauche  cas impair vers pair : ( 0; -1) cas pair vers impair: ( -1; -1)
      if (pos.y%2 == 1 || pos.y == 0){
          pos.y--;
        }
      else{
          pos.x--;
          pos.y--;
        }
      break;
    case LEFT:	 //( -1;  0)
      pos.x--;
      break;
    case DOWN_LEFT: // cas impair vers pair : (  0; +1) cas pair vers impair : ( -1; +1)
      if (pos.y%2 == 1){
          pos.y++;
        }
      else{
          pos.x--;
          pos.y++;
        }
      break;
    case DOWN_RIGHT: // cas impair vers pair : ( +1; +1) cas pair vers impair: (  0; +1)
      if (pos.y%2 == 1){
          pos.x++;
          pos.y++;
        }
      else{
          pos.y++;
        }
      break;
    default:
      break;
    }
  return pos;
}

void Player::draw_on(sf::RenderWindow &window)
{
    if (m_clock.getElapsedTime().asSeconds() >= FRAME_TIME)
    {
        if (m_is_moving)
        {
          if (m_frame == 5)
            m_position = m_end_position;
          if (m_frame >= 10)
            {
                m_frame = 0;
                m_is_moving = false;
                m_sprite.setTexture(m_texture_idle);
            }
          refresh();
        }
        else
            m_frame = m_frame%6;

        m_sprite.setTextureRect(sf::IntRect(m_frame*300, m_direction*300, 300, 300));
        m_sprite.setScale(sf::Vector2f(1/3.,1/3.)); //pour passer de 300px a 100px
        m_frame++;

        m_clock.restart();
    }
  window.draw(m_sprite);
}

void Player::move()
{
  move(m_position.z);
}

void Player::move(int newHeight)
{
  if (!m_is_moving)
    {
      m_is_moving = true;
      m_frame = 0;

      m_sprite.setTexture(m_texture_jump);
      m_sprite.setScale(sf::Vector2f(1./3.,1./3.)); //pour passer de 300px a 100px

      m_start_position = m_position;
      // Definition du decalage lors du mouvement
      m_end_position = changePos(m_position, m_direction);
      m_end_position.z = newHeight;
    }
}

void Player::turnLeft()
{
    m_direction = (m_direction+1)%6;
    m_sprite.setTextureRect(sf::IntRect(m_frame*300, m_direction*300, 300, 300));
}

void Player::turnRight()
{
    if (m_direction == 0)
        m_direction+=6;
    m_direction = m_direction-1;
    m_sprite.setTextureRect(sf::IntRect(m_frame*300, m_direction*300, 300, 300));
}

sf::Vector3i Player::getPosition()
{
    return m_position;
}

void Player::setPosition(sf::Vector3i position)
{
    m_position = position;
}

void Player::setHeight(int height)
{
    m_position.z = height;
}

int Player::getDirection()
{
    return m_direction;
}

void Player::setDirection(int direction)
{
    m_direction = direction;
}

void Player::refresh()
{
    if (m_is_moving) // Si le personnage est dans son animation de deplacement
    {
      sf::Vector2f endPoint (xORIGIN + HEXAGON_WIDTH * m_end_position.x + m_end_position.y%2 * RADIUS,
                             yORIGIN + HEXAGON_DEPTH / 2 * (m_end_position.y - 1) - HEXAGON_HEIGHT * (m_end_position.z + 1));

      sf::Vector2f startPoint (xORIGIN + HEXAGON_WIDTH * m_start_position.x + m_start_position.y%2 * RADIUS,
                               yORIGIN + HEXAGON_DEPTH / 2 * (m_start_position.y - 1) - HEXAGON_HEIGHT * (m_start_position.z + 1));

      sf::Vector2f vector = endPoint - startPoint; // Calcul du vecteur deplacment puis ajout progressif d'une partie de ce dernier
      //Lors de la 5eme Frame (la moitie de l'animation) le personnage est deplacé (pour eviter les problemes liés au differents niveaux de hauteurs

          int height;
          if (m_frame < 5)
            {
              height = tanh(m_frame/3.) * m_sprite.getGlobalBounds().height / 2.;
              m_sprite.setPosition( startPoint.x + vector.x / 10. * m_frame, startPoint.y + vector.y / 10. * m_frame - height);
            }
          else if (m_frame >= 5)
            {
              height = tanh(( 10 - m_frame )/3.) * m_sprite.getGlobalBounds().height / 2.;
              m_sprite.setPosition( endPoint.x - vector.x / 10. * (10 - m_frame), endPoint.y - vector.y / 10. * (10 - m_frame) - height);

            }
      }
    else
      {
        m_sprite.setPosition({xORIGIN + HEXAGON_WIDTH * m_position.x + m_position.y%2 * RADIUS,
                              yORIGIN + HEXAGON_DEPTH / 2 * (m_position.y - 1) - HEXAGON_HEIGHT * (m_position.z + 1)});
      }
}

bool Player::isMoving()
{
    return m_is_moving;
}

int Player::getFrame()
{
  return m_frame;
}

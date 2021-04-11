#include "../include/Button.h"
#include <iostream>

sf::Texture Button::m_texture;

Button::Button()
    : m_position { 0, 0 },
      m_size { 0, 0 }

{
    loadTexture();
    if (! m_police.loadFromFile(POLICE)) throw "Police "+POLICE+" manquante";
    m_text.setFont(m_police);
    m_text.setFillColor(sf::Color::Black);
    m_text.setCharacterSize(10);

    centerText();
}

Button::Button(sf::Vector2f position, sf::Vector2f size)
    : m_position { position },
      m_size { size }

{
    loadTexture();
    updateSize();

    if (! m_police.loadFromFile(POLICE)) throw "Police "+POLICE+" manquante";
    m_text.setFont(m_police);
    m_text.setFillColor(sf::Color::Black);
    m_text.setCharacterSize(10);

    centerText();
}

Button::Button(sf::Vector2f position, sf::Vector2f size, sf::Color color)
    : m_position { position },
      m_size { size },
      m_color { color }
{
    loadTexture();
    updateSize();

    if (! m_police.loadFromFile(POLICE)) throw "Police "+POLICE+" manquante";
    m_text.setFont(m_police);
    m_text.setFillColor(sf::Color::Black);
    m_text.setCharacterSize(10);

    centerText();
}

Button::Button(sf::Vector2f position, sf::Vector2f size, sf::Color color, std::string font, int text)
    : m_position { position },
      m_size { size },
      m_color { color }
{
    loadTexture();
    updateSize();

    if (! m_police.loadFromFile(POLICE)) throw "Police "+POLICE+" manquante";
    m_text.setFont(m_police);
    m_text.setFillColor(sf::Color::Black);
    m_text.setCharacterSize(text);
    m_text.setString(font);

    centerText();
}

void Button::setString(const std::string& text)
{
    m_text.setString(text);
    centerText();
}

void Button::setCharacterSize(int size)
{
    m_text.setCharacterSize(size);
    centerText();
}

void Button::setSize(sf::Vector2f size)
{
    m_size = size;
    updateSize();
    centerText();
}

void Button::updateSize()
{
    int width  = (( m_size.x - TILE_SIZE*TILE_SCALE + 4 ) / (TILE_SIZE*TILE_SCALE)) + 1;
    int height = (( m_size.y - TILE_SIZE*TILE_SCALE + 4 ) / (TILE_SIZE*TILE_SCALE)) + 1;
    if(width<=1)
        width=2;
    if(height<=1)
        height=2;
    m_size.x = width  * TILE_SIZE * TILE_SCALE;
    m_size.y = height * TILE_SIZE * TILE_SCALE;
}

void Button::setPosition(Position position)
{
    m_position = position;
    centerText();
}

Position Button::getPosition()
{
    return m_position;
}

void Button::setFont(std::string font)
{
    m_police.loadFromFile(font);
    m_text.setFont(m_police);
}

void Button::setFillColor(sf::Color color)
{
    m_coinHG.setColor(color);
    m_coteH.setColor(color);
    m_coinHD.setColor(color);
    m_coteG.setColor(color);
    m_centre.setColor(color);
    m_coteD.setColor(color);
    m_coinBG.setColor(color);
    m_coteB.setColor(color);
    m_coinBD.setColor(color);
}

void Button::setTextColor(sf::Color color)
{
    m_text.setFillColor(color);
}

bool Button::contains(Position position)
{
    sf::FloatRect hitbox ( m_position, m_size);
    return hitbox.contains(position);
}

void Button::centerText()
{
    // Place le texte au centre du bouton
    m_text.setPosition(m_position.x + m_size.x / 2 - m_text.getGlobalBounds().width / 2,
                       m_position.y + m_size.y / 2 - m_text.getGlobalBounds().height / 2 + m_text.getPosition().y - m_text.getGlobalBounds().top);
}

void Button::draw_on(sf::RenderWindow& window, sf::Vector2f position)
{
    sf::Color initialColor = m_centre.getColor();
    sf::Color hoveringColor = initialColor;
    hoveringColor.a -= 100; // couleur du bouton changée lorsque l'on passe la souris
    if (contains(position))
        setFillColor(hoveringColor);
    applyTexture(window);
    window.draw(m_text);
    setFillColor(initialColor);
}

void Button::setType(int type)
{
    m_type = type;
}

int Button::getType()
{
    return m_type;
}

void Button::loadFromFile()
{
  if(!(m_texture.loadFromFile("ressources/Button.png"))){exit(EXIT_FAILURE);}
}

void Button::loadTexture()
{

    // coin Haut-gauche
    m_coinHG.setTexture(m_texture);
    m_coinHG.setScale({TILE_SCALE, TILE_SCALE});
    m_coinHG.setTextureRect(sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE));
    m_coinHG.setColor(m_color);
    // cote Haut
    m_coteH.setTexture(m_texture);
    m_coteH.setScale({TILE_SCALE, TILE_SCALE});
    m_coteH.setTextureRect(sf::IntRect(18, 0, TILE_SIZE, TILE_SIZE));
    m_coteH.setColor(m_color);
    // coin Haut-droit
    m_coinHD.setTexture(m_texture);
    m_coinHD.setScale({TILE_SCALE, TILE_SCALE});
    m_coinHD.setTextureRect(sf::IntRect(36, 0, TILE_SIZE, TILE_SIZE));
    m_coinHD.setColor(m_color);
    // cote Gauche
    m_coteG.setTexture(m_texture);
    m_coteG.setScale({TILE_SCALE, TILE_SCALE});
    m_coteG.setTextureRect(sf::IntRect(0, 18, TILE_SIZE, TILE_SIZE));
    m_coteG.setColor(m_color);
    // centre
    m_centre.setTexture(m_texture);
    m_centre.setScale({TILE_SCALE, TILE_SCALE});
    m_centre.setTextureRect(sf::IntRect(18, 18, TILE_SIZE, TILE_SIZE));
    m_centre.setColor(m_color);
    // cote Droit
    m_coteD.setTexture(m_texture);
    m_coteD.setScale({TILE_SCALE, TILE_SCALE});
    m_coteD.setTextureRect(sf::IntRect(36, 18, TILE_SIZE, TILE_SIZE));
    m_coteD.setColor(m_color);
    // coin Bas-Gauche
    m_coinBG.setTexture(m_texture);
    m_coinBG.setScale({TILE_SCALE, TILE_SCALE});
    m_coinBG.setTextureRect(sf::IntRect(0, 36, TILE_SIZE, TILE_SIZE));
    m_coinBG.setColor(m_color);
    // cote Bas
    m_coteB.setTexture(m_texture);
    m_coteB.setScale({TILE_SCALE, TILE_SCALE});
    m_coteB.setTextureRect(sf::IntRect(18, 36, TILE_SIZE, TILE_SIZE));
    m_coteB.setColor(m_color);
    // coin Bas-Droit
    m_coinBD.setTexture(m_texture);
    m_coinBD.setScale({TILE_SCALE, TILE_SCALE});
    m_coinBD.setTextureRect(sf::IntRect(36, 36, TILE_SIZE, TILE_SIZE));
    m_coinBD.setColor(m_color);
}

void Button::applyTexture(sf::RenderWindow& window)
{
    int width  = (( m_size.x - TILE_SIZE*TILE_SCALE + 4 ) / (TILE_SIZE*TILE_SCALE));
    int height = (( m_size.y - TILE_SIZE*TILE_SCALE + 4 ) / (TILE_SIZE*TILE_SCALE));

    for(int i=0; i<=width; i++)
        for(int j=0; j<=height; j++)
        {
            if(i == 0 and j == 0)                          // coin Haut-Gauche
            {
                m_coinHG.setPosition(m_position);
                window.draw(m_coinHG);
            }
            else if(i != 0 and i != width and j == 0)    // cote Haut
            {
                m_coteH.setPosition({m_position.x + TILE_SIZE*TILE_SCALE * i, m_position.y});
                window.draw(m_coteH);
            }
            else if(i == width and j == 0)               // coin Haut-Droit
            {
                m_coinHD.setPosition({m_position.x + (TILE_SIZE*TILE_SCALE) * (width), m_position.y});
                window.draw(m_coinHD);
            }
            else if(i == 0 and j != 0 and j != height)     // cote Gauche
            {
                m_coteG.setPosition({m_position.x, m_position.y + TILE_SIZE*TILE_SCALE * j});
                window.draw(m_coteG);
            }
            else if(i == width and j != 0 and j != height) // cote Droit
            {
                m_coteD.setPosition({m_position.x + (TILE_SIZE*TILE_SCALE) * (width), m_position.y + (TILE_SIZE*TILE_SCALE) * j});
                window.draw(m_coteD);
            }
            else if(i == 0 and j == height)                // coin Bas-Gauche
            {
                m_coinBG.setPosition({m_position.x, m_position.y + TILE_SIZE*TILE_SCALE * (height)});
                window.draw(m_coinBG);
            }
            else if(i != 0 and i != width and j == height) // cote Bas
            {
                m_coteB.setPosition({m_position.x + TILE_SIZE*TILE_SCALE * i, m_position.y + TILE_SIZE*TILE_SCALE * (height)});
                window.draw(m_coteB);
            }
            else if(i == width and j == height)            // coin Bas-Droit
            {
                m_coinBD.setPosition({m_position.x + TILE_SIZE*TILE_SCALE * (width), m_position.y + TILE_SIZE*TILE_SCALE * (height)});
                window.draw(m_coinBD);
            }
            else                                               // centre
            {
                m_centre.setPosition({m_position.x + TILE_SIZE*TILE_SCALE * i, m_position.y + TILE_SIZE*TILE_SCALE * j});
                window.draw(m_centre);
            }
        }
}



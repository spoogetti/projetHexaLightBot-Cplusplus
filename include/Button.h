#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <string>

#include "Tools.h"

class Button
{
private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    sf::Text m_text; //texte contenu dans le bouton
    sf::Font m_police; //police utilisée par le texte
    int m_type;

    // Textures
    static sf::Texture m_texture;

    sf::Sprite m_coinHG;
    sf::Sprite m_coteH;
    sf::Sprite m_coinHD;
    sf::Sprite m_coteG;
    sf::Sprite m_centre;
    sf::Sprite m_coteD;
    sf::Sprite m_coinBG;
    sf::Sprite m_coteB;
    sf::Sprite m_coinBD;

    sf::Color m_color;

    void centerText();
public:
    Button();
    Button(sf::Vector2f p, sf::Vector2f s);
    Button(sf::Vector2f p, sf::Vector2f s, sf::Color c);
    Button(sf::Vector2f p, sf::Vector2f s, sf::Color c, std::string font, int textSize);

    static void loadFromFile();

    // Change le texte du bouton
    void setString(const std::string &str);

    // Change la taille du texte
    void setCharacterSize(int s);

    // Change la taille du bouton et mets à jour la position du texte
    void setSize(sf::Vector2f s);
    void updateSize();

    // Getter et setter de la position du bouton
    void setPosition(Position p);
    Position getPosition();

    // Police d'ecriture
    void setFont(std::string font);

    // Couleur du Rectangle d'affichage / du texte
    void setFillColor(sf::Color c);
    void setTextColor(sf::Color c);

    // Retourne si la position rentrée se trouve dans le rectangle clickable
    bool contains(Position p);

    // Affiche sur la fenetre de rendue le rectangle et le texte
    void draw_on(sf::RenderWindow &window, sf::Vector2f position);

    // Contient un type
    void setType(int type);
    int getType();

    // Methodes de textures
    void loadTexture();
    void applyTexture(sf::RenderWindow& window);
};

#endif // BUTTON_H

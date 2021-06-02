#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>
#include "IA.h"

class Map : public sf::Drawable, public sf::Transformable
{
public:

    bool creerMap(const std::string lien, sf::Vector2u tailleTuille, const int* carte, unsigned int largeur, unsigned int hauteur)
    {
        int i, j;
        sf::Vertex* carre;
        int nbCase, posX, posY;

        // on charge la texture de la palette
        if (!textureMap.loadFromFile(lien))
            return false;

        // on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
        plateau.setPrimitiveType(sf::Quads);
        plateau.resize(largeur * hauteur * 4);

        // on remplit le tableau de vertex, avec un carr� par tuile
        for (i = 0; i < largeur; i++) {
            for (j = 0; j < hauteur; j++){
                // on r�cup�re le num�ro de tuile courant
                nbCase = carte[i + j * largeur];

                // on en d�duit sa position dans la texture du tileset
                posX = nbCase % (textureMap.getSize().x / tailleTuille.x);
                posY = nbCase / (textureMap.getSize().x / tailleTuille.x);

                // on r�cup�re un pointeur vers le carr� � d�finir dans le tableau de vertex
                carre = &plateau[(i + j * largeur) * 4];

                // on d�finit ses quatre coins
                carre[0].position = sf::Vector2f(i * tailleTuille.x, j * tailleTuille.y);
                carre[1].position = sf::Vector2f((i + 1) * tailleTuille.x, j * tailleTuille.y);
                carre[2].position = sf::Vector2f((i + 1) * tailleTuille.x, (j + 1) * tailleTuille.y);
                carre[3].position = sf::Vector2f(i * tailleTuille.x, (j + 1) * tailleTuille.y);

                // on d�finit ses quatre coordonn�es de texture
                carre[0].texCoords = sf::Vector2f(posX * tailleTuille.x, posY * tailleTuille.y);
                carre[1].texCoords = sf::Vector2f((posX + 1) * tailleTuille.x, posY * tailleTuille.y);
                carre[2].texCoords = sf::Vector2f((posX + 1) * tailleTuille.x, (posY + 1) * tailleTuille.y);
                carre[3].texCoords = sf::Vector2f(posX * tailleTuille.x, (posY + 1) * tailleTuille.y);
            }
        }
        tableau = carte;
        return true;
    }

    /*int getEmplacement() {
        Donne un nombre al�atoire qui d�signe une piece de la map
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<int> distr(1, 8);
        return distr(eng);
    }*/

    const int* getTableau() {
        return this->tableau;
    }

    const int* tableau;

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // on applique la transformation
        //states.transform = getTransform();

        // on applique la texture du tileset
        states.texture = &textureMap;

        // et on dessine enfin le tableau de vertex
        target.draw(plateau, states);
    }

    sf::VertexArray plateau;
    sf::Texture textureMap;
};

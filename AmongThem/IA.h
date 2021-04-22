#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

class IA : public sf::Drawable, public sf::Transformable
{
public:
    void mouvementIA(const int* map, int longueur, int hauteur)
    {
        sf::Vector2u positionMap(persoSprite.getPosition().x / 150, persoSprite.getPosition().y / 150);
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<int> distr(1, 4);
        if (arriveDesti) {
            nbMouv = distr(eng);
            arriveDesti = false;
        }
        else {
            if (nbMouv == 1 && positionMap.y > 0 && (map[positionMap.x + (positionMap.y - 1) * longueur] == 0 || map[positionMap.x + (positionMap.y - 1) * longueur] == 3)) {//Haut
                persoSprite.move(0.0, -15.0);
                if (persoSprite.getPosition().y / tailleTuile == positionMap.y) {
                    arriveDesti = true;
                }
            }
            else if (nbMouv == 2 && positionMap.y < hauteur && (map[positionMap.x + (positionMap.y + 1) * longueur] == 0 || map[positionMap.x + (positionMap.y + 1) * longueur] == 3)) {//Bas
                persoSprite.move(0.0, 15.0);
                if (persoSprite.getPosition().y / tailleTuile >= positionMap.y + 1) {
                    arriveDesti = true;
                }
            }
            else if (nbMouv == 3 && positionMap.x > 0 && (map[(positionMap.x - 1) + positionMap.y * longueur] == 0 || map[(positionMap.x - 1) + positionMap.y * longueur] == 3)) {//Gauche
                persoSprite.setScale(-1, 1);
                if (direcPerso == true) {
                    direcPerso = false;
                    persoSprite.setPosition(persoSprite.getPosition().x + 92, persoSprite.getPosition().y);
                }
                persoSprite.move(-15.0, 0.0);
                if ((int)persoSprite.getPosition().x / tailleTuile <= positionMap.x - 1) {
                    arriveDesti = true;
                }
            }
            else if (nbMouv == 4 && positionMap.x < longueur && (map[(positionMap.x + 1) + positionMap.y * longueur] == 0 || map[(positionMap.x + 1) + positionMap.y * longueur] == 3)) {//Droite
                persoSprite.setScale(1, 1);
                if (direcPerso == false) {
                    direcPerso = true;
                    persoSprite.setPosition(persoSprite.getPosition().x - 92, persoSprite.getPosition().y);
                }
                persoSprite.move(15.0, 0.0);
                if ((int)persoSprite.getPosition().x / tailleTuile >= positionMap.x + 1) {
                    arriveDesti = true;
                }
            }
            else {
                nbMouv = distr(eng);
            }
        }
    }

    bool load(const std::string& texture, sf::Vector2u emplacement)
    {
        // on charge la texture du tileset
        if (!perso.loadFromFile(texture)) {
            std::cout << "Probleme a telecharger l'IA" << std::endl;
            return false;
        }

        persoSprite.setTexture(perso);
        persoSprite.setPosition(emplacement.x, emplacement.y);

        return true;
    }

    sf::Texture perso;
    sf::Sprite persoSprite;

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // on applique la transformation
        states.transform *= getTransform();

        // on applique la texture du tileset
        states.texture = &perso;

        // et on dessine enfin le tableau de vertex
        target.draw(persoSprite);
    }

    bool direcPerso = true;
    int nbMouv;
    bool arriveDesti = true;
    int tailleTuile = 150;
};

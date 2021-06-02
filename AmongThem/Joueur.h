#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "IA.h"

class Joueur : public sf::Drawable, public sf::Transformable
{
public:
    void mouvementJoueur(const int* map, int longueur, int hauteur)
    {
        sf::Vector2u positionMap(persoSprite.getPosition().x / 150, persoSprite.getPosition().y / 150);
        int up = map[positionMap.x + (positionMap.y) * longueur];
        int down = map[positionMap.x + (positionMap.y + 1) * longueur];
        int left = map[(positionMap.x - 1) + positionMap.y * longueur];
        int right = map[(positionMap.x + 1) + positionMap.y * longueur];
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            if (up == 0 || up == 3 || up == 18) {
                persoSprite.move(0.0, -16.0);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            if (down == 0 || down == 3 || down == 18) {
                persoSprite.move(0.0, 16.0);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if (left == 0 || left == 3 || left == 18) {
                persoSprite.move(-16.0, 0.0);
            }
            persoSprite.setScale(-1, 1);
            if (direcPerso == true) {
                direcPerso = false;
                persoSprite.setPosition(persoSprite.getPosition().x + 78, persoSprite.getPosition().y);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (right == 0 || right == 3 || right == 18) {
                persoSprite.move(16.0, 0.0);
            }
            persoSprite.setScale(1, 1);
            if (direcPerso == false) {
                direcPerso = true;
                persoSprite.setPosition(persoSprite.getPosition().x - 78, persoSprite.getPosition().y);
            }
        }
        /*for (int i = 0; i < hauteur; i++) {
            for (int j = 0; j < longueur; j++) {
                int haut = i * 150;
                int bas = i * 150 + 150;
                int gauche = j * 150;
                int droite = j * 150 + 150;
                if ((map[j + i * longueur] != 0 && map[j + i * longueur] != 3) && (posX + 150 <= gauche && posX >= droite) && (posY + 150 <= haut && posY >= bas)) {
                    std::cout << "Emplacement " << map[i + j * longueur] << std::endl;
                    std::cout << i << " and " << j << std::endl;
                    std::cout << "gauche = " << gauche << "/ droite = " << droite << std::endl;
                    std::cout << "posX = " << posX << "/ posY = " << posY << std::endl;
                    test = true;
                }
            }
        }
        if (test) {
            persoSprite.setPosition(posX, posY);
            test = false;
        }*/
    }

    bool load(const std::string& texture, sf::Vector2u emplacement)
    {
        if (!perso.loadFromFile(getImagePerso())) {
            std::cout << "Probleme a telecharger un joueur" << std::endl;
            return false;
        }

        persoSprite.setTexture(perso);
        persoSprite.setPosition(emplacement.x, emplacement.y);

        return true;
    }

    bool load()
    {
        if (!perso.loadFromFile(getImagePerso())) {
            std::cout << "Probleme a telecharger l'IA" << std::endl;
            return false;
        }

        persoSprite.setTexture(perso);

        return true;
    }

    sf::Vector2f getPosition() {
        return persoSprite.getPosition();
    }

    bool getDirection(){
        return direcPerso;
    }

    sf::View getView() {
        sf::View vu;
        vu.reset(sf::FloatRect(0, 0, longueurPage, hauteurPage));
        if (persoSprite.getPosition().x < 0) persoSprite.setPosition(0, persoSprite.getPosition().y);
        if (persoSprite.getPosition().y < 0) persoSprite.setPosition(persoSprite.getPosition().x, 0);
        if (direcPerso == true) {
            vu.setCenter(persoSprite.getPosition().x + (78 / 2), persoSprite.getPosition().y + (103 / 2));
        }
        else {
            vu.setCenter(persoSprite.getPosition().x - (78 / 2), persoSprite.getPosition().y + (103 / 2));
        }
        return vu;
    }


    std::string getImagePerso() {
        nbSpritePerso++;
        if (nbSpritePerso > 12) {
            nbSpritePerso = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (nbSpritePerso < 10) {
                return "SpritePlayer/Among Us - Player Base/Individual Sprites/Walk/walkcolor000" + std::to_string(nbSpritePerso) + ".png";
            }
            else {
                return "SpritePlayer/Among Us - Player Base/Individual Sprites/Walk/walkcolor00" + std::to_string(nbSpritePerso) + ".png";
            }
        }
        nbSpritePerso = 0;
        return "SpritePlayer/Among Us - Player Base/Individual Sprites/idle.png";
    }

    IA* kill(std::vector<IA*> cible)
    {
        for (IA *elem : cible) {
            IA actu = *elem;
            if (actu.persoSprite.getPosition().x - 150 <= this->getPosition().x && this->getPosition().x <= actu.persoSprite.getPosition().x + 150 && actu.persoSprite.getPosition().y - 150 <= this->getPosition().y && this->getPosition().y <= actu.persoSprite.getPosition().y + 150) {
                return elem;
            }
        }
        return NULL;
    }

    bool Report(std::vector<IA*> cible) {
        //IA* result = NULL;
        for (IA* elem : cible) {
            IA actu = *elem;
            if (actu.persoSprite.getPosition().x - 150 <= this->getPosition().x && this->getPosition().x <= actu.persoSprite.getPosition().x + 150 && actu.persoSprite.getPosition().y - 150 <= this->getPosition().y && this->getPosition().y <= actu.persoSprite.getPosition().y + 150 && !actu.estVivant) {
                persoSprite.setPosition(1500,1500);
                return true;
            }
        }
        return false;
    }

    void Respawn() {
        persoSprite.setPosition(1500, 1500);
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
    int tailleTuile = 150;
    int longueurPage = 1600;
    int hauteurPage = 900;
    int nbSpritePerso = 0;
};
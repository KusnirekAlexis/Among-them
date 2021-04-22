#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class Joueur : public sf::Drawable, public sf::Transformable
{
public:
    void mouvementJoueur(const int* map, int longueur, int hauteur)
    {
        sf::Vector2u positionMap(persoSprite.getPosition().x / 150, persoSprite.getPosition().y / 150);
        //std::cout << persoSprite.getPosition().y / 150 << " == " << positionMap.y << std::endl;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            if (map[positionMap.x + (positionMap.y ) * longueur] == 0 || map[positionMap.x + (positionMap.y ) * longueur] == 3) {
                persoSprite.move(0.0, -16.0);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            if (map[positionMap.x + (positionMap.y + 1) * longueur] == 0 || map[positionMap.x + (positionMap.y + 1) * longueur] == 3) {
                persoSprite.move(0.0, 16.0);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if (map[(positionMap.x - 1) + positionMap.y * longueur] == 0 || map[(positionMap.x - 1) + positionMap.y * longueur] == 3) {
                persoSprite.move(-16.0, 0.0);
            }
            persoSprite.setScale(-1, 1);
            if (direcPerso == true) {
                direcPerso = false;
                persoSprite.setPosition(persoSprite.getPosition().x + 92, persoSprite.getPosition().y);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (map[(positionMap.x + 1) + positionMap.y * longueur] == 0 || map[(positionMap.x + 1) + positionMap.y * longueur] == 3) {
                persoSprite.move(16.0, 0.0);
            }
            persoSprite.setScale(1, 1);
            if (direcPerso == false) {
                direcPerso = true;
                persoSprite.setPosition(persoSprite.getPosition().x - 92, persoSprite.getPosition().y);
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
};
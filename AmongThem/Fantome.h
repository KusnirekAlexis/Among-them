#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class Fantome : public sf::Drawable, public sf::Transformable
{
public:
    void mouvementJoueur(const int* map, int longueur, int hauteur)
    {
        float posX = persoSprite.getPosition().x;
        float posY = persoSprite.getPosition().y;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            if (posY >= 0) {
                persoSprite.move(0.0, -16.0);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            if (posY <= hauteur * 150 - hauteurPerso) {
                persoSprite.move(0.0, 16.0);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            if (posX >= 0) {
                persoSprite.move(-16.0, 0.0);
            }
            persoSprite.setScale(-1, 1);
            if (direcPerso == true) {
                direcPerso = false;
                persoSprite.setPosition(persoSprite.getPosition().x + largeurPerso, persoSprite.getPosition().y);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (posX <= longueur * 150 - largeurPerso) {
                persoSprite.move(16.0, 0.0);
            }
            persoSprite.setScale(1, 1);
            if (direcPerso == false) {
                direcPerso = true;
                persoSprite.setPosition(persoSprite.getPosition().x - largeurPerso, persoSprite.getPosition().y);
            }
        }
    }

    bool load(sf::Vector2u emplacement)
    {
        if (!perso.loadFromFile(getImagePerso())) {
            std::cout << "Probleme a telecharger un fantome" << std::endl;
            return false;
        }

        persoSprite.setTexture(perso);
        persoSprite.setPosition(emplacement.x, emplacement.y);

        return true;
    }

    bool animation()
    {
        if (clock.getElapsedTime().asMilliseconds() > 40) {//Pour ralentir l'animation
            if (!perso.loadFromFile(getImagePerso())) {
                std::cout << "Probleme a telecharger un fantome" << std::endl;
                return false;
            }
            clock.restart();
        }

        persoSprite.setTexture(perso);

        return true;
    }

    sf::Vector2f getPosition() {
        return persoSprite.getPosition();
    }

    bool getDirection() {
        return direcPerso;
    }

    sf::View getView() {
        sf::View vu;
        vu.reset(sf::FloatRect(0, 0, longueurPage, hauteurPage));
        if (persoSprite.getPosition().x < 0) persoSprite.setPosition(0, persoSprite.getPosition().y);
        if (persoSprite.getPosition().y < 0) persoSprite.setPosition(persoSprite.getPosition().x, 0);
        if (direcPerso == true) {
            vu.setCenter(persoSprite.getPosition().x + (largeurPerso / 2), persoSprite.getPosition().y + (hauteurPerso / 2));
        }
        else {
            vu.setCenter(persoSprite.getPosition().x - (largeurPerso / 2), persoSprite.getPosition().y + (hauteurPerso / 2));
        }
        return vu;
    }


    std::string getImagePerso() {
        //sf::Clock temps;
        nomPicture = "SpritePlayer/Among Us - Player Base/Individual Sprites/Ghost/ghostbob0001.png";
        //Chargement de l'image du perso
        //if (temps.getElapsedTime().asMilliseconds() >= 40) {//Pour ralentir l'animation
            if (nbSprite < 10) {
                nomPicture = "SpritePlayer/Among Us - Player Base/Individual Sprites/Ghost/ghostbob000" + std::to_string(nbSprite) + ".png";
            }
            else {
                nomPicture = "SpritePlayer/Among Us - Player Base/Individual Sprites/Ghost/ghostbob00" + std::to_string(nbSprite) + ".png";
            }
            nbSprite++;
            if (nbSprite > 48) nbSprite = 1;
            //temps.restart();
        //}
        return nomPicture;
    }

    sf::Texture perso;
    sf::Sprite persoSprite;
    int nbSprite = 1;
    std::string nomPicture;
    int hauteurPerso = 150;
    int largeurPerso = 92;
    sf::Clock clock;

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

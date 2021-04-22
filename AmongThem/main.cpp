#pragma region include
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "TileMap.h"
#include "IA.h"
#include "Joueur.h"
#pragma endregion include

#pragma region Variable
sf::RenderWindow window;
sf::Vector2i posSouris;

sf::Texture fantome;
sf::Sprite fantomeSprite;
sf::Texture perso;
sf::Sprite persoSprite;

sf::View vu;

//Pour enpecher le décalage en fesant demitour
bool direcFantome = true;
bool direcPerso = true;

/*sf::Texture map;
sf::Sprite mapSprite;*/

int nbSprite = 1;
int nbSpritePerso = 1;
std::string nomPicture;
int longueurPage = 1600;
int hauteurPage = 900;
#pragma endregion Variable

#pragma region Mouvement

void gestionSouris() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        posSouris = sf::Mouse::getPosition(window);
        fantomeSprite.setPosition(posSouris.x, posSouris.y);
        //std::cout << "Position x de la souris = " << posSouris.x << " ,et y = " << posSouris.y << std::endl;
    }
}

void gestionFantome() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        fantomeSprite.move(0.0, -20.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        fantomeSprite.move(0.0, 20.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        fantomeSprite.move(-20.0, 0.0);
        fantomeSprite.setScale(-1, 1);
        if (direcFantome == true) {
            direcFantome = false;
            fantomeSprite.setPosition(fantomeSprite.getPosition().x + 92, fantomeSprite.getPosition().y);
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        fantomeSprite.move(20.0, 0.0);
        fantomeSprite.setScale(1, 1);
        if (direcFantome == false) {
            direcFantome = true;
            fantomeSprite.setPosition(fantomeSprite.getPosition().x - 92, fantomeSprite.getPosition().y);
        }
    }
}

std::string getImagePerso() {
    nbSpritePerso++;
    if (nbSpritePerso > 12) {
        nbSpritePerso = 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        if (nbSpritePerso < 10) {
            return "SpritePlayer/Among Us - Player Base/Individual Sprites/Walk/walkcolor000" + std::to_string(nbSpritePerso) + ".png";
        }
        else {
            return "SpritePlayer/Among Us - Player Base/Individual Sprites/Walk/walkcolor00" + std::to_string(nbSpritePerso) + ".png";
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        if (nbSpritePerso < 10) {
            return "SpritePlayer/Among Us - Player Base/Individual Sprites/Walk/walkcolor000" + std::to_string(nbSpritePerso) + ".png";
        }
        else {
            return "SpritePlayer/Among Us - Player Base/Individual Sprites/Walk/walkcolor00" + std::to_string(nbSpritePerso) + ".png";
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        if (nbSpritePerso < 10) {
            return "SpritePlayer/Among Us - Player Base/Individual Sprites/Walk/walkcolor000" + std::to_string(nbSpritePerso) + ".png";
        }
        else {
            return "SpritePlayer/Among Us - Player Base/Individual Sprites/Walk/walkcolor00" + std::to_string(nbSpritePerso) + ".png";
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
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
#pragma endregion Mouvement

int main()
{
    sf::Clock time;
    sf::Clock temps;
    window.create(sf::VideoMode(longueurPage, hauteurPage), "Among them");
    window.setFramerateLimit(30);

    // on définit le niveau à l'aide de numéro de tuiles
    const int niveau[] = {
        3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 4, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3,

        6, 6, 6, 6, 6, 6, 6, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6,
        6, 0, 0, 0, 0, 0, 6, 0, 0, 6, 0, 2, 0, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 6, 0, 10, 9, 6,
        6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6,
        6, 0, 0, 0, 0, 0, 6, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6,
        6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2, 0, 2, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6,
        6, 0, 0, 0, 0, 0, 6, 0, 0, 6, 6, 6, 6, 6, 6, 6, 0, 0, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 6, 6, 6, 0, 6,
        6, 6, 0, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6,
        6, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6,
        6, 0, 0, 11, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6,
        6, 0, 7, 11, 8, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 1, 0, 0, 0, 0, 0, 0, 1, 6,
        6, 0, 0, 11, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1, 1, 1, 1, 0, 0, 6,
        6, 0, 7, 11, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 6,
        6, 0, 0, 11, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1, 0, 0, 1, 0, 0, 6,
        6, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 1, 0, 0, 0, 0, 0, 0, 1, 6,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    };

    // on crée la tilemap avec le niveau précédemment défini
    TileMap map;
    if (!map.load("TextureMap/palette.png", sf::Vector2u(150, 150), niveau, 35, 19))
        return -1;
    IA partenaire;
    if (!partenaire.load("SpritePlayer/Among Us - Player Base/Individual Sprites/idle.png", sf::Vector2u(1500, 1500)))
        return -1;
    Joueur joueur;
    if (!joueur.load("SpritePlayer/Among Us - Player Base/Individual Sprites/idle.png", sf::Vector2u(1500, 1500)))
        return -1;

    // on fait tourner le programme jusqu'à ce que la fenêtre soit fermée
    while (window.isOpen())
    {

        //Chargement de l'image du perso
        if (time.getElapsedTime().asMilliseconds() >= 40) {//Pour ralentir l'animation
            if (nbSprite < 10) {
                nomPicture = "SpritePlayer/Among Us - Player Base/Individual Sprites/Ghost/ghostbob000" + std::to_string(nbSprite) + ".png";
            }
            else {
                nomPicture = "SpritePlayer/Among Us - Player Base/Individual Sprites/Ghost/ghostbob00" + std::to_string(nbSprite) + ".png";
            }
            nbSprite++;
            if (nbSprite > 48) nbSprite = 1;
            time.restart();
        }

        /*if (!map.loadFromFile("SpriteMap/lobby.png"))
        {
            std::cout << "Erreur de chargement d'image" << std::endl;
        }
        mapSprite.setTexture(map);*/

        if (!fantome.loadFromFile(nomPicture))
        {
            std::cout << "Erreur de chargement d'image" << std::endl;
        }
        fantomeSprite.setTexture(fantome);

        std::string chemSprite = getImagePerso();
        if (temps.getElapsedTime().asMilliseconds() > 40) {//Pour ralentir l'animation
            partenaire.mouvementIA(niveau, 35, 19);
            temps.restart();
        }

        // on inspecte tous les évènements de la fenêtre qui ont été émis depuis la précédente itération
        sf::Event event;
        while (window.pollEvent(event))
        {
            // évènement "fermeture demandée" : on ferme la fenêtre
            if (event.type == sf::Event::Closed)
                window.close();
            //Gestion d'evenement pour deplacement
            gestionSouris();
            gestionFantome();
            joueur.mouvementJoueur(niveau, 35, 19);
        }

        /*for (int i = 0; partenaire.persoSprite.getPosition().y <= 1700) {
            if (temps.getElapsedTime().asMilliseconds() > 50) {//Pour ralentir l'animation
                std::cout << "y = " << partenaire.persoSprite.getPosition().y << std::endl;
                partenaire.persoSprite.move(0.f, 1.f);
                temps.restart();
            }
        }*/

        vu = joueur.getView();

        window.setView(vu);

        //window.draw(mapSprite);
        window.draw(map);
        window.draw(fantomeSprite);
        window.draw(joueur);
        window.draw(partenaire);
        window.display();
        window.clear();
    }

    return 0;
}
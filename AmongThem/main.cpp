#pragma region include
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include "Map.h"
#include "IA.h"
#include "Joueur.h"
#include "Fantome.h"
#pragma endregion include

#pragma region Variable
sf::RenderWindow window;
sf::Image icon;
sf::View vu;
sf::Texture textureFin;
sf::Sprite spriteFin;

//Pour enpecher le décalage en fesant demitour
bool direcFantome = true;
bool direcPerso = true;
bool doublon;
bool SurvivantInsuffisant;

std::string nomPicture;
int longueurPage = 1600;
int hauteurPage = 900;

IA* victime;
sf::Event event;
#pragma endregion Variable

void nettoieCadavre(std::vector<IA*> &IAvivante ,std::vector<IA*>& IAmort) {
    IAmort.clear();
    for (int i = 0; i < IAvivante.size(); i++) {
        if (!IAvivante[i]->estVivant) {
            IAvivante.erase(IAvivante.begin() + i);
            i--;
        }
        else
            IAvivante[i]->Respawn();
    }
}


int main()
{
    sf::Clock time;
    sf::Clock temps;
    sf::Clock temps2;
    window.create(sf::VideoMode(longueurPage, hauteurPage), "Among them");
    window.setFramerateLimit(60);

    if (!icon.loadFromFile("TextureMap/icone.png"))
        EXIT_FAILURE;

    window.setIcon(32, 32, icon.getPixelsPtr());

    // on définit la carte à l'aide de numéro de tuiles
    const int niveau[] = {
        3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 4, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3,

        6, 6, 6, 6, 6, 6, 6, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6,
        6, 0,13, 0,13, 0, 6, 0, 0, 6, 0, 2, 0, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 6, 0,10, 9, 6,
        6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6,
        6, 0, 0, 0, 0, 0, 6, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6,
        6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2, 0, 2, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6,
        6, 0, 0, 0, 0, 0, 6, 0, 0, 6, 6, 6, 6, 6, 6, 6, 0, 0, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 6, 6, 6, 0, 6,
        6, 6, 0, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6,
        6, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6,
        6, 0, 0,11, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6,
        6, 0, 7,11, 8, 0, 6, 0, 0, 0, 0, 0, 0, 0,18,18,18, 0, 0, 0, 0, 0, 0, 0, 0, 6, 1, 0, 0, 0, 0, 0, 0, 1, 6,
        6, 0, 0,12, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0,18,17,18, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1, 1, 1, 1, 0, 0, 6,
        6, 0, 7,11, 8, 0, 0,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18, 0, 0, 0, 1, 0, 0, 1, 0, 0, 6,
        6, 0, 0,11, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0,18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1, 0, 0, 1, 0, 0, 6,
        6, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0,18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 1, 0, 0, 0, 0, 0, 0, 1, 6,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    };

    // on crée la map avec le niveau précédemment défini
    Map map;
    if (!map.creerMap("TextureMap/palette.png", sf::Vector2u(150, 150), niveau, 35, 19))
        EXIT_FAILURE;
    IA partenaire1;                                                                                       
    if (!partenaire1.load("SpritePlayer/Among Us - Player Base/Individual Sprites/idle.png", sf::Vector2u(1500, 1500)))
        EXIT_FAILURE;
    IA partenaire2;
    if (!partenaire2.load("SpritePlayer/Among Us - Player Base/Individual Sprites/idle.png", sf::Vector2u(1500, 1500)))
        EXIT_FAILURE;
    IA partenaire3;
    if (!partenaire3.load("SpritePlayer/Among Us - Player Base/Individual Sprites/idle.png", sf::Vector2u(1500, 1500)))
        EXIT_FAILURE;
    Joueur joueur;
    if (!joueur.load("SpritePlayer/Among Us - Player Base/Individual Sprites/idle.png", sf::Vector2u(1600, 1600)))
        EXIT_FAILURE;
    Fantome fantome;
    if (!fantome.load(sf::Vector2u(1600, 1600)))
        EXIT_FAILURE;

    std::vector<IA*> IAmort{};
    std::vector<IA*> IAvivante{ &partenaire1,&partenaire2,&partenaire3 };
    std::vector<Joueur*> ListeImposteur{ &joueur };

    // on fait tourner le programme jusqu'à ce que la fenêtre soit fermée
    while (window.isOpen()){
        //Pour ralentir l'animation
        if (temps.getElapsedTime().asMilliseconds() > 40) {
            for (IA* elem : IAvivante) {
                if ((*elem).estVivant) {
                    if ((*elem).corpReperer) {
                        (*elem).cheminPlusCourt(niveau);
                        if ((*elem).Report(IAvivante)) {
                            nettoieCadavre(IAvivante, IAmort);
                            for (Joueur* player : ListeImposteur) {
                                (*player).Respawn();
                            }
                        }
                    }
                    else {
                        (*elem).MortReperer(niveau, IAvivante);
                        (*elem).cheminPlusCourt(niveau);
                    }
                }
                else if(!(*elem).load())
                    EXIT_FAILURE;
            }
            temps.restart();
        }

        while (window.pollEvent(event))
        {
            SurvivantInsuffisant = (IAvivante.size() - IAmort.size()) <= ListeImposteur.size();
            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
                doublon = false;
                victime = joueur.kill(IAvivante);
                //Pour éviter de mettre deux fois la meme cible dans IAmort
                for (IA* elem : IAmort) {
                    if (elem == victime) {
                        doublon = true;
                    }
                }
                if (victime != NULL && !doublon) {
                    (*victime).estMort();
                    IAmort.push_back(victime);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                if (joueur.Report(IAvivante)) {
                    nettoieCadavre(IAvivante, IAmort);
                }
            }
            //Gestion d'evenement de deplacement
            joueur.mouvementJoueur(niveau, 35, 19);
            if (temps2.getElapsedTime().asMilliseconds() > 40) {//Pour ralentir l'animation
                if (!joueur.load())
                    EXIT_FAILURE;
                temps2.restart();
            }
            fantome.mouvementJoueur(niveau, 35, 19);
        }

        if (!fantome.animation())
            EXIT_FAILURE;

        vu = joueur.getView();

        window.setView(vu);

        if (SurvivantInsuffisant) {
            textureFin.loadFromFile("FinDePartie/ecranFinImposteur.png");
            spriteFin.setTexture(textureFin);
            spriteFin.setPosition(0, 0);
            vu.setCenter(longueurPage/2, hauteurPage/2);
            window.setView(vu);
            window.draw(spriteFin);
            window.display();
            window.clear();

        }
        else {
            window.draw(map);
            window.draw(fantome);
            window.draw(joueur);
            for (IA* elem : IAvivante) {
                window.draw(*elem);
            }
            window.display();
            window.clear();
        }
    }

    return 0;
}
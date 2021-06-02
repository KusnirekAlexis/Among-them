#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <queue>
#include <vector>
#include "IA.h"

class IA : public sf::Drawable, public sf::Transformable
{
public:

    struct InfoIA
    {
        int x = 0, y = 0;
        int cout = 0, heuristique = 0;
        std::vector<int> cheminEmprunter {};
    };

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
            if (nbMouv == 1 && (map[positionMap.x + (positionMap.y) * longueur] == 0 || map[positionMap.x + (positionMap.y - 1) * longueur] == 3)) {//Haut
                persoSprite.move(0.0, -15.0);
                if (persoSprite.getPosition().y / tailleTuile == positionMap.y) {
                    arriveDesti = true;
                }
            }
            else if (nbMouv == 2 && (map[positionMap.x + (positionMap.y + 1) * longueur] == 0 || map[positionMap.x + (positionMap.y + 1) * longueur] == 3)) {//Bas
                persoSprite.move(0.0, 15.0);
                if (persoSprite.getPosition().y / tailleTuile >= positionMap.y + 1) {
                    arriveDesti = true;
                }
            }
            else if (nbMouv == 3 && (map[(positionMap.x - 1) + positionMap.y * longueur] == 0 || map[(positionMap.x - 1) + positionMap.y * longueur] == 3)) {//Gauche
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
            else if (nbMouv == 4 && (map[(positionMap.x + 1) + positionMap.y * longueur] == 0 || map[(positionMap.x + 1) + positionMap.y * longueur] == 3)) {//Droite
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

    bool reconstituerChemin() {
        sf::Vector2u positionMap(persoSprite.getPosition().x / 150, persoSprite.getPosition().y / 150);
        nbMouv = cheminIA.back();
        if (nbMouv == 1) {//Haut
            persoSprite.move(0.0, -15.0);
            if (persoSprite.getPosition().y / tailleTuile == positionMap.y) {
                cheminIA.pop_back();
                return true;
            }
        }
        else if (nbMouv == 2) {//Bas
            persoSprite.move(0.0, 15.0);
            if (persoSprite.getPosition().y / tailleTuile >= positionMap.y + 1) {
                cheminIA.pop_back();
                return true;
            }
        }
        else if (nbMouv == 3) {//Gauche
            persoSprite.setScale(-1, 1);
            if (direcPerso == true) {
                direcPerso = false;
                persoSprite.setPosition(persoSprite.getPosition().x + 92, persoSprite.getPosition().y);
            }
            persoSprite.move(-15.0, 0.0);
            if (persoSprite.getPosition().x / tailleTuile <= positionMap.x) {
                cheminIA.pop_back();
                return true;
            }
        }
        else if (nbMouv == 4) {//Droite
            persoSprite.setScale(1, 1);
            if (direcPerso == false) {
                direcPerso = true;
                persoSprite.setPosition(persoSprite.getPosition().x - 92, persoSprite.getPosition().y);
            }
            persoSprite.move(15.0, 0.0);
            if (persoSprite.getPosition().x / tailleTuile >= positionMap.x + 1) {
                cheminIA.pop_back();
                return true;
            }
        }
        return false;
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

    bool load()
    {
        if (SpriteMort < 21) {
            SpriteMort++;
        }
        if (SpriteMort < 10) {
            if (!perso.loadFromFile("SpritePlayer/Among Us - Player Base/Individual Sprites/Death/Dead000" + std::to_string(SpriteMort) + ".png")) {
                std::cout << "Probleme a telecharger l'IA" << std::endl;
                return false;
            }
        }
        else {
            if (!perso.loadFromFile("SpritePlayer/Among Us - Player Base/Individual Sprites/Death/Dead00" + std::to_string(SpriteMort) + ".png")) {
                std::cout << "Probleme a telecharger l'IA" << std::endl;
                return false;
            }
        }

        persoSprite.setTexture(perso);

        return true;
    }

    template<typename T> void print_queue(T& q) {
        while (!q.empty()) {
            std::cout << q.top().x << ", " << q.top().y << std::endl;
            q.pop();
        }
    }
    /*
    Faire un test des voisin de 10/10 solo 
    afficher chaque putain d'info
    */
    std::vector<InfoIA> voisin(InfoIA caseActu, const int* map) {
        std::vector<InfoIA> result;
        int nbElem;
        int up = map[caseActu.x + (caseActu.y - 1) * 35];
        int down = map[caseActu.x + (caseActu.y + 1) * 35];
        int left = map[(caseActu.x - 1) + caseActu.y * 35];
        int right = map[(caseActu.x + 1) + caseActu.y * 35];
        if (up == 0 || up == 3 || up == 18) {
            //on rajoute la direction haut
            InfoIA haut = caseActu;
            haut.y -= 1;
            haut.cout += 1;
            haut.cheminEmprunter.push_back(1);
            result.push_back(haut);
        }
        if (down == 0 || down == 3 || down == 18) {
            //on rajoute la direction bas
            InfoIA bas = caseActu;
            bas.y += 1;
            bas.cout += 1;
            bas.cheminEmprunter.push_back(2);
            result.push_back(bas);
        }
        if (left == 0 || left == 3 || left == 18) {
            //on rajoute la direction gauche
            InfoIA gauche = caseActu;
            gauche.x -= 1;
            gauche.cout += 1;
            gauche.cheminEmprunter.push_back(3);
            result.push_back(gauche);
        }
        if (right == 0 || right == 3 || right == 18) {
            //on rajoute la direction droite
            InfoIA droite = caseActu;
            droite.x += 1;
            droite.cout += 1;
            droite.cheminEmprunter.push_back(4);
            result.push_back(droite);
        }
        return result;
    }

    std::vector<int> reverse(std::vector<int> liste){
        std::vector<int> result{};
        for (int i = liste.size()-1; i >= 0; i--) {
            result.push_back(liste[i]);
        }
        return result;
    }

    int manhattan(InfoIA depart, InfoIA arrive) {
        return abs(arrive.x - depart.x) + abs(arrive.y - depart.y);
    }

    void cheminPlusCourt(const int* map) {
        InfoIA caseActu;
        InfoIA depart{ persoSprite.getPosition().x / 150, persoSprite.getPosition().y / 150,0,0 };
        InfoIA ret;
        int verifDoublon;
        std::vector<InfoIA> dejaVu;
        auto cmp = [](InfoIA gauche, InfoIA droite) {return gauche.heuristique + gauche.cout > droite.heuristique + droite.cout; };
        std::priority_queue<InfoIA, std::vector<InfoIA>, decltype(cmp)> cheminActu(cmp);
        if (cheminIA.empty()) {
            InfoIA arrive = donneDestination();
            std::cout << "Arrive a destination" << std::endl;
            cheminActu.push(depart);
            while (!cheminActu.empty()) {
                caseActu = cheminActu.top();
                cheminActu.pop();
                if (caseActu.x == arrive.x && caseActu.y == arrive.y) {
                    cheminIA = reverse(caseActu.cheminEmprunter);
                    return;
                }
                for (InfoIA elem : voisin(caseActu, map)) {
                    //Verifie les doublon
                    verifDoublon = 0;
                    for (InfoIA retenu : dejaVu) {
                        if (retenu.x == elem.x && retenu.y == elem.y) {
                            verifDoublon = 1;
                            break;
                        }
                    }
                    std::priority_queue<InfoIA, std::vector<InfoIA>, decltype(cmp)> copie = cheminActu;
                    while (!copie.empty()) {
                        ret = copie.top();
                        copie.pop();
                        if (ret.x == elem.x && ret.y == elem.y && ret.cout < elem.cout) {
                            verifDoublon = 1;
                            break;
                        }
                    }
                    if (!verifDoublon) {
                        elem.heuristique = manhattan(elem, arrive);
                        cheminActu.push(elem);
                    }
                }
                dejaVu.push_back(caseActu);
            }
            std::cout << "Probleme pour trouver un chemin pour l'IA" << std::endl;
            return;
        }
        else {
            reconstituerChemin();
        }
    }

    void cheminPlusCourt(const int* map, int x, int y) {
        InfoIA caseActu;
        InfoIA depart{ persoSprite.getPosition().x / 150, persoSprite.getPosition().y / 150,0,0};
        InfoIA ret;
        int verifDoublon;
        std::vector<InfoIA> dejaVu;
        auto cmp = [](InfoIA gauche, InfoIA droite) {return gauche.heuristique + gauche.cout > droite.heuristique + droite.cout; };
        std::priority_queue<InfoIA, std::vector<InfoIA>, decltype(cmp)> cheminActu(cmp);
        if (cheminIA.empty()) {
            InfoIA arrive{x/150,y/150,0,0};
            std::cout << "Arrive a destination" << std::endl;
            cheminActu.push(depart);
            while (!cheminActu.empty()) {
                caseActu = cheminActu.top();
                cheminActu.pop();
                if (caseActu.x == arrive.x && caseActu.y == arrive.y) {
                    //terminer le programme
                    cheminIA = reverse(caseActu.cheminEmprunter);
                    return;
                }
                for (InfoIA elem : voisin(caseActu, map)) {
                    //Verifie les doublon
                    verifDoublon = 0;
                    for (InfoIA retenu : dejaVu) {
                        if (retenu.x == elem.x && retenu.y == elem.y) {
                            verifDoublon = 1;
                            break;
                            //break;
                        }
                    }
                    std::priority_queue<InfoIA, std::vector<InfoIA>, decltype(cmp)> copie = cheminActu;
                    while (!copie.empty()) {
                        ret = copie.top();
                        copie.pop();
                        if (ret.x == elem.x && ret.y == elem.y && ret.cout < elem.cout) {
                            verifDoublon = 1;
                            break;
                            //break;
                        }
                    }
                    if (!verifDoublon) {
                        elem.heuristique = manhattan(elem, arrive);
                        cheminActu.push(elem);
                    }
                }
                dejaVu.push_back(caseActu);
            }
            //terminer le programme(avec erreur)
            std::cout << "Probleme pour trouver un chemin pour l'IA" << std::endl;
            return;
        }
        else {
            reconstituerChemin();
        }
    }

    InfoIA donneDestination() {
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<int> distr(1, 8);
        switch (distr(eng)) {
        case 1:
            //1 = Salle a manger
            std::cout << "Salle a manger" << std::endl;
            return InfoIA{ 1,17,0,0 };
        case 2:
            //2 = Cuisine
            std::cout << "Cuisine" << std::endl;
            return InfoIA{ 1,5,0,0 };
        case 3:
            //3 = Chambre
            std::cout << "Chambre" << std::endl;
            return InfoIA{ 13,6,0,0 };
        case 4:
            //4 = Jardin
            std::cout << "Jardin" << std::endl;
            return InfoIA{ 17,2,0,0 };
        case 5:
            //5 = Grenier
            std::cout << "Grenier" << std::endl;
            return InfoIA{ 32,6,0,0 };
        case 6:
            //6 = Bibliotheque
            std::cout << "Bibliotheque" << std::endl;
            return InfoIA{ 30,15,0,0 };
        case 7:
            //7 = Salle de spawn
            std::cout << "Salle de spawn" << std::endl;
            return InfoIA{ 18,7,0,0 };
        case 8:
            //8 = Hall principa
            std::cout << "Hall principal" << std::endl;
            return InfoIA{ 17,13,0,0 };
        default:
            std::cout << "Defaut" << std::endl;
            return InfoIA{ 10,10,0,0 };
        };
    }

    sf::View getView() {
        sf::View vu;
        vu.reset(sf::FloatRect(0, 0, 1600, 900));
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
    
    void estMort() {
        estVivant = false;
    }

    void Respawn(){
        cheminIA.clear();
        persoSprite.setPosition(1500, 1500);
    }

    bool MortReperer(const int* map,std::vector<IA*> partenaire) {
        for (IA* elem : partenaire) {
            IA actu = *elem;
            int posX = actu.persoSprite.getPosition().x;
            int posY = actu.persoSprite.getPosition().y;
            if (posX - tailleTuile*3 <= persoSprite.getPosition().x && persoSprite.getPosition().x <= posX + tailleTuile * 3 && actu.persoSprite.getPosition().y - tailleTuile * 3 <= persoSprite.getPosition().y && persoSprite.getPosition().y <= actu.persoSprite.getPosition().y + tailleTuile * 3 && !actu.estVivant) {
                cheminIA.clear();
                cheminPlusCourt(map, posX, posY);
                corpReperer = true;
                return corpReperer;
            }
        }
        return corpReperer;
    }

    bool Report(std::vector<IA*> partenaire) {
        for (IA* elem : partenaire) {
            IA actu = *elem;
            if (actu.persoSprite.getPosition().x - 150 <= persoSprite.getPosition().x && persoSprite.getPosition().x <= actu.persoSprite.getPosition().x + 150 && actu.persoSprite.getPosition().y - 150 <= persoSprite.getPosition().y && persoSprite.getPosition().y <= actu.persoSprite.getPosition().y + 150 && !actu.estVivant) {
                for (IA* survivant : partenaire) {
                    if ((*survivant).estVivant) {
                        (*survivant).corpReperer = false;
                        (*survivant).Respawn();
                    }
                }
                return true;
            }
        }
        return false;
    }

    sf::Texture perso;
    sf::Sprite persoSprite;
    bool arriveDesti = false;
    bool estVivant = true;
    bool corpReperer = false;
    int SpriteMort = 0;
    std::vector<int> cheminIA;

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
    bool arriveEtape = true;
    int tailleTuile = 150;
};

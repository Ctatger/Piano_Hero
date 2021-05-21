#ifndef Jeu_h
#define Jeu_h
#define Init_button Bouton(0,0,0)
#include <Arduino.h>
#include<stdio.h>
#include <vector>
#include <memory>
#include "Playlist.h"
#include "Joystick.h"
#include "Bouton.h"
using namespace  std;


/*//////////////////////////////////////////////////////////////////////////
            DEFINITION DE LA CLASSE JEU
  /////////////////////////////////////////////////////////////////////*/
class Jeu {
  private:
    Playlist playlist;                //Playlist utilisée lors du jeu
    int score;                        //Variable comptabilisant le score du joueur lors de sa partie
    int streak;                       //Variable comptabilisant le nombre de notes juste jouées à la suite actuellement
    
  public:
    //Constructeur
    Jeu(Playlist pl);

    //Fonction qui lance le jeu
    void Launch_Game(std::vector<Bouton> Bp);

    //Affectation des notes aux 5 boutons de notre jeu
    std::vector<Bouton> Set_notes(std::vector<int> notes);

    //Récupération du score du joueur
    int get_score();

    //Récupération du streak du joueur
    int get_streak();

    //Vérification que le bouton appuyé par le joueur est le bon
    bool check_note(int ans, int val_1, int val_2, int appui);
};
#endif

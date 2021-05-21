#ifndef Bouton_h
#define Bouton_h

#include <Arduino.h>
#include<stdio.h>

using namespace std;

/*//////////////////////////////////////////////////////////////////////////
                   DEFINITION DE LA CLASSE BOUTON
  /////////////////////////////////////////////////////////////////////*/
class Bouton {

  private:
    int bpin;            //numéro de pin du bouton
    char bnote;         //note associée au bouton, pour le bouton ok, la note sera "K"

  public:
    // Constructeur et destructeur
    Bouton(int pin, char note);
    ~Bouton();
    // Récupération des ordres du bouton
    char get_order();
};


#endif

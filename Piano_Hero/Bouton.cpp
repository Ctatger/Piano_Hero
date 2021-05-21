#include "Bouton.h"


/*//////////////////////////////////////////////////////////////////////////
  Constructeur et destructeur
  /////////////////////////////////////////////////////////////////////*/
Bouton::Bouton(int pin, char note){
    bpin = pin;
    bnote = note;
    pinMode(bpin, INPUT);    // declare pushbutton as input
}

Bouton::~Bouton(){
}
/*//////////////////////////////////////////////////////////////////////////
  Récupération des ordres du bouton
  /////////////////////////////////////////////////////////////////////*/
char Bouton:: get_order(){
  int val;
  val = digitalRead(bpin);
  
  if (val==HIGH) {
    return bnote;
  }
  if (val==LOW) {
    return ' ';
  }
}

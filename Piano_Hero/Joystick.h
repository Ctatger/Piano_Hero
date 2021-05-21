#ifndef Joystick_h
#define Joystick_h
#include "arduino.h"

/*//////////////////////////////////////////////////////////////////////////
            DEFINITION DE LA CLASSE JOYSTICK
  /////////////////////////////////////////////////////////////////////*/
class Joystick {
  public:
    //Constructeur
    Joystick(int pin);
    //Récupération des instructions du joystick
    int get_order();  

  private:
    int Joystick_pin;
    int is_dz=true;         // Variable vérifiant si on "réinitialise" bien le joystick a chaque fois
    int sens;               // Variable définissant si le Joystick pointe vers le haut ou le bas, utilisé pour la navigation dans la playlist
};


#endif

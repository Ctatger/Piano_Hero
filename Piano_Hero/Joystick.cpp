#include "Joystick.h"


/*//////////////////////////////////////////////////////////////////////////
  Constructeur de la classe Joystick
  /////////////////////////////////////////////////////////////////////*/
Joystick::Joystick(int pin) {
  Joystick_pin = pin;
  is_dz = true;
  sens=-1;
}
/*//////////////////////////////////////////////////////////////////////////
  Fonction permettant de savoir dans quel sens le joystick a été activé
  /////////////////////////////////////////////////////////////////////*/
int Joystick::get_order() {
  int sensorValue;
  sensorValue = analogRead(Joystick_pin);

  if (sensorValue >= 700 && is_dz == true) {
    is_dz = false;
    sens=0;
  }
  else if (sensorValue <= 300 && is_dz == true) {
    is_dz = false;
    sens=1;
  }
  else if (sensorValue < 700 && sensorValue > 300) {
    is_dz = true;
    sens=-1;
  }
  return sens;
}

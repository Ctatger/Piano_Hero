#ifndef GPI_h
#define GPI_h
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <Arduino.h>
#include<stdio.h>

using namespace std;

/*//////////////////////////////////////////////////////////////////////////
      DEFINITION DE LA CLASSE GPI
  /////////////////////////////////////////////////////////////////////*/
class GPI
{
private:
    int port;         //port attribué au GPI

public:
    //Constructeur
    GPI(int n_port){port = n_port};
    //Destructeur
    ~GPI(){};
};

/*//////////////////////////////////////////////////////////////////////////
                   DEFINITION DE LA CLASSE BOUTON
  /////////////////////////////////////////////////////////////////////*/
class Bouton : public GPI 
{
  private:
  int bnote[2];         //notes associées au bouton
  
  public:
    // Constructeur et destructeur
    Bouton(int pin, char note):GPI(pin);
    ~Bouton();
    // Récupération des ordres du bouton
    char get_order();
};
/*//////////////////////////////////////////////////////////////////////////
                  DEFINITION DE LA CLASSE JOYSTICK
  /////////////////////////////////////////////////////////////////////*/
class Joystick : public GPI 
{
public:
    //Constructeur
    Joystick(int pin):GPI(pin);
    //Récupération de la valeur du joystick
    int get_order();

  private:
    int is_dz=true;     // Variable vérifiant si on "réinitialise" bien le joystick a chaque fois
    int sens;           // Variable définissant si le Joystick pointe vers le haut ou le bas, utilisé pour la navigation dans la playlist
};
/*//////////////////////////////////////////////////////////////////////////
                  DEFINITION DE LA CLASSE SCREEN
  /////////////////////////////////////////////////////////////////////*/
class Screen
{
private:
    int height;
    int width;

public:
    Screen(int TFT_CS, int TFT_RST, int TFT_DC, int TFT_MOSI, int TFT_SCLK){ 
      Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
      tft.init(240, 240); 
    };
    ~Screen();

};



#endif

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "Joystick.h"
#include "Playlist.h"
using namespace std;

/*//////////////////////////////////////////////////////////////////////////
                     INITIALISATION DE L'ECRAN LCD
  /////////////////////////////////////////////////////////////////////*/
#define TFT_CS         0    //D3
#define TFT_RST        4    //D2                                 
#define TFT_DC         5    //D1
#define TFT_MOSI       13   //D7
#define TFT_SCLK       14   //D5
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

/*//////////////////////////////////////////////////////////////////////////
                AFFECTATION DU PORT ANALOG AU JOYSTICK
  /////////////////////////////////////////////////////////////////////*/
Joystick joystick(A0);

/*//////////////////////////////////////////////////////////////////////////
              DEFINITION DES VARIABLES GLOBALES DU PROGRAMME
  /////////////////////////////////////////////////////////////////////*/
uint16_t notes_tot=500;       //nombre total de notes dans la chanson (variable créée pour la validation de l'affichage car la partie jeu n'est pas opératonnelle)
uint16_t notes_ok=120;        //nombre total de notes bien jouées par le joueur dans la chanson //nombre total de notes dans la chanson (variable créée pour la validation de l'affichage car la partie jeu n'est pas opératonnelle)
uint16_t streak=110;          //streak du joueur (variable créée pour la validation de l'affichage car la partie jeu n'est pas opératonnelle)
uint16_t phase=0;             //définit la phase du jeu dans laquelle on se situe, elle sert à gérer l'affichage
//=0 cinématique, 
//=1 initialisation de l'affichage du menu,
//=2 navigation dans la playlist dans le menu,
//=3 initialisation de l'affichage du jeu, 
//=4 actualisation de l'affichage du jeu déjà lancé,
//=5 fin de jeu

//Récupération des notes attribuées aux boutons pour gérer l'affichage
int ref_note_1=0;
int ref_note_2=0;
int ref_note_3=0;
int ref_note_4=0;

//direction du joystick initialisé à -1 (0=pointe en haut et 1= =pointe en bas)
int joy=-1;

//chaine de caractère qui récupère le titre de la chanson
char * song;

//variable utilisée pour observer le rafraichissement de l'affichage
int i=0;

/*//////////////////////////////////////////////////////////////////////////
                  CREATION DE LA PLAYLIST
  /////////////////////////////////////////////////////////////////////*/
std::vector<int> HappyBirthday = {140,NOTE_C4,4, NOTE_C4,8, NOTE_D4,-4, NOTE_C4,-4, NOTE_F4,-4,NOTE_E4,-2, NOTE_C4,4, NOTE_C4,8, NOTE_D4,-4, NOTE_C4,-4, NOTE_G4,-4,NOTE_F4,-2, NOTE_C4,4, NOTE_C4,8,NOTE_C5,-4,
NOTE_A4,-4, NOTE_F4,-4, NOTE_E4,-4, NOTE_D4,-4, NOTE_AS4,4, NOTE_AS4,8,NOTE_A4,-4, NOTE_F4,-4, NOTE_G4,-4,NOTE_F4,-2};

std::vector<int> zelda = {
  88, NOTE_AS4,-2,  NOTE_F4,8,  NOTE_F4,8,  NOTE_AS4,8,//1
  NOTE_GS4,16,  NOTE_FS4,16,  NOTE_GS4,-2,
  NOTE_AS4,-2,  NOTE_FS4,8,  NOTE_FS4,8,  NOTE_AS4,8,
  NOTE_A4,16,  NOTE_G4,16,  NOTE_A4,-2,
  REST,1, 

  NOTE_AS4,4,  NOTE_F4,-4,  NOTE_AS4,8,  NOTE_AS4,16,  NOTE_C5,16, NOTE_D5,16, NOTE_DS5,16,//7
  NOTE_F5,2,  NOTE_F5,8,  NOTE_F5,8,  NOTE_F5,8,  NOTE_FS5,16, NOTE_GS5,16,
  NOTE_AS5,-2,  NOTE_AS5,8,  NOTE_AS5,8,  NOTE_GS5,8,  NOTE_FS5,16,
  NOTE_GS5,-8,  NOTE_FS5,16,  NOTE_F5,2,  NOTE_F5,4, 

  NOTE_DS5,-8, NOTE_F5,16, NOTE_FS5,2, NOTE_F5,8, NOTE_DS5,8, //11
  NOTE_CS5,-8, NOTE_DS5,16, NOTE_F5,2, NOTE_DS5,8, NOTE_CS5,8,
  NOTE_C5,-8, NOTE_D5,16, NOTE_E5,2, NOTE_G5,8, 
  NOTE_F5,16, NOTE_F4,16, NOTE_F4,16, NOTE_F4,16,NOTE_F4,16,NOTE_F4,16,NOTE_F4,16,NOTE_F4,16,NOTE_F4,8, NOTE_F4,16,NOTE_F4,8,

  NOTE_AS4,4,  NOTE_F4,-4,  NOTE_AS4,8,  NOTE_AS4,16,  NOTE_C5,16, NOTE_D5,16, NOTE_DS5,16,//15
  NOTE_F5,2,  NOTE_F5,8,  NOTE_F5,8,  NOTE_F5,8,  NOTE_FS5,16, NOTE_GS5,16,
  NOTE_AS5,-2, NOTE_CS6,4,
  NOTE_C6,4, NOTE_A5,2, NOTE_F5,4,
  NOTE_FS5,-2, NOTE_AS5,4,
  NOTE_A5,4, NOTE_F5,2, NOTE_F5,4,

  NOTE_FS5,-2, NOTE_AS5,4,
  NOTE_A5,4, NOTE_F5,2, NOTE_D5,4,
  NOTE_DS5,-2, NOTE_FS5,4,
  NOTE_F5,4, NOTE_CS5,2, NOTE_AS4,4,
  NOTE_C5,-8, NOTE_D5,16, NOTE_E5,2, NOTE_G5,8, 
  NOTE_F5,16, NOTE_F4,16, NOTE_F4,16, NOTE_F4,16,NOTE_F4,16,NOTE_F4,16,NOTE_F4,16,NOTE_F4,16,NOTE_F4,8, NOTE_F4,16,NOTE_F4,8
};

std::vector<int> Mario = {
  200, NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,8, //1
  NOTE_G5,4, REST,4, NOTE_G4,8, REST,4, 
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // 3
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // repeats from 3
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,

  REST,4, NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4, NOTE_E5,8,//7
  REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_C4,8, REST,8, NOTE_A4,8, NOTE_C5,8, NOTE_D5,8,
  REST,4, NOTE_DS5,4, REST,8, NOTE_D5,-4,
  NOTE_C5,2, REST,2,

  REST,4, NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4, NOTE_E5,8,//repeats from 7
  REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_C4,8, REST,8, NOTE_A4,8, NOTE_C5,8, NOTE_D5,8,
  REST,4, NOTE_DS5,4, REST,8, NOTE_D5,-4,
  NOTE_C5,2, REST,2,

  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,4,//11
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2,

  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,8, NOTE_E5,8,//13
  REST,1, 
  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,4,
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2,
  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,4,
  NOTE_G5,4, REST,4, NOTE_G4,4, REST,4, 
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // 19
  
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4, NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,

  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // repeats from 19
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4, NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,

  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,//23
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_D5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_G5,-8, NOTE_F5,-8,
  
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2, //26
  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_B4,8, NOTE_F5,4, NOTE_F5,8, NOTE_F5,-8, NOTE_E5,-8, NOTE_D5,-8,
  NOTE_C5,8, NOTE_E4,4, NOTE_E4,8, NOTE_C4,2,

  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,//repeats from 23
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_D5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_G5,-8, NOTE_F5,-8,
  
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2, //26
  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_B4,8, NOTE_F5,4, NOTE_F5,8, NOTE_F5,-8, NOTE_E5,-8, NOTE_D5,-8,
  NOTE_C5,8, NOTE_E4,4, NOTE_E4,8, NOTE_C4,2,
  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,8, NOTE_E5,8,
  REST,1,

  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,4, //33
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2,
  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,4,
  NOTE_G5,4, REST,4, NOTE_G4,4, REST,4, 
  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_D5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_G5,-8, NOTE_F5,-8,
  
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2, //40
  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_B4,8, NOTE_F5,4, NOTE_F5,8, NOTE_F5,-8, NOTE_E5,-8, NOTE_D5,-8,
  NOTE_C5,8, NOTE_E4,4, NOTE_E4,8, NOTE_C4,2,
  
  //game over sound
  NOTE_C5,-4, NOTE_G4,-4, NOTE_E4,4, //45
  NOTE_A4,-8, NOTE_B4,-8, NOTE_A4,-8, NOTE_GS4,-8, NOTE_AS4,-8, NOTE_GS4,-8,
  NOTE_G4,8, NOTE_D4,8, NOTE_E4,-2,  

};

std::vector<int> StarWars = {

  120,NOTE_A4,-4, NOTE_A4,-4, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_F4,8, REST,8,
  NOTE_A4,-4, NOTE_A4,-4, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_F4,8, REST,8,
  NOTE_A4,4, NOTE_A4,4, NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16,

  NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,//4
  NOTE_E5,4, NOTE_E5,4, NOTE_E5,4, NOTE_F5,-8, NOTE_C5,16,
  NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,
  
  NOTE_A5,4, NOTE_A4,-8, NOTE_A4,16, NOTE_A5,4, NOTE_GS5,-8, NOTE_G5,16, //7 
  NOTE_DS5,16, NOTE_D5,16, NOTE_DS5,8, REST,8, NOTE_A4,8, NOTE_DS5,4, NOTE_D5,-8, NOTE_CS5,16,

  NOTE_C5,16, NOTE_B4,16, NOTE_C5,16, REST,8, NOTE_F4,8, NOTE_GS4,4, NOTE_F4,-8, NOTE_A4,-16,//9
  NOTE_C5,4, NOTE_A4,-8, NOTE_C5,16, NOTE_E5,2,

  NOTE_A5,4, NOTE_A4,-8, NOTE_A4,16, NOTE_A5,4, NOTE_GS5,-8, NOTE_G5,16, //7 
  NOTE_DS5,16, NOTE_D5,16, NOTE_DS5,8, REST,8, NOTE_A4,8, NOTE_DS5,4, NOTE_D5,-8, NOTE_CS5,16,

  NOTE_C5,16, NOTE_B4,16, NOTE_C5,16, REST,8, NOTE_F4,8, NOTE_GS4,4, NOTE_F4,-8, NOTE_A4,-16,//9
  NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,
  
};

std::vector<int> rickroll = {

  114,NOTE_D5,-4, NOTE_E5,-4, NOTE_A4,4, //1
  NOTE_E5,-4, NOTE_FS5,-4, NOTE_A5,16, NOTE_G5,16, NOTE_FS5,8,
  NOTE_D5,-4, NOTE_E5,-4, NOTE_A4,2,
  NOTE_A4,16, NOTE_A4,16, NOTE_B4,16, NOTE_D5,8, NOTE_D5,16,
  NOTE_D5,-4, NOTE_E5,-4, NOTE_A4,4, //repeat from 1
  NOTE_E5,-4, NOTE_FS5,-4, NOTE_A5,16, NOTE_G5,16, NOTE_FS5,8,
  NOTE_D5,-4, NOTE_E5,-4, NOTE_A4,2,
  NOTE_A4,16, NOTE_A4,16, NOTE_B4,16, NOTE_D5,8, NOTE_D5,16,
  REST,4, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_D5,8, NOTE_E5,8, NOTE_CS5,-8,
  NOTE_B4,16, NOTE_A4,2, REST,4, 

  REST,8, NOTE_B4,8, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,4, NOTE_A4,8, //7
  NOTE_A5,8, REST,8, NOTE_A5,8, NOTE_E5,-4, REST,4, 
  NOTE_B4,8, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,8, NOTE_D5,8, NOTE_E5,8, REST,8,
  REST,8, NOTE_CS5,8, NOTE_B4,8, NOTE_A4,-4, REST,4,
  REST,8, NOTE_B4,8, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,8, NOTE_A4,4,
  NOTE_E5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,4, REST,4,
   
  NOTE_D5,2, NOTE_E5,8, NOTE_FS5,8, NOTE_D5,8, //13
  NOTE_E5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,4, NOTE_A4,4,
  REST,2, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,8,
  REST,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,

  NOTE_E5,-8, NOTE_E5,-8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,-8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16, //18
  NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,8, NOTE_A4,8, NOTE_A4,8, 
  NOTE_E5,4, NOTE_D5,2, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_A5,4, NOTE_CS5,8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,

  NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,4, NOTE_A4,8,  //23
  NOTE_E5,4, NOTE_D5,2, REST,4,
  REST,8, NOTE_B4,8, NOTE_D5,8, NOTE_B4,8, NOTE_D5,8, NOTE_E5,4, REST,8,
  REST,8, NOTE_CS5,8, NOTE_B4,8, NOTE_A4,-4, REST,4,
  REST,8, NOTE_B4,8, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,8, NOTE_A4,4,
  REST,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8, NOTE_D5,8,
  
  REST,8, NOTE_A4,8, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,8, //29
  REST,8, NOTE_CS5,8, NOTE_B4,8, NOTE_A4,-4, REST,4,
  NOTE_B4,8, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,8, NOTE_A4,4, REST,8,
  REST,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,4, NOTE_E5,-4, 
  NOTE_D5,2, NOTE_D5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,4, 
  NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8, NOTE_A4,8, NOTE_A4,4,

  REST,-4, NOTE_A4,8, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,8, //35
  REST,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_E5,-8, NOTE_E5,-8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,4, NOTE_A4,8, 

   NOTE_E5,4, NOTE_D5,2, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16, //40
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_A5,4, NOTE_CS5,8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,4, NOTE_A4,8,  
  NOTE_E5,4, NOTE_D5,2, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
   
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16, //45
  NOTE_A5,4, NOTE_CS5,8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,4, NOTE_A4,8,  
  NOTE_E5,4, NOTE_D5,2, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16, //45
  
  NOTE_A5,4, NOTE_CS5,8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,4, NOTE_A4,8, 

  NOTE_E5,4, NOTE_D5,2, REST,4
};
  

Playlist PL("HappyBirthdays", HappyBirthday);
int inutile=PL.Add_Song("Zelda",zelda);
int inutile2=PL.Add_Song("Mario",Mario);
int inutile3=PL.Add_Song("Star Wars",StarWars);
int inutile4=PL.Add_Song("Rick Roll",rickroll);


void setup() {
  Serial.begin(9600);

  // OR use this initializer (uncomment) if using a 1.3" or 1.54" 240x240 TFT:
  tft.init(240, 240);           // Init ST7789 240x240

  
}

void loop() {
  if (phase==0){
    start_cinematique();
    }
  else if(phase==1){
    interface_acceuil();
    song=PL.get_song();
    update_nom_chanson(song);
    }
  else if(phase==2){
  joy=joystick.get_order();
    if (joy == 0){
      PL++;
      song=PL.get_song();
      update_nom_chanson(song);
      }
    else if(joy == 1){
      PL--;
      song=PL.get_song();
      update_nom_chanson(song);
      }
    if(digitalRead(16)==HIGH){
      phase++;
      }
    }
  else if(phase==3){
    display_jeu();
    }
    
  else if(phase==4){
    PL.start_song(PL.get_note());
    PL.play_song();
    /*if(i==0){update_note_a_jouer(1,2,3,4,5);i++;}
    else if(i==1){update_note_a_jouer(2,3,4,5,6); i++;}
    else if(i==2){update_note_a_jouer(3,4,5,6,7);i++;}
    else if(i==3){update_note_a_jouer(4,5,6,7,8);i++;}
    else if(i==4){update_note_a_jouer(5,6,7,8,9);i=0;}*/
    if (digitalRead(16)==HIGH){
      phase++;
      }
    }
  else if(phase==5){
    display_fin_jeu(notes_tot,notes_ok,streak);
    }
}

/*//////////////////////////////////////////////////////////////////////////
               CREATION DES CREDITS DU DEBUT  DU JEU
  /////////////////////////////////////////////////////////////////////*/
void start_cinematique(){
  tft.fillScreen(ST77XX_BLACK);
  
  tft.setCursor(40, 20);
  tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.println("Welcome to...");
  
  tft.setCursor(60, 70);
  tft.setTextColor(ST77XX_RED,ST77XX_BLACK);
  tft.setTextSize(4);
  tft.println("PIANO");
  tft.setCursor(70, 130);
  tft.setTextColor(ST77XX_RED,ST77XX_BLACK);
  tft.setTextSize(4);
  tft.println("HERO");

  tft.setCursor(0, 200);
  tft.setTextColor(ST77XX_GREEN,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.println("Made by:");
  tft.setCursor(0, 220);
  tft.setTextColor(ST77XX_GREEN,ST77XX_BLACK);
  tft.setTextSize(1);
  tft.println("Alexandre Collobert - Corentin Tatger");
  delay(5000);
  phase++;
}

/*//////////////////////////////////////////////////////////////////////////
                  CREATION DE L'AFFICHAGE DU MENU
  /////////////////////////////////////////////////////////////////////*/
void interface_acceuil(){
  tft.fillScreen(ST77XX_BLACK);
  
  tft.setCursor(40, 20);
  tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.println("Select a song");
  
  tft.fillTriangle(115, 70, 120, 60, 125, 70, ST77XX_WHITE);
  tft.setCursor(40, 80);
  tft.setTextColor(ST77XX_GREEN,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.println("Song name");
  tft.fillTriangle(115, 110, 120, 120, 125, 110, ST77XX_WHITE);

  tft.setCursor(50, 150);
  tft.setTextColor(ST77XX_RED,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.println("Choose mode:");
  tft.setCursor(20, 180);
  tft.setTextColor(ST77XX_RED,ST77XX_BLACK);
  tft.setTextSize(1);
  tft.println("Left button");
  tft.setCursor(20, 200);
  tft.setTextColor(ST77XX_RED,ST77XX_BLACK);
  tft.setTextSize(1);
  tft.println("Hear the song");
  tft.setCursor(140, 180);
  tft.setTextColor(ST77XX_RED,ST77XX_BLACK);
  tft.setTextSize(1);
  tft.println("Right button");
  tft.setCursor(140, 200);
  tft.setTextColor(ST77XX_RED,ST77XX_BLACK);
  tft.setTextSize(1);
  tft.println("Play the song");

  phase++;
}

void update_nom_chanson(char* song){
  tft.fillRect(0, 80, 240, 20, ST77XX_BLACK);
  tft.setCursor(30, 80);
  tft.setTextColor(ST77XX_GREEN,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.println(song);
}

/*//////////////////////////////////////////////////////////////////////////
                  CREATION DE L'AFFICHAGE DU JEU
  /////////////////////////////////////////////////////////////////////*/
void display_jeu(){
  tftinterfacejeu();
  tftbandeaunote();
  display_nb_notes_ok(notes_ok);
  display_nb_notes_total(notes_tot);
  display_streak(streak);
  phase++;
}
void tftinterfacejeu(){
  tft.fillScreen(ST77XX_BLACK);
  
  tft.drawRect(0, 165, 240, 35, ST77XX_WHITE);
  for(int i=1;i<6;i++){
    tft.drawLine(48*i, 165, 48*i, 199, ST77XX_WHITE);
  }

  tft.setCursor(0, 205);
  tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.println("Notes ok:");
  
  tft.drawChar(40, 225, '/', ST77XX_WHITE, ST77XX_BLACK, 2);
  
  tft.setCursor(150, 205);
  tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.println("Streak:");
}

void tftbandeaunote(){
  tft.setTextColor(ST77XX_GREEN,ST77XX_BLACK);
  tft.setTextSize(2);
  for(int i=0;i<5;i++){
    tft.setCursor(20+48*i, 175);
    tft.print(i+1);
    }
}

void display_nb_notes_ok(uint16_t nb){
  tft.setCursor(0, 225);
  tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.print(nb);
}

void display_nb_notes_total(uint16_t nb){
  tft.setCursor(55, 225);
  tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.print(nb);
}

void display_streak(uint16_t nb){
  tft.setCursor(170, 225);
  tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.print(nb);
}

void update_note_a_jouer(int un, int de, int tr, int qu, int ci){
  tft.setTextColor(ST77XX_RED,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setCursor(116, 130);
  tft.print(ci);
  tft.setCursor(116, 100);
  tft.print(qu);
  tft.setCursor(116, 70);
  tft.print(tr);
  tft.setCursor(116, 40);
  tft.print(de);
  tft.setCursor(116, 10);
  tft.print(un);
}

/*//////////////////////////////////////////////////////////////////////////
                  AFFICHAGE DE LA FIN DE JEU
  /////////////////////////////////////////////////////////////////////*/
void display_fin_jeu(uint16_t notes_tot, uint16_t notes_ok, uint16_t streak){
  tft.fillScreen(ST77XX_BLACK);
  
  tft.setCursor(60, 20);
  tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.println("Your score");

  tft.setCursor(30, 70);
  tft.setTextColor(ST77XX_RED,ST77XX_BLACK);
  tft.setTextSize(4);
  tft.println(notes_ok);
  tft.drawChar(110, 70, '/', ST77XX_WHITE, ST77XX_BLACK, 4);
  tft.setCursor(140, 70);
  tft.setTextColor(ST77XX_RED,ST77XX_BLACK);
  tft.setTextSize(4);
  tft.println(notes_tot);

  tft.setCursor(55, 130);
  tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.println("Best streak");
  tft.setCursor(90, 170);
  tft.setTextColor(ST77XX_RED,ST77XX_BLACK);
  tft.setTextSize(4);
  tft.println(streak);

  delay(5000);
  phase=1;
}

/*//////////////////////////////////////////////////////////////////////////
             CONVERSION DE LA NOTE EN NUMERO DE BOUTON
  /////////////////////////////////////////////////////////////////////*/
int conv_note_bouton(int nb){
  int bouton;
  if (nb<=ref_note_1){bouton=1;}
  else if (nb<=ref_note_2){bouton=2;}
  else if (nb<=ref_note_3){bouton=3;}
  else if (nb<=ref_note_4){bouton=4;}
  else {bouton=5;}
  return bouton;
}

/*//////////////////////////////////////////////////////////////////////////
   RECUPERATION DES NOTES DE REFERENCES ET DE NOTE_TOT
  /////////////////////////////////////////////////////////////////////*/
void get_ref_notes(vector<int> vsong){
  vector<int> vsorted = vsong;
  std::sort(vsorted.begin(), vsorted.end());
  vsorted.erase(vsorted.begin());
  notes_tot=vsorted.size()/2;
  ref_note_1 = vsorted[notes_tot+1];    //les mesures<fréquence des not
  ref_note_2 = vsorted[notes_tot+3];
  ref_note_3 = vsorted[notes_tot+5];
  ref_note_4 = vsorted[notes_tot+7];
}

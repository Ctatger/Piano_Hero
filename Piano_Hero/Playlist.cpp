#include "Playlist.h"
#include <vector>
using namespace std;


/*//////////////////////////////////////////////////////////////////////////
   Constructeur de la classe playlist qui prend comme argument les notes
              le titre et l'url youtube de la chanson
  /////////////////////////////////////////////////////////////////////*/
Playlist::Playlist(char *nom, std::vector<int> notes) {
  Playlist::Add_Song(nom, notes);
  PlaylistL = 1;
  Curr_note = 0;
  pinBuzzer = 15;
  Curr_song = PlaylistNote.begin();
}

/*//////////////////////////////////////////////////////////////////////////
   Fonction permettant d'ajouter une chanson en fournissant son titre et
                        son vecteur de notes
  /////////////////////////////////////////////////////////////////////*/
int Playlist::Add_Song(char *nom, std::vector<int> notes) {
  PlaylistNote.insert(std::make_pair(nom, notes));
  PlaylistL++;
  return 1;
}
/*//////////////////////////////////////////////////////////////////////////
        Fonction renvoyant la note actuelle a jouer (en frequence)
               Renvoit -1 quand la chanson est terminée
  /////////////////////////////////////////////////////////////////////*/
int Playlist::get_value(char* nom) {
  int note;
  std::map<char*, std::vector<int>>::iterator Partition;

  Partition = PlaylistNote.find(nom);
  note = Partition->second[Curr_note];
  if (Curr_note <= Partition->second.size()) {
    Curr_note++;
  }
  else {
    return -1; //penser à catch dans la classe jeu, stop chanson si -1
  }
  return note;
}
/*//////////////////////////////////////////////////////////////////////////
   Fonction renvoyant la longueur actuelle de la playlist
  /////////////////////////////////////////////////////////////////////*/
int Playlist::get_PlaylistL() {
  return PlaylistL;
}
/*//////////////////////////////////////////////////////////////////////////
   Fonction renvoyant le titre de la chanson pointée par Curr_song
  /////////////////////////////////////////////////////////////////////*/
char* Playlist::get_song() {
  return Curr_song->first;
}
/*//////////////////////////////////////////////////////////////////////////
   Remet la note actuelle à 0 quand on démarre une nouvelle chanson
  /////////////////////////////////////////////////////////////////////*/
int Playlist::start_song(std::vector<int> nts) {
  Curr_note = 1;
  nb_notes=(nts.size()-1)/2;
  tempo = nts.front();
  return 1;
}
/*//////////////////////////////////////////////////////////////////////////
   Surcharge de l'opérateur d'incrémentation ++
  /////////////////////////////////////////////////////////////////////*/
Playlist& Playlist::operator ++(int) {
  if (Curr_song != PlaylistNote.end()) {
    Curr_song++;
    if (Curr_song == PlaylistNote.end()) {
      Curr_song =  PlaylistNote.begin();
    }
  }
  return (*this);
}
/*//////////////////////////////////////////////////////////////////////////
   Surcharge de l'opérateur de décrémentation --
  /////////////////////////////////////////////////////////////////////*/
Playlist& Playlist::operator --(int) {
  if (Curr_song != PlaylistNote.begin()) {
    Curr_song--;
  }
  return (*this);
}
/*//////////////////////////////////////////////////////////////////////////
   Fonction qui commande au buzzer de jouer une note
  /////////////////////////////////////////////////////////////////////*/
void Playlist::play_note(int n) {             
  int wholenote = (60000 * 4) / tempo;
  int divider = 0, noteDuration = 0;

  // calcule la durée de chaque note
  divider = Curr_song->second[n + 1];
  if (divider > 0) {
    // note classique
    noteDuration = (wholenote) / divider;
  } 
  else if (divider < 0) {
    // note avec un point de prolongation (notion de solfège)
    noteDuration = (wholenote) / abs(divider);
    // augmentation de la durée de un demi pour les notes avec un point de prolongation
    noteDuration *= 1.5; 
  }
  // we only play the note for 90% of the duration, leaving 10% as a pause
  tone(pinBuzzer, Curr_song->second[n], noteDuration * 0.9);

  // Wait for the specief duration before playing the next note.
  delay(noteDuration);

  // stop the waveform generation before the next note.
  noTone(pinBuzzer);
}
/*//////////////////////////////////////////////////////////////////////////
   Fonction qui récupère le vecteur de notes de la chanson jouée
  /////////////////////////////////////////////////////////////////////*/
std::vector<int> Playlist::get_note() {
  return Curr_song->second;
}
/*//////////////////////////////////////////////////////////////////////////
   Fonction qui joue entièrement la chanson
  /////////////////////////////////////////////////////////////////////*/
void Playlist::play_song() {
  Serial.println("Song start");
  while (Curr_note < nb_notes * 2) {
    play_note(Curr_note);
    Curr_note = Curr_note + 2;
  }
  Curr_note = 0;
}

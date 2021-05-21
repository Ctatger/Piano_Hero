#include "Jeu.h"

/*//////////////////////////////////////////////////////////////////////////
  Constructeur
  /////////////////////////////////////////////////////////////////////*/
Jeu::Jeu(Playlist pl):playlist(pl){
  score=0;
  streak=0;
  }
/*//////////////////////////////////////////////////////////////////////////
  Fonction qui vérifie que le bouton appuyé par le joueur est le bon
  /////////////////////////////////////////////////////////////////////*/
bool Jeu::check_note(int ans, int val_1,int val_2, int appui) {
  if (appui < 5) {
    if (ans == val_1 || ans == val_2) {
      score++;
      streak++;
      return true;
    }
    else {
      streak=0;
      return false;
    }
  }
  else {
    if (ans == val_1 || ans <= val_2) {
      score++;
      streak++;
      return true;
    }
    else {
      streak=0;
      return false;
    }
  }
}
/*//////////////////////////////////////////////////////////////////////////
  Fonction qui lance la partie jeu de Piano Hero
  /////////////////////////////////////////////////////////////////////*/
void Jeu::Launch_Game(std::vector<Bouton> Bp) {
  int appui;
  // Boucle d'attente de l'appui bouton
  while (!(Bp[1].get_state() || Bp[2].get_state() || Bp[3].get_state() || Bp[4].get_state() || Bp[5].get_state())) {}
  
  // identification du bouton sur lequel on a appuyé
  for (int i = 0; i < 5; i++) {
    if (Bp[i].get_state()) {
      appui = i;
    }
  }
  int note1 = Bp[appui].get_note(0);
  int note2 = Bp[appui].get_note(1);
  int ans = playlist.get_value(playlist.get_song());
  bool note_ok = check_note(ans,note1,note2,appui))
}
/*//////////////////////////////////////////////////////////////////////////
  Fonction qui attribue les notes aux boutons
  /////////////////////////////////////////////////////////////////////*/
std::vector<Bouton> Jeu::Set_notes(std::vector<int> notes) {
    std::vector<int> temp= notes;
    temp.erase(temp.begin());// On enlève le tempo avant de trier
    std::sort (temp.begin(), temp.end()); // On trie les notes par fréquences pour les assigner aux boutons dans l'ordre
    int nb_notes = (temp.size())/2;
    
    std::vector<Bouton> Bp;
    Bouton *pt =new Bouton(0, temp[nb_notes], temp[nb_notes+1]);
    Bp.push_back(*pt);
    for (int i = 1; i < 8; i++) {
      pt =new Bouton(i, temp[nb_notes+i + 1], temp[nb_notes+i + 2]);
      Bp.push_back(*pt); // On affecte deux notes à chaque bouton dans l'ordre croissant
    }
    return Bp;
}
/*//////////////////////////////////////////////////////////////////////////
  Fonction qui retourne le score du joueur
  /////////////////////////////////////////////////////////////////////*/
int Jeu::get_score(){
  return score;
  }
/*//////////////////////////////////////////////////////////////////////////
  Fonction qui retourne le streak du joueur
  /////////////////////////////////////////////////////////////////////*/
int Jeu::get_streak(){
  return streak;
  }

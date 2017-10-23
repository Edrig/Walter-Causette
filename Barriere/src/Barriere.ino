/* SCRIPT Fermeture Ouverture Barrière:
PRINCIPE : Les ILS de commande déclenchant la manoeuvre barrière sont en parallèle.
Un ILS devant la PW voie AB, l'autre sur le PS.
Les ILS sont ouverts au repos, fermés quand ils sont activés par l'aimant du loco.
Une commande peut être réalisée par le maître, un interrrupteur coupera cette possibilité.
Une commande déclanche une manoeuvre, la commande suivante l'inverse de celle qui a été précédemment effectuée.
Dès le passage sur l'ILS, ou si le maître commande,
la sonnette retentit, le feu clignote et la barrière se baisse.
Nouveau passage,
la barrière se relève, le feu s'éteint et la sonnette est muette.

SEQUENCE : Dès que l'ordre de fermeture barrière est donné :
la sonnerie retentit, le feu clignote (LED clignotante)
après 3 secondes la barrière se ferme
la barrière est fermée, la sonnette retentit et le feu clignote toujours
Dès que l'ordre d'ouverture de la barrière est donné :
la barrière s'ouvre
dès que la barrière est ouverte la sonnette est muette et le feu s'éteint

PARTICULARITES : Les servos CORONA DS-238MG fonctionne à partir de 35°. Angle d'ouverture 120°, donc maxi à 155).
*/

//DEFINITIONS

// SERVO
#include <VarSpeedServo.h>
#include <Button.h>
#include <ButtonEventCallback.h>
#include <PushButton.h>
#include <Bounce2.h>

#include <Blinker.h>

VarSpeedServo ServoBarriere; //Donne un nom à la servo pour communiquer

int PosOpen = 35;
int PosClose = 100;

enum EtatBar {
  open,
  close,
  unknown
};

EtatBar LastEtat = unknown;

// LES COMMANDES
PushButton Cmd = PushButton(2); // Les Pédales de commande sont raccordées sur la broche 2 de la carte Arduino.

// ACCESSOIRES
Blinker BkR(4); // le feu clignotant sur la broche 4, il convient d'utiliser une led clignotante,
// cette broche peut être utilisée pour l'interdiction de manoeuvre du loco
int BrocheSonnette = 8; // la Sonnette sur la broche 8


// INITIALISATION
void setup()
{

  // SERVO sur la broche 9, et sa position de départ, ouverte soit à 35°
  ServoBarriere.attach(9); // broche 9
  CmdBarriere(close);

  // ACCESSOIRES

  pinMode(BrocheSonnette, OUTPUT);  //Initialise la broche de la Sonnette comme une sortie
  digitalWrite(BrocheSonnette, LOW);  // Sonnette muette

  BkR.stop(); //feu clignotant OFF

  Cmd.onPress(BoutonBarriereOnPress);

} // Fin de Setup

// BOUCLE
void loop()
{
  Cmd.update();
}

void BoutonBarriereOnPress(Button& btn)
{
  switch (LastEtat)
  {
    case close:
    CmdBarriere(open);
    break;
    case open:
    CmdBarriere(close);
    break;
    case unknown:

    break;
  }

}

void CmdBarriere(EtatBar e)
{
  switch (e)
  {
    case open:
    ServoBarriere.write(PosOpen, 20, false);
    break;
    case close:
    ServoBarriere.write(PosClose, 20, false);
    break;
    case unknown:

    break;
  }

}

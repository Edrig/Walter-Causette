/* SCRIPT OUverture Fermeture Portes de la remise du loco;
Principe :
Arduino reçoit un ordre de manoeuvrer les portes. L'ordre est donné soit par l'arduino maître,
soit par un bouton poussoir sur le shield afin de passer en mode manuel. Pour éviter les perturbations électriques environnantes, l'ordre est un front bas.
Un poussoir sur le shield permet de réinitialiser le programme, c'est à dire de se positionner en fin de setup.

Séquence :
A la mst, l'arduino procède à l'initialisation , c'est à dire que les portes sont fermées et la lampe éteinte.
L'arduino est alors prêt pour les séquences ouvertures fermetures portes.
Dès que l'ordre d'ouverture portes est donné :
la lampe s'allume 2 sec avant la manoeuvre,
après 2 secondes, la porte de gauche s'ouvre, puis après une tempo de 1 seconde, la porte de droite s'ouvre.
Au prochain ordre, il s'agira de fermer les portes selon la manoeuvre inverse. la lampe s'éteint.
Porte de gauche quand on regarde la remise depuis l'extérieur.

Accessoires :
2 ILS qui renvoient un état haut
une LED bleue pour indiquer l'état prêt, renvoyé vers le panneau de commande général
une LED verte pour indiquer que les portes sont ouvertes
une LED rouge pour indiquer que les portes sont fermées ou en court de manoeuvre.
une LED jaune pour la lampe.
2 servos
*/

//DEFINITIONS
#include <VarSpeedServo.h>
#include <Button.h>
#include <ButtonEventCallback.h>
#include <PushButton.h>
#include <Bounce2.h>

VarSpeedServo MaServoG; //Donne un nom à la servo pour communiquer: servo gauche
VarSpeedServo MaServoD; //Donne un nom à la servo pour communiquer: servo droite

PushButton BoutonPortes = PushButton(2); // le bouton poussoir est sur la broche 2

int Lampe = 8;  // Branchement de la lampe sur la broche 8

int PorteG_open  = 140;
int PorteG_close =  40;
int PorteD_open  =  35;
int PorteD_close = 140;

enum EtatPorte {
  open,
  close,
  unknown
};

EtatPorte LastEtat = unknown;


// INITIALISATION
void setup()
{
  MaServoG.attach(9); // initialise la Servo Gauche sur la broche 9,
  MaServoD.attach(10); // initialise la Servo Droite sur la broche 10,

  BoutonPortes.onPress(BoutonPortesOnPress);

  //Initialisation de la position des portes au lancement du programme : les portes sont fermées, la lampe éteint
  MaServoD.write(PorteD_close, 20, true);
  MaServoG.write(PorteG_close, 20, true);


  // initialisation de la lampe, éteinte au départ et lorsque les portes sont fermées, elle s'allume 2 sec avant l'ouverture
  // s'éteint 2 sec après la fermeture.
  pinMode(Lampe,OUTPUT);
  digitalWrite(Lampe,LOW);
  digitalWrite(LedPortesFermees,HIGH);  //  Les portes sont fermées, pas d'autorisation de sortie
}

void loop()
{
  BoutonPortes.update();

}

void BoutonPortesOnPress(Button& btn)
{
  switch (LastEtat)
  {
    case close:
      CmdPorte(open);
      break;
    case open:
      CmdPorte(close);
      break;
    case unknown:

      break;
  }

}

void CmdPorte(EtatPorte e)
{
  switch (e)
  {
    case open:
      digitalWrite(Lampe, HIGH);
      delay(2000);
      MaServoD.write(PorteD_open, 20, true);
      MaServoG.write(PorteG_open, 20, true);
      break;
    case close:
      MaServoD.write(PorteD_close, 20, true);
      MaServoG.write(PorteG_close, 20, true);
      delay(2000);
      digitalWrite(Lampe, LOW);
      break;
    case unknown:

      break;
  }

}

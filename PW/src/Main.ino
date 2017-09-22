/* SCRIPT Rotation de la Plaque Wagon:
    Principe : La PW est constituée de 4 voies perpendiculaires les unes aux autres, donc en croix.
        Elle est chargée avec 3 wagons, Bois, PQ Rose et PQ Bleu. La 4° est vide
        Le Loco vient récupérer le wagon Bois, le dépose à l'usine de PQ.
        Il ressort de l'usine avec le wagon de PQ de la couleur choisie par le spectateur.
        Il retourne en gare déposer le wagon.

    Initialisation : A l'initialisation, la PW se met en position 1 qui correspond à:
        voie Bois callée par rapport à la voie C, ce sera la référence, le point 0, P1. Ce qui fait qu'il n'y a pas de voie en face de la voie AB.
        (sens de rotation: + correspond au sens horaire)

    Séquence :
        Dès que le loco est arrivé devant la PW, ILS envoie info au Maître. Le Maître envoie ordre de fabrication.
        rotation de de la PW de + 27° pour accrocher le wagon Bois, soit P2. (bien que l'angle soit de 27.5°)
        L'ordre est alors donné au Maître pour le départ du convoi, et la rotation de la PW de - 27° pour réceptionner le wagon Bois sur la voie C.
        Sur la voie C, le loco en arrivant devant la PW, déclanche l'ILS qui donne l'info au Maître pour déclancher la suite des opérations.
        On est donc revenu au point de référence P1.


      Si le choix est PQ Rose:
      Le Maître envoie une information pour la rotation de la PW
          rotation de + 90° pour accrocher le wagon PQ Rose voie C, soit P3. Ordre est donné au maître pour le départ du convoi.
          Le Maître envoie l'info de mise en place de la PW pour réceptionner le wagon sur la voie AB
          rotation de + 27° soit + 117° ( 90° + 27° ) pour réceptionner le wagon PQ Rose soit P4.
          rotation de - 27° pour décrocher le wagon PQ Rose soit P3,
          ordre est donné pour le départ du loco,
          rotation de - 90° pour revenir en position initiale, 0° soit P0.

      Si le choix est PQ Bleu:
          rotation de - 90° pour accrocher le wagon PQ Bleu voie C, soit P5. Ordre est donné au maître pour le départ du convoi.
          rotation de + 27° soit - 63° ( - 90° + 27°) pour réceptionner le wagon sur la voie AB soit P6.
          rotation de - 27° pour décrocher le wagon soit P5,
          ordre est donné pour le départ du loco
          rotation de + 90° pour revenir en position initiale 0° soit P0.


    Les positions angulaires:
        Position 1 = 0°, référence, wagon Bois sur voie C
        Position 2 = +27°, wagon Bois sur voie AB
        Position 3 = +90°, wagon PQ Rose sur voie C
        Position 4 = +117°, wagon PQ Rose sur voie AB
        Position 5 = -90°, wagon PQ Bleu sur voie C
        Position 6 = -63°, wagon PQ Bleu sur voie AB
        Les angles sont ici théoriques, en fait, il convient de réajuster en fonction de chaque servo. C'est le pourquoi de la différence dans le programme.

     Les Servos:
        Les servos sont limitées en débattement à 120°, ce qui est insuffisant pour réaliser la rotation totale de 207°
        Le principe est de relier par leurs structures 2 servos têtes bêches, fixées sur la plaque de base pour l'une et sur la pW pour l'autre.
        L'une réalise le débattement pour le PQ Rose, l'autre pour le PQ Bleu. Il convient entre les 2 débattements Rose et Bleu, de les ramener
        au point de référence afin d'obtenir l'angle souhaité dans le programme. L'une tourne dans le sens horaire, l'autre dans le sens anti-horaire.

*/

#include <VarSpeedServo.h>
#include <Button.h>
#include <ButtonEventCallback.h>
#include <PushButton.h>
#include <Bounce2.h>

#include <Blinker.h>

VarSpeedServo SR; //servo Rose
VarSpeedServo SB; //servo bleu

//------ BP choix couleur ------//

PushButton Bp_AB_1 = PushButton (A0);  //Bp choix voie AB_1 Bois
PushButton Bp_AB_2 = PushButton (A1);  //Bp choix voie AB_2 Rose
PushButton Bp_AB_4 = PushButton (A2);  //Bp choix voie AB_4 Bleu

PushButton Bp_C_1 = PushButton (A3);
PushButton Bp_C_2 = PushButton (A4);
PushButton Bp_C_4 = PushButton (A5);

//------ Led ------//
Blinker BkAB_1(2);
Blinker BkAB_2(3);
Blinker BkAB_4(4);

Blinker BkC_1(5);
Blinker BkC_2(6);
Blinker BkC_4(7);

//------ Boolean ------//
boolean Cmd = false;


// Voie AB-1
int AB_R1 = 0;
int AB_B1 = 0;

// Voie AB-2
int AB_R2 = -90;
int AB_B2 = 0;

// Voie AB-4
int AB_R4 = 0;
int AB_B4 = 90;

// Voie C-1
int C_R1 = 0;
int C_B1 = 30;

// Voie C-2
int C_R2 = -80;
int C_B2 = 30;

// Voie C-4
int C_R4 = 80;
int C_B4 = 0;

enum Pos {
  AB_1,
  AB_2,
  AB_4,
  C_1,
  C_2,
  C_4
};

int val = 0;
int val2 = 0;
int val3 = 0;

int a(int val)
{

  if ( val >= 0)
  {
    return val + 90;
  }
  else
  {
    val2 = val - 90 ;
    val3 = - val2;
    return val3;
  }
}

void Led (Pos pos)
{
  BkAB_1.off();
  BkAB_2.off();
  BkAB_4.off();
  BkC_1.off();
  BkC_2.off();
  BkC_4.off();

  switch (pos)
  {
    case AB_1:
      BkAB_1.on();
      break;
    case AB_2:
      BkAB_2.on();
      break;
    case AB_4:
      BkAB_4.on();
      break;
    case C_1:
      BkC_1.on();
      break;
    case C_2:
      BkC_2.on();
      break;
    case C_4:
      BkC_4.on();
      break;
  }
}

void PosW (Pos pos)
{
  BkAB_1.off();
  BkAB_2.off();
  BkAB_4.off();
  BkC_1.off();
  BkC_2.off();
  BkC_4.off();
  Cmd = true;
  switch (pos)
  {
    case AB_1:
      SR.write(a(AB_R1), 50);
      SB.write(a(AB_B1), 50);

      BkAB_1.blink();
      break;
    case AB_2:
      SR.write(a(AB_R2), 50);
      SB.write(a(AB_B2), 50);

      BkAB_2.blink();
      break;
    case AB_4:
      SR.write(a(AB_R4), 50);
      SB.write(a(AB_B4), 50);

      BkAB_4.blink();
      break;
    case C_1:
      SR.write(a(C_R1), 50);
      SB.write(a(C_B1), 50);

      BkC_1.blink();
      break;
    case C_2:
      SR.write(a(C_R2), 50);
      SB.write(a(C_B2), 50);

      BkC_2.blink();
      break;
    case C_4:
      SR.write(0, 50);
      SB.write(110, 50);
      //SR.write(a(C_R4), 50);
      //SB.write(a(C_B4), 50);

      BkC_4.blink();
      break;
  }
  SR.wait();

  Cmd = false;
  Led(pos);
}

void setup()
{

  //Initialisation Broche Servo
  SR.attach(10, 560, 2400);
  //SB.attach(10);

  // Extinction Led position

  BkAB_1.start();
  BkAB_2.start();
  BkAB_4.start();
  BkC_1.start();
  BkC_2.start();
  BkC_4.start();

  BkAB_1.off();
  BkAB_2.off();
  BkAB_4.off();
  BkC_1.off();
  BkC_2.off();
  BkC_4.off();

  //Initialisation Bp choix voie
  Bp_AB_1.onPress(AB_1Pressed);
  Bp_AB_2.onPress(AB_2Pressed);
  Bp_AB_4.onPress(AB_4Pressed);

  Bp_C_1.onPress(C_1Pressed);
  Bp_C_2.onPress(C_2Pressed);
  Bp_C_4.onPress(C_4Pressed);


  PosW(AB_1);

  Cmd = false;

  /*PosW(AB_1);
  delay(600);
  PosW(AB_2);
  delay(600);
  PosW(AB_4);
  delay(600);
  PosW(C_1);*/
}

void loop()
{
  if (!Cmd)
  {

    Bp_AB_1.update();
    Bp_AB_2.update();
    Bp_AB_4.update();

    Bp_C_1.update();
    Bp_C_2.update();
    Bp_C_4.update();

  }

}

void AB_1Pressed(Button& btn)
{
  PosW(AB_1);
}

void AB_2Pressed(Button& btn)
{
  PosW(AB_2);
}

void AB_4Pressed(Button& btn)
{
  PosW(AB_4);
}

void C_1Pressed(Button& btn)
{
  PosW(C_1);
}

void C_2Pressed(Button& btn)
{
  PosW(C_2);
}

void C_4Pressed(Button& btn)
{
  PosW(C_4);
}

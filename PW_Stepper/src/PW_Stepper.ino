/* SCRIPT Rotation de la Plaque Wagon:
Principe : La PW est constituée de 4 voies perpendiculaires les unes aux autres, donc en croix.
Elle est chargée avec 3 wagons, Bois(1), PQ Rose(2) et PQ Bleu(4). La 3° est vide
Le Loco vient récupérer le wagon Bois, le dépose à l'usine de PQ.
Il ressort de l'usine avec le wagon de PQ de la couleur choisie par le spectateur.
Il retourne en gare déposer le wagon.
*/

#include <Button.h>
#include <ButtonEventCallback.h>
#include <PushButton.h>
#include <Bounce2.h>

#include <Blinker.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

//------ BP choix couleur ------//
// 2 0 1 5 3 4
PushButton Bp_AB_1 = PushButton (A2);  //Bp choix voie AB_1 Bois
PushButton Bp_AB_2 = PushButton (A0);  //Bp choix voie AB_2 Rose
PushButton Bp_AB_4 = PushButton (A1);  //Bp choix voie AB_4 Bleu

PushButton Bp_C_1 = PushButton (A5);  // C_1 B
PushButton Bp_C_2 = PushButton (A3);  // C_2 R
PushButton Bp_C_4 = PushButton (A4); // C_4  B

PushButton Bp_Z = PushButton (10); //BP init Zero
PushButton BpP = PushButton(4); //Bp Deplacement -
PushButton BpM = PushButton(5); //Bp Deplacement +

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

//----------- Led -----------//
//Blinker BkAB_1(11);
//Blinker BkAB_2(12);
//Blinker BkAB_4(13);

//Blinker BkC_1(7);
//Blinker BkC_2(8);
//Blinker BkC_4(9);

//-------- Boolean --------//
boolean Cmd = false;

//------ Pos plaque ------//

// Voie AB-1
int AB_R1 = 47; //Bois

// Voie AB-2
int AB_R2 = 197; //Rose

// Voie AB-4
int AB_R4 = 498; //BLeu

// Voie C- 1
int C_R1 = 0; //Bois

// Voie C-2
int C_R2 = 150; //Rose

// Voie C-4
int C_R4 = 450; //Bleu

//------ Retour Com ------//

//int Rm = ;

int pos = 0;

int chemin;
bool dir;

enum Pos
{
  AB_1,
  AB_2,
  AB_4,
  C_1,
  C_2,
  C_4
};

Pos oldPos = C_1;

int posCour = 0;

bool CheckZ = false;

void Bp(int s)
{
  //Serial.println("BP: ");
  //Serial.print(s);
  digitalWrite(s, HIGH);
  delay(600);
  digitalWrite(s, LOW);
  //delay(4000);
  //Serial.println("Test");
}

/*void Led (Pos pos)
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
}*/

void PosW (Pos pos)
{
  /*BkAB_1.off();
  BkAB_2.off();
  BkAB_4.off();
  BkC_1.off();
  BkC_2.off();
  BkC_4.off();*/
  Cmd = true;

  switch (pos)
  {
    case AB_1:
    //SR.write(AB_R1, speed, true);
    //myMotor->step(AB_R1,FORWARD,MICROSTEP);
    GotoPos(AB_R1);

    //BkAB_1.blink();
    break;
    case AB_2:
    //SR.write(AB_R2, speed, true);
    GotoPos(AB_R2);

    //BkAB_2.blink();
    break;
    case AB_4:
    //SR.write(AB_R4, speed, true);
    GotoPos(AB_R4);

    //BkAB_4.blink();
    break;
    case C_1:
    //SR.write(C_R1, speed, true);
    GotoPos(C_R1);

    //BkC_1.blink();
    break;
    case C_2:
    //SR.write(C_R2, speed, true);
    GotoPos(C_R2);

    //BkC_2.blink();
    break;
    case C_4:
    //SR.write(C_R4, speed, true);
    GotoPos(C_R4);

    //BkC_4.blink();
    break;
  }
oldPos = pos;
  Cmd = false;
  //Led(pos);
}

void GotoPos(int posDest)
{
  if (posDest >= 0 && posDest < 600)
  {
    int chemin1 = posDest - posCour;
    int chemin2;
    if (posDest > posCour)
    chemin2 = - (posCour + 600 - posDest);
    else
    chemin2 = 600 - posCour + posDest;

    int chemin;
    if (abs(chemin1) > abs(chemin2))
    chemin = chemin2;
    else
    chemin = chemin1;

    /* Si chemin < 0, la direction est mise à 1 */
    if (chemin < 0)
    dir = true;
    else
    dir = false;

    if (chemin != 0)
    {

      int distance = abs(chemin);

      if (dir)
      {

        myMotor->step(distance, FORWARD, MICROSTEP);
      } else
      {

        myMotor->step(distance, BACKWARD, MICROSTEP);
      }
    }


  }

  /* Mise à jour de la position */
  posCour = posDest;
}

void CheckZero()
{
  while(!CheckZ)
  {
    int pos = 0;
    //myMotor->step(1, FORWARD, DOUBLE);
    pos++;
    Serial.print(pos);
    Bp_Z.update();
    //Bp0.update();
  }
}

void setup()
{

  Serial.begin(115200);

  AFMS.begin();

  //myMotor->setSpeed(10);

  // Extinction Led position
  /*BkAB_1.start();
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
  BkC_4.off();*/

  //Initialisation Bp choix voie
  Bp_AB_1.onPress(AB_1Pressed);
  Bp_AB_2.onPress(AB_2Pressed);
  Bp_AB_4.onPress(AB_4Pressed);

  Bp_C_1.onPress(C_1Pressed);
  Bp_C_2.onPress(C_2Pressed);
  Bp_C_4.onPress(C_4Pressed);

  //Bp_Z.onPress(Z_Pressed);

  BpP.onPress(BpP_Pressed);
  BpM.onPress(bpM_Pressed);

  PosW(C_1);

  Cmd = false;

  /*PosW(C_1);
  delay(600);
  PosW(C_2);
  delay(600);
  PosW(C_4);*/

  //CheckZero();
}

void loop()
{
  //Bp_Z.update();
  /*PosW(AB_1);
  delay(600);
  PosW(AB_2);
  delay(600);
  PosW(AB_4);
  delay(600);
  PosW(C_1);
  delay(600);
  PosW(C_2);
  delay(600);
  PosW(C_4);
  delay(600);*/
  if (!Cmd)
  {

    Bp_AB_1.update();
    Bp_AB_2.update();
    Bp_AB_4.update();

    Bp_C_1.update();
    Bp_C_2.update();
    Bp_C_4.update();

    BpP.update();
    BpM.update();

    //Serial.println(pos);
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

void Z_Pressed(Button& btn)
{

  CheckZ = true;
  //Serial.println("Bp");
  //CheckZ = true;
}

void BpP_Pressed(Button& btn)
{
  Serial.println("Bp Plus: ");
  Serial.print(pos);
  myMotor->step(1, FORWARD, MICROSTEP);
  pos++;
}

void bpM_Pressed(Button& btn)
{
  Serial.println("Bp Moins: ");
  Serial.print(pos);
  myMotor->step(1,BACKWARD, MICROSTEP);
  pos--;
}

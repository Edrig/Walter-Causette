#include <Button.h>
#include <ButtonEventCallback.h>
#include <PushButton.h>
#include <Bounce2.h>

#include <Blinker.h>

//------ BP choix couleur ------//

PushButton Bp_R = PushButton(A0);  //Bp choix rose
PushButton Bp_B = PushButton(A1);   //Bp choix bleu

//------ Inverseur mode ------//
#define IntF_R //Inter force mode rose
#define IntF_B //Inter force mode bleu

//------ Ils ------//

PushButton Ils_R = PushButton(A2);  //loco stop remise
PushButton Ils_P = PushButton(A3); //loco stop pont-secteur
PushButton Ils_AB = PushButton(A4); //loco stop voie AB plaque
PushButton Ils_C = PushButton(A5); //loco stop voie C plaque

//------ Contact ------//

// ON/OFF
int Led = A0;
int Lamp = A1;

//BruitL = ;
//BruitU = ;

// Moment
int porteR = 4;
int porteU = 5;

int barriere = 6;

int cmdAigA = 7;
int cmdAigB = 8;

int cmdSectB = 9;
int cmdSectC = 10;

int AlimVL = 11;
int AlimVR = 12;

//------ Actionneur ------//


//------ Led ------//
Blinker BkR(2); // clignotement Led Rose
Blinker BkB(3); // clignotement Led Bleu

//------ Boolean ------//
boolean Cmd = false; // Choix couleur
boolean MdR = false; // Mode Rose
boolean MdB = false; // Mode Bleu

boolean Alv = false; // ALim Voie

//----- Memoire -----//

int t = 0;

enum AlimVoie {
  left,
  zero,
  right
};


void setup()
{
  Serial.begin(115200);

  Bp_R.onPress(onBp_RPressed);
  Bp_B.onPress(onBp_BPressed);

  Ils_R.onPress(onIls_R_Pressed);
  Ils_P.onPress(onIls_P_Pressed);
  Ils_AB.onPress(onIls_AB_Pressed);
  Ils_C.onPress(onIls_C_Pressed);

  BkR.setDelay(250);
  BkR.start();

  BkB.setDelay(250);
  BkB.start();

  pinMode(Led, OUTPUT);
  digitalWrite(Led, HIGH);
  //Pt Voie C
  //Aig Voie A
  //Ps voie C

  pinMode(AlimVL, OUTPUT);
  pinMode(AlimVR, OUTPUT);

  AlimV(zero);

  //digitalWrite(AlimVL, HIGH);
  t = 0;

}

void loop()
{
  Bp_R.update();
  Bp_B.update();

  Ils_R.update();
  Ils_P.update();
  Ils_AB.update();
  Ils_C.update();

  BkR.blink();
  BkB.blink();

  //if()
  switch (t)
  {
    case 1:
      Serial.println("case 1");
      //Led fade out

      delay(2000);
      t++;
      break;
    case 2:
      Serial.println("case 2");
      digitalWrite(Lamp, HIGH);
      t++;
      break;
    case 3:
      Serial.println("case 3");
      //BMoteur Ralentie
      Bp(porteR);
      delay(1000);
      t++;
      break;
    case 4:
    Serial.println("case 4");
      //BMoteur Demarrage
      AlimV(right);
      break;
    case 5:
    Serial.println("case 5");
      //Bmoteur Ralentie
      AlimV(zero);
      delay(2000);
      t++;
      break;
    case 6:
    Serial.println("case 6");
      //Led Fade in
      //Pt C+25°
      t++;
      break;
    case 7:
    Serial.println("case 7");
      Bp(barriere);
      delay(2000);
      t++;
      break;
    case 8:
    Serial.println("case 8");
      Bp(cmdAigB);
      delay(2000);
      t++;
      break;
    case 9:
    Serial.println("case 9");
      //Attente position Pt
      //BMoteur Demarrage
      AlimV(left);
      break;
    case 10:
    Serial.println("case 10");
      //Bmoteur Ralentie
      AlimV(zero);
      t++;
      delay(2000);
      break;
    case 11:
    Serial.println("case 11");
      //Pt AB-25°
      t++;
      break;
    case 12:
    Serial.println("case 12");
      Bp(porteU);
      break;
    case 13:
    Serial.println("case 13");
      //Attente position Pt
      //BMoteur Demarrage
      break;
    case 14:
    Serial.println("case 14");

      break;
    case 15:
    Serial.println("case 15");

      break;
    case 16:
    Serial.println("case 16");

      break;
    case 17:
    Serial.println("case 17");

      break;
    case 18:
    Serial.println("case 18");

      break;
    case 19:
    Serial.println("case 19");

      break;
    case 20:
    Serial.println("case 20");

      break;
    case 21:
    Serial.println("case 21");

      break;
    case 22:
    Serial.println("case 22");

      break;
    case 23:
    Serial.println("case 23");

      break;
    case 24:
    Serial.println("case 24");

      break;
    case 25:
    Serial.println("case 25");

      break;
    case 26:
    Serial.println("case 26");

      break;
    case 27:
    Serial.println("case 27");

      break;
    case 28:
    Serial.println("case 28");

      break;
    case 29:
    Serial.println("case 29");

      break;
    case 30:
    Serial.println("case 30");

      break;
  }
}

void onBp_RPressed(Button& btn)
{
  MdR = true;

  if (!Cmd)
  {
    Cmd = true;

    Serial.println("Choix PQ Rose");
    BkR.stop();
    BkB.stop();

    BkR.on();

    t = 1;
  }

}

void onBp_BPressed(Button& btn)
{
  MdB = true;

  if (!Cmd)
  {
    Cmd = true;

    Serial.println("Choix PQ Bleu");
    BkR.stop();
    BkB.stop();

    BkB.on();

    t = 1;
  }
}


void onIls_R_Pressed(Button& btn)
{
  Serial.println("Ils loco-Remise");
}

void onIls_P_Pressed(Button& btn)
{
  Serial.println("Ils pont-secteur");
  switch (t)
  {
    case 9:
      t++;
      break;
  }
}


void onIls_AB_Pressed(Button& btn)
{
  Serial.println("Ils loco-AB");
  switch (t)
  {
    case 4:
      t++;
      break;
  }
}

void onIls_C_Pressed(Button& btn)
{
  Serial.println("Ils loco-C");
}

void Bp(int s)
{
  Serial.println("BP: "+ s);
  digitalWrite(s, HIGH);
  delay(60);
  digitalWrite(s, LOW);
}

void AlimV(AlimVoie v)
{
  switch (v)
  {
    case left:
      Serial.println("AlimVoie: Left");
      digitalWrite(AlimVL, HIGH);
      digitalWrite(AlimVR, LOW);
      break;
    case zero:
      Serial.println("AlimVoie: Zero");
      digitalWrite(AlimVL, LOW);
      digitalWrite(AlimVR, LOW);
      break;
    case right:
      Serial.println("AlimVoie: Right");
      digitalWrite(AlimVL, LOW);
      digitalWrite(AlimVR, HIGH);
      break;
  }

}

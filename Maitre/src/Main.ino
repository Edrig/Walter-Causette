
#include <Button.h>
#include <ButtonEventCallback.h>
#include <PushButton.h>
#include <Bounce2.h>

#include <Blinker.h>

//------ BP choix couleur ------//

PushButton Bp_R = PushButton(50);  //Bp choix rose
PushButton Bp_B = PushButton(51);   //Bp choix bleu

//------ Inverseur mode ------//
#define IntF_R //Inter force mode rose
#define IntF_B //Inter force mode bleu

//------ Ils ------//

PushButton Ils_P = PushButton(22);  //loco stop secteur
PushButton Ils_R = PushButton(23); //loco stop remise
PushButton Ils_AB = PushButton(24); //loco stop voie AB plaque
PushButton Ils_C = PushButton(25); //loco stop voie C plaque

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
int cmdAigB = 8 ;

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
  AV,
  zero,
  AR
};


void setup()
{
  Serial.begin(115200);
  Serial.print("Walter&Causette Debug\n");

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

  pinMode(cmdAigA, OUTPUT);
  pinMode(cmdAigB, OUTPUT);

  pinMode(cmdSectB, OUTPUT);
  pinMode(cmdSectC, OUTPUT);

  pinMode(porteR, OUTPUT);
  pinMode(porteU, OUTPUT);

  AlimV(zero);
  //delay(2000);
  //Bp(cmdAigA);
  //delay(2000);
  //Bp(cmdSectB);
  //delay(2000);
  //Bp(cmdAigA);

  MdR = false;
  t = 0;
  //Bp(cmdAigB);
  //digitalWrite(cmdAigA, HIGH);
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
      AlimV(AR);
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
      AlimV(AV);
      break;
    case 10:
    Serial.println("case 10");
      //Bmoteur Ralentie
      AlimV(zero);
      delay(2000);
      t++;
      break;
    case 11:
    Serial.println("case 11");
      //Pt AB-25°
      delay(6000);
      t++;
      break;
    case 12:
    Serial.println("case 12");
      Bp(porteU);
      delay(2000);
      t++;
      break;
    case 13:
    Serial.println("case 13");
      //Attente position Pt
      //BMoteur Demarrage
      Bp(cmdSectC);
      delay(5000);
      AlimV(AR);
      t++;
      break;
    case 14:
    Serial.println("Case 14");

      break;
    case 15:
    Serial.println("case 15");
      AlimV(zero);
      t++;
      break;
    case 16:
    Serial.println("case 16");
      //BMoteur arret
      Bp(porteU);
      //attente porteU ferme
      delay(2000);
      t++;
      break;
    case 17:
    Serial.println("case 17");
      Bp(barriere);
      delay(2000);

      if(MdR)
      {
          //PW C_Rose
      }
      else if(MdB)
      {
          //PW C_Bleu
      }
      delay(2000);
      t++;
      break;
    case 18:
    Serial.println("case 18");
      Bp(porteU);
      delay(2000);
      t++;
      break;
    case 19:
    Serial.println("case 19");
      Bp(barriere);
      delay(2000);
      t++;
      break;
    case 20:
    Serial.println("case 20");
      // Bmoteur Ralentie
      t++;
      break;
    case 21:
    Serial.println("case 21");
      //Bmoteur Demarrage
      AlimVoie(AV);
      break;
    case 22:
    Serial.println("case 22");
      AlimVoie(zero);
      Bp(porteU);
      delay(2000);
      Bp(cmdSectB);
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
    case 31:
    Serial.println("case 31");

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
    case 20:
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
  switch (t)
  {
    case 14:
      t++;
      break;
  }
}

void Bp(int s)
{
  Serial.println("BP: "+ s);
  digitalWrite(s, HIGH);
  delay(600);
  digitalWrite(s, LOW);
}

void AlimV(AlimVoie v)
{
  switch (v)
  {
    case AV:
      Serial.println("AlimVoie: AV");
      digitalWrite(AlimVL, HIGH);
      digitalWrite(AlimVR, LOW);
      break;
    case zero:
      Serial.println("AlimVoie: Zero");
      digitalWrite(AlimVL, LOW);
      digitalWrite(AlimVR, LOW);
      break;
    case AR:
      Serial.println("AlimVoie: AR");
      digitalWrite(AlimVL, LOW);
      digitalWrite(AlimVR, HIGH);
      break;
  }

}

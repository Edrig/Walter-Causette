//#include <Button.h>
//#include <ButtonEventCallback.h>
//#include <PushButton.h>
//#include <Bounce2.h>

#include <Blinker.h>
#include "OneButton.h"

//------ BP choix couleur ------//

OneButton Bp_R(A1, true);
OneButton Bp_B(A2, true);
//PushButton Bp_R = PushButton(2);  //Bp choix rose
//PushButton Bp_B = PushButton(3);   //Bp choix bleu

//------ Inverseur mode ------//
#define IntF_R //Inter force mode rose
#define IntF_B //Inter force mode bleu

//------ Ils ------//
#define Ils_R  //loco stop remise
#define Ils_P  //loco stop pont-secteur
#define Ils_AB //loco stop voie AB plaque
#define Ils_C  //loco stop voie C plaque

//------ Contact ------//

#define Cto_R // porte remise open
#define Ctc_R // porte remise close

#define Cto_U // porte usine open
#define Ctc_U // porte usine close

#define Cto_B // barriere cour open
#define Ctc_B // barriere cour close

const int Aig_A = 8; // aiguille voie A
const int Aig_B = 9; // aiguille voie B

const int Pts_B = 10; // secteur voie B
const int Pts_C = 11;// secteur voie C
const int Pts_D = 12;// secteur voie D

//------ Actionneur ------//


//------ Led ------//
Blinker BkR(5); // clignotement Led Rose
Blinker BkB(6); // clignotement Led Bleu

//------ Boolean ------//
boolean Cmd = false; // Choix couleur
boolean MdR = false; // Mode Rose
boolean MdB = false; // Mode Bleu

//----- Memoire -----/
int LastStep = 0;
char LastPos_Aig = 'A';
char LastPos_Sect = 'C';


enum posAig {
  voieA,
  voieB
};

enum posSect {
  voieBs,
  voieCs,
  voieDs
};

void setup()
{
  Serial.begin(115200);

  //Bp_R.onPress(onBp_RPressed);
  //Bp_B.onPress(onBp_BPressed);
  Bp_R.attachClick(onBp_R_Pressed)
  Bp_B.attachClick(onBp_B_Pressed)

  BkR.setDelay(250);
  BkR.start();

  BkB.setDelay(250);
  BkB.start();

  pinMode(Aig_A, OUTPUT);
  pinMode(Aig_B, OUTPUT);

}

void loop()
{
  Bp_R.tick();
  Bp_B.tick()();

  BkR.blink();
  BkB.blink();
}

void onBp_R_Click()
{
  MdR = true;

  if (!Cmd)
  {
    Cmd = true;

    Serial.println("Choix PQ Rose");
    BkR.stop();
    BkB.stop();

    BkR.on();
  }

  /*if (CmdAiguille(voieA));
  {
  Serial.println("Manoeuvre Aig voieA Reusie");
}*/
}

void onBp_B_Click()
{
  MdB = true;

  if (!Cmd)
  {
    Cmd = true;

    Serial.println("Choix PQ Bleu");
    BkR.stop();
    BkB.stop();

    BkB.on();
  }

  /*if (CmdAiguille(voieB));
  {
  Serial.println("Manoeuvre Aig VoieB Reusie");
}*/
}

bool CmdAiguille(posAig ps)
{
  switch (ps)
  {
    case voieA:

    if (digitalRead(Aig_A) == HIGH)
    {
      LastPos_Aig == 'A';
      Serial.println("position Aig Voie: " + posAig);
      return true;
    }
    else
    {
      while (digitalRead(Aig_A) == LOW)
      {
        Serial.println("aiguille => Voie A");
      }
      return true;
    }
    return false;
    break;

    case voieB:

    if (digitalRead(Aig_B) == HIGH)
    {
      LastPos_Aig == 'B';
      Serial.println("position Aig Voie: " + posAig);
      return true;
    }
    else
    {
      while (digitalRead(Aig_B) == LOW)
      {
        Serial.println("aiguille => Voie B");
      }
      return true;
    }
    return false;

    break;
  }
}

bool CmdSect(posSect ps)
{

  switch ( ps )
  {

    case voieBs:

      if (digitalRead(Pts_B) == HIGH)
      {
        Serial.println("position Sect Voie Bs");

      }
      else
      {

      }
      break;

    case voieCs:
      if (digitalRead(Pts_C) == HIGH)
      {
        Serial.println("position Sect Voie Cs");
      }
      else
      {

      }
      break;

    case voieDs:
      if (digitalRead(Pts_D) == HIGH)
      {
        Serial.println("position Sect Voie Ds");
      }
      break;
}

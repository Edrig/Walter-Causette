#include <Button.h>
#include <ButtonEventCallback.h>
#include <PushButton.h>
#include <Bounce2.h>

#include <Blinker.h>

//------ BP choix couleur ------//

PushButton Bp_R = PushButton(2);  //Bp choix rose
PushButton Bp_B = PushButton(3);   //Bp choix bleu

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

#define Pts_B // secteur voie B
#define Pts_C // secteur voie C
#define Pts_D // secteur voie D

//------ Actionneur ------//


//------ Led ------//
Blinker BkR(5); // clignotement Led Rose
Blinker BkB(6); // clignotement Led Bleu

//------ Boolean ------//
boolean Cmd = false; // Choix couleur
boolean MdR = false; // Mode Rose
boolean MdB = false; // Mode Bleu

enum posAig {
  voieA,
  voieB
};

void setup()
{
  Serial.begin(115200);

  Bp_R.onPress(onBp_RPressed);
  Bp_B.onPress(onBp_BPressed);

  BkR.setDelay(250);
  BkR.start();

  BkB.setDelay(250);
  BkB.start();

  pinMode(Aig_A, OUTPUT);
  pinMode(Aig_B, OUTPUT);

}

void loop()
{
    Bp_R.update();
    Bp_B.update();

    /*BkR.blink();
    BkB.blink();*/
}

void onBp_RPressed(Button& btn)
{
  /*MdR = true;

  if (!Cmd)
  {
    Cmd = true;

    Serial.println("Choix PQ Rose");
    BkR.stop();
    BkB.stop();

    BkR.on();
  }*/
  if (CmdAiguille(voieA));
  {
    Serial.println("Manoeuvre Aig Reusie");
  }
}

void onBp_BPressed(Button& btn)
{
  /*MdB = true;

  if (!Cmd)
  {
    Cmd = true;

    Serial.println("Choix PQ Bleu");
    BkR.stop();
    BkB.stop();

    BkB.on();
  }*/
  if (CmdAiguille(voieB));
  {
    Serial.println("Manoeuvre Aig Reusie");
  }
}

bool CmdAiguille(posAig ps)
{
  if (ps==voieA)
  {
    if (digitalRead(Aig_A) == HIGH)
    {
      Serial.println("position Voie A");
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
  }
  else if (ps==voieB)
  {
    if (digitalRead(Aig_B) == HIGH)
    {
      Serial.println("position Voie B");
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
  }
}

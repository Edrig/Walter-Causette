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

#define Aig_A // aiguille voie A
#define Aig_B // aiguille voie B

#define Pts_B // secteur voie B
#define Pts_C // secteur voie C
#define Pts_D // secteur voie D

//------ Actionneur ------//


//------ Led ------//


//------ Boolean ------//
boolean MdR = false; // Mode Rose
boolean MdB = false; // Mode Bleu

Blinker BkR(5);
Blinker BkB(6);

void setup()
{
  Serial.begin(115200);

  Bp_R.onPress(onBp_RPressed);
  Bp_B.onPress(onBp_BPressed);

  BkR.setDelay(250);
  BkR.start();

  BkB.setDelay(250);
  BkB.start();

}

void loop()
{
    Bp_R.update();
    Bp_B.update();

    BkR.blink();
    BkB.blink();
}

void onBp_RPressed(Button& btn)
{
  MdR = true;

  if (!MdR || !MdB)
  {
  	Serial.println("Bp_R pressed");
    BkR.stop();
    BkB.stop();

    digitalWrite(5, HIGH);
  }
}

void onBp_BPressed(Button& btn)
{
  MdB = true;

  if (!MdR || !MdB)
  {
  Serial.println("Bp_B pressed");
  BkR.stop();
  BkB.stop();

  digitalWrite(6, HIGH);
}

}

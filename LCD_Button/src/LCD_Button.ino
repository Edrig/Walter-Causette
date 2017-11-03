
#include <LiquidCrystal.h>

#include <Button.h>
#include <ButtonEventCallback.h>
#include <PushButton.h>
#include <Bounce2.h>

#include <Blinker.h>

//Afficheur

  LiquidCrystal Lcd(12,11,5,4,3,2);

  //------ Actionneur ------//

    PushButton BpR = PushButton(7);
    PushButton BpB = PushButton(8);

  //------ Led ------//

  Blinker BkR(13); // clignotement Led Rose
  Blinker BkB(A2); // clignotement Led Bleu

  //------ Boolean ------//
  boolean Cmd = false; // Choix couleur
  boolean MdR = false; // Mode Rose
  boolean MdB = false; // Mode Bleu

  void BpR_Pressed(Button& btn)
  {
    Serial.println("Bp Rose");
    BkR.stop();
    BkR.on();

  }

  void BpB_Pressed(Button& btn)
  {
    Serial.println("Bp Bleu");
    BkB.stop();
    BkB.on();
  }

  void setup()
  {
    Serial.begin(115200);

    BpR.onPress(BpR_Pressed);
    BpB.onPress(BpB_Pressed);

    BkR.start();
    BkB.start();
  }

  void loop()
  {
      BkR.blink();
      BkB.blink();

      /*if(!Cmd)
      {
        BpR.update();
        BpB.update();
      }*/
  }

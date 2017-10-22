# 1 "c:\\users\\cube\\appdata\\local\\temp\\tmpsqmppp"
#include <Arduino.h>
# 1 "C:/Users/cube/Desktop/Walter-Causette/Maitre/src/Main.ino"


#include <Button.h>

#include <ButtonEventCallback.h>

#include <PushButton.h>

#include <Bounce2.h>



#include <Blinker.h>







PushButton Bp_R = PushButton(50);

PushButton Bp_B = PushButton(51);





#define IntF_R 

#define IntF_B 







PushButton Ils_P = PushButton(22);

PushButton Ils_R = PushButton(23);

PushButton Ils_AB = PushButton(24);

PushButton Ils_C = PushButton(25);
# 55 "C:/Users/cube/Desktop/Walter-Causette/Maitre/src/Main.ino"
int Led = A0;

int Lamp = A1;
# 71 "C:/Users/cube/Desktop/Walter-Causette/Maitre/src/Main.ino"
int porteR = 4;

int porteU = 5;



int barriere = 6;



int cmdAigA = 7;

int cmdAigB = 8 ;



int cmdSectB = 9;

int cmdSectC = 10;



int AlimVL = 11;

int AlimVR = 12;
# 105 "C:/Users/cube/Desktop/Walter-Causette/Maitre/src/Main.ino"
Blinker BkR(2);

Blinker BkB(3);





boolean Cmd = false;

boolean MdR = false;

boolean MdB = false;



boolean Alv = false;







int t = 0;



enum AlimVoie {

  AV,

  zero,

  AR

};
void setup();
void loop();
void onBp_RPressed(Button& btn);
void onBp_BPressed(Button& btn);
void onIls_R_Pressed(Button& btn);
void onIls_P_Pressed(Button& btn);
void onIls_AB_Pressed(Button& btn);
void onIls_C_Pressed(Button& btn);
void Bp(int s);
void AlimV(AlimVoie v);
#line 147 "C:/Users/cube/Desktop/Walter-Causette/Maitre/src/Main.ino"
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
# 195 "C:/Users/cube/Desktop/Walter-Causette/Maitre/src/Main.ino"
  pinMode(AlimVL, OUTPUT);

  pinMode(AlimVR, OUTPUT);



  pinMode(cmdAigA, OUTPUT);

  pinMode(cmdAigB, OUTPUT);



  pinMode(cmdSectB, OUTPUT);

  pinMode(cmdSectC, OUTPUT);



  pinMode(porteR, OUTPUT);

  pinMode(porteU, OUTPUT);



  AlimV(zero);
# 235 "C:/Users/cube/Desktop/Walter-Causette/Maitre/src/Main.ino"
  MdR = false;

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





  switch (t)

  {

    case 1:

      Serial.println("case 1");





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



      Bp(porteR);

      delay(1000);

      t++;

      break;

    case 4:

    Serial.println("case 4");



      AlimV(AR);

      break;

    case 5:

    Serial.println("case 5");



      AlimV(zero);

      delay(2000);

      t++;

      break;

    case 6:

    Serial.println("case 6");





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





      AlimV(AV);

      break;

    case 10:

    Serial.println("case 10");



      AlimV(zero);

      delay(2000);

      t++;

      break;

    case 11:

    Serial.println("case 11");



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



      Bp(porteU);



      delay(2000);

      t++;

      break;

    case 17:

    Serial.println("case 17");

      Bp(barriere);

      delay(2000);



      if(MdR)

      {



      }

      else if(MdB)

      {



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



      t++;

      break;

    case 21:

    Serial.println("case 21");



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
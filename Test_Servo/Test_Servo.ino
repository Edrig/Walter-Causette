#include <Servo.h>

int temps = 1500; //censé être à mi-chemin entre 1000 et 2000, un bon point de départ

Servo monServo;

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello World");
    
    monServo.attach(10);
    //on démarre à une valeur censé être la moitié de
    //l'excursion totale de l'angle réalisé par le servomoteur
    monServo.writeMicroseconds(temps);
}

void loop()
{
    //des données sur la liaison série ? (lorsque l'on appuie sur 'a' ou 'd')
    if(Serial.available())
    {
        char commande = Serial.read(); //on lit
        
        //on modifie la consigne si c'est un caractère qui nous intéresse
        if(commande == 'a')
            temps += 10;  //ajout de 10µs au temps HAUT
        else if(commande == 'd')
            temps -= 10;  //retrait de 10µs au temps HAUT
        
        //on modifie la consigne du servo
        monServo.writeMicroseconds(temps);
        monServo.detach();        
        //et on fait un retour sur la console pour savoir où on est rendu
        Serial.println(temps, DEC);
    }
}

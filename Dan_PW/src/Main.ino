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

//DEFINITIONS
  // SERVO
      #include <Servo.h>
      Servo MaServoPwRose; //Donne un nom à la servo basse pour communiquer
      Servo MaServoPwBleu; //Donne un nom à la servo haute pour communiquer


  // COMMANDES
      int BrocheCommandeProgramme = 2;  // La commande du MAITRE ou commande manuelle pour lancer le programme arrive sur la broche 2 de la carte Arduino.
      int BrochePqRose = 3; // La commande du MAITRE ou manuelle pour lancer la fabrication du PQ Rose.
      int BrochePqBleu = 4; // La commande du MAITRE ou manuelle pour lancer la fabrication du PQ Bleu.
      int BrocheInit = 6; // Initialisation de la PW; Commande du Maître ou manuelle
      int BrocheCommandeDan = A3;  // La commande de Secours du Maître


  //ETATS
      int BrocheAvanceProgramme = 5;  // Permet de renvoyer une info au Maître pour avancer dans le programme.
      int BrocheLedPret = 7;  // Led signalant l'Etat Prêt sur la broche 7.

      int BrochePosition1 = 8;   //  Retour d'info Position 1.
      int BrochePosition2 = 11;   //  Retour d'info Position 2.
      int BrochePosition3 = 12;  //  Retour d'info Position 3.
      int BrochePosition4 = 13;  //  Retour d'info Position 4.
      int BrochePosition5 = A0;  //  Retour d'info Position 5.
      int BrochePosition6 = A1;  //  Retour d'info Position 6.
      int BrochePosition7 = A2;  //  Retour d'info Position 6.

      int EtatCommandeProgramme; // variable de l'état de la commande venant du Maître
      int EtatBrochePqRose; // état de la broche fonction de la couleur choisie
      int EtatBrochePqBleu; // état de la broche fonction de la couleur choisie
      int EtatInit; // état de la broche Init. Réinitialisation de la PW en Position 1, soit par le Maître soit sur la carte ou commande manuelle

      int EtatCommandeDan;

  // VARIABLES
      int i;  // variable d'incrémentation
      int E;  // Ecart de position
      int PositionServoPwRose; // variable qui permet de stocker la position angulaire de la plaque servo Rose
      int PositionServoPwBleu; // variable qui permet de stocker la position angulaire de la plaque servo Bleu
      int Etape1;
      int Etape2;
      int Etape3;
      int Etape4;
      int Etape5;
      int Etape6;
      int FIN;

      boolean MdR = false; // Mode Rose
      boolean MdB = false; // Mode Bleu

  // POSITIONS ANGULAIRES
      int P1 = 0;   // Bois vers C, AB vers vide.
      int P2 = 27;  // Bois vers AB
      int P3 = 83;  // Rose vers C
      int P4 = 110; // Rose vers AB
      int P5 = 83;  // Bleu vers c
      int P6 = 56;  // Bleu vers AB



// INITIALISATION
  void setup()  {

    // SERVO
        MaServoPwRose.attach(9); // initialise la Servo sur la broche A0,
        MaServoPwBleu.attach(10); // initialise la Servo sur la broche A0,


    // COMMANDES ENTREES
        pinMode(BrocheCommandeProgramme, INPUT);  // Initialise la broche de la commande lancement programme PW comme une entrée.
        pinMode(BrochePqRose, INPUT); // Initialise la broche de la commande programme PQ Rose comme une entrée.
        pinMode(BrochePqBleu, INPUT); // Initialise la broche de la commande programme PQ Bleu comme une entrée.
        pinMode(BrocheInit, INPUT); // Initialise la broche de la commande programme Init comme une entrée.

        pinMode(BrocheCommandeDan, INPUT);

    // ETAT SORTIES
        pinMode(BrocheLedPret, OUTPUT);  // Initialise la broche de la Led Etat Prêt comme une sortie.
        pinMode(BrochePosition1, OUTPUT); // Initialise la broche de la Led Position 1 comme une sortie.
        pinMode(BrochePosition2, OUTPUT); // Initialise la broche de la Led Position 2 comme une sortie.
        pinMode(BrochePosition3, OUTPUT); // Initialise la broche de la Led Position 3 comme une sortie.
        pinMode(BrochePosition4, OUTPUT); // Initialise la broche de la Led Position 4 comme une sortie.
        pinMode(BrochePosition5, OUTPUT); // Initialise la broche de la Led Position 5 comme une sortie.
        pinMode(BrochePosition6, OUTPUT); // Initialise la broche de la Led Position 6 comme une sortie.
        pinMode(BrochePosition7, OUTPUT); // Initialise la broche de la Led Position 7 comme une sortie.

        pinMode(BrocheAvanceProgramme, OUTPUT); // Initialidse la broche de l'avancement du programme en mode sortie vers le Maître.

    // INITIALISATION POSITION PW A 0°
        PositionServoPwRose = P1;
        MaServoPwRose.write(PositionServoPwRose);  // le servo se positionne impérativement à 0°.
        PositionServoPwBleu = P1;
        MaServoPwBleu.write(PositionServoPwBleu);  // le servo se positionne impérativement à 0°.

        digitalWrite(BrocheLedPret, HIGH);  //   Led Etat Prêt allumée, info vers le Maître

    // INITIALISATION LEDS
        digitalWrite(BrochePosition1, HIGH);
        digitalWrite(BrochePosition2, LOW);
        digitalWrite(BrochePosition3, LOW);
        digitalWrite(BrochePosition4, LOW);
        digitalWrite(BrochePosition5, LOW);
        digitalWrite(BrochePosition6, LOW);
        digitalWrite(BrochePosition7, LOW);
                } // Fin de Set Up

// BOUCLE
  void loop()   {

    //PROGRAMME INIT, POSITIONNEMENT SERVOS A 0°.

    if (digitalRead(BrocheInit) == HIGH){

        digitalWrite(BrocheLedPret, LOW);  //   Led Etat Prêt éteinte, programme en cours pour la réinitialisation

        if (PositionServoPwRose > 0) {

            for (i = 1; i = PositionServoPwRose; i--){ // on envoie un ordre de rotation
               PositionServoPwRose--;
               MaServoPwRose.write(PositionServoPwRose);
               delay(50);
                                                  } // Fin de for

            PositionServoPwRose = P1;
            MaServoPwRose.write(PositionServoPwRose);

                    } // Fin de if PositionServoPwRose > 0

      if (PositionServoPwBleu > 0) {

            for (i = 1; i = PositionServoPwBleu; i--){
               PositionServoPwBleu--;
               MaServoPwBleu.write(PositionServoPwBleu);
               delay(50);

            PositionServoPwBleu = P1;
            MaServoPwBleu.write(PositionServoPwBleu);
                                                  } // Fin de for

                    } // Fin de if PositionServoPwBleu < 0

        digitalWrite(BrocheLedPret, HIGH);  //   Led Etat Prêt allumée, info vers le Maître

        digitalWrite(BrochePosition1, HIGH);  //   Led position 1 allumée
        digitalWrite(BrochePosition2, LOW);
        digitalWrite(BrochePosition3, LOW);
        digitalWrite(BrochePosition4, LOW);
        digitalWrite(BrochePosition5, LOW);
        digitalWrite(BrochePosition6, LOW);
        digitalWrite(BrochePosition7, LOW);

                          } // Fin de if Init


    // PROGRAMME PRET, INFO VERS MAITRE

        digitalWrite(BrocheAvanceProgramme, HIGH);  //   Info retour pour le Maître afin de poursuivre le programme


    // MAITRE LANCE PROGRAMME DE FABRICATION

        // LIVRAISON DU BOIS

        // Le loco arrive par la voie AB devant la PW, il faut tourner la PW pour accrocher le wagon

            if(digitalRead(BrochePosition1) == HIGH){    // IF1 : si on est en position 1

                if (digitalRead(BrocheCommandeProgramme) == HIGH) {    // IF2 : Le MAITRE donne l'ordre de fabrication

                    digitalWrite(BrocheLedPret, LOW);  //   Led Etat Prêt éteinte, programme en cours pour la fabrication
                    digitalWrite(BrochePosition1, LOW);  //   Led position 1 éteinte

                    for (i = 1; i <= P2; i++) {

                        PositionServoPwRose++;
                        MaServoPwRose.write(PositionServoPwRose);  // le servo change de position de 27°, sens horaire, passant de 0° à 27° par pas de 1°. Accrochage du wagon Bois.
                        delay(50);
                                              } // Fin de la boucle for

                    digitalWrite(BrochePosition2, HIGH);  //   Led position 2 allumée
                    digitalWrite(BrocheLedPret, HIGH);  //   Led Etat Prêt allumée, info vers le Maître pour faire partir le loco
                                                                  } // Fin de IF2

                    delay(250);  // tempo de 0.250 seconde

                                                        }      // Fin de IF1

        /* le wagon Bois est accroché. Une fois que le convoi quitte la PW,
            rotation de la PW pour mettre cette voie libre en face de la voie C et réceptionner le wagon Bois. On revient en position 1*/

            if(digitalRead(BrochePosition2) == HIGH){ //IF3 : est ce que le servo est en position 2?

                if (digitalRead(BrocheCommandeProgramme) == HIGH) {    // If4 :Le MAITRE donne l'ordre de poursuivre la fabrication dès que le convoi est parti

                    digitalWrite(BrocheAvanceProgramme, LOW);  //   Plus d'info retour pour le Maître
                    digitalWrite(BrocheLedPret, LOW);  //   Led Etat Prêt éteinte,

                        for (i = 1; i <= P2; i++) { // rotation de la PW par pas de 1° pour réceptionner le wagon en face de la voie C

                            PositionServoPwRose--;
                            MaServoPwRose.write(PositionServoPwRose);  // le servo change de position de 27°, sens anti-horaire, passant de 27° à 0° par pas de 1°
                            delay(50);
                                                  } // Fin de la boucle for

                    digitalWrite(BrochePosition2, LOW);  //   Led position 1 allumée
                    digitalWrite(BrochePosition1, HIGH);  //   Led position 1 allumée
                    delay(250);  // tempo de 0.250 seconde

                                                } // Fin de If4

                                            } // Fin de If3

                digitalWrite(BrocheAvanceProgramme, HIGH);  //   Info retour pour le Maître afin de poursuivre le programme
                digitalWrite(BrocheLedPret, HIGH);  // Led Etat Prêt allumée,


        /* A ce stade, le wagon bois est réceptionné sur la PW, sur sa voie, le bois est livré à l'usine de PQ,
            il faut continuer la fabrication en fonction de la couleur du PQ choisie.*/

     // PQ ROSE

        if(digitalRead(BrochePosition1) == HIGH){    // IF5 : si on est en position 1

            EtatBrochePqRose = digitalRead(BrochePqRose);

            if (EtatBrochePqRose == HIGH) { // IF6, est ce que la couleur choisie est le Rose?

                digitalWrite(BrocheLedPret, LOW);  //   Led Etat Prêt éteinte,

                    for (i = 1; i <= 83; i++) { // rotation de la PW par pas de 1° pour positionner le wagon PQ Rose en face de la voie C et l'accrocher au loco.

                        PositionServoPwRose++;
                        MaServoPwRose.write(PositionServoPwRose);  // le servo change de position de 90°, sens horaire, passant de 0° à +90° par pas de 1°
                        delay(50);
                                              } // Fin de la boucle for

                    digitalWrite(BrochePosition1, LOW);  //   Led position 1 éteinte
                    digitalWrite(BrochePosition3, HIGH);  //   Led position 3 allumée
                    digitalWrite(BrocheLedPret, HIGH);  //   Led Etat Prêt allumée
                    digitalWrite(BrocheAvanceProgramme, HIGH);  //   Info retour pour le Maître pour faire partir le loco. Le convoi quitte l'usine.
                    delay(250);  // tempo de 0.250 seconde


        /* le wagon PQ Rose est accroché, le convoi peut quitter l'usine,
            Le Maître donne l'ordre de rotation pour mettre cette voie libre en face de la voie AB et réceptionner le wagon PQ Rose en gare.*/

            // si on est en position 3 Le MAITRE donne l'ordre de poursuivre, le convoi a quitté la PW

            if(EtatBrochePqRose == HIGH) {
                EtatCommandeProgramme = digitalRead(BrocheCommandeProgramme);

            if(EtatCommandeProgramme == HIGH){    // IF7 :


                    digitalWrite(BrocheAvanceProgramme, LOW);  //   Plus d'info retour pour le Maître
                    digitalWrite(BrocheLedPret, LOW);  //   Led Etat Prêt éteinte,

                    for (i = 1; i <= 27; i++) { // rotation de la PW par pas de 1° pour réceptionner le wagon PQ Rose en face de la voie C

                        PositionServoPwRose++;
                        MaServoPwRose.write(PositionServoPwRose);  // le servo change de position de 27°, sens horaire, passant de 90 à 117° par pas de 1°
                        delay(50);
                                              } // Fin de la boucle for

                    digitalWrite(BrochePosition3, LOW);  //   Led position 3 éteinte
                    digitalWrite(BrochePosition4, HIGH);  //   Led position 4 allumée
                    digitalWrite(BrocheLedPret, HIGH);  //   Led Etat Prêt allumée, info vers le Maître pour faire partir le loco
                    digitalWrite(BrocheAvanceProgramme, HIGH);  //   Info retour pour le Maître pour faire partir le loco. Le convoi quitte l'usine.
                    delay(250);  // tempo de 0.250 seconde

            }

        /* Le loco amène la wagon PQ Rose sur la PW, sur sa voie par la voie AB. Il faut dételer le wagon. Le détellage se fait par une rotation suffisante de 27°,
           mais pas de 117° d'un coup pour éviter l'accrochage avec le wagon Bois lors de la rotation*/

                    if(digitalRead(BrochePosition4) == HIGH){    // IF9 : si on est en position 4

                        if (digitalRead(BrocheCommandeProgramme) == HIGH) {    // IF10 : Le MAITRE donne l'ordre de poursuivre, le wagon PQ Rose est sur sa voie

                            digitalWrite(BrocheAvanceProgramme, LOW);  //   Plus d'info retour pour le Maître
                            digitalWrite(BrocheLedPret, LOW);  //   Led Etat Prêt éteinte,

                            for (i = 1; i <= 27; i++) { // rotation de la PW de 27° par pas de 1° pour dételler le wagon PQ Rose

                                PositionServoPwRose--;
                                MaServoPwRose.write(PositionServoPwRose);  // le servo change de position de 27°, sens anti-horaire, passant de 117° à 90° par pas de 1°
                                delay(50);
                                                      } // Fin de la boucle for

                            digitalWrite(BrochePosition4, LOW);  //   Led position 4 éteinte
                            digitalWrite(BrochePosition3, HIGH);  //   Led position 3 allumée
                            digitalWrite(BrocheLedPret, HIGH);  //   Led Etat Prêt allumée, info vers le Maître pour faire partir le loco
                            digitalWrite(BrocheAvanceProgramme, HIGH);  //   Info retour pour le Maître pour faire partir le loco. Le convoi quitte l'usine.
                            delay(250);  // tempo de 0.250 seconde

        // Le wagon PQ Rose est détellé; il faut remettre la PW en position 1.

                            if(digitalRead(BrochePosition3) == HIGH){    // IF11 : si on est revenu en position 3

                                if (digitalRead(BrocheCommandeProgramme) == HIGH) {    // IF12 : Le MAITRE donne l'ordre de poursuivre, le loco rentre dans la remise

                                    digitalWrite(BrocheAvanceProgramme, LOW);  //   Plus d'info retour pour le Maître
                                    digitalWrite(BrocheLedPret, LOW);  //   Led Etat Prêt éteinte,

                                    for (i = 1; i <= 83; i++) { // rotation de la PW par pas de 1° pour revenir en position de début de fabrication : wagon Bois en face de la voie C

                                        PositionServoPwRose--;
                                        MaServoPwRose.write(PositionServoPwRose);  // le servo change de position de 90°, sens anti-horaire, passant de +90° à 0° par pas de 1°
                                        delay(50);
                                                              } // Fin de la boucle for

                                    digitalWrite(BrochePosition3, LOW);  //   Led position 3 éteinte
                                    digitalWrite(BrochePosition1, HIGH);  //   Led position 1 allumée
                                    digitalWrite(BrocheLedPret, HIGH);  //   Led Etat Prêt allumée, info vers le Maître pour faire partir le loco vers la remise
                                    digitalWrite(BrocheAvanceProgramme, HIGH);  //   Info retour pour le Maître pour faire partir le loco. Le convoi quitte l'usine.
                                    delay(250);  // tempo de 0.250 seconde


        // A ce stade, la séquence est terminée, le wagon PQ Rose est sur la PW, sur sa voie; La PW est en position initiale wagon Bois en face de la voie C.
        // Le programme est terminé, prêt à repartir pour un autre choix

                                                                                  } // Fin de IF12

                                                                        } // Fin de IF11

                                                                  } // Fin de IF10

                                                        } // Fin de IF9

                                                   // } // Fin de IF8

                                             } // Fin de IF7

                                        } // Fin de IF6

                                } // Fin de IF5

        // Fin de la séquence PQ Rose


     // PQ BLEU

        if(digitalRead(BrochePosition1) == HIGH){    // IF13 : si on est en position 1

            if (digitalRead(BrochePqBleu) == HIGH) { // IF14, est ce que la couleur choisie est le Bleu?

                digitalWrite(BrocheAvanceProgramme, LOW);  //   Plus d'info retour pour le Maître
                digitalWrite(BrocheLedPret, LOW);  //   Led Etat Prêt éteinte,

                    for (i = 1; i <= 73; i++) { // rotation de la PW par pas de 1° pour accrocher le wagon Bleu en face de la voie C

                        PositionServoPwBleu++;
                        MaServoPwBleu.write(PositionServoPwBleu);  // le servo change de position de 90°, sens anti-horaire, passant de 0° à -90° par pas de 1°
                        delay(50);

                                              } // Fin de la boucle for

                        digitalWrite(BrochePosition1, LOW);  //   Led position 1 éteinte
                        digitalWrite(BrochePosition5, HIGH);  //   Led position 5 allumée
                        digitalWrite(BrocheLedPret, HIGH);  //   Led Etat Prêt allumée, info vers le Maître pour faire partir le loco
                        digitalWrite(BrocheAvanceProgramme, HIGH);  //   Info retour pour le Maître pour faire partir le loco. Le convoi quitte l'usine.
                        delay(250);  // tempo de 0.250 seconde


        /* le wagon PQ Bleu est accroché, le convoi peut quitter la PW,
            Le Maître donne l'ordre de rotation pour mettre cette voie libre en face de la voie AB et réceptionner le wagon PQ Bleu en gare.*/

             if(digitalRead(BrochePosition5) == HIGH){    // IF15 : si on est en position 5

                if (digitalRead(BrocheCommandeProgramme) == HIGH) {    // If16 : Le MAITRE donne l'ordre de poursuivre, le convoi a quitté la PW

                    digitalWrite(BrocheAvanceProgramme, LOW);  //   Plus d'info retour pour le Maître
                    digitalWrite(BrocheLedPret, LOW);  //   Led Etat Prêt éteinte,

                    for (i = 1; i <= 30; i++) { // rotation de la PW par pas de 1° pour réceptionner le wagon en face de la voie C

                        PositionServoPwBleu--;
                        MaServoPwBleu.write(PositionServoPwBleu);  // le servo change de position de 27°, sens horaire, passant de -90° à -63° (-90+27=63) par pas de 1°
                        delay(50);
                                              } // Fin de la boucle for

                    digitalWrite(BrochePosition5, LOW);  //   Led position 5 éteinte
                    digitalWrite(BrochePosition6, HIGH);  //   Led position 6 allumée
                    digitalWrite(BrocheLedPret, HIGH);  //   Led Etat Prêt allumée, info vers le Maître pour accueillir le wagon PQ Bleu venant de la voie AB
                    digitalWrite(BrocheAvanceProgramme, HIGH);  //   Info retour pour le Maître pour faire partir le loco. Le convoi quitte l'usine.
                    delay(250);  // tempo de 0.250 seconde


        // Le loco amène la wagon PQ Bleu sur sa voie par la voie AB. Il faut dételer le wagon.

                    if(digitalRead(BrochePosition6) == HIGH){    // IF17 : si on est en position 6

                        if (digitalRead(BrocheCommandeProgramme) == HIGH) {    // IF18 : Le MAITRE donne l'ordre de poursuivre, le wagon est sur la PW

                            digitalWrite(BrocheAvanceProgramme, LOW);  //   Plus d'info retour pour le Maître
                            digitalWrite(BrocheLedPret, LOW);  //   Led Etat Prêt éteinte,

                            for (i = 1; i <= 43; i++) { // rotation de la PW par pas de 1° pour réceptionner le wagon en face de la voie C

                                PositionServoPwBleu--;
                                MaServoPwBleu.write(PositionServoPwBleu);  // le servo change de position de 63°, sens horaire, passant de -63° à 0° par pas de 1°
                                delay(50);
                                                      } // Fin de la boucle for

                            digitalWrite(BrochePosition6, LOW);  //   Led position 6 éteinte
                            digitalWrite(BrochePosition1, HIGH);  //   Led position 1 allumée
                            digitalWrite(BrocheLedPret, HIGH);  //   Led Etat Prêt allumée, info vers le Maître pour faire partir le loco vers la remise
                            digitalWrite(BrocheAvanceProgramme, HIGH);  //   Info retour pour le Maître pour faire partir le loco. Le convoi quitte l'usine.
                            delay(250);  // tempo de 0.250 seconde


        // A ce stade, la séquence est terminée, le wagon PQ Bleu est sur la PW, sur sa voie; La PW est en position initiale wagon Bois en face de la voie C.
        // Le programme est terminé, prêt à repartir pour un autre choix

                                                                        } // Fin de IF18

                                                                  } // Fin de IF17

                                                          } // fin de IF16

                                                    }  // Fin de IF15

                                            } // Fin de IF14

                                         } // Fin de IF13

  // Fin de séquence PQ BLEU


                }       //fin de la Boucle Loop

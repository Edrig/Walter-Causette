
/* SCRIPT AFFICHAGE LCD PUBLIC;
PRINCIPE : 
  L'affichege LCD est destiné au public. Il demande de choisir entre papier rose ou papier bleu.
  Arduino déroule un affichage :
  "LES POTES A G"
  "RAMMA 2017"
  "Vos papiers !"
  "choisir une couleur: rose ou bleu ?"
  Le programme est alors en attente du choix. Dès que le choix est fait, il affichera le résultat, confirme par LED de couleur et lance le processus pour la fabrication.
  "Choix: le bleu" ou Choix: le rose"
  "Fabrication PQ bleu en cours" ou "Fabrication PQ rose en cours"
  Le ventilateur de la couleur choisie, se met en route, diffusant le parfum fonction de la couleur.
  Dès que le wagon est arrivé en gare de distribution, un servo est actionné pour présenter le chargement de PQ de la couleur en sortie de dévidoir.
  

SEQUENCE : 
  A la mst, l'arduino procède à l'initialisation , c'est à dire écran vierge et LED éteintes.
  L'arduino est alors prêt pour démarrer les séquences affichages et choix
  Dès que l'ordre de démarrage est donné :
    le texte s'affiche sur le LCD, après la présentation, le spectateur a le choix de la couleur du PQ, les Leds Rose et Bleu clignotent, 
    la Led rouge " Interdiction de choix" est éteinte.
    dès que le choix est réalisé, l'afficheur confirme et la LED correspondante s'allume, la Led Prêt est allumée renvoyant au maître l'ordre de fabrication du PQ de la couleur choisie.
    Tout nouveau choix est alors interdit par la Led rouge. Il faudra attendre la réinitialisation soit par le Maître soit sur la carte.
Le capot s'entrouvre et le ventilateur correspondant à la couleur choisie est alimenté pour diffuser l'odeur en rapport.
  Lorsque le cycle de fabrication de PQ est terminé, c'est à dire que le wagon chargé de PQ de la couleur choisie est arrivé en gare de livraison, le capot s'entrouvre et un distributeur présente le rouleau de PQ de la couleur choisie.
  Si on veut réinitialiser le programme, c'est à dire retourner au début du loop, mais encore choisir la couleur du papier, 2 voies possibles:
    par appui sur un bouton INIT, pour toute opération manuelle
    par le maître dès lors que la livraison a été réalisée, c'est ce mode qui est privilégié en expo.
  La séquence est terminée.
  
ACCESSOIRES :
    carte Arduino UNO
    shield proto
    L'afficheur LCD
    2 poussoir pour le choix des couleurs
    1 poussoir pour réinitialiser le programme
    2 Leds rose et bleu correspondant à la couleur du papier choisi,
    une Led bleue pour indiquer l'état prêt, renvoyé vers le panneau de commande général
     une Led rouge pour indiquer que le choix n'est pas possible, il faut patienter.
    2 ventilateurs miniatures 12V
    2 diffuseur de parfum Rose et Océan type W5 Car Air chez LIDL (!)
    1 relais REED 1RT 5V
    1 transistors MOSFET N type IRF 530 ou Relais REED 5V 1T
    2 Servo moteur    
 */

//DEFINITIONS

 // BIBLIOTHEQUE AFFICHEUR
      #include <LiquidCrystal.h> 
      LiquidCrystal monEcran(12,11,5,4,3,2); // on crée l'objet écran

 // SERVO
    #include <Servo.h>
    Servo MaServoPq; // Donne un nom à la servo pour communiquer
    Servo MaServoCapot; // Donne un nom à la servo pour communiquer
      
  // LES COMMANDES
      const int BrocheBoutonInit = 6;  //Le bouton de réinitialisation est sur 6, pour retourner au début du setup       
      const int BrocheBoutonRose = 7; // Le bouton rose est sur la broche 7
      const int BrocheBoutonBleu = 8; // Le bouton bleu est sur la broche 8
      const int BrocheFinLivraison = A0; // Le Retour du Maître pour la fin du programme, livraison du pq.

  // LES ETATS
      int EtatBoutonBleu = 0; //  variable de l'état du bouton poussoir bleu
      int EtatBoutonRose = 0; //  variable de l'état du bouton poussoir rose       
      int EtatLedPret = 0;  //   variable de l'état de la Led pret
      int EtatFinLivraison = 0; //  variable de l'état du bouton poussoir Commande
      int EtatLedRose = 0; //  variable de l'état de la Led rose, utilisée pour le clignotement
      int EtatLedBleu = 0;  //   variable de l'état de la Led bleu, utilisée pour le clignotement
      int EtatLedRouge = 0; // variable de l'état de la led pour interdiction de choix
      int ValInit;  // variable de stockage de l'état du bouton Init
      int PositionDistri;  // variable de stockage de l'état du distributeur
      int PositionCapot; // variable de stockage de l'état du capot
            
  // LES VOYANTS
      const int BrocheLedRose = 13;  // la LED rose est sur la broche 9
      const int BrocheLedBleu = A2; // La LED bleu est sur la broche 10      
      const int BrocheLedPret = A5;  // la LED indiquant que le programme est prêt et attend une action extérieure  
      int BrocheLedRouge = A1;  //  La LED rouge est sur la broche analogique A0
      
      
  // LES VARIABLES
      int i = 0;  // variable d'incrémentation clignotement LCD
      int j = 0;  // variable d'incrémentation délai de fonctionnement ventilateur



  // ODEURS
      int BrocheCdeMosFet = A3;  // C'est le relais monostable qui commande le ventilateur alimenté en 12V
      int BrocheRelaisBleu = A4;  // C'est la commande qui va alimenter le ventilateur en 12V          

        
// INITIALISATION
    void setup() {
          // INITIALISATION DES BROCHES
              pinMode(BrocheBoutonRose,INPUT); // Le bouton Rose est en mode entrée
              pinMode(BrocheLedRose,OUTPUT);  //  La LED Rose est en mode sortie

              pinMode(BrocheBoutonBleu,INPUT); // Le bouton Bleu est en mode entrée
              pinMode(BrocheLedBleu,OUTPUT);  //  La LED Bleu est en mode sortie

              pinMode(BrocheFinLivraison,INPUT); // Le bouton Commande est en mode entrée  
              pinMode(BrocheBoutonInit,INPUT);  // le bouton Init est en mode entrée      
              
              pinMode(BrocheLedPret,OUTPUT);  // La Led Prêt est en mode sortie
              pinMode(BrocheLedRouge,OUTPUT); // La Led Rouge est en mode sortie

              pinMode(BrocheCdeMosFet,OUTPUT); // le Mosfet est en mode sortie
              pinMode(BrocheRelaisBleu,OUTPUT); // le relais est en mode sortie
              

          // INITIALISATION DES ETATS
              digitalWrite(BrocheLedRose,LOW);  // on force l'état de la Led Rose à 0
              digitalWrite(BrocheLedBleu,LOW);  // on force l'état de la Led Bleu à 0
    
              digitalWrite(BrocheLedPret,LOW);  // on force l'état de la Led Pret à 0
      
              digitalWrite(BrocheLedRouge,HIGH);  // on force l'état de la Led Rouge à 1

              digitalWrite(BrocheCdeMosFet,LOW);  // on force la cde du mosfet à 0
              digitalWrite(BrocheRelaisBleu,LOW);  // on force l'état du relais à 0              


          // INITIALISATION DE L'ECRAN
              monEcran.begin(16,2); //on initialise la communication avec 16 colonnes et deux lignes
              monEcran.clear(); // on efface l'écran des fois que.....
              monEcran.home(); //on se positionne sur la première colonne de la première ligne

          // INITIALISATION DE SERVO
              MaServoPq.attach(9); // initialise le Servo sur la broche 9,
              PositionDistri = 80; // 80° est la position Distributeur rentré
              MaServoPq.write(PositionDistri);
            
              MaServoCapot.attach(10); // initialise le Servo sur la broche 10,
              PositionCapot = 0; // 0° est la position Capot fermé
              MaServoCapot.write(PositionCapot);

          // PRESENTATION
              monEcran.print(" Les POTES a G");  //on écrit sur la première ligne avec un espace pour centrer le texte
    
              for( i=0; i<=3; i++){  // boucle d'incrémentation
                    monEcran.noDisplay();
                    delay(500);  //  pause de 0.5 secondes
                    monEcran.display();
                    delay(500);  //  pause de 0.5 secondes 
                                } // fin de for boucle d'incrémentation
  
              delay(3000);  //  pause de 3 secondes
 
              monEcran.home(); //on se positionne sur la première colonne de la première ligne
              monEcran.print("   RAMMA 2017   "); // on écrit sur la deuxième ligne avec un espace pour centrer le texte
              delay(2000);  //  pause de 2 secondes 

              monEcran.setCursor(0,1); //on se positionne sur la première colonne de la deuxième ligne 
              monEcran.print("  VOS PAPIERS !"); // on écrit sur la deuxième ligne
              delay(3000); // pause de 3 secondes

              monEcran.clear(); // on efface l'écran
  
              monEcran.home(); //on se positionne sur la première colonne de la première ligne
              monEcran.print("Choisir une couleur: rose ou bleu?"); // on écrit

        // On force les sorties rose et bleu à l'état haut  
              delay(750); // attendre 3/4 secondes

                                    } // fin de setup

  
// BOUCLE
void loop() {

  // REINIT
      ValInit = digitalRead(BrocheBoutonInit);
      EtatFinLivraison = digitalRead(BrocheFinLivraison);
      if ((ValInit == LOW) || (EtatFinLivraison == HIGH))  { // Soit on réinitialise par le bouton sur la carte ou par commande du maître
          // VARIABLES
            int i = 0; 
            int j = 0;
            (EtatLedRose = 0);  // On force l'état à 0
            (EtatLedBleu = 0);  // On force l'état à 0 
                        
          // COMMANDES
            digitalWrite(BrocheLedPret,LOW);  // on force l'état de la Led pret à 0
            digitalWrite(BrocheCdeMosFet,LOW);  // on force l'état de la commande à 0
            digitalWrite(BrocheRelaisBleu,LOW);  // on force l'état de la commande à 0

          // LEDS
            digitalWrite(BrocheLedRose,LOW);
            digitalWrite(BrocheLedBleu,LOW);

          // ECRAN
            monEcran.clear(); // on efface l'écran
            monEcran.home(); //on se positionne sur la première colonne de la première ligne
            monEcran.print("Choisir une couleur: rose ou bleu?"); // on écrit
          
          // SERVOS
             PositionDistri = 80; // 80° est la position Distributeur rentré
             MaServoPq.write(PositionDistri);
             PositionCapot = 0; // 0° est la position Capot fermé
             MaServoPq.write(PositionCapot);
                   
          delay(3000); // attendre 3 secondes

                        } // fin de If ValInit

  // AUTORISATION DE CHOIX
        if ((EtatLedRose == 0) && (EtatLedBleu == 0))  { // En attente de la commande, clignotement des Leds
          digitalWrite(BrocheLedRouge,LOW);  // on force l'état de la Led Rouge à 0 pour autoriser un choix
                                
                                                       }  // Fin de if autorisation de choix


  // CHOIX DU PAPIER
      EtatBoutonRose = digitalRead(BrocheBoutonRose); //  lecture de l'état du Bouton Rose
      EtatBoutonBleu = digitalRead(BrocheBoutonBleu); //  lecture de l'état du Bouton Bleu
      EtatLedRouge = digitalRead(BrocheLedRouge);  // lecture de l'état de la Led Rouge 

 // FABRICATION PAPIER ROSE
      // il faut un appui sur le bouton rose mais il ne doit pas y avoir eu de choix précédent
      if((EtatBoutonRose == HIGH) && (EtatLedRouge == 0)) {
          digitalWrite(BrocheLedPret,LOW);  // on force l'état de la Led pret à 0
          digitalWrite(BrocheBoutonBleu, LOW);  //  on force l'état du Bouton Bleu à l'état bas
          digitalWrite(BrocheLedBleu, LOW); //  on force l'état éteint de la Led Bleu 
          digitalWrite(BrocheLedRose, HIGH);  //on éclaire la Led Rose
          digitalWrite(BrocheRelaisBleu,LOW);  // on positionne le relais vers Rose          
          digitalWrite(BrocheCdeMosFet,HIGH);  // on déclenche le ventilateur

          digitalWrite(BrocheLedRouge,HIGH);  // on force l'état de la Led Rouge à 1
                    
          monEcran.clear(); // on efface l'écran
          monEcran.setCursor(1,0); //on se positionne sur la colonne 1 de la première ligne
          monEcran.print("choix: le rose");  // confirmation du choix
          delay(3000);  

        // on force l'état des variables pour confirmer le choix mais aussi pour interdire un autre choix tant que le prcessus de fabrication n'est pas terminé.
          (EtatLedRose = 1);
          (EtatLedBleu = 0);          

        // On confirme à l'écran le choix de la couleur
          monEcran.clear(); // on efface l'écran
          monEcran.setCursor(0,0); //on se positionne sur la première colonne de la première ligne
          monEcran.print("Fabrication PQ rose en cours. Patience! ");   //on écrit sur la première ligne pour demander de patienter

        // On renvoie vers le maître l'état prêt pour lancer la fabrication  
           digitalWrite(BrocheLedPret, HIGH);  //  On force la led prêt à 1 ce qui permet au maître de lancer le processus de fabrication
                  } // fin de if état bouton rose      

  // FABRICATION PAPIER BLEU 
      // il faut un appui sur le bouton bleu mais il ne doit pas y avoir eu un choix précédent
      if ((EtatBoutonBleu == HIGH)  && (EtatLedRouge == 0)){
          digitalWrite(BrocheLedPret,LOW);  // on force l'état de la Led prêt à 0
          digitalWrite(BrocheLedBleu, HIGH);  //  On éclaire la Led Bleue
          digitalWrite(BrocheBoutonRose, LOW);    //  on force l'état du Bouton Rose à l'état bas
          digitalWrite(BrocheLedRose, LOW);  // on force l'état éteint de la Led Rose
          
          digitalWrite(BrocheRelaisBleu,HIGH);  // on positionne le relais vers bleu
          digitalWrite(BrocheCdeMosFet,HIGH);   // on déclenche le ventilateur  

          digitalWrite(BrocheLedRouge,HIGH);  // on force l'état de la Led Rouge à 1
                                        
          monEcran.clear(); // on efface l'écran
          monEcran.setCursor(1,0); //on se positionne sur la colonne 1 de la première ligne
          monEcran.print("choix: le bleu");  // confirmation du choix
          delay(3000);    

        // on force l'état des variables pour confirmer le choix mais aussi pour interdire un autre choix tant que le prcessus de fabrication n'est pas terminé.
          (EtatLedRose = 0);
          (EtatLedBleu = 1);

        // On confirme à l'écran le choix de la couleur
          monEcran.clear(); // on efface l'écran
          monEcran.setCursor(0,0); //on se positionne sur la première colonne de la première lignee 
          monEcran.print("Fabrication PQ bleu en cours. Patience! ");   //on écrit sur la première ligne pour demander de patienter

        // On renvoie vers le maître l'état prêt pour lancer la fabrication           
          digitalWrite(BrocheLedPret, HIGH);  //  On force la led prêt à 1 ce qui permet au maître de lancer le processus de fabrication
                  } // fin de if état bouton bleu

             
  // DEFILEMENT DU TEXTE EN ATTENDANT QUE LE CHOIX SOIT FAIT OU QUE LA FABRICATION SOIT TERMINEE
      monEcran.setCursor(11,0); // positionner le curseur sur la première ligne de l'écran: colonne 11 car 12 est un caractère vide
      monEcran.scrollDisplayLeft();    // on active le déroulement
      monEcran.print(" ");      // écrire le texte précédent
      delay(450); //  c'est le délai pour le défilement entre changement de colonne. En dessous, ça va trop vite


 // CLIGNOTEMENT DES LEDS EN ATTENTE ACTION PUBLIC
      if ((EtatLedRose == 0) && (EtatLedBleu == 0))  { // En attente de la commande, clignotement des Leds
            digitalWrite(BrocheLedBleu, LOW);  //  On éteind la Led Bleue
            digitalWrite(BrocheLedRose, HIGH);  // on éclaire la Led Rose
            delay(100);

            digitalWrite(BrocheLedBleu, HIGH);  //  On éclaire la Led Bleue
            digitalWrite(BrocheLedRose, LOW);  // on éteind la Led Rose
            delay(100);      

  // INTERDICTION DE CHOIX
      // il y a déjà eu un  choix et la fabrication n'est pas terminée, on ne peut pas relancer un processus de choix, il faudra attendre la fin de la fabrication.
      if( ((EtatLedRose == 1) && (EtatLedBleu == 0)) || ((EtatLedRose == 0) && (EtatLedBleu == 1))) {
            digitalWrite(BrocheLedRouge,HIGH);  // on force l'état de la Led Rouge à 1, c'est ce qui bloquera une action de choix pour le public

                                                            } // Fin de if interdiction de choix
                                                                                                    
                                                      } // Fin de clignotement


  // VENTILATION ODEURS
      // dès lors qu'il y a eu choix, le ventilateur se déclenche pour expulser l'odeur correspondante à la couleur choisie.
       if ( (EtatLedRose == 1) || (EtatLedBleu == 1)){ // pour le fonctionnement des ventilateurs durant le délai, il faut que celui ci ventile pendant un laps de temps suffisant
  PositionCapot = 20;
            MaServoCapot.write(PositionCapot);  

          j=j+1;  // boucle
          if (j < 100){ // Tant que le délai n'est pas dépassé, on ventile
               delay(30);
                    } // Fin de if j>=50                       

          else {
              digitalWrite(BrocheCdeMosFet,LOW);  // on arrête le ventilateur
              digitalWrite(BrocheRelaisBleu,LOW);  // on repositionne le relais vers rose

                      } // Fin de else            

            PositionCapot = 0;
            MaServoCapot.write(PositionCapot); 

                                                              } // Fin de if choix rose ou bleu
          
  // DISTRIBUTEUR DE PQ
       EtatFinLivraison = digitalRead(BrocheFinLivraison);
              if ((EtatLedRose == 1) && (EtatFinLivraison == 1))  { // Distribution du rouleau de la couleur choisie
                PositionCapot = 20;
                MaServoCapot.write(PositionCapot);  
                delay(1000); 
                PositionDistri = 0;
                MaServoPq.write(PositionDistri);   

                monEcran.clear(); // on efface l'écran
                monEcran.setCursor(16,0); //on se positionne sur la colonne 15 de la première ligne
                monEcran.print("Livraison PQ rose....");  // Livraison du PQ selon le choix
                delay(3000);
                                                 }  // Fin de if Led Rose livraison                  

              if ((EtatLedBleu == 1) && (EtatFinLivraison == 1)) {
                PositionCapot = 20;
                MaServoCapot.write(PositionCapot);  
                delay(1000); 
                PositionDistri = 160;
                MaServoPq.write(PositionDistri);  
                  
                monEcran.clear(); // on efface l'écran
                monEcran.setCursor(16,0); //on se positionne sur la colonne 12 de la première ligne
                monEcran.print("Livraison PQ bleu....");  // confirmation du choix
                delay(1000);                                      
                                                  } // Fin de if Led bleu et livraison
                                                      
            } // fin de boucle LOOP


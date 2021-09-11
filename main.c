#include "U:\Semestre 2\Architecture\Projet Archi\main.h"
#define c1on  output_high(pin_d0)           //Allumage colonne digicode 1
#define c1off  output_low(pin_d0)           //Extinction colonne digicode 1
#define c2on  output_high(pin_d1)           //...
#define c2off  output_low(pin_d1)
#define c3on  output_high(pin_d2)
#define c3off  output_low(pin_d2)
#define l1  input(pin_d3)                   //Capteur de ligne 1 . 2 . 3
#define l2  input(pin_d4)
#define l3  input(pin_d5)
#define l4  input(pin_d6)

//buzzer on et off
#define buzzer_on output_high(pin_a0)
#define buzzer_off output_low(pin_a0)

//Leds on
#define ledAlarmeActive output_high(pin_c0)
#define ledAlarmeDesactive output_low(pin_c0)

#define Boucle1 output_high(pin_c1)                 //Allumage Leds 1 à 6
#define Boucle2 output_high(pin_c2)
#define Boucle3 output_high(pin_c5)
#define Boucle4 output_high(pin_c4)
#define Boucle5 output_high(pin_e0)
#define Boucle6 output_high(pin_e1)
//Leds off
#define Leds_zones1234OFF output_c(0)              //Extinction de toutes les Leds (y compris celle de l'alarme active)
#define Leds_zones56OFF output_e(0)

//capteurs input zones
#define CapteurZone1 input(pin_b1)
#define CapteurZone2 input(pin_b2)
#define CapteurZone3 input(pin_b3)
#define CapteurZone4 input(pin_b4)
#define CapteurZone5 input(pin_b5)
#define CapteurZone6 input(pin_b6)

//sirene on et off
#define sireneon output_high(pin_b7)
#define sireneoff output_low(pin_b7)

int16 code_armement=33,code_desarmement=1236;
int16 temps_desarmement = 20, temps_alarme = 180, MAX_temps_alarme = 180;  //La durée de l'armement, du désarmement et de l'alarme.
int16 sec;
int16 timer,timer2;  //Les timers
int32 n;
int32 code_saisi; //int32 pour éviter le débordement
int16 i;
int16 MAXRearmement = 2, nbRearmement;              //2 réarmements possible pour l'alarme
int1 ArmementAlarme,alarme_active;                  //changement de valeur si alarme en activation ou activee
int16 retard_sortie=30;

boolean intrusion = false;    //Booléen pour indiquer si intrusion détectée
boolean code_bon = false;     //Booléen si code bon saisi
void clearScreen(void){
   printf("%c%c",254,1); //Efface écran LCD
}
#int_TIMER1
void  TIMER1_isr(void) 
{ int8 dix;
  set_timer1(3036); //on passe ici tous les dixiemes de seconde
  dix++;
  if(dix==10) {
      // on passe ici toutes les secondes
      sec++;
      dix=0;
      timer--;
  }
}

#int_EXT
void  EXT_isr(void) 
{ //lecture colonne1
    c1on;c2off;c3off;
    if (l1){
        printf("1");
        n=10*n+1;
    }
    if (l2){
        printf("4");
        n=10*n+4;
    }
    if (l3){
        printf("7");
        n=10*n+7;
    }
    if (l4){
        printf("*");
         
    }
    //lecture colonne2
    c1off;c2on;c3off;
    if (l1){
        printf("2");
        n=10*n+2;
    }
    if (l2){
        printf("5");
        n=10*n+5;
    }
    if (l3){
        printf("8");
        n=10*n+8;
    }
    if (l4){
        printf("0");
        n=10*n; 
    }
    
     //lecture colonne3
    c1off;c2off;c3on;
    if (l1){
        printf("3");
        n=10*n+3;
    }
    if (l2){
        printf("6");
        n=10*n+6;
    }
    if (l3){
        printf("9");
        n=10*n+9;
    }
    if (l4){
        code_saisi=n;n=0;
        printf("\n\r");
        //printf("code saisi:%lu\n\);
        if (code_saisi==code_armement) {
               printf("   alarme active dans %lu secondes \n\r",retard_sortie);
               timer=retard_sortie;timer2=timer;
               ArmementAlarme=1;   //Activation de l'alarme
               nbRearmement = MAXRearmement; //NbRearmement reprend sa valeur initiale après armement.
               Leds_zones1234OFF;
               Leds_zones56OFF;     //Extinctions des Leds et annonce de l'extinction de l'alarme
             
        }
        if (code_saisi==code_desarmement) {
                    printf("   alarme desaactivee\n\r");
                    alarme_active = 0;    //Coupe l'alarme
                    ArmementAlarme = 0;
                    ledAlarmeDesactive;   //Eteind la led alarme
                    code_saisi = 0;       //Le code saisi utilisé repasse à 0
                    delay_ms(3000);
                    clearScreen();
        }
        if(code_saisi/100 == 10){
                  code_armement = code_saisi%100;
                  clearScreen();
                  printf(" Le nouveau code     armement est : %ld \n\r", code_armement);
                  delay_ms(5000);
                  clearScreen();
        }
        if(code_saisi/10000 == 11){
                  code_desarmement = code_saisi%10000;
                  clearScreen();
                  printf(" Le nouveau code desarmement est : %ld \n\r", code_desarmement);
                  delay_ms(5000);
                  clearScreen();
        }
        if(code_saisi/100 == 20){
                  retard_sortie = code_saisi%100;
                  clearScreen();
                  printf(" Le nouveau temps de sortie est : %ld sec\n\r", retard_sortie);
                  delay_ms(5000);
                  clearScreen();
        }
        if(code_saisi/100 == 21){
                  temps_desarmement = code_saisi%100;
                  clearScreen();
                  printf(" Le nouveau temps de desarmement est : %ld \n\r", temps_desarmement);
                  delay_ms(5000);
                  clearScreen();
         }
         if(code_saisi/1000 == 30){
                  temps_alarme = code_saisi%1000;
                  if(temps_alarme > 180){temps_alarme = MAX_temps_alarme;}
                  clearScreen();
                  printf(" Le nouveau temps d'alarme est : %ld sec\n\r", temps_alarme);
                  delay_ms(5000);
                  clearScreen();
         }
         if(code_saisi/100 == 31){
                  MAXRearmement = code_saisi%100;
                  clearScreen();
                  printf(" Le nouveau nombre de rearmement est : %ld \n\r", MAXRearmement);
                  delay_ms(5000);
                  clearScreen();
         }
         if(code_saisi==99000){
                  code_armement=33;
                  code_desarmement=1664;
                  temps_desarmement = 20;
                  temps_alarme = 180;
                  MAXRearmement = 2;
                  retard_sortie=30;
                  clearScreen();
                  printf(" Valeurs             reinitialisee");
                  delay_ms(5000);
                  clearScreen();
         }
    }
    c1on;c2on;c3on;
}
/*          Tentative d'une procédure alarme
void alarme(int sec){
   for(i = 0; i < sec;i++){
      delay_ms(1000);
      sireneon;
      buzzer_on;
   }
   buzzer_off;
   sireneoff;
}
*/
void main()
{

   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_CLOCK_DIV_2);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
   setup_timer_2(T2_DISABLED,0,1);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(INT_EXT);
   enable_interrupts(GLOBAL);
//Setup_Oscillator parameter not selected from Intr Oscillator Config tab

   // TODO: USER CODE!!
   c1on;c2on;c3on;
   
   while(true) { 
                  
                  if (ArmementAlarme && timer>0 && timer2!=timer) { //Timer d'armement de l'alarme
                      
                      if(!Capteurzone1 || !CapteurZone2 || !CapteurZone3 || !Capteurzone4){     //On ne met pas les zones 5 et 6 puisque ces dernières sont différés et sont des points de passage après l'armement.
                        clearScreen();
                        buzzer_off;
                        if(!capteurzone1){
                           printf("\n   Ouverture Zone 1\r");
                           Boucle1;}
                        if(!capteurzone2){
                           printf("\n   Ouverture Zone 2\r");
                           Boucle2;}
                        if(!capteurzone3){
                           printf("\n   Ouverture Zone 3\r");
                           Boucle3;}
                        if(!capteurzone4){
                           printf("\n   Ouverture Zone 4\r");
                           Boucle4;}
                        ArmementAlarme = 0;
                        return;
                      }
                      buzzer_on;
                      timer2=timer;
                      clearScreen();
                      printf("  Alarme activee dans   %lu secondes \n\r",timer);   //affichage alarme
                      delay_ms(1000);
                  }
                  if (ArmementAlarme && timer<=0) {        //Une fois l'armement terminé
                  
                      //nbRearmement = MAXRearmement; //NbRearmement reprend sa valeur initiale après armement.
                      buzzer_off;            //Coupure buzzer
                      ArmementAlarme=0;                 //L'armement est terminé
                      alarme_active=1;       //L'alarme est activée
                      ledAlarmeActive;       //Allumage de la led d'alarme active
                      clearScreen();
                      printf("  Alarme enclenchee\n\r");
                  } 
                  while (nbRearmement >=0 && alarme_active){  //Tant que le nombre de réarmement est supérieur à 0 et alarme active
                  
                  if (alarme_active && !CapteurZone1){      //Capteur zone 1 débranché
                      
                      printf("  Intrusion zone 1\n\r");  //Annonce de l'alarme
                      Boucle1;                     //Allumage led zone 1
                      intrusion = true;            //Le booléen d'intrusion passe en true;
                  }
                  if (alarme_active && !CapteurZone2){
                      
                      printf("  Intrusion zone 2\n\r");
                      Boucle2;
                      intrusion = true;
                  }
                  if (alarme_active && !CapteurZone3){
                      
                      printf("  Intrusion zone 3\n\r");
                      Boucle3;
                      intrusion = true;
                  }
                  if (alarme_active && !CapteurZone4){
                      
                      printf("  Intrusion zone 4\n\r");
                      Boucle4;
                      intrusion = true;
                  }
                  if (alarme_active && !CapteurZone5){
                                                     //Alarme retardée de 20 secondes
                      printf("  Intrusion zone 5\n\r");
                      Boucle5;
                      clearScreen();
                      for(i =0; i <= temps_desarmement;i++){
                        clearScreen();
                        buzzer_on;
                        printf("  Alarme   enclenchee dans %ld secondes \n\r", temps_desarmement-i);   //Décompte sur terminal
                        delay_ms(1000);                                                            //délai d'1 seconde entre chaque bip
                        //if(code_saisi == code_armement){alarme_active = 0; crb = 0; buzzer_off; break;}
                        if(alarme_active == 0){
                           code_bon = true;
                           buzzer_off;
                           return;
                        } //si alarme active alors affichage, code bon devient vrai et break.
                        
                       }
                       if(alarme_active == 1){
                           intrusion = true;
                           clearScreen();
                           printf("   Intrusion zone 5 \n\r");
                           printf("Sirene %ld secondes", temps_alarme);
                       }                        //Si le code est bon dans le temps imparti, alors rien ne se passe
                                                //Si tel n'est pas le cas, alors l'alarme s'enclenche
                  }
                  if (alarme_active && !CapteurZone6){
                                                      //Alarme retardée de 20 secondes
                      printf("  Intrusion zone 6\n\r");
                      Boucle6;
                      clearScreen();
                      for(i =0; i <= temps_desarmement;i++){
                        buzzer_on;
                        clearScreen();
                        printf("  Alarme   enclenchee dans %ld secondes \n\r", temps_desarmement-i);   
                        delay_ms(1000);
                        //if(code_saisi == code_armement){alarme_active = 0; crb = 0; buzzer_off; break;}
                        if(alarme_active == 0){
                           code_bon = true;
                           buzzer_off;
                           return;
                        }
                        
                       }
                       if(alarme_active == 1){
                           intrusion = true;
                           clearScreen();
                           printf("   Intrusion zone 6  \n\r");
                           printf("Sirene %ld secondes", temps_alarme);
                       }
                  }
                  
                  
                  //Si le booléen intrusion est vrai, l'alarme se déclenche ici. 
                  //Si plusieurs zones sont touchées, l'alarme ne s'active qu'une fois.
                  if(intrusion == true){
                     buzzer_on;     //Allumage du buzzer et de la sirene
                     sireneon;
                     for(i=0;i<temps_alarme;i++){  //Ici, l'alarme sonne pendant le temps déterminé.
                        //clearScreen();
                        //printf("Alarme coupee dans \n %ld secondes \n\r", temps_alarme - i);           Ici, il y a un décompte pour la fin de l'alarme 
                        delay_ms(1000);
                        if(alarme_active == 0){return;}   //Si l'alarme est arrêtée alors les 180 secondes s'arrêtent
                     }
                     buzzer_off;
                     sireneoff;                             //Une fois les 3 minutes passées, on coupe la sirene et le buzzer
                     Intrusion = false;                     //Le booléen intrusion passe à 0, alarme finie
                     //nbRearmement = nbRearmement -1;  //L'alarme perd un réarmement possible
                     if(nbRearmement <= 0){                 //Si le nombre de réarmement restant est inférieur ou égal à 0, alors l'alarme s'éteind.
                        alarme_active = 0;                  //L'alarme se coupe.
                        clearScreen();
                        ledAlarmeDesactive;
                        printf("  Le nombre de rearmements est nul.\n\r");
                        delay_ms(4000);
                        clearScreen();
                        printf("   Veuillez rearmer   avec le code\n\r");
                        delay_ms(5000);
                        clearScreen();
                        return;
                     }else{
                        if(nbRearmement > 0){ //S'il reste un ou plusieurs réarmements possible
                           clearScreen();
                           ledAlarmeDesactive;
                           printf("   il reste %ld rearmement(s) automatique\n\r", nbRearmement);
                           delay_ms(3000);
                           clearScreen();
                           printf("  Alarme en cours de rearmement\n\r");
                           delay_ms(5000);
                           clearScreen();              //Nettoie le terminal de tout le texte
                           ArmementAlarme = 1;                    //Rearme l'alarme
                           alarme_active = 0;
                           nbRearmement = nbRearmement -1;  //L'alarme perd un réarmement possible
                        }
                     }
                     timer=retard_sortie;timer2=timer;   //Remet les timers à de bonnes valeurs
                     }
                  }
            }
}

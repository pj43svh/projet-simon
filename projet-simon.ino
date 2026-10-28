/*
Auteur : Théo Läderach
Date : 01.10.2026
Description : jeu du Simon
*/

int BLUE_LED_PIN = 2;
int YELLOW_LED_PIN = 3;
int RED_LED_PIN = 4;
int GREEN_LED_PIN = 5;
int BUZZER_PIN = 6;

int BLUE_BTN_PIN = A2;
int YELLOW_BTN_PIN = A3;
int RED_BTN_PIN = A4;
int GREEN_BTN_PIN = A5;

int BLUE = 1;
int YELLOW = 2;
int RED = 3;
int GREEN = 4;

int OFF = 0;
int DURATION = 200;
int BLUE_SOUND = 329;
int YELLOW_SOUND = 261;
int RED_SOUND = 220;
int GREEN_SOUND = 164;
int VICTORY_SOUND = 1000;
int ERROR = 65;

int sequence[30];
int sequence_index = 0;
int current_color = 0;
int current_index = 0;
int nextColor;

bool failed = false;

#include <Bounce2.h>

Bounce Blue_btn = Bounce();
Bounce Yellow_btn = Bounce();
Bounce Red_btn = Bounce();
Bounce Green_btn = Bounce();

void setup() {

  Blue_btn.attach(BLUE_BTN_PIN, INPUT_PULLUP);
  Yellow_btn.attach(YELLOW_BTN_PIN, INPUT_PULLUP);
  Red_btn.attach(RED_BTN_PIN, INPUT_PULLUP);
  Green_btn.attach(GREEN_BTN_PIN, INPUT_PULLUP);

  pinMode(BLUE_LED_PIN,OUTPUT);
  pinMode(YELLOW_LED_PIN,OUTPUT);
  pinMode(GREEN_LED_PIN,OUTPUT);
  pinMode(RED_LED_PIN,OUTPUT);
  pinMode(BUZZER_PIN,OUTPUT);

}

void loop() {
  // Attente que le bouton bleu soit appuyé pour démarrer le jeu
  while (!Blue_btn.fell()){
    Blue_btn.update();
    // Pour éviter les répétitions, utiliser le temps que le joueur met à réfléchir afin d'obtenir la couleur la plus aléatoire possible.
    // PS : je me suis basé sur le système d'aléatoire de TempleOS.
    nextColor = random(1,5);
  };

  // Animation de démarrage
  for (int i = 1; i < 5; i++){
    led(i);
    delay(100);
  }
  for (int i = 4; i >= 1; i--){
    led(i);
    delay(100);
  }
  victory();

  delay(1000);
  // Initialisation de la partie
  sequence_index = 0;
  sequence[sequence_index] = nextColor;
  show_sequence(300);
  failed = false;
  current_index = 0;

  // Boucle principale du jeu
  while (!failed){

    // Actualisation de l'état des boutons
    Blue_btn.update();
    Yellow_btn.update();
    Red_btn.update();
    Green_btn.update();

    current_color = 0;

    // Vérifie si l'un des boutons est appuyé
    if (Blue_btn.fell()){
      current_color = BLUE;
      led(BLUE);
    }
    if (Yellow_btn.fell()){
      current_color = YELLOW;
      led(YELLOW);
    }
    if (Red_btn.fell()){
      current_color = RED;
      led(RED);
    }
    if (Green_btn.fell()){
      current_color = GREEN;
      led(GREEN);
    }

    nextColor = random(1,5);

    if (current_color != 0){
      // Vérifie si la couleur choisie est correcte
      if (current_color == sequence[current_index]){
        // Si la couleur est correcte, on passe à la suivante
        current_index++;
      }else{
        failed = true;
        error();
      }
      if (current_index == (sequence_index+1)){
        if (sequence_index >= 30) {
          // Si on dépasse la longueur maximale de la séquence, victoire.
          delay(DURATION*2);
          failed = true;
          victory();
        }else{
          // Si on n'est pas encore à la fin, on affiche la prochaine séquence.
          current_index = 0;
          delay(DURATION*2);
          sequence_index ++;
          sequence[sequence_index] = nextColor;
          show_sequence(300);
        }
      }
    }
  }
}


void led(int color){
  /*
  Allume la LED correspondante et émet le son associé
  */
  int ledpin = 0;
  int sound = 0;

  if (color == BLUE){
    ledpin = BLUE_LED_PIN;
    sound = BLUE_SOUND;
  }else if (color == YELLOW){
    ledpin = YELLOW_LED_PIN;
    sound = YELLOW_SOUND;
  }else if (color == RED){
    ledpin = RED_LED_PIN;
    sound = RED_SOUND;
  }else if (color == GREEN){
    ledpin = GREEN_LED_PIN;
    sound = GREEN_SOUND;
  }else{
    return;
  }

  digitalWrite(ledpin,HIGH);
  tone (BUZZER_PIN, sound);

  delay(DURATION);

  digitalWrite(ledpin,LOW);
  noTone(BUZZER_PIN);

}

void error(){
  /*
  Allume toutes les LED et émet le son d'erreur.
  */
  for (int ledPin = 2; ledPin < 6; ledPin++){
    digitalWrite(ledPin,HIGH);
  }
  tone(BUZZER_PIN, ERROR);

  delay(DURATION*2);

  for (int ledPin = 2; ledPin < 6; ledPin++){
    digitalWrite(ledPin,LOW);
  }
  noTone(BUZZER_PIN);
}

void victory(){
  /*
  Allume toutes les LED et émet le son de victoire.
  */
  for (int _ = 0; _ < 3; _++){
    for (int ledPin = 2; ledPin < 6; ledPin++){
      digitalWrite(ledPin,HIGH);
    }
    tone(BUZZER_PIN, VICTORY_SOUND);

    delay(DURATION/2);

    for (int ledPin = 2; ledPin < 6; ledPin++){
      digitalWrite(ledPin,LOW);
    }
    noTone(BUZZER_PIN);
    delay(DURATION/2);
    }
}

void show_sequence(int delay_time){
  /*
  Affiche la séquence de couleurs à l'utilisateur
  */
  for (int i = 0; i <= sequence_index; i++){
    led(sequence[i]);
    delay(delay_time);
  }
}

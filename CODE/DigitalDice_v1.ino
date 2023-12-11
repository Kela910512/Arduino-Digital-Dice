/******** INCLUDES ********/
#include <TM1637Display.h>
#include "pitches.h"


/******** SET PINS ********/
const byte buzzerPin = 10;       // BUZZER
const byte menuButtonPin = 11;   // CHANGE MENU BUTTON
const byte buttonPin = 12;       // ROLL | GENERATE | CHANGE BUTTON

/******** TM1637 DISPLAY OBJECT ********/
TM1637Display display(8, 9);

/******** SET TM1637 DISPLAY SEGMENTS FOR OPTIONS ********/
const byte OFF[] = {
  0, 0, 0, 0
};

const byte MENU[] = {
  SEG_G,
  SEG_G,
  SEG_G,
  0
};

const byte AUTO_EXIT_ON[] = {
  SEG_A| SEG_F| SEG_G| SEG_E| SEG_D,
  0,
  SEG_B| SEG_C,
  0
};

const byte AUTO_EXIT_OFF[] = {
  SEG_A| SEG_F| SEG_G| SEG_E| SEG_D,
  0,
  SEG_A| SEG_F| SEG_B| SEG_E| SEG_C| SEG_D,
  0
};

const byte SOUND_ON[] = {
  SEG_A| SEG_F| SEG_G| SEG_C| SEG_D,
  SEG_A| SEG_F| SEG_B| SEG_E| SEG_C| SEG_D,
  SEG_F| SEG_B| SEG_E| SEG_C| SEG_D,
  SEG_B| SEG_C
};

const byte SOUND_OFF[] = {
  SEG_A| SEG_F| SEG_G| SEG_C| SEG_D,
  SEG_A| SEG_F| SEG_B| SEG_E| SEG_C| SEG_D,
  SEG_F| SEG_B| SEG_E| SEG_C| SEG_D,
  SEG_G
};

/******** FOR BUZZER MELODY ********/
int winnerMelody[] = {
  NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5
};

int winnerMelodyDurations[] = {
  4, 4, 4, 4
};

int loserMelody[] = {
  NOTE_A3, NOTE_AS3, NOTE_A3
};

int loserMelodyDurations[] = {
  8, 8, 4
};

/******** VARIABLES ********/
bool buttonState = LOW;
bool menuButtonState = LOW;
bool lastButtonState = LOW;
bool lastMenuButtonState = LOW;
bool shouldUpdateDisplay = true;
bool autoExit = 0;
bool buzzerState = 0;
byte menuItem = 1;
short randNumber = 0;
short DELAY = 3000;
char lastButton = '-';

void setup() {
  pinMode(menuButtonPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  display.setSegments(OFF);
  display.setBrightness(6);
  display.setSegments(MENU);
  display.showNumberDec(menuItem, false, 1, 3);
}

/******** LOOP ********/
void loop() {
  // READ BUTTONS STATE
  buttonState = digitalRead(buttonPin);
  menuButtonState = digitalRead(menuButtonPin);

  // CHANGE MENU
  if (menuButtonState == HIGH && lastMenuButtonState == LOW){
    lastMenuButtonState = HIGH;
    shouldUpdateDisplay = true; // NEED TO UPDATE THE DISPLAY
    menuItem <= 6 ? menuItem += 1 : menuItem = 1; // INCREASE THE VALUE OF THE MENU ITEM
    shouldUpdateDisplay == true ? setDisplay('M', 0, menuItem) : none();
  }   

  // AFTER WE PUSHED THE MENU BUTTON
  if (menuButtonState == LOW && lastMenuButtonState == HIGH){
    lastMenuButtonState = LOW; // WE NEED TO CHANGE THE LAST STATE OF THE MENU BUTTON
  }
  
  // ENTER CURRENT MENU: ROLLING THE DICE | GENERATE RANDOM NUMBER | CHANGE SETTINGS
  if (buttonState == HIGH && lastButtonState == LOW){
    lastButtonState = HIGH;
    shouldUpdateDisplay = true;
    (shouldUpdateDisplay == true) ? menuControl(menuItem) : none();
  }

  // AFTER WE PUSHED THE FUNCTION BUTTON (ROLLING | GENERATE | SETTINGS)
  if (buttonState == LOW && lastButtonState == HIGH){
    lastButtonState = LOW;
  }
  
  delay(100);
}

/******** DISPLAY CONTROL ********/
void setDisplay(char type, int num, int menuItem){
  display.setSegments(OFF);
  // SHOW THE MENU
  if (type == 'M'){
    display.setSegments(MENU);
    display.showNumberDec(menuItem, false, 1, 3);
  }

  // SHOW THE ROLLED | GENERATED NUMBER
  if (type == 'R'){
    display.showNumberDec(num, false, menuItem, 0);
  }

  // SHOW SOUND SETTING
  if (type == 'S'){
    (buzzerState == 1) ? display.setSegments(SOUND_ON) : display.setSegments(SOUND_OFF);
  }

  // SHOW AUTO EXIT SETTING
  if (type == 'E'){
    (autoExit == 0) ? display.setSegments(AUTO_EXIT_OFF) : display.setSegments(AUTO_EXIT_ON);
  }

  // SHOW AUTO EXIT TIME
  if (type == 'D'){
    display.showNumberDec(num);
  }
   
  shouldUpdateDisplay = false;
}

/******** MENU CONTROL ********/
void menuControl(int menuItem) {
  switch (menuItem) {
    
    // ROLLING A DICE
    case 1:
      randNumber = random(1,7);
      setDisplay('R', randNumber, menuItem);
      // PLAYING THE WINNER MELODY IF THE ROLLED NUMBER IS 6 AND THE SOUND OPTION IS ON
      (randNumber == 6 && buzzerState == 1) ? playMelody(winnerMelody, winnerMelodyDurations, sizeof(winnerMelody) / sizeof(winnerMelody[0])) : none();
      // PLAYING THE LOSING MELODY IF THE ROLLED NUMBER IS 1 AND THE SOUND OPTION IS ON
      (randNumber == 1 && buzzerState == 1) ? playMelody(loserMelody, loserMelodyDurations, sizeof(loserMelody) / sizeof(loserMelody[0])) : none();
      // IF AUTO EXIT IS ENABLED, DELAY IS ACTIVATED
      (autoExit == 0) ? none() : delay(DELAY);
      // IF AUTO EXIT IS ENABLED, YOU WILL BE AUTOMATICALLY TAKEN BACK TO THE MENU
      (autoExit == 0) ? none() : setDisplay('M', 0, menuItem);
      return;
      
    // ROLLING TWO DICE
    case 2:
      randNumber = random(2,13);
      setDisplay('R', randNumber, menuItem);
      // PLAYING THE WINNER MELODY IF THE ROLLED NUMBER IS 12 AND THE SOUND OPTION IS ON
      (randNumber == 12 && buzzerState == 1) ? playMelody(winnerMelody, winnerMelodyDurations, sizeof(winnerMelody) / sizeof(winnerMelody[0])) : none();
      // PLAYING THE LOSING MELODY IF THE ROLLED NUMBER IS 2 AND THE SOUND OPTION IS ON
      (randNumber == 2 && buzzerState == 1) ? playMelody(loserMelody, loserMelodyDurations, sizeof(loserMelody) / sizeof(loserMelody[0])) : none();
      // IF AUTO EXIT IS ENABLED, DELAY IS ACTIVATED
      (autoExit == 0) ? none() : delay(DELAY);
      // IF AUTO EXIT IS ENABLED, YOU WILL BE AUTOMATICALLY TAKEN BACK TO THE MENU
      (autoExit == 0) ? none() : setDisplay('M', 0, menuItem);
      return;
      
    // GENERATE RANDOM NUMBER BETWEEN 100 AND 1000
    case 3:
      setDisplay('R', random(100,1000), menuItem);
      // IF AUTO EXIT IS ENABLED, DELAY IS ACTIVATED
      (autoExit == 0) ? none() : delay(DELAY);
      // IF AUTO EXIT IS ENABLED, YOU WILL BE AUTOMATICALLY TAKEN BACK TO THE MENU
      (autoExit == 0) ? none() : setDisplay('M', 0, menuItem);
      return;
      
    // GENERATE RANDOM NUMBER BETWEEN 1 AND 10000
    case 4:
      setDisplay('R', random(1,10000), menuItem);
      // IF AUTO EXIT IS ENABLED, DELAY IS ACTIVATED
      (autoExit == 0) ? none() : delay(DELAY);
      // IF AUTO EXIT IS ENABLED, YOU WILL BE AUTOMATICALLY TAKEN BACK TO THE MENU
      (autoExit == 0) ? none() : setDisplay('M', 0, menuItem);
      return;
      
    // SET THE SOUND ON/OFF
    case 5:
      (buzzerState == 0) ? buzzerState = 1 : buzzerState = 0;
      setDisplay('S', 0, 0);
      delay(3000);
      setDisplay('M', 0, 1);
      setDeafultMenuItem();
      return;
      
    // SET THE AUTO EXIT ON/OFF
    case 6:
      (autoExit == 0) ? autoExit = 1 : autoExit = 0;
      setDisplay('E', 0, 0);
      delay(3000);
      setDisplay('M', 0, 1);
      setDeafultMenuItem();
      return;
      
    // SET THE AUTO EXIT TIME BETWEEN 3 SEC AND 9,5 SEC
    case 7:
      unsigned long int lastDelayChange = millis();
      // SETTINGS WILL BE SAVED WITHOUT PRESSING THE BUTTON FOR 3 SECONDS
      while(3000 > (millis() - lastDelayChange)){
        buttonState = digitalRead(buttonPin);
        if(buttonState == HIGH ){
          lastDelayChange = millis();
          (DELAY == 9500) ? DELAY = 3000 : DELAY += 500;
          setDisplay('D', DELAY, 0);
          delay(200);
        }
      }
      lastDelayChange = 0;
  }
  
  setDeafultMenuItem();
  setDisplay('M', 0, 1); // IT IS 1 and not 'menuItem', BECAUSE THE DEFAULT MENU VALUE IS NOT YET SET DUE TO THE FAST RUNNING
  return;
}

/******** PLAYING MELODY ********/
void playMelody(int melody[], int noteDurations[], int length) {
  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    int pauseBetweenNotes = noteDuration * 1.30;
    tone(buzzerPin, melody[i], noteDuration);
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}

/******** SET THE MENU ITEM FOR DEFAULT (1) ********/
void setDeafultMenuItem(){
  menuItem = 1;
}

/******** NOTHING TO DO: FOR TERNARY IF STATEMENT ********/
void none(){
  return;
}

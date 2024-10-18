#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DEBOUNCE_TIME 200

LiquidCrystal_I2C lcd(0x27, 16, 2);
unsigned long time_started = 0;
int deltaT;
int currentDelta;

const int BUTTONS[] = {2, 3, 4, 5};
const int LEDS[] = {8, 9, 10, 11};
long prevts[4];
bool ledStates[] = {LOW, LOW, LOW, LOW};

//variables needed for level selector (potentiometer)
int currentValue;
int newValue;
int difficulty;

bool isGameStarted = false;
bool shouldDisplayNumber = true;
int randomNum = 0;


void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  randomSeed(analogRead(0));

  currentValue = 0;

  for (int i = 0; i < 4; i++) {
    pinMode(BUTTONS[i], INPUT);
    pinMode(LEDS[i], OUTPUT);
    prevts[i] = 0;
  }
}

void loop() {
  if (!isGameStarted) {
    chooseDifficulty();
    bool startButtonState = digitalRead(BUTTONS[0]);
    if (startButtonState) {
      isGameStarted = true;
      delay(1000);
    }
  } 
  else {
    if (shouldDisplayNumber) {
      randomNum = random(1, 16);
      lcd.print(randomNum, DEC);
      shouldDisplayNumber = false;
      time_started = millis();
    } else {
      no_more_time();
      for (int i = 0; i < 4; i++) {
        handle_button(BUTTONS[i], LEDS[i], i);
        if (randomNum == convertButtonsToDecimal()) {
          shouldDisplayNumber = true;
        }
      }
    }
  }
}


void chooseDifficulty() {
    //read new potentiometer value from analog pin
  newValue = analogRead(A1);
  //compare if new value is different from the previous
  if(newValue != currentValue){
    //assign new value
    currentValue = newValue;
    //assigns the difficulty level according to potentiometer values
    //if new difficulty is same as previous it DOES NOT print it again (&& difficulty != x)
    if(currentValue >= 0 && currentValue <= 256 && difficulty != 1){
      Serial.println("EASY :)");
      difficulty = 1;
    }else if(currentValue > 256 && currentValue <= 512 && difficulty != 2){
      Serial.println("MEDIUM :|");
      difficulty = 2;
    }else if(currentValue > 512 && currentValue <= 768 && difficulty != 3){
      Serial.println("HARD >:)");
      difficulty = 3;
    }else if(currentValue > 768 && difficulty != 4){
      Serial.println("EXPERT >:D");
      difficulty = 4;
    }
  }
}


void handle_button(int BUTTON, int LED, int i) {
  int button_state = digitalRead(BUTTON);
  long ts = millis();
  if (ts - prevts[i] > DEBOUNCE_TIME) {
    if (button_state == HIGH) {
      ledStates[i] = !ledStates[i];
      digitalWrite(LED, ledStates[i]);
      prevts[i] = ts;
    }
  }
}


int convertButtonsToDecimal() {
  int decimal = 0;
  for (int i = 0; i < 4; i++) {
    decimal += ledStates[i] << (i);
  }
  return decimal;
}

void no_more_time(){
  deltaT = (millis() - time_started)/1000 ;
  
  if(currentDelta != deltaT){
    Serial.println(5 - currentDelta);
  }
  currentDelta = deltaT;
  if(currentDelta > 5){
    Serial.println("Game over");
    isGameStarted = false;
    shouldDisplayNumber = true;
    lcd.clear();
  }
}
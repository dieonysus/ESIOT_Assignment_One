#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DEBOUNCE_TIME 200

LiquidCrystal_I2C lcd(0x27, 16, 2);
unsigned long time_started = 0;
int deltaT;
int currentDelta;

const int BUTTONS[] = {2, 3, 4, 5};
const int LEDS[] = {8, 9, 10, 11};
bool ledStates[] = {LOW, LOW, LOW, LOW};
long prevButtonPressTime[4];

int currentPotValue = 0;
int difficulty = 1;

bool isIntroDisplayed = false;
bool isDifficultySelected = false;
bool isGameStarted = false;
bool isGameOver = false;

bool shouldDisplayNumber = true;
int targetNumber = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  randomSeed(analogRead(0));

  for (int i = 0; i < 4; i++) {
    pinMode(BUTTONS[i], INPUT);
    pinMode(LEDS[i], OUTPUT);
    prevButtonPressTime[i] = 0;
  }
}

void loop() {
  if (!isIntroDisplayed) {
    displayIntro();
  }
  else if (!isDifficultySelected) {
    selectDifficulty();
  } 
  else if (!isGameStarted) {
    startGame();
  } 
  else if (!isGameOver) {
    playGame();
  } 
  else {
    finishGame();
  }
}


void displayIntro() {
  lcd.clear();
  lcd.print("Difficulty");
  isIntroDisplayed = true;
}


void selectDifficulty() {
  int newPotValue = analogRead(A1);
  if(currentPotValue != newPotValue){
    currentPotValue = newPotValue;
    if(currentPotValue >= 0 && currentPotValue <= 256 && difficulty != 1){
      lcd.clear();
      lcd.print(" :) EASY");
      difficulty = 1;
    }else if(currentPotValue > 256 && currentPotValue <= 512 && difficulty != 2){
      lcd.clear();
      lcd.print(" :| MEDIUM");
      difficulty = 2;
    }else if(currentPotValue > 512 && currentPotValue <= 768 && difficulty != 3){
      lcd.clear();
      lcd.print(">:) HARD");
      difficulty = 3;
    }else if(currentPotValue > 768 && difficulty != 4){
      lcd.clear();
      lcd.print(">:D EXPERT");
      difficulty = 4;
    }
  }
  if (digitalRead(BUTTONS[0])) {
    isDifficultySelected = true;
  }
}


void startGame() {
  isGameStarted = true;
  lcd.clear();
  lcd.print("Get ready!");
  delay(1000);
}


void playGame() {
  if (shouldDisplayNumber) {
    targetNumber = random(1, 16);
    lcd.clear();
    lcd.print(targetNumber, DEC);
    shouldDisplayNumber = false;
    time_started = millis();
  } else {
    no_more_time();
    handleButtonPresses();
    if (targetNumber == getButtonStatesAsDecimal()) {
      shouldDisplayNumber = true;
      turnOffAllLeds();
    }
  }
}


void handleButtonPresses() {
  for (int i = 0; i < 4; i++) {
    int buttonState = digitalRead(BUTTONS[i]);
    long currentTime = millis();
    if (buttonState == HIGH && currentTime - prevButtonPressTime[i] > DEBOUNCE_TIME) {
      ledStates[i] = !ledStates[i];
      digitalWrite(LEDS[i], ledStates[i]);
      prevButtonPressTime[i] = currentTime;
    }
  }
}


int getButtonStatesAsDecimal() {
  int decimal = 0;
  for (int i = 0; i < 4; i++) {
    decimal += ledStates[i] << (i);
  }
  return decimal;
}


void turnOffAllLeds() {
  for (int i = 0; i < 4; i++) {
    ledStates[i] = LOW;
    digitalWrite(LEDS[i], ledStates[i]);
  }
}


void finishGame() {
  lcd.clear();
  lcd.print("Game over!");
  delay(1000);
  lcd.clear();
  isIntroDisplayed = false;
  isDifficultySelected = false;
  isGameStarted = false;
  isGameOver = false;
}

void no_more_time(){
  deltaT = (millis() - time_started)/1000 ;
  
  if(currentDelta != deltaT){
    Serial.println(5 - currentDelta);
  }
  currentDelta = deltaT;
  if(currentDelta > 5){
    finishGame();
  }
}
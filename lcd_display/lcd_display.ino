#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DEBOUNCE_TIME 200
#define LED_BLINK_INTERVAL 500

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int REDLED = 13;
bool redLedState = LOW;
long previousRedLedBlinkTime = 0;

const int BUTTONS[] = {2, 3, 4, 5};
const int LEDS[] = {8, 9, 10, 11};
bool ledStates[] = {LOW, LOW, LOW, LOW};
long previousButtonPressTime[4];

int currentPotValue = 1;
int difficulty = 0;

bool isIntroDisplayed = false;
bool isDifficultySelected = false;
bool isGameStarted = false;
bool isGameOver = false;

bool shouldDisplayNumber = true;
int targetNumber = 0;

unsigned long timeRoundStart = 0;
int deltaT = 0;
int currentDelta = 0;
int time = 15;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  randomSeed(analogRead(0));

  for (int i = 0; i < 4; i++) {
    pinMode(BUTTONS[i], INPUT);
    pinMode(LEDS[i], OUTPUT);
    previousButtonPressTime[i] = 0;
  }

  pinMode(REDLED, OUTPUT);
}

void loop() {
  if (!isIntroDisplayed) {
    displayIntro();
  } else if (!isDifficultySelected) {
    selectDifficulty();
  } else if (!isGameStarted) {
    startGame();
  } else if (!isGameOver) {
    playGame();
  } else {
    finishGame();
  }
}


void displayIntro() {
  lcd.clear();
  lcd.print("Welcome to GMB!");
  isIntroDisplayed = true;
}


void selectDifficulty() {
  blinkRedLed();

  int newPotValue = analogRead(A1);
  if(currentPotValue != newPotValue){
    currentPotValue = newPotValue;
    if(currentPotValue >= 0 && currentPotValue <= 256 && difficulty != 1){
      lcd.setCursor(0,1);
      lcd.print("          ");
      lcd.setCursor(0,1);
      lcd.print(" :) EASY");
      difficulty = 1;
      time = 15;
    }else if(currentPotValue > 256 && currentPotValue <= 512 && difficulty != 2){
      lcd.setCursor(0,1);
      lcd.print("          ");
      lcd.setCursor(0,1);
      lcd.print(" :| MEDIUM");
      difficulty = 2;
      time = 10;
    }else if(currentPotValue > 512 && currentPotValue <= 768 && difficulty != 3){
      lcd.setCursor(0,1);
      lcd.print("          ");
      lcd.setCursor(0,1);
      lcd.print(">:) HARD");
      difficulty = 3;
      time = 7;
    }else if(currentPotValue > 768 && difficulty != 4){
      lcd.setCursor(0,1);
      lcd.print("          ");
      lcd.setCursor(0,1);
      lcd.print(">:D EXPERT");
      difficulty = 4;
      time = 5;
    }
  }
  if (digitalRead(BUTTONS[0])) {
    isDifficultySelected = true;
    redLedState = LOW;
    digitalWrite(REDLED, redLedState);
  }
}


void blinkRedLed() {
  long currentRedLedTime = millis();
  if (currentRedLedTime - previousRedLedBlinkTime >= LED_BLINK_INTERVAL) {
    previousRedLedBlinkTime = currentRedLedTime;
    redLedState = !redLedState;
    digitalWrite(REDLED, redLedState);
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
    timeRoundStart = millis();
  } else {
    no_more_time();
    handleButtonPresses();
    if (targetNumber == convertButtonsStatesToDecimal()) {
      shouldDisplayNumber = true;
      turnOffAllLeds();
      time = time - (time*0.05);
      deltaT = 0;
      currentDelta = 0;
    }
  }
}


void handleButtonPresses() {
  for (int i = 0; i < 4; i++) {
    int buttonState = digitalRead(BUTTONS[i]);
    long currentTime = millis();
    if (buttonState == HIGH && currentTime - previousButtonPressTime[i] > DEBOUNCE_TIME) {
      ledStates[i] = !ledStates[i];
      digitalWrite(LEDS[i], ledStates[i]);
      previousButtonPressTime[i] = currentTime;
    }
  }
}


int convertButtonsStatesToDecimal() {
  int decimal = 0;
  for (int i = 0; i < 4; i++) {
    decimal += ledStates[i] << (i);
  }
  return decimal;
}


void turnOffAllLeds() {
  for (int i = 0; i < 4; i++) {
    ledStates[i] = LOW;
    digitalWrite(LEDS[i], LOW);
  }
}


void finishGame() {
  lcd.clear();
  lcd.print("Game over!");
  digitalWrite(REDLED, HIGH);
  delay(1000);
  resetGame();
}


void resetGame() {
  lcd.clear();
  isIntroDisplayed = false;
  isDifficultySelected = false;
  isGameStarted = false;
  isGameOver = false;
  timeRoundStart = 0;
  deltaT = 0;
  currentDelta = 0;
  difficulty = 0;
  currentPotValue = 0;
  shouldDisplayNumber = true;
  turnOffAllLeds();
}


void no_more_time(){
deltaT = (millis() - timeRoundStart)/1000 ;
 if(currentDelta != deltaT){
    lcd.setCursor(8,0);
    lcd.print("Time:");
    lcd.setCursor(13,0);
    lcd.print("  ");
    lcd.setCursor(13,0);
    lcd.print(time - currentDelta);
  }
  currentDelta = deltaT;
  if(currentDelta > time){
    finishGame();
  }
}

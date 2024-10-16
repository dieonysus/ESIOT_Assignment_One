#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DEBOUNCE_TIME 150

LiquidCrystal_I2C lcd(0x27, 16, 2);

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
    }
  } 
  else {
    if (shouldDisplayNumber) {
      int randomNum = random(1, 16);
      lcd.print(randomNum, DEC);
      shouldDisplayNumber = false;
    } else {
      for (int i = 0; i < 4; i++) {
        handle_button(BUTTONS[i], LEDS[i], i);
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
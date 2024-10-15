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
int difficulty;
int newValue;

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  randomSeed(analogRead(0));
  int randomNum = random(16);
  lcd.print(randomNum, DEC);
  //current value of potentiometer
  currentValue = 0;

  for (int i = 0; i < 4; i++) {
    pinMode(BUTTONS[i], INPUT);
    pinMode(LEDS[i], OUTPUT);
    prevts[i] = 0;
  }
}

void loop() {
  //read new potentiometer value from analog pin
  newValue = analogRead(A0);
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
      
      Serial.println("MEDIUM ;)");
      difficulty = 2;
      
    }else if(currentValue > 512 && currentValue <= 768 && difficulty != 3){
      
      Serial.println("HARD!");
      difficulty = 3;
      
    }else if(currentValue > 768 && difficulty != 4){
      Serial.println("EXPERT!!!");
      difficulty = 4;
    }
    
     
  }
  for (int i = 0; i < 4; i++) {
    handle_button(BUTTONS[i], LEDS[i], i);
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

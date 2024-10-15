
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DEBOUNCE_TIME 150

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int BUTTONS[] = {2, 3, 4, 5};
const int LEDS[] = {8, 9, 10, 11};
long prevts[4];
bool ledStates[] = {LOW, LOW, LOW, LOW};

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  randomSeed(analogRead(0));
  int randomNum = random(16);
  lcd.print(randomNum, DEC);

  for (int i = 0; i < 4; i++) {
    pinMode(BUTTONS[i], INPUT);
    pinMode(LEDS[i], OUTPUT);
    prevts[i] = 0;
  }
}

void loop() {
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

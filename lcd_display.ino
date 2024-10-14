
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  randomSeed(analogRead(0));
  int randomNum = random(16);
  lcd.print(randomNum, DEC);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

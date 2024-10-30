# Get My Binary 

## Table of Contents 
- [Overview](#overview)
- [How to Play](#how-to-play)
- [How It Works](#how-it-works)
- [Libraries Used](#libraries-used)
- [Key Constants and Variables](#key-constants-and-variables)
## Overview 

Get My Binary is an arduino based game designed to test a player's ability to translate decimal numbers to binary under a time contraint. The game consists of multiple rounds and it will generate a random number between 1 and 15 and the player's goal is to represent that number using binary. The game increases in difficulty with each round by decreasing the allowed response time, creating a fast-paced and challenging experience.

## How to Play

1. Choose the difficult by turning the potentiometer. The game offers four difficulty levels: Easy, Medium, Hard and Expert. The levels affect the time limit of your response. 

2. Press the leftmost button to begin the game 

3. Input your binary answer by pressing the button. Each button corresponds to a green LED light. 
    - Button 1 (leftmost): represents the most significant bit. 
    - Button 4 (rightmost): represents the least significant bit.
4. The LEDS will indicate of you set a bit to 1. 
    - If the LED is turned on it represents 1. 
5. Correct answers will be acknowledged and you will move to the next round in which the time will decrease. If you do not provide the answer during the time given the game is over.

6. Once the game is over you will see your score. If the score is greater than the previous highscore arduino will show it on the main menu. 


## How it Works

### 1. Display the number
At the start of each round a number between 1 and 15 is displayed in decimal notation on the LCD screen.

### 2. LED Setup

There are five LEDS. One red LED and four green LEDS. The green LEDs L1 to L4 represent the binary digits of the displayed number L1 being Most Significat Bit (MSB) and L4 being Least Significant Bit (LSB).

For example, if the number 13 is displayed the correct binary representation would be 1101, therefore LEDS L1, L2 and L4 should be turned on. 

The red LED is there to represent the end and beginning of the game. When the game ends the red LED lights up for one second. 


### 3. Button Controls 

Each button (B1, B2, B3, B4) corresponds to an LED (L1, L2, L3, L4) and pressing a button turns on the corresponding LED. 

### 4. Game Rounds 

The game starts with an initial time limit T. If the player matches the binary configuration within T, they score a point. 

When the next round begins, T will be reduced based on a factor F. The factor F will be based on the difficulty. 


### 5. Game End

If the player fails to complete the binary configuration within the time limit, the red LED will light up for one second. Indicating that the game is over. 


The player's final score is then displayed on the LCD. 


## Libraries Used
1. **LiquidCrystal_I2Ch** and **Wire.h**: used to control LCD that is I2C controlled.
2. **avr/sleep.h**: used for sleep mode for power saving purposes.
3. **EnableEnableInterrupt.h**: Enables handling of button interrupts for responsive button presses and sleep-wake functionality.

## Key Constants and Variables

- `DEBOUNCE_TIME`: Used to prevent multiple triggers from a single button press by waiting 200 ms between reads.
- `LED_BLINK_INTERVAL`: Controls the blinking interval for the red LED (500 ms).
- `WAKE_UP_TIME`: Sets an inactivity period of 10 seconds before the device goes to sleep.
- **LCD Address**: Sets up the LCD screen with I2C address 0x27, 16 columns, and 4 rows.
- **REDLED (pin 13)**: Controls a red LED that indicates system states.
- `LEDS[]`: List taht includes the pins of the green LED lights (can be changed based on your setup).
- `BUTTONS[]`: List that includes the pins of the buttons (can be changed based on your setup) that are used to change status of LED lights.
- **Game States**: `isIntroDisplayed`, `isDifficultySelected`, `isGameStarted`, `isGameOver`, etc., keep track of where the user is in the game flow.
- **Score Variables**: `currentScore` and `highScore` store the user’s score for the current round and highest score achieved.
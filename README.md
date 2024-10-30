# Get My Binary 

## Table of Contents 
- [Overview](#overview)
- [How to Play](#how-to-play)
- [How It Works](#how-it-works)
- [Libraries Used](#libraries-used)
- [Key Constants and Variables](#key-constants-and-variables)
- [Game Setup](#game-setup-setup)
- [Game Flow](#game-flow-loop)
- [Key Functions](#key-functions)
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
- `LEDS[]`: An array that includes the pins of the green LED lights (can be changed based on your setup).
- `BUTTONS[]`: An array that includes the pins of the buttons (can be changed based on your setup) that are used to change status of LED lights.
- **Game States**: `isIntroDisplayed`, `isDifficultySelected`, `isGameStarted`, `isGameOver`, etc., keep track of where the user is in the game flow.
- **Score Variables**: `currentScore` and `highScore` store the user’s score for the current round and highest score achieved.

## Game Setup (`setup()`)
- Initializes serial communication for debugging.
- Sets up the LCD, activates the backlight, and seeds the random number generator.
- Configures each button in the `BUTTONS` array as an input and each `LED` in LEDS as an output.
- Enables interrupts for each button to wake up the microcontroller from sleep when pressed.
- Initializes the last activity time to detect inactivity.

## Game Flow (`loop()`)

The game operates in a sequence of states or stages:

1. **Intro Display**: Shows a welcome message and the current high score.
2. **Difficulty Selection**: Allows players to set a difficulty level via a potentiometer.
3. **Game Start**: Displays a "Get ready!" message and initiates gameplay.
4. **Gameplay**: Displays a target number that players must match by pressing button combinations.
5. **Game Over**: Displays the final score, updates the high score if applicable, and resets the game.

## Key Functions

1. `displayIntro()`: Clears the LCD and displays the welcome screen and high score. Sets isIntroDisplayed to true to ensure the intro is only shown once.

2. `heckInactivity()`: If there's no activity for WAKE_UP_TIME, the device calls goToSleepMode() to save power.

3. `goToSleepMode()`:

- Displays a "Sleeping..." message.
- Puts the device in a low-power mode and disables interrupts.
- On wake-up (button press), re-enables interrupts and restores `lastActivityTime`.


4. `selectDifficulty()`:

- Reads from the potentiometer to adjust the difficulty between four levels (Easy, Medium, Hard, Expert).
- Displays the selected difficulty on the LCD.
- Updates F, which scales the time reduction per correct answer based on difficulty.
- Sets `isDifficultySelected` to true once confirmed with a button press.


5. `blinkRedLed()`: Toggles the state of the red LED every `LED_BLINK_INTERVAL` to provide visual feedback during difficulty selection.

6. `startGame()`: Clears the LCD and displays a "Get ready!" message for 1 second before gameplay.

7. `playGame()`:

- If `shouldDisplayNumber` is true, generates and displays a random target number.
- Tracks the time for each round and checks if the button combination matches the target.
- Increments the score and reduces available time based on the F factor.
- Updates timeRoundStart and resets LEDs after each correct guess.


8. `handleButtonPresses()`:

- Reads each button state and uses `DEBOUNCE_TIME` to prevent multiple detections from a single press.
- Toggles the corresponding LED and saves the press time to handle individual button debouncing.


9. `convertButtonsStatesToDecimal()`: Converts the four-button states into a single decimal number by bit-shifting, so the player's button combination can be compared to the target number.

8. `turnOffAllLeds()`: Ensures all LEDs are turned off after each round.

11. `finishGame()`:

- Displays the "Game over!" message, shows the final score, and updates the high score if necessary.
- Delays for 10 seconds before resetting the game with `resetGame()`.


12. `resetGame()`: Resets all game state variables, clears the LCD, and prepares for a new game cycle.


13. `no_more_time()`:
- Tracks elapsed time since `timeRoundStart`.
- If the round time exceeds the allowed time, ends the game by calling `finishGame()`.

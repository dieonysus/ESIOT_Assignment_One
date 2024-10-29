# Get My Binary 

## Table of Contents 
- [Overview](#overview)
- [How It Works](#how-it-works)

## Overview 

Get My Binary is an arduino based game designed to test a player's ability to translate decimal numbers to binary under a time contraint. The game consists of multiple rounds and it will generate a random number between 1 and 15 and the player's goal is to represent that number using binary. The game increases in difficulty with each round by decreasing the allowed response time, creating a fast-paced and challenging experience.

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
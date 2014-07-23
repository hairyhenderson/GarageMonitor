/*
 GarageMonitor
 
 By Dave Henderson <dhenderson@gmail.com>
 
 Some very simple code to light an LED if the garage door is open, and
 start blinking the LED if the garage door is open for longer than ~5mins.
 
 A reed switch and rare earth magnet are used to determine if the
 garage door is open or not.
 
 This is written for the ATTiny85.
 */
#include "MorseDictionary.h"

const int LED_PIN = 0;

const int SW_PIN = 4;

// digitalRead reads a 1 when the switch is open (no magnet)
const int SW_OPEN = 1;
const int SW_CLOSED = 0;

// The last time the door was closed, in milliseconds since startup
unsigned long lastClosed = 0;

// The maximum amount of time (ms) that the door can remain open before the LED starts blinking
const unsigned long OPEN_LIMIT = 300000;

// Morse dot duration in ms (50ms ~= 20wpm)
const unsigned long DOT_LEN = 100; // use 10wpm to give keen observers a chance to decode it ;)
const unsigned long DASH_LEN = DOT_LEN * 3;
const unsigned long GAP = DOT_LEN;
const unsigned long CHAR_GAP = GAP * 3;
const unsigned long WORD_GAP = GAP * 7;
const unsigned long MSG_GAP = GAP * 10;

unsigned long previousMillis = 0;

int morseIndex = 0;
int msgIndex = 0;

char * message = "THE DOOR IS AJAR";

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setup() {
  pinMode(SW_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // Activate internal pullup
  digitalWrite(SW_PIN, HIGH);
  
//  Serial.begin(115200);
//
//  int freeMem = freeRam();
//  int used = 2048 - freeMem;
//  Serial.print(freeMem);
//  Serial.print("/2048 available (");
//  Serial.print(used);
//  Serial.println(" used)");
}

void loop() {
  int switchState = digitalRead(SW_PIN);

  switch(switchState) {
  case SW_OPEN:
    if (shouldBlink()) {
      blinkLED();
    }
    else {
      digitalWrite(LED_PIN, HIGH);
    }
    break;
  case SW_CLOSED:
    digitalWrite(LED_PIN, LOW);
    lastClosed = millis();
    resetMessage();
    break;
  }

  delay(1);
}

boolean shouldBlink() {
  return millis() - lastClosed > OPEN_LIMIT; 
}

void blinkLED() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= GAP) {
    previousMillis = currentMillis;

    char currChar = message[msgIndex];

    if (currChar == ' ') {
      nextWord();
    }
    else
    {
      const char * morse = lookup(currChar);
      switch(morse[morseIndex]) {
      case '-':
        dash();
        break;
      case '.':
        dot();
        break;
      }

      morseIndex++;

      if (morseIndex > strlen(morse)) { // we're at the end of a character, time to go to the next character
        nextCharacter();
      }

      if (msgIndex >= strlen(message)) { // we're at the end of the message, time to restart
        resetMessage();
      }
    }
  }
}

void dot() {
  digitalWrite(LED_PIN, HIGH);
  delay(DOT_LEN);
  digitalWrite(LED_PIN, LOW);
  delay(GAP);
}

void dash() {
  digitalWrite(LED_PIN, HIGH);
  delay(DASH_LEN);
  digitalWrite(LED_PIN, LOW);
  delay(GAP);
}

void nextWord() {
  delay(WORD_GAP - CHAR_GAP - GAP);
  msgIndex++;
  morseIndex = 0;
}

void nextCharacter() {
  delay(CHAR_GAP - GAP);
  msgIndex++;
  morseIndex = 0;
}

void resetMessage() {
  delay(MSG_GAP - (CHAR_GAP - GAP));
  msgIndex = 0;
  morseIndex = 0;
}




















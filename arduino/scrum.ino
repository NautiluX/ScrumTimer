#include "pitches.h"
#include <LEDDisplay.h>
//LEDDisplay library can be found here: https://github.com/rastating/LEDDisplay
//Tone generation based on https://www.arduino.cc/en/Tutorial/toneMelody

LEDDisplay *led;
unsigned long lastUpdate = 0;
int counter;
 
int melody[] = {
  NOTE_C6, NOTE_CS6, NOTE_D6, 0, 0, NOTE_G4, NOTE_F5, 0, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_E4, 0, NOTE_E4, NOTE_C4
};

int noteDurations[] = {
  16, 16, 8, 4, 2, 4, 4, 4, 4, 3, 3, 3, 4, 4, 4, 4, 4
};

void playMelody() {
  for (int thisNote = 0; thisNote < 17; thisNote++) {
    int noteDuration = 700 / noteDurations[thisNote];
    tone(12, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(2);
  }
}

void setup() {
  int digitFlagPins[] = {10, 11};
  int segmentPins[] = {2, 3, 4, 5 ,6 ,7 ,8, 9};
  int decimalPointPin = 9;
  led = new LEDDisplay(2, digitFlagPins, segmentPins, decimalPointPin);
  pinMode(13, INPUT);
}


bool startPressed() {
  int state = digitalRead(13);
  return state == HIGH;
}

void showNumber(int number) {
    for(int i = 0; i < 2; i++)
    {
        led->displayNumber(number % 10, i);
        delay(2);
        number = number / 10;
    }
}
bool finished = true;
void updateCounter() {
    if (counter == 0)
    {
      playMelody();
      resetCounter();
      finished = true;
    }
    
    unsigned long now = millis();
    if (now - lastUpdate > 1000)
    {
        lastUpdate = now;
        counter--;
    }
}

int readStart() {
  int number = (analogRead(A0) -100)*0.15;
  if (number > 99) {
    return 99;
  } else if (number < 0){
    return 0;
  }
  return number;
}

int start = 0;
void resetCounter() {
  lastUpdate = millis();
  start = readStart();
  counter = start;
}

int lastButtonUpdate = 0;
void checkStartPressed() {
  if (startPressed()) {
    unsigned long now = millis();
    if (now - lastButtonUpdate > 1000)
    {
      resetCounter();
      lastButtonUpdate = now;
      finished = !finished;
    }
  }
}

void checkPotiMoved() {
  int poti = readStart();
  int diff = abs(poti - start);
  if (diff > 20) {
    resetCounter();
    finished = true;
  }
}

void loop() {
  noTone(12);
  if (!finished) {
    showNumber(counter);
    updateCounter();
    checkPotiMoved();
  } else {
    showNumber(readStart());
  }
  checkStartPressed();
}

/*

  References:

  Assignment: 03
  Version:    03.06 (comments and good var names)
  Theme:      Nursing Bed
  Developer:  Roberta Netzova

  DONE: Basic functionality, Methods
  General alarm, Still need improvements in temperature tracking

  Methods:

*/

#include "button.h"
#include "Display.h"

const int CONTROLLER_KEY              = 9;
const int START_GAME_KEY              = 8;
const int LED_B                       = 6;
const int BUZZER                      = 3;

Button ControllerBtn                 = Button (CONTROLLER_KEY);
Button StartGameBtn                  = Button (START_GAME_KEY);


byte rouletteStates[] = {0, SEG_a, 1, SEG_a, 2, SEG_a, 3, SEG_a, 3, SEG_b, 3, SEG_c, 3, SEG_d, 2, SEG_d, 1, SEG_d, 0, SEG_d, 0, SEG_e, 0, SEG_f};
byte targetState[] = {0, SEG_a};
byte wheelIndex, pos, seg;
int steps;
int steps_done;
int half;
int third;
int blink_tracker = 6;

bool gameStarted = false;
unsigned long timeInterval = 300;
unsigned long startTime;
void setup() {
  // put your setup code here, to run once:
  Display.clear();
  Serial.begin(9600);
  Serial.println ("Start....");

  pinMode (LED_B, OUTPUT);
  digitalWrite(LED_B, LOW);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  startTime = millis();
  //  steps = random(10, 100);
  steps  = 1;
  int half = steps / 2;
  int third = half / 2;
  Serial.println(steps);
  Serial.println(half);
  Serial.println(third);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (gameStarted) {
    if (ControllerBtn.is_pushed() || StartGameBtn.is_pushed()) {
      tone(BUZZER, 100, 100);
    }

    if (doRotation() < 0) {
      //Serial.println ("Rotatiton Stopped");
//            if (blinkDigit() < 0) {
//              Serial.println(".................");
//              gameStarted = false;
//              Serial.println("END");
//              }
      finishGame();
    }
  } else {
    if (ControllerBtn.is_pushed()) {
      Serial.println("move");
      wheelStep();
    }
    if (StartGameBtn.is_pushed()) {
      //      Serial.println ("play");
      play();
    }
  }
  //  wheelStep();
  //  delay(500);
}
void finishGame () {
  if (blinkDigit() >= 0) { return;}
  
    Serial.println(".................");
//    gameStarted = false;
    Serial.println("END");

    compareResults();
    setGameValues();
}
void wheelStep() {
  pos = rouletteStates[wheelIndex++];
  seg = rouletteStates[wheelIndex++];
  wheelIndex %= 24;
  Display.clear();
  Display.showDigitAt(pos, seg);
}
void displayTarget() {
  Display.showDigitAt(targetState[0], targetState[1]);
}
void play() {
  gameStarted = true;
  targetState[0] = pos;
  targetState[1] = seg;
  //  steps = random(10, 100);
  steps = 3;
  //  steps = 11;
  half = steps / 2;
  third = half / 2;
  Serial.println(steps);
  Serial.println(half);
  Serial.println(third);
}

int doRotation() {

  if (steps - steps_done == 0 )
    return -1;

  if (!timeMeasure(timeInterval)) {
    return steps - steps_done; //0
  }
  if  (steps_done < steps - 20) {
    timeInterval = 50;
  } else {
    timeInterval += 20;
  }

  if (steps_done > steps - 1) {
    //    gameStarted = false;
    //    compareResults();
    //reset counters
    //    if (blinkDigit() < 0)
    //    setGameValues();

  }
  wheelStep();
  steps_done++;
  return steps - steps_done; //0
}

void setGameValues() {
  gameStarted = false;
  steps = 0;
  steps_done = steps;
  blink_tracker = 6;
  
}
void compareResults() {
  if (pos ==  targetState[0] && seg == targetState[1]) {
    Serial.println ("SUCCESS");
  }
  else
    Serial.println ("NO");
  Serial.println ("....................");
}

bool timeMeasure(unsigned long period) {

  unsigned long current = millis();

  if (current - startTime >= period) {
    startTime = current;
    return true;
  }
  return false;
}

int blinkDigit() {
  if (timeMeasure(100)) {
    if (checkIfOdd (blink_tracker--)) {
      Display.showDigitAt(pos, seg);
    } else {
      Display.clear();
    }
  }
  return blink_tracker;
}

bool checkIfOdd (int num) {
  if (num % 2 != 0)
    return true;
  return false;
}

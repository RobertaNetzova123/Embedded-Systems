
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
int blink_tracker = 6;
int melody_tracker = 7;
char* resultWord = "PLAY";
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
  steps = random(10, 100);
   displayWord(resultWord);
   setGameValues();
}

void loop() {

  if (gameStarted) {
    if (ControllerBtn.is_pushed() || StartGameBtn.is_pushed()) {
      tone(BUZZER, 100, 100);
    }

    if (doRotation() < 0) {
      if (resultWord == "PLAY") {
        finishGame();
      }if (doMusic() <= 0) {
          setGameValues();
        }
      

    }
  } else {
    if (ControllerBtn.is_pushed()) {
      Serial.println("move");
      wheelStep();
    }
    if (StartGameBtn.is_pushed()) {
      Serial.println ("play");
      play();
    }
  }
}

//.............GAME METHODS..................

void play() {
  gameStarted = true;
  setTargetDigit (targetState, pos, seg);
  //  steps = random(10, 100);
  steps = 3;
  Serial.println(steps);
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
  //tone(BUZZER, timeInterval, timeInterval);
  wheelStep();
  steps_done++;
  return steps - steps_done; //0
}

void compareResults() {

  if (pos ==  getTargetPos(targetState) && seg == getTargetSeg(targetState)) {
    Serial.println ("SUCCESS");
    resultWord = "YEAH";
    //        displayWord("GAIN");
  }
  else {
    Serial.println ("NO");
    resultWord = "LOSS";
    //    displayWord("LOSS");
  }
  displayWord(resultWord);
  Serial.println ("....................");

}

void finishGame () {
  if (blinkDigit() >= 0) {
    return;
  }

  compareResults();
//  setGameValues();

  Serial.println(".................");
  Serial.println("END");
}


void setGameValues() {
  gameStarted = false;
  steps = 0;
  steps_done = steps;
  blink_tracker = 6;
  melody_tracker = 7;
  resultWord = "PLAY";
  displayWord(resultWord);
}

//.......................HELPING METHODS................

void wheelStep() {
  pos = rouletteStates[wheelIndex++];
  seg = rouletteStates[wheelIndex++];
  wheelIndex %= 24;
  Display.clear();
  Display.showDigitAt(pos, seg);
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
    if (checkIfOdd(blink_tracker--)) {
      Display.showDigitAt(pos, seg);
    } else {
      Display.clear();
    }
  }
  return blink_tracker;
}

int doMusic() {
  
  int toneOdd =  100;
  int toneEven = 500;
  int duration = 500;
  if (resultWord == "YEAH") {
    toneOdd = 1000;
    toneEven = 300;
    duration = 100;
    }
  if (timeMeasure(duration)) {
    if (checkIfOdd(melody_tracker--)) {
      tone(BUZZER,toneOdd,500);
    } else {
       tone(BUZZER,toneEven);
    }
  }
  return melody_tracker;
}
int blinkWord(char* word) {
  if (timeMeasure(500)) {
    if (checkIfOdd(melody_tracker--)) {
      displayWord(word);
    } else {
      Display.clear();
    }
  }
  Serial.println("Digits:");
  Serial.println(melody_tracker);
  return melody_tracker;
}

bool checkIfOdd (int num) {
  if (num % 2 != 0)
    return true;
  return false;
}

void displayWord(char* word) {
  for (int i  = 0; i < 4; i ++) {
    Display.showCharAt(i, word[i]);
  }
}
//...................GETTERS AND SETTERS....................

void setTargetDigit(byte arr[], byte pos, byte seg) {
  arr[0] = pos;
  arr[1] = seg;
}
byte getTargetPos (byte arr[]) {
  return arr[0];
}
byte getTargetSeg (byte arr[]) {
  return arr[1];
}

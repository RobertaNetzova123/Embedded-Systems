/*
  Class Button
  Used for debouncing and reading when a button is clicked
  Version:    01.02
  Developer:  Roberta Netzova
*/
#include "button.h"
#include <Arduino.h>

Button::Button (int buttonPin) {
  pinMode(buttonPin, INPUT_PULLUP);
	btn = buttonPin;
	btnLastState = HIGH;
  pushed = false;
}

bool Button::debounce() {
  bool last = HIGH;
  bool reading  = digitalRead(btn);
  if (reading != last) {
    delay(10);
    reading = digitalRead(btn);
  }
  return reading;
}

bool Button::is_pushed() {
  // put your main code here, to run repeatedly:
//  btnLastState = HIGH;

  bool reading =  debounce();
  if (reading != btnLastState) {
//    Serial.println("reading differs");
//     reading =  debounce(btn);
    if (reading == HIGH) {
//      Serial.println("reading TRUE");
      pushed = true;
      // if (pushed == true) {
      //   digitalWrite (led, HIGH);
      //   delay(50);
      //   digitalWrite (led, LOW);
      //   Serial.println(++count);
      // }
    }
    btnLastState = reading;
  }
  if (pushed == true) {pushed = false; return !pushed;}
  return pushed;
}

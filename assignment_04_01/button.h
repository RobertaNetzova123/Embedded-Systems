#ifndef button_h
#define button_h	

#include <Arduino.h>

class Button {
public:
	Button(int buttonPin);
	bool debounce();
	bool is_pushed();

private:
	int btn;
	bool btnLastState;
  bool pushed;

};

#endif

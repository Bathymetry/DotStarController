#include <Arduino.h>
#include "DebounceButton.h"

namespace nDebounceButton
{
  const unsigned long DEBOUNCE_MILLIS = 500;
}

DebounceButton::DebounceButton(int pin)
: PIN_NUMBER(pin)
, state(LOW)
, changeStart(0)
{

}

void DebounceButton::setup()
{
  pinMode(PIN_NUMBER, INPUT);
  
  state = digitalRead(PIN_NUMBER);
  //Serial.println(state);

  changeStart = 0;
}

bool DebounceButton::checkIfChanged()
{
  int curState = digitalRead(PIN_NUMBER);
  if (state == curState)
  {
    changeStart = 0;
    return false;
  }
  else if (changeStart == 0)
  {
    changeStart = millis();
    return false;
  }
  else if (millis() - changeStart >= nDebounceButton::DEBOUNCE_MILLIS)
  {
    state = curState;
    Serial.println(state);
    return true;
  }

  return false;
}
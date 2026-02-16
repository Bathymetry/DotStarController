#ifndef DEBOUNCE_BUTTON
#define DEBOUNCE_BUTTON

class DebounceButton
{
public:
  DebounceButton(int pin);

  void setup();
  bool checkIfChanged();

  inline int getState() const
  {
    return state;
  }

private:
  int PIN_NUMBER = -1;
  int state = LOW;
  unsigned long changeStart = 0;
};

#endif
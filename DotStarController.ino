/*

*/

#define WITH_DEBUG 0

#include <Adafruit_DotStar.h>
#include <SPI.h>
#include "DebounceButton.h"

int POWER_CHECK_PIN = 9;
DebounceButton powerButton(POWER_CHECK_PIN);

int FADE_CONTROL_PIN = A7;
const float V_REF = 5.0;     // Analog reference voltage (e.g., 5V or 3.3V)
const float R_BITS = 10.0;   // ADC resolution (bits)
const float ADC_STEPS = (1 << int(R_BITS)) - 1; // Number of steps (2^R_BITS - 1)

#define NUM_PIXELS 72
const uint8_t MIN_BRIGHTNESS = 20;
const uint8_t MAX_BRIGHTNESS = 200;
Adafruit_DotStar strip(NUM_PIXELS, DOTSTAR_BRG);
const uint32_t DOTSTAR_WHITE = strip.Color(255, 255, 255);

const float THRESHOLD_DV = 0.02f;
float previousVoltage = 0.f;

bool shouldForceRefresh = false;

// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);     // initialize digital pin LED_BUILTIN as an output.
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW

  powerButton.setup();

  strip.begin();
  strip.show();

  strip.fill(DOTSTAR_WHITE, 0, NUM_PIXELS);
  shouldForceRefresh = true;
}

// the loop function runs over and over again forever
void loop()
{
  bool isPowerOn = powerButton.getState() == 1;
  
  if (powerButton.checkIfChanged()) // this updates the button
  {
    // since button was just updated, refresh the value of this variable
    isPowerOn = powerButton.getState() == 1;

    if (isPowerOn)
    {
      shouldForceRefresh = true;
    }
    else
    {
      strip.setBrightness(0);
      strip.show();
    }
  }

  if (isPowerOn)
  {
    int fadeRaw = analogRead(FADE_CONTROL_PIN);
    float voltage = (fadeRaw / ADC_STEPS) * V_REF; // Convert to voltage

    if (voltage < 0.f)
    {
      voltage = 0.f;
    }
    else if (voltage > V_REF)
    {
      voltage = V_REF;
    }

    float dv = fabs(voltage - previousVoltage);
    if (shouldForceRefresh || dv >= THRESHOLD_DV)
    {
      previousVoltage = voltage;

      float brightnessRange = (float)(MAX_BRIGHTNESS - MIN_BRIGHTNESS);
      float wipe = previousVoltage/V_REF;
      uint8_t brightness = (uint8_t)round(brightnessRange * wipe) + MIN_BRIGHTNESS;

#if WITH_DEBUG
      Serial.print("Brightness: ");
      Serial.println(brightness);
#endif

      strip.setBrightness(brightness);
      strip.show();

      shouldForceRefresh = false;
    }
  }

  delay(50);
}

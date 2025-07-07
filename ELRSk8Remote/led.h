#ifndef LEDVGR_H
#define LEDVGR_H

#include <Adafruit_NeoPixel.h>

uint8_t ledState = LOW;
uint8_t oldState = HIGH;
unsigned long previousMillis = 0;

Adafruit_NeoPixel pixels(1, RGBLED_PIN, NEO_GRB + NEO_KHZ800);

struct RGB {
  byte r; // Red (0 to 255)
  byte g; // Green (0 to 255)
  byte b; // Blue (0 to 255)
};

struct HSV {
  byte h; // Hue (0 to 255)
  byte s; // Saturation (0 to 255)
  byte v; // Value (0 to 255)
};

RGB HSVToRGB(HSV hsv) {
  RGB rgb;
  float h = hsv.h * 360.0 / 255.0; // Convert hue to 0-360 range
  float s = hsv.s / 255.0;         // Convert saturation to 0-1 range
  float v = hsv.v / 255.0;         // Convert value to 0-1 range

  float c = v * s;                 // Chroma
  float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1)); // Second largest component
  float m = v - c;

  float r_prime, g_prime, b_prime;

  if (h >= 0 && h < 60) {
    r_prime = c; g_prime = x; b_prime = 0;
  } else if (h >= 60 && h < 120) {
    r_prime = x; g_prime = c; b_prime = 0;
  } else if (h >= 120 && h < 180) {
    r_prime = 0; g_prime = c; b_prime = x;
  } else if (h >= 180 && h < 240) {
    r_prime = 0; g_prime = x; b_prime = c;
  } else if (h >= 240 && h < 300) {
    r_prime = x; g_prime = 0; b_prime = c;
  } else {
    r_prime = c; g_prime = 0; b_prime = x;
  }

  // Convert to 0-255 scale
  rgb.r = (r_prime + m) * 255.0;
  rgb.g = (g_prime + m) * 255.0;
  rgb.b = (b_prime + m) * 255.0;

  return rgb;
}


void SetupLED(int pin) {
  pixels.setPin(pin);
  pixels.begin();
}

HSV LEDColorHSV = {60, 255, 255};
RGB LEDColorRGB = {255, 255, 0};

void blinkLED(uint16_t blinkRate) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= blinkRate) {
        previousMillis = currentMillis;     // save the last time you blinked the LED
        oldState = ledState;
        ledState ^= 1;                      // if the LED is off turn it on and vice-versa
        //digitalWrite(ledPin, ledState);
        if(oldState != ledState) {

          //RGB rgb = HSVToRGB(LEDColorHSV);
          RGB rgb = LEDColorRGB;
          
          if(ledState == LOW) {
            pixels.setPixelColor(0, pixels.Color(0, 0, 0));
          }
          else {
            pixels.setPixelColor(0, pixels.Color(rgb.r, rgb.g, rgb.b));
          }
          pixels.show();
        }
    }
}

void LightLed() {
  pixels.setPixelColor(0, pixels.Color(LEDColorRGB.r, LEDColorRGB.g, LEDColorRGB.b));
  pixels.show();
}

#endif

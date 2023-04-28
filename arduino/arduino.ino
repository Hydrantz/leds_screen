#include <Adafruit_NeoPixel.h>

#define NEO_PIN 6
#define NUMPIXELS 56
#define BUNS_LENGTH 10

#define BAUDRATE 9600
#define TIMEOUT 100

#include "controller.h"
#include "communication.h"

Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

Controller ctrl( &pixels, NUMPIXELS, BUNS_LENGTH);
Communication comm;
  
void setup() {
  Serial.begin(BAUDRATE);
  Serial.setTimeout(TIMEOUT);
  pixels.begin(); // This initializes the NeoPixel library.
}

// Main Loop
// ============================

void loop() {
  if (Serial.available() > 0) {
    comm.get_input(ctrl);
  }
  ctrl.fire_effect();
}

#include <Adafruit_NeoPixel.h>

#define NEO_PIN 6
#define NUMPIXELS 56
#define BUNS_LENGTH 10

#include "controller.h"

Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

// Current data variables
String data = ""; // whole data
String first = ""; // command
String last = ""; // argument

Controller ctrl( &pixels, NUMPIXELS, BUNS_LENGTH);
  
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
  pixels.begin(); // This initializes the NeoPixel library.
//  ctrl.set_buns_leds_default();
//  ctrl.showStrip();
}

// Buttons Helper Functions
// =========================

void manage_buns(String direction) {
  ctrl.update_buttons(direction[0]-48);
}

// Main Loop
// ============================

void get_input() {
  int ind = data.indexOf('|');
  first = data.substring(0, ind);
  last = data.substring(ind+1);
  Serial.println(first);
  Serial.println(last);
  if (first == "a") {
    manage_buns(last);
  }
  else {
   if (first == "blank") {
     ctrl.current_effect = &ctrl.blank;
   }
   else if (first == "y") {
     ctrl.current_effect = &ctrl.yellow;
   }
   else if (first == "b") {
     ctrl.current_effect = &ctrl.blue; 
   }
   else if (first == "c") {
     ctrl.current_effect = &ctrl.red_cylon;
   }
   else if (first == "s") {
     ctrl.current_effect = &ctrl.white_strobe;
   }
  }
}

void loop() {
  if (Serial.available() > 0) {
    data = Serial.readStringUntil('@');
    get_input();
  }
   ctrl.fire_effect();
}

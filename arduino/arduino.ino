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
String bun_state = ""; // state of buttons

Controller ctrl( &pixels, NUMPIXELS, BUNS_LENGTH);
  
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
  pixels.begin(); // This initializes the NeoPixel library.
  ctrl.set_buns_leds(255,255,255);
  ctrl.showStrip();
}

// Buttons Helper Functions
// =========================

void manage_buns(String direction) {
  if (direction == bun_state){
    return;
  }
  else if (direction != "u" &&
           direction != "d" &&
           direction != "l" &&
           direction != "r" &&
           direction != "a") {
            return;
           }
  else {
    bun_state = direction;
  }
  ctrl.set_buns_leds(255,255,255);
  if (direction == "u") {
     ctrl.setPixel(2,0,0,0);
     ctrl.setPixel(3,0,0,0);
  }
  else if (direction == "d") {
     ctrl.setPixel(0,0,0,0);
     ctrl.setPixel(1,0,0,0);
  }
  else if (direction == "l") {
     ctrl.setPixel(6,0,0,0);
     ctrl.setPixel(7,0,0,0);
  }
  else if (direction == "r") {
     ctrl.setPixel(4,0,0,0);
     ctrl.setPixel(5,0,0,0);
  }
  ctrl.showStrip();
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
   ctrl.fireEffect();
}

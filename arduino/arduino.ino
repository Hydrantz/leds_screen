#include <Adafruit_NeoPixel.h>

#define NEO_PIN 6
#define NUMPIXELS 56
#define BUNS_LENGTH 10

#define BAUDRATE 9600
#define TIMEOUT 100

#define SCORE_CLK 8
#define SCORE_LATCH 9
#define SCORE_SEVEN 10
#define SCORE_SIXTEEN 11

#include "controller.h"
#include "communication.h"
#include "scoreboard.h"

Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

Controller ctrl( &pixels, NUMPIXELS, BUNS_LENGTH);
Communication comm;
Scoreboard score(SCORE_CLK, SCORE_LATCH, SCORE_SEVEN, SCORE_SIXTEEN);
  
void setup() {
  Serial.begin(BAUDRATE);
  Serial.setTimeout(TIMEOUT);
  pinMode(SCORE_CLK, OUTPUT);
  pinMode(SCORE_LATCH, OUTPUT);
  pinMode(SCORE_SEVEN, OUTPUT);
  pinMode(SCORE_SIXTEEN, OUTPUT);
  digitalWrite(SCORE_CLK, LOW);
  digitalWrite(SCORE_LATCH, LOW);
  pixels.begin(); // This initializes the NeoPixel library.
}

// Main Loop
// ============================

void loop() {
  comm.get_input(ctrl, score);
  ctrl.fire_effect();
  score.transmit_score();
}

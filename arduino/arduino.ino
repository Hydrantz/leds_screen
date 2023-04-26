#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define NEO_PIN 6
#define NUMPIXELS 56

// Manual communication pins
#define RX_CLOCK 2
#define RX_DATA 3

Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

int bun_led_amount = 10; // how many leds are in the buttons

// Current Controller Lighting Variables
// =========================
String bun_state = ""; // state of buttons
void (*current_effect)() = &blank; // Current showing effect

// Effects Variables
// =========================
bool lighting_override = false;
int effect_last_time = 0;
int effect_step = 0;

// Communication variables
String message = "";
volatile byte rx_byte = 0;
volatile int bit_position = 0;
volatile bool new_data = false;

// Current data variables
String data = ""; // whole data
String first = ""; // command
String last = ""; // argument
  
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
  pixels.begin(); // This initializes the NeoPixel library.
  set_buns_leds(255,255,255);
  showStrip();

  // Communication setup
  pinMode(RX_DATA, INPUT);
  attachInterrupt(digitalPinToInterrupt(RX_CLOCK), onClockPulse, RISING);
}

// Communication Functions
// ========================

void onClockPulse() {
  bool rx_bit = digitalRead(RX_DATA);
  if (bit_position == 8) {
    rx_byte = 0;
    bit_position = 0;
  }
  
  if (rx_bit) {
    rx_byte |= (0x80 >> bit_position);
  }

  bit_position += 1;

  if (bit_position == 8) {
    const char *character = (const char *)&rx_byte;
    if (*character == '@') {
      data = message;
      message = "";
      new_data = true;
      return;
    }
    message += *character;
  }
}

// Main Loop
// ============================

void get_input() {
  int ind = data.indexOf('|');
  first = data.substring(0, ind);
  last = data.substring(ind+1);
  if (first == "a") {
    manage_buns(last);
  }
  else if (first == "w"){
    bun_state = true;
    int intensity = last.toInt();
    mono_color(intensity,intensity,intensity);
    showStrip();
  }
  else if (first == "release"){
    bun_state = false;
  }
  else {
    if (first == "blank") {
      current_effect = &blank;
    }
    else if (first == "y") {
      current_effect = &yellow;
    }
    else if (first == "b") {
      current_effect = &blue; 
    }
    else if (first == "c") {
      current_effect = &red_cylon;
    }
    else if (first == "s") {
      current_effect = &white_strobe;
    }
  }
}

void loop() {
  if (Serial.available() > 0) {
    data = Serial.readStringUntil('@');
    get_input();
  }
  (*current_effect)();
}

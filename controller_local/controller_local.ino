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

// Communication variables
String message = "";
volatile byte rx_byte = 0;
volatile int bit_position = 0;
volatile bool new_data = false;

// Current data variables
String data = ""; // whole data
String first = ""; // command
String last = ""; // argument
String bun_state = ""; // state of buttons
void (*current_effect)() = &blank; // Current showing effect

// Effects Variables
bool lighting_override = false;
int effect_last_time = 0;
int effect_step = 0;
  
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
  set_buns_leds(255,255,255);
  if (direction == "u") {
      setPixel(2,0,0,0);
      setPixel(3,0,0,0);
  }
  else if (direction == "d") {
      setPixel(0,0,0,0);
      setPixel(1,0,0,0);
  }
  else if (direction == "l") {
      setPixel(6,0,0,0);
      setPixel(7,0,0,0);
  }
  else if (direction == "r") {
      setPixel(4,0,0,0);
      setPixel(5,0,0,0);
  }
  showStrip();
}

// Lighting Helper Functions
// =========================

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   pixels.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void showEffect() {
  if (!lighting_override){
    showStrip();
  }
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
//  Serial.println(blue);
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   pixels.setPixelColor(Pixel, pixels.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void set_multi_leds(byte red, byte green, byte blue, int start, int end) {
  for(int i = start; i < end; i++ ) {
    setPixel(i, red, green, blue);
  }
}

void set_effect_leds(byte red, byte green, byte blue) {
  set_multi_leds(red, green, blue, bun_led_amount, NUMPIXELS);
}

void set_buns_leds(byte red, byte green, byte blue) {
  set_multi_leds(red, green, blue, 0, bun_led_amount);
}

void set_all_leds(byte red, byte green, byte blue) {
  set_multi_leds(red, green, blue, 0, NUMPIXELS);
}

// Helper Effects
// ========================

void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
  for(int i = bun_led_amount; i < NUMPIXELS-EyeSize-2; i++) {
    set_effect_leds(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      if (new_data) { return; }
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showEffect();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
  for(int i = NUMPIXELS-EyeSize-2; i > bun_led_amount; i--) {
    set_effect_leds(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      if (new_data) { return; }
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showEffect();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void mono_color_cycle(byte red, byte green, byte blue) {
  int delay = 10; // Delay time between steps in milliseconds
  int cur_time = millis();
  // Manage Delay
  if (cur_time - effect_last_time < delay){
    return;
  }
  effect_last_time = cur_time;
  if (effect_step >= NUMPIXELS){
    effect_step = 0;
  }
  if (effect_step == 0) {
    set_effect_leds(0,0,0);
  }
  int i = bun_led_amount + effect_step;
  setPixel(i, red, green, blue);
  showEffect(); // This sends the updated pixel color to the hardware.
  effect_step += 1;
}

void strobe(byte red, byte green, byte blue, int FlashDelay){
  int cur_time = millis();
  // Manage Delay
  if (cur_time - effect_last_time < FlashDelay){
    return;
  }
  effect_last_time = cur_time;
  if (effect_step >= 2){
    effect_step = 0;
  }
  if (effect_step == 0) {
    set_effect_leds(red,green,blue);
    effect_step = 1;
  }
  else {
    set_effect_leds(0,0,0);
    effect_step = 0;
  }
  showEffect();
}

void mono_color(byte red, byte green, byte blue) {
  if (effect_step < 9999){
      set_effect_leds(red, green, blue);
      showEffect(); // This sends the updated pixel color to the hardware.
      effect_step = 9999;
  }
}

// Effects
// ========================

void blank() {
  mono_color(0,0,0);
}

void yellow() {
  mono_color(255,255,0);
}

void blue() {
  mono_color_cycle(0,0,255);
}

void red_cylon() {
  CylonBounce(0xff, 0, 0, 4, 10, 50);
}

void white_strobe() {
  strobe(0xff, 0xff, 0xff, 20);
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

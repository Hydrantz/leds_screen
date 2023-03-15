#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define NEO_PIN 6
#define NUMPIXELS 56
#define RX_CLOCK 2
#define RX_DATA 3


Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);
int bun_led_amount = 10;

const int numChars = 16;
char message[numChars];
volatile byte rx_byte = 0;
volatile int bit_position = 0;
volatile bool new_data = false;

String data = "";
String first = "";
String last = "";

// should change this variable to int type in future
String prev_bun = "";

void (*current_effect)() = NULL;

bool lighting_override = false;
  
void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  set_effect_leds(255,255,255);
  showStrip();

  // Communication setup
  pinMode(RX_DATA, INPUT);
  strcpy(message, "");
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
      strcpy(data, message);
      strcpy(message, "");
      get_input();
      return();
    }
    strncat(message, character, 1);
  }
}

// Buttons Helper Functions
// =========================

void manage_buns(String direction) {
  if (direction == prev_bun){
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
    prev_bun = direction;
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

void set_effect_leds(byte red, byte green, byte blue) {
  for(int i = bun_led_amount; i < NUMPIXELS; i++ ) {
    setPixel(i, red, green, blue);
  }
}

void set_buns_leds(byte red, byte green, byte blue) {
  for(int i = 0; i < bun_led_amount; i++ ) {
    setPixel(i, red, green, blue);
  }
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
  set_effect_leds(0,0,0);
  for(int i=bun_led_amount;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    setPixel(i, red, blue, green); // Moderately bright green color.
    showEffect(); // This sends the updated pixel color to the hardware.
    delay(10); // Delay for a period of time (in milliseconds).
    if (get_input()) { return; }
  }
}

void strobe(byte red, byte green, byte blue, int FlashDelay){
  set_effect_leds(red,green,blue);
  showEffect();
  delay(FlashDelay);
  set_effect_leds(0,0,0);
  showEffect();
  delay(FlashDelay);
}

void mono_color(byte red, byte green, byte blue) {
  set_effect_leds(red, blue, green);
  showEffect(); // This sends the updated pixel color to the hardware.
}

// Effects
// ========================

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
  strobe(0xff, 0xff, 0xff, 20)
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
      lighting_override = true;
      int intensity = last.toInt();
      mono_color(intensity,intensity,intensity);
      showStrip();
    }
    else if (first == "release"){
      lighting_override = false;
    }
    else {
      void (*new_effect)();

      if (first == "y") {
        new_effect = &yellow;
      }
      else if (first == "b") {
        new_effect = &blue; 
      }
      else if (first == "c") {
        new_effect = &red_cylon;
      }
      else if (first == "s") {
        new_effect = &white_strobe;
      }

      if (new_effect != current_effect) {
        new_data = true;
        current_effect = new_effect;
      }
    }
}

void loop() {
  if (new_data) {
    new_data = false;   
  }
  *current_effect();
}

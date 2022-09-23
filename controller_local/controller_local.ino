#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN      6
#define NUMPIXELS 56


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int bun_led_amount = 10;

const int numChars = 32;
char receivedChars[numChars];

String data = "";
String first = "";
String last = "";

String prev_bun = "";
  
void setup() {
  Serial.begin(230400);
  Serial.setTimeout(100);
  #if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.fill(pixels.Color(255,255,255));
  pixels.show();
}

bool get_input() {
    if (Serial.available() > 0) {
      data = Serial.readStringUntil('\n');
      int ind = data.indexOf('|');
      String new_first = data.substring(0, ind);
      String new_last = data.substring(ind+1);
      Serial.println(data);
      Serial.println(new_first);
      Serial.println(new_last);
      if (new_first == first && new_last == last){
        return false;
      }
      else {
        first = new_first;
        last = new_last;
      }
      if (first == "a") {
        manage_buns(last);
        return false;
      }
      else {
        return true;
      }
    }
    else {
      return false;
    }
}

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
  setBuns(255,255,255);
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
  pixels.show();
}

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

void setAll(byte red, byte green, byte blue) {
  for(int i = bun_led_amount; i < NUMPIXELS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

void setBuns(byte red, byte green, byte blue) {
  for(int i = 0; i < bun_led_amount; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
  while (true) {
      for(int i = bun_led_amount; i < NUMPIXELS-EyeSize-2; i++) {
        setAll(0,0,0);
        setPixel(i, red/10, green/10, blue/10);
        for(int j = 1; j <= EyeSize; j++) {
          if (get_input()) { return; }
          setPixel(i+j, red, green, blue);
        }
        setPixel(i+EyeSize+1, red/10, green/10, blue/10);
        showStrip();
        delay(SpeedDelay);
      }
      delay(ReturnDelay);
      for(int i = NUMPIXELS-EyeSize-2; i > bun_led_amount; i--) {
        setAll(0,0,0);
        setPixel(i, red/10, green/10, blue/10);
        for(int j = 1; j <= EyeSize; j++) {
          if (get_input()) { return; }
          setPixel(i+j, red, green, blue);
        }
        setPixel(i+EyeSize+1, red/10, green/10, blue/10);
        showStrip();
        delay(SpeedDelay);
      }
      delay(ReturnDelay);
  }
}

void yellow() {
  setAll(255,255,0);
  pixels.show(); // This sends the updated pixel color to the hardware.
  while(true){
    if (get_input()) { return; }
  }
}

void blue() {
  while (true){
    setAll(0,0,0);
    for(int i=bun_led_amount;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,0,255)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(10); // Delay for a period of time (in milliseconds).
      if (get_input()) { return; }
    }
  }
}

void white(String value) {
  int amount = value.toInt();
  setAll(amount,amount,amount);
  showStrip();
  Serial.println(amount);
  while(true){
    if (get_input()) { return; }
  }
}

void Strobe(byte red, byte green, byte blue, int FlashDelay){
  while(true) {
    if (get_input()) { return; }
    setAll(red,green,blue);
    showStrip();
    delay(FlashDelay);
    setAll(0,0,0);
    showStrip();
    delay(FlashDelay);
  }
}

void loop() {
  get_input();
  if (first == "y") {
    Serial.println("hi");
    yellow();
  }
  else if (first == "b") {
    Serial.println("bye");
    blue(); 
  }
  else if (first == "w") {
    Serial.println("white");
    white(last);
  }
  else if (first == "c") {
    Serial.println("cylon");
    CylonBounce(0xff, 0, 0, 4, 10, 50);
  }
  else if (first == "s") {
    Serial.println("strobe");
    Strobe(0xff, 0xff, 0xff, 20);
  }
}

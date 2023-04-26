// Effects Hash Table
// =========================

enum class Effect : byte {
	blank,
	static_yellow,
	cycle_blue,
  cylon,
	strobe,

	COMMANDS_ERROR,
};

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
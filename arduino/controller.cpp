#include "controller.h"
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

Color color_none = {0,0,0};

// Constructors
// ===========================

Controller::Controller( 
                        Adafruit_NeoPixel* led_strip,
                        int strip_length,
                        int bun_led_length) {
    this->led_strip = led_strip;
    this->strip_length = strip_length;
    this->bun_led_length = bun_led_length;
    this->effect_last_time = 0;
    this->effect_step = 0;
    this->lighting_override = false;
    this->current_effect = 1;
    this->current_direction = BUTTON_ERROR;
    this->button_color = {24, 84, 128};
    this->sel_bun_enabled = false;
}

// Led Strip Control Functions
// ===========================

void Controller::showStrip() {
    led_strip->show();
}
void Controller::showEffect() {
    if (!lighting_override){
        showStrip();
    }
}
void Controller::setPixel(int Pixel, Color color) {
    led_strip->setPixelColor(Pixel, led_strip->Color(color.r, color.g, color.b));
}
void Controller::set_multi_leds(Color color, int start, int end) {
    for(int i = start; i < end; i++ ) {
        setPixel(i, color);
    }
}
void Controller::turnoff_two_leds(int i, int j){
    setPixel(i,color_none);
    setPixel(j,color_none);
    return;
}
void Controller::set_effect_leds(Color color) {
    set_multi_leds(color, bun_led_length, strip_length);
}
void Controller::set_buns_leds(Color color) {
    set_multi_leds(color, 0, bun_led_length);
}
void Controller::set_buns_leds_default() {
    set_buns_leds(this->button_color);
}
void Controller::set_all_leds(Color color) {
    set_multi_leds(color, 0, strip_length);
}

void Controller::set_sel(bool sel_active){
    if (sel_active == true){
        this->sel_bun_enabled = true;
    }
    if (sel_active == false){
        this->sel_bun_enabled = false;
    }
}

void Controller::update_buttons(Buttons direction) {
    if (direction >= BUTTON_ERROR){
        return;
    }
    if (direction == 0){
        return;
    }
    
    if (direction == this->current_direction){
        return;
    }
    set_buns_leds_default();
    if (!this->sel_bun_enabled){
        turnoff_two_leds(8,9);
    }
    switch (direction) {
        case up:
            turnoff_two_leds(2,3);
            break;

        case down:
            turnoff_two_leds(0,1);
            break;

        case left:
            turnoff_two_leds(6,7);
            break;

        case right:
            turnoff_two_leds(4,5);
            break;
    }
    this->showStrip();
    return;
}

void Controller::update_effect(Effect new_effect) {
    if (new_effect >= EFFECT_ERROR){
        return;
    }
    if (new_effect == 0){
        return;
    }
    this->current_effect = new_effect;
    return;
}

void Controller::fire_effect(){
    switch (this->current_effect) {
        case effect_blank:
            this->blank();
            break;
        case effect_blue:
            this->blue();
            break;
        case effect_yellow:
            this->yellow();
            break;
        case effect_cylon:
            this->red_cylon();
            break;
        case effect_strobe:
            this->white_strobe();
            break;
    }
    return;
}

// Effects Helper Functions
// ========================

void Controller::CylonBounce(Color color, int EyeSize, int SpeedDelay, int ReturnDelay){
    Color faded_color = {color.r/10, color.g/10, color.b/10};
    for(int i = bun_led_length; i < strip_length -EyeSize-2; i++) {
        set_effect_leds(color_none);
        setPixel(i, faded_color);
        for(int j = 1; j <= EyeSize; j++) {
    //      if (new_data) { return; }
        setPixel(i+j, color);
        }
        setPixel(i+EyeSize+1, faded_color);
        showEffect();
        delay(SpeedDelay);
    }
    delay(ReturnDelay);
    for(int i = strip_length-EyeSize-2; i > bun_led_length; i--) {
        set_effect_leds(color_none);
        setPixel(i, faded_color);
        for(int j = 1; j <= EyeSize; j++) {
    //      if (new_data) { return; }
        setPixel(i+j, color);
        }
        setPixel(i+EyeSize+1, faded_color);
        showEffect();
        delay(SpeedDelay);
    }
    delay(ReturnDelay);
}

void Controller::mono_color_cycle(Color color) {
    int delay = 10; // Delay time between steps in milliseconds
    int cur_time = millis();
    // Manage Delay
    if (cur_time - effect_last_time < delay){
        return;
    }
    effect_last_time = cur_time;
    if (effect_step >= strip_length){
        effect_step = 0;
    }
    if (effect_step == 0) {
        set_effect_leds(color_none);
    }
    int i = bun_led_length + effect_step;
    setPixel(i, color);
    showEffect(); // This sends the updated pixel color to the hardware.
    effect_step += 1;
}

void Controller::strobe(Color color, int FlashDelay){
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
        set_effect_leds(color);
        effect_step = 1;
    }
    else {
        set_effect_leds(color_none);
        effect_step = 0;
    }
    showEffect();
}

void Controller::mono_color(Color color) {

    int delay = 100; // Delay time between steps in milliseconds
    int cur_time = millis();
    // Manage Delay
    if (cur_time - effect_last_time < delay){
        return;
    }
    effect_last_time = cur_time;
    effect_step = 0;
    set_effect_leds(color);
    showEffect(); // This sends the updated pixel color to the hardware.


}

// Effects
// ========================

void Controller::blank() {
    mono_color(color_none);
}

void Controller::yellow() {
    mono_color({255,255,0});
}

void Controller::blue() {
    mono_color_cycle({0,0,255});
}

void Controller::red_cylon() {
    CylonBounce({255, 0, 0}, 4, 10, 50);
}

void Controller::white_strobe() {
    strobe({255,255,255}, 20);
}
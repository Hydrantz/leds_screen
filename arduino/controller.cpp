#include "controller.h"
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

Color color_none = {0,0,0};

uint8_t buttons_indexes[5][2]  = {
    {0,1},
    {2,3},
    {4,5},
    {6,7},
    {8,9}
};

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
    this->current_effect = Effect::effect_blank;
    this->button_color = {24, 84, 128};
    this->effect_color = {0, 255, 0};
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
void Controller::set_all_leds(Color color) {
    set_multi_leds(color, 0, strip_length);
}
void Controller::set_effect_leds(Color color) {
    set_multi_leds(color, bun_led_length, strip_length);
}
void Controller::set_buns_leds(Color color) {
    set_multi_leds(color, 0, bun_led_length);
    showStrip();
}
void Controller::turn_buttons_default(String buttons) {
    turn_buttons_manually(buttons, this->button_color);
}
void Controller::turn_buttons_manually(String buttons, Color color){
    for (int i = 0; i < buttons.length(); i++){
        int bun_id = this->bun_2_id(buttons[i]);
        int a = buttons_indexes[bun_id][0];
        int b = buttons_indexes[bun_id][1];
        this->setPixel(a, color);
        this->setPixel(b, color);
    }
    this->showStrip();
}
void Controller::update_buttons_default(Color color){
    this->button_color = color;
    return;
}
void Controller::clear_given_buns(String buttons){
    this->turn_buttons_manually(buttons, {0,0,0});
    this->showStrip();
} 
void Controller::clear_buns(){
    this->set_buns_leds({0,0,0});
    this->showStrip();
} 
int Controller::bun_2_id(char bun){
    switch (bun) {
        case 'u':
            return 0;
            break;
        case 'd':
            return 1;
            break;
        case 'l':
            return 2;
            break;
        case 'r':
            return 3;
            break;
        case 's':
            return 4;
            break;
    }
}
void Controller::fire_effect(){
    switch (this->current_effect) {
        case Effect::effect_blank:
            this->blank();
            break;
        case Effect::effect_blue:
            this->blue();
            break;
        case Effect::effect_yellow:
            this->yellow();
            break;
        case Effect::effect_cylon:
            this->red_cylon();
            break;
        case Effect::effect_strobe:
            this->white_strobe();
            break;
        case Effect::effect_default:
            this->mono_color_default();
            break;
        case Effect::effect_flash:
            this->flash_default();
            break;
    }
    return;
}
void Controller::update_effect(Effect new_effect) {
    if (new_effect >= Effect::EFFECT_ERROR || new_effect <= Effect::zero){
        return;
    }
    if (new_effect == this->current_effect){
        return;
    }
    this->current_effect = new_effect;
    this->effect_step = 0;
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

void Controller::single_flash(Color color, int FlashDelay){
    int cur_time = millis();
    // Manage Delay
    if (cur_time - effect_last_time < FlashDelay){
        return;
    }
    effect_last_time = cur_time;
    if (effect_step >= 2){
        update_effect(Effect::effect_blank);
        return;
    }
    if (effect_step == 0) {
        set_effect_leds(color);
        effect_step = 1;
    }
    else if (effect_step == 1){
        set_effect_leds(color_none);
        effect_step = 2;
    }
    showEffect();
}

void Controller::strobe(Color color, int FlashDelay){
    if (effect_step >= 2){
        effect_step = 0;
    }
    single_flash(color, FlashDelay);
}

void Controller::mono_color(Color color) {

    if (this->effect_step >= 1){
        return;
    }
    int delay = 100; // Delay time between steps in milliseconds
    int cur_time = millis();
    // Manage Delay
    if (cur_time - effect_last_time < delay){
        return;
    }
    effect_last_time = cur_time;
    effect_step = 1;
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

void Controller::mono_color_default(){
    mono_color(this->effect_color);
}

void Controller::flash_default(){
    single_flash(this->effect_color, 100);
}
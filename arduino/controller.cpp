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
    this->current_effect = Effect::effect_blank;
    this->current_direction = Direction::none;
    this->button_color = {24, 84, 128};
    this->sel_color = this->button_color;
    this->is_sel = false;
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
void Controller::clear_buns(){
    this->set_buns_leds({0,0,0});
} 
void Controller::set_buns_leds_default() {
    set_buns_leds(this->button_color);
}
void Controller::set_all_leds(Color color) {
    set_multi_leds(color, 0, strip_length);
}

void Controller::turn_buttons_manually(String buttons, Color color){
    this->is_sel = false;
    this->current_direction = Direction::DIRECTION_ERROR;
    for (int i = 0; i < buttons.length(); i++){
        int a = bun_2_led_index(buttons[i], true);
        int b = bun_2_led_index(buttons[i], false);
        this->setPixel(a, color);
        this->setPixel(b, color);
    }
    this->showStrip();
}

void Controller::turn_buttons_by_direction(Direction direction) {
    if (direction >= Direction::DIRECTION_ERROR){
        return;
    }
    if (direction == 0){
        return;
    }
    if (direction == this->current_direction){
        return;
    }
    if (direction == Direction::none){
        this->clear_buns();
    }
    else {
        set_buns_leds_default();
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
    }
    setPixel(8, this->sel_color);
    setPixel(9, this->sel_color);
    if (!is_sel){
        this->turnoff_two_leds(8,9);
    }
    this->showStrip();
    this->current_direction = direction;
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

void Controller::update_color_default(Color color, bool show, bool sel){
    if (sel){
        this->sel_color = color;
    }
    else{
        this->button_color = color;
    }
    if (show){
        Direction dir = this->current_direction;
        this->current_direction = Direction::DIRECTION_ERROR;
        this->turn_buttons_by_direction(dir);
    }
    return;
}

void Controller::update_sel_default(Color color, bool show=true){
    this->update_color_default(color, show, true);
}

void Controller::update_buttons_default(Color color, bool show=true){
    this->update_color_default(color, show, false);
}

void Controller::update_sel_state(bool sel, bool show=true){
    this->is_sel = sel;
    if (show){
        Direction dir = this->current_direction;
        this->current_direction = Direction::DIRECTION_ERROR;
        this->turn_buttons_by_direction(dir);
    }
    return;
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

int Controller::bun_2_led_index(char bun, bool first){
    switch (bun) {
        case 'u':
            return 2 + !first;
            break;
        case 'd':
            return 0 + !first;
            break;
        case 'l':
            return 6 + !first;
            break;
        case 'r':
            return 4 + !first;
            break;
        case 's':
            return 8 + !first;
            break;
    }
}
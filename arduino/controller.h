#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
    struct Color {
        int r;
        int g;
        int b;
    };
class Controller {

    enum Direction : int{
        all = 1,
        up,
        down,
        left,
        right,
        none,
        
        DIRECTION_ERROR
    };

    enum Effect : int {
        zero,
        effect_blank,
        effect_blue,
        effect_yellow,
        effect_cylon,
        effect_strobe,
        
        EFFECT_ERROR
    };

    private:
        Adafruit_NeoPixel* led_strip;
        int strip_length;
        int bun_led_length; // how many leds are in all of the buttons
        bool lighting_override;
        int effect_last_time;
        int effect_step;
        Effect current_effect;
        Direction current_direction;
        Color button_color;
        Color sel_color;
        bool is_sel;

    public:
        Controller(Adafruit_NeoPixel* led_strip, int strip_length, int bun_led_length);
        Controller(Controller&) = delete;
        Controller(const Controller&) = delete;
        Controller& operator=(Controller&) = delete;
        const Controller& operator=(const Controller&) = delete;

        // Led Strip Utility Functions
        // ===========================

        void showStrip(); // updates led strip
        void showEffect(); // updates led strip only if effects are not overriden
        void setPixel(  int Pixel,
                        Color color); // sets color of a single pixel
        void set_multi_leds(Color color,
                            int start,
                            int end); // sets color of a pixels range
        void turnoff_two_leds(int i, int j);
        void set_effect_leds(Color color); // sets color of all effect leds
        void set_buns_leds(Color color); // sets color of all buttons leds
        void clear_buns(); // turns off every button LED
        void set_buns_leds_default(); // sets color of all buttons leds to default color
        void set_all_leds(Color color); // sets color of every single led
        void fire_effect(); // runs a step of current effect
        void update_effect(Effect new_effect);
        void turn_buttons_by_direction(Direction direction); // updates buttons lighting
        void turn_buttons_manually(String buttons, Color color); // updates buttons lighting
        void update_color_default(Color color, bool show, bool sel); // updates buttons default color
        void update_buttons_default(Color color, bool show=true); // updates buttons default color
        void update_sel_default(Color color, bool show=true); // updates buttons default color
        void update_sel_state(bool sel, bool show=true); // updates sel state
        int bun_2_led_index(char bun, bool firs);

        // Effects Helper Functions
        // ========================

        void CylonBounce(   Color color,
                            int EyeSize,
                            int SpeedDelay,
                            int ReturnDelay );
        void mono_color_cycle(Color color);
        void strobe(Color color, int FlashDelay);
        void mono_color(Color color);

        // Effects
        // ========================

        void blank();
        void yellow();
        void blue();
        void red_cylon();
        void white_strobe();

    };


#endif
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
        int effect_last_time; // last time an effect has been fired
        int effect_step; // current step of effect's state machine
        Effect current_effect;
        Color button_color;

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
        void set_all_leds(Color color); // sets color of every single led in strip
        void set_effect_leds(Color color); // sets color of all effect leds
        void set_buns_leds(Color color); // sets color of all buttons as the given color

        // Buttons Color Utility Functions
        // ===========================

        void turn_buttons_default(String buttons); // sets color of given buttons as the default color
        void turn_buttons_manually(String buttons, Color color); // sets color of given buttons as the given color
        void update_buttons_default(Color color);   // updates buttons default color
                                                    // NOTICE: this won't take effect until
                                                    // the next time set_buns_leds_default()
                                                    // is called
        void clear_given_buns(String buttons); // turns off selected button LED
        void clear_buns(); // turns off every button LED
        int bun_2_id(char bun);

        // Effects Utility Functions
        // ===========================

        void fire_effect(); // runs a step of current effect
        void update_effect(Effect new_effect); // changes the current effect which should be fired

        // Effects Helper Functionss
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
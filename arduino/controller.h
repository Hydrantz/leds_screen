#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>


class Controller {
    private:
        Adafruit_NeoPixel* led_strip;
        int strip_length;
        int bun_led_length; // how many leds are in the buttons
        bool lighting_override;
        int effect_last_time;
        int effect_step;

    public:
        Controller(Adafruit_NeoPixel* led_strip,int strip_length,int bun_led_length);

        void (Controller::*current_effect)(); // Current showing effect

        // Led Strip Control Functions
        // ===========================

        void fireEffect();
        void showStrip(); // updates led strip
        void showEffect(); // updates led strip only if effects are not overriden
        void setPixel(  int Pixel,
                        byte red,
                        byte green,
                        byte blue); // sets color of a single pixel
        void set_multi_leds(byte red,
                            byte green,
                            byte blue,
                            int start,
                            int end); // sets color of a pixels range
        void set_effect_leds(   byte red,
                                byte green,
                                byte blue); // sets color of all effect leds
        void set_buns_leds( byte red,
                            byte green,
                            byte blue); // sets color of all buttons leds
        void set_all_leds(  byte red,
                            byte green,
                            byte blue); // sets color of every single led

        // Effects Helper Functions
        // ========================

        void CylonBounce(   byte red,
                            byte green,
                            byte blue,
                            int EyeSize,
                            int SpeedDelay,
                            int ReturnDelay );
        void mono_color_cycle(byte red, byte green, byte blue);
        void strobe(byte red, byte green, byte blue, int FlashDelay);
        void mono_color(byte red, byte green, byte blue);

        // Effects
        // ========================

        void blank();
        void yellow();
        void blue();
        void red_cylon();
        void white_strobe();

    };


#endif
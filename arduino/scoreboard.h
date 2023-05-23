#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "Arduino.h"


enum class Character : int{
    zero = 0,
    one,
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    A, B, C, D, E, F, G, H, I, J,
    K, L, M, N, O, P, Q, R, S, T,
    U, V, W, X, Y, Z, dollar, percent,
    ampersand, star, all, blank,

    ERROR
};

class Scoreboard {

  private:
    int clk_pin;
    int latch_pin;
    int seven_pin;
    int sixteen_pin;
    int transmitting_step;
    int current_digit;
    String seven_segment_text;
    String sixteen_segment_text;

  public:
    Scoreboard(int clk, int latch, int seven, int sixteen);
    Scoreboard(Scoreboard&) = delete;
    Scoreboard(const Scoreboard&) = delete;
    Scoreboard& operator=(Scoreboard&) = delete;
    const Scoreboard& operator=(const Scoreboard&) = delete;

    void transmit_score();

    void transmit_sixteen();

    void clock_signals(bool bit_seven, bool bit_sixteen);

    void transmit_character(int pin, bool is_high);

    int char_to_index(char chr);

    bool get_current_bit(bool is_seven_segment);

    void set_segment_text(bool is_seven_segment, String new_text);
  };

#endif
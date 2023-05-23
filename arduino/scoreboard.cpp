#include "scoreboard.h"
#include "Arduino.h"

byte seven_segment_map[10][8] = {
  {1,1,1,1,1,1,0,0},//0
  {0,1,1,0,0,0,0,0},//1
  {1,1,0,1,1,0,1,0},//2
  {1,1,1,1,0,0,1,0},//3
  {0,1,1,0,0,1,1,0},//4
  {1,0,1,1,0,1,1,0},//5
  {1,0,1,1,1,1,1,0},//6
  {1,1,1,0,0,0,0,0},//7
  {1,1,1,1,1,1,1,0},//8
  {1,1,1,1,0,1,1,0}, //9
};

byte sixteen_segment_map[42][16] = {
  {1,1,1,1,1,1,1,1,0,0,0,0,1,0,0,1},
  {0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0},
  {1,1,1,0,1,1,1,0,1,1,0,0,0,0,0,0},
  {1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0},
  {0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0},
  {1,1,0,1,1,1,0,1,1,1,0,0,0,0,0,0},
  {1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0},
  {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
  {1,1,1,1,1,1,0,1,1,1,0,0,0,0,0,0},
  {1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0},
  {1,1,1,1,1,1,0,0,0,1,0,1,0,0,1,0},
  {1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,0,0,0,0,0,1,0,0,1,0},
  {1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0},
  {1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {1,1,0,1,1,1,1,1,0,1,0,0,0,0,0,0},
  {0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0},
  {1,1,0,0,1,1,0,0,0,0,0,1,0,0,1,0},
  {0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,1,1,0,0},
  {0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,1,1,0,0,1,1,0,0,1,0,1,0,0,0},
  {0,0,1,1,0,0,1,1,0,0,1,0,0,1,0,0},
  {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
  {1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0},
  {1,1,1,0,0,0,1,1,1,1,0,0,0,1,0,0},
  {1,1,0,1,1,1,0,1,1,1,0,0,0,0,0,0},
  {1,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0},
  {0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,0,0,0,1,0,0,1},
  {0,0,1,1,0,0,1,1,0,0,0,0,0,1,0,1},
  {0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1},
  {0,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0},
  {1,1,0,0,1,1,0,0,0,0,0,0,1,0,0,1},
  {1,1,0,1,1,1,0,1,1,1,0,1,0,0,1,0},
  {1,0,0,1,0,1,0,1,1,1,0,1,1,0,1,1},
  {1,0,0,0,1,1,1,0,1,0,1,1,0,1,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

Scoreboard::Scoreboard(int clk, int latch, int seven, int sixteen){
  this->clk_pin = clk;
  this->latch_pin = latch;
  this->seven_pin = seven;
  this->sixteen_pin = sixteen;
  this->transmitting_step = 0;
  this->current_digit = 0;
  this->seven_segment_text = "        ";
  this->sixteen_segment_text = "        ";
}

void Scoreboard::transmit_score(){
  if (this->transmitting_step == 0){
    digitalWrite(this->latch_pin, LOW);
  }
  if (this->transmitting_step < 16){
    bool bit_seven = false;
    if (this->transmitting_step >= 8){
      bit_seven = this->get_current_bit(true);
    }
    bool bit_sixteen = this->get_current_bit(false);
    this->clock_signals(bit_seven, bit_sixteen);
    this->transmitting_step += 1;
  }
  else if (this->transmitting_step < 24){
    bool bit_digit = (this->transmitting_step != 23 - this->current_digit);
    this->clock_signals(bit_digit, bit_digit);
    this->transmitting_step += 1;
  }
  if (this->transmitting_step >= 24) {
    digitalWrite(this->latch_pin, HIGH);
    this->transmitting_step = 0;
    if (this->current_digit < 7){
      this->current_digit += 1;
    }
    else{
      this->current_digit = 0;
    }
  }
}

void Scoreboard::clock_signals(bool bit_seven, bool bit_sixteen){
  // Serial.print(this->transmitting_step);
  // Serial.print(" ");
  // Serial.print(bit_sixteen);
  // Serial.println();
  digitalWrite(this->clk_pin, LOW);
  this->transmit_character(this->seven_pin, bit_seven);
  this->transmit_character(this->sixteen_pin, bit_sixteen);
  digitalWrite(this->clk_pin, HIGH);
}

void Scoreboard::transmit_character(int pin, bool bit){
  if (bit){
    digitalWrite(pin, HIGH);
  }
  else{
    digitalWrite(pin, LOW);
  }
}

int Scoreboard::char_to_index(char chr){
  if (chr >= '0' && chr <= '9'){
    return chr - '0';
  }
  if (chr >= 'A' && chr <= 'Z'){
    return chr - 'A' + (int)Character::A;
  }
  switch (chr)
  {
  case ' ':
    return int(Character::blank);
  case '$':
    return int(Character::dollar);
  case '%':
    return int(Character::percent);
  case '&':
    return int(Character::ampersand);
  case '*':
    return int(Character::star);
  case '#':
    return int(Character::all);
  }
}

bool Scoreboard::get_current_bit(bool is_seven_segment){
  char current_char;
  if (is_seven_segment){
    current_char = this->seven_segment_text[this->current_digit];
  }
  else {
    current_char = this->sixteen_segment_text[this->current_digit];
  }
  int current_char_index = this->char_to_index(current_char);
  int bit;
  if (is_seven_segment){
    bit = seven_segment_map[current_char_index][this->transmitting_step-8];
  }
  else{
    bit = sixteen_segment_map[current_char_index][this->transmitting_step];
  }
  return bit;
}

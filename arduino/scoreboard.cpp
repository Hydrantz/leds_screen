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

byte sixteen_segment_map[10][16] = {
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

Scoreboard::Scoreboard(int clk, int latch, int seven, int sixteen){
  this->clk = clk;
  this->latch = latch;
  this->seven = seven;
  this->sixteen = sixteen;
  this->transmitting_step = 0;
  this->current_digit = 0;
  this->seven_segment_text = "12345678";
  this->sixteen_segment_text = "12345678";
}

void Scoreboard::transmit_score(){
  if (this->transmitting_step == 0){
    digitalWrite(this->latch, LOW);
  }
  if (this->transmitting_step < 8){
    int bit = this->get_current_bit(true);
    this->transmit_character(this->seven, bit);
    this->transmitting_step += 1;
  }
  else if (this->transmitting_step < 16){
    this->transmit_character(this->seven, this->transmitting_step != 15 - this->current_digit);
    this->transmitting_step += 1;
  }
  else {
    digitalWrite(this->latch, HIGH);
    this->transmitting_step = 0;
    if (this->current_digit < 7){
      this->current_digit += 1;
    }
    else{
      this->current_digit = 0;
    }
  }
}

void Scoreboard::transmit_character(int pin, bool is_high){
  digitalWrite(this->clk, LOW);
    if (is_high){
      digitalWrite(pin, HIGH);
    }
    else{
      digitalWrite(pin, LOW);
    }
  digitalWrite(this->clk, HIGH);
}

int Scoreboard::char_to_index(char chr){
  if (chr >= '0' && chr <= '9'){
    return chr - '0';
  }
  if (chr >= 'A' && chr <= 'Z'){
    return chr - 'A' + 10;
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
    return int(Character::hashtag);
  }
}

int Scoreboard::get_current_bit(bool is_seven_segment){
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
    bit = seven_segment_map[current_char_index][this->transmitting_step];
  }
  else{
    bit = sixteen_segment_map[current_char_index][this->transmitting_step];
  }
  return bit;
}

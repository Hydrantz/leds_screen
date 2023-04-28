#ifndef COMMUNICATION_H
#define COMMUNICATION_H

// Communication variables
String message = "";
volatile byte rx_byte = 0;
volatile int bit_position = 0;
volatile bool new_data = false;

void get_input(String data, String first, String last,
            String bun_state, void (*current_effect)()) {
  int ind = data.indexOf('|');
  first = data.substring(0, ind);
  last = data.substring(ind+1);
  if (first == "a") {
    // manage_buns(last);
  }
  else if (first == "w"){
    bun_state = true;
    int intensity = last.toInt();
    // mono_color(intensity,intensity,intensity);
    // showStrip();
  }
  else if (first == "release"){
    bun_state = false;
  }
  else {
    if (first == "blank") {
    //   current_effect = &blank;
    }
    else if (first == "y") {
    //   current_effect = &yellow;
    }
    else if (first == "b") {
    //   current_effect = &blue; 
    }
    else if (first == "c") {
    //   current_effect = &red_cylon;
    }
    else if (first == "s") {
    //   current_effect = &white_strobe;
    }
  }
}

#endif
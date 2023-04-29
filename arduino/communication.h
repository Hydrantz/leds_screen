#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "Arduino.h"
#include "controller.h"

class Communication {
  private:
    char rx_char;
    String data;
    char command;
    String argument;

  public:
    Communication();

    void get_input(Controller &ctrl);
    void use_input(Controller &ctrl);

  };

#endif
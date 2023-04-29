#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "Arduino.h"
#include "controller.h"

class Communication {
  private:
    String data;
    String command;
    String argument;

  public:
    Communication();

    void get_input(Controller &ctrl);

  };

#endif
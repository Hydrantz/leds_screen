#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "Arduino.h"
#include "controller.h"
#include "scoreboard.h"

class Communication {
  private:
    char rx_char;
    String cumulative_data;
    char command;
    String argument;

  public:
    Communication();
    Communication(Communication&) = delete;
    Communication(const Communication&) = delete;
    Communication& operator=(Communication&) = delete;
    const Communication& operator=(const Communication&) = delete;

    void get_input(Controller &ctrl, Scoreboard &score);
    void interpret_input(Controller &ctrl, Scoreboard &score);

  };

#endif
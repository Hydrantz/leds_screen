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
    Communication(){
      data = "";
      command = "";
      argument = "";
    }

    void get_input(Controller ctrl) {
      data = Serial.readStringUntil('@');
      command = data[0];
      argument = data.substring(1);
      if (command == "a") {
        ctrl.update_buttons(argument.toInt());
      }
      else if (command == "e") {
        ctrl.update_effect(argument.toInt());
      }
      return;
    }

  };

#endif
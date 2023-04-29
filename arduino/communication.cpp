#include "communication.h"
#include "Arduino.h"

Communication::Communication(){
    this->data = "";
    this->command = "";
    this->argument = "";
}

void Communication::get_input(Controller &ctrl) {
    this->data = Serial.readStringUntil('@');
    this->command = this->data[0];
    this->argument = this->data.substring(1);
    if (this->command == "a") {
    ctrl.update_buttons(this->argument.toInt());
    }
    else if (this->command == "e") {
    ctrl.update_effect(this->argument.toInt());
    }
    return;
}
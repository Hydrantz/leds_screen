#include "communication.h"
#include "Arduino.h"

Communication::Communication(){
    this->rx_char = ' ';
    this->data = "";
    this->command = "";
    this->argument = "";
}

void Communication::get_input(Controller &ctrl) {
    if (Serial.available() <= 0) {
        return;
    }    
    this->rx_char = Serial.read();
    if (this->rx_char == '@'){
        this->command = this->data[0];
        this->argument = this->data.substring(1);
        this->data = "";
        this->use_input(ctrl);
        return;
    }
    if (this->rx_char != '\n'){
        this->data += this->rx_char;
    }
    return;
}

void Communication::use_input(Controller &ctrl){
    switch (this->command) {
    case 'a':
        ctrl.update_buttons(this->argument.toInt());
        break;
    case 'e':
        ctrl.update_effect(this->argument.toInt());
        break;
    case 's':
        ctrl.set_sel(this->argument.toInt());
        break;
    }
}
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
        Serial.println(this->data);
        this->command = this->data[0];
        this->argument = this->data.substring(1);
        this->data = "";
        if (this->command == "a") {
            ctrl.update_buttons(this->argument.toInt());
        }
        else if (this->command == "e") {
            ctrl.update_effect(this->argument.toInt());
        }
        return;
    }
    if (this->rx_char != '\n'){
        this->data += this->rx_char;
    }
    Serial.println(this->data);
    return;
}
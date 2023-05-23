#include "communication.h"
#include "scoreboard.h"
#include "Arduino.h"

Communication::Communication(){
    this->rx_char = ' ';
    this->data = "";
    this->command = "";
    this->argument = "";
}

void Communication::get_input(Controller &ctrl, Scoreboard &score) {
    if (Serial.available() <= 0) {
        return;
    }    
    this->rx_char = Serial.read();
    if (this->rx_char == '@'){
        this->command = this->data[0];
        this->argument = this->data.substring(1);
        this->data = "";
        this->use_input(ctrl, score);
        return;
    }
    if (this->rx_char != '\n'){
        this->data += this->rx_char;
    }
    return;
}

void Communication::use_input(Controller &ctrl, Scoreboard &score){
    switch (this->command) {
    case 'a':
        ctrl.update_buttons(this->argument.toInt());
        break;
    case 'e':
        ctrl.update_effect(this->argument.toInt());
        break;
    case 'o':
        ctrl.set_sel(this->argument.toInt());
        break;
    case 's':
        score.set_segment_text(true, this->argument);
        break;
    case 't':
        score.set_segment_text(false, this->argument);
        break;
    }
}
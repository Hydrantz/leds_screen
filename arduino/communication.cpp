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
    char sel;
    Color color;
    switch (this->command) {
        case 'a':
            ctrl.turn_buttons_by_direction(this->argument.toInt());
            break;
        case 'b':
            color.r = this->argument.substring(0,3).toInt();
            color.g = this->argument.substring(3,6).toInt();
            color.b = this->argument.substring(6,9).toInt();
            sel = this->argument.charAt(9);
            if (sel == '0'){
                ctrl.update_sel_default(color);
            }
            else{
                ctrl.update_buttons_default(color);
            }
            break;
        case 'm':
            color.r = this->argument.substring(0,3).toInt();
            color.g = this->argument.substring(3,6).toInt();
            color.b = this->argument.substring(6,9).toInt();
            ctrl.turn_buttons_manually(this->argument.substring(9), color);
            break;
        case 'o':
            sel = this->argument.toInt();
            ctrl.update_sel_state(sel);
            break;
        case 'e':
            ctrl.update_effect(this->argument.toInt());
            break;
        case 's':
            score.set_segment_text(true, this->argument);
            break;
        case 't':
            score.set_segment_text(false, this->argument);
            break;
    }
}
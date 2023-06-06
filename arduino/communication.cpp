// this class manages serial communication at the Slave device.
// also responsible for its interpretation and for sending the
// data to the Controller and Scoreboard modules.


#include "communication.h"
#include "scoreboard.h"
#include "Arduino.h"

Communication::Communication(){
    this->rx_char = ' ';    // a char containing last received character
    this->cumulative_data = ""; // a String containing the accumulative data (not necessarily completed) 
    this->command = ""; // a String containing the command part of the last COMPLETED 
                        // message received
    this->argument = "";    // a String containing the argument part of the last COMPLETED 
                            // message received
}

void Communication::get_input(Controller &ctrl, Scoreboard &score) {
    // this function is called in every iteration of the mainloop ( in arduino.ino).
    // it gets a reference to the Controller manager and to the Scoreboard manager.
    // each iteration, the function checks if new data is available in the
    // serial communication channel. Otherwise, it returns void for the mainloop to
    // continue running without further delays.
    if (Serial.available() <= 0) {
        return;
    }
    // reads a single character in each iteration (in order to minimize delays for
    // other functions in the mainloop)
    this->rx_char = Serial.read();
    // and checks if the character is "@" (end of message)
    if (this->rx_char == '@'){
        // which in this case, cumulative_data is completed and whole,
        // and needs to be updated into command and argument
        this->command = this->cumulative_data[0];
        this->argument = this->cumulative_data.substring(1);
        // and afterwards reset for the next message.
        this->cumulative_data = "";
        // finally, call the interpretation function and return to mainloop. 
        this->interpret_input(ctrl, score);
        return;
    }
    // otherwise, the read character should be concatenated to cumulative_data
    // (unless in is a new-line character)
    if (this->rx_char != '\n'){
        this->cumulative_data += this->rx_char;
    }
    // then return to mainloop.
    return;
}

void Communication::interpret_input(Controller &ctrl, Scoreboard &score){
    // this function is called every time a new message was just received entirely
    // and has been updated into "command" and "argument".
    // it gets a reference to the Controller manager and to the Scoreboard manager.
    // the function analyzes the "command" and then sends "argument" to the
    // appropriate property of the appropriate module.

    // since new variables cannot be declared inside a switch-case clock,
    // these two helper variables will be used in some of the cases (independently).
    char sel;
    Color color;

    // checks which command was received
    switch (this->command) {
        case 'b':
            // Controller: update buttons' default color.
            // this won't achieve immediate results, but
            // will affect the next call of 
            color.r = this->argument.substring(0,3).toInt();
            color.g = this->argument.substring(3,6).toInt();
            color.b = this->argument.substring(6,9).toInt();
            ctrl.update_buttons_default(color);
            break;
        case 'm':
            // Controller: turn on buttons lights manually.
            // this will immediately turn on every selected
            // button with the selected color.
            color.r = this->argument.substring(0,3).toInt();
            color.g = this->argument.substring(3,6).toInt();
            color.b = this->argument.substring(6,9).toInt();
            ctrl.turn_buttons_manually(this->argument.substring(9), color);
            break;
        case 'd':
            // Controller: turn on buttons lights as default.
            // this will immediately turn on every selected
            // button with default color.
            ctrl.turn_buttons_default(this->argument);
            break;
        case 'r':
            // turns off selected button LED
            ctrl.clear_given_buns(this->argument);
            break;
        case 'c':
            // turns off every button LED
            ctrl.clear_buns();
            break;
        case 'e':
            // Controller: update current controller effect
            ctrl.update_effect(this->argument.toInt());
            break;
        case 's':
            // Scoreboard: update showing score.
            score.set_segment_text(true, this->argument);
            break;
        case 't':
            // Scoreboard: update showing text.
            score.set_segment_text(false, this->argument);
            break;
    }
}
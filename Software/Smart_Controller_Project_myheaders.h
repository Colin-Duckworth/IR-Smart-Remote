//I am the author of this file, it contains function definitons to improve readability of Smart_Controller_Build.ino//

#include <Arduino.h>
#include <IRremote.hpp>
#include "PinDefinitionsAndMore.h" // Sets I/O pins for IR communication
#include <EEPROM.h>

//tell compiler variables and functions are declared in elsewhere
extern int HDMI;  
extern int soundbar;
extern int soundbar_mute;
extern int projector;
extern int power_state;
extern int mute_RLED; //Mute LED
extern int BLED; //blue LED
extern int GLED; //green LED
extern int YLED; //yellow LED
extern int error_led; //ERROR LED
extern int input_pin_nums[]; //input pin numbers
extern int raw_pins[]; //polled pin values
extern int _pins[]; //saved pin values
extern int counters[]; //pin change counters
extern int signal_counters[]; //signal counters to test if send_signal() function
extern int test_counter;

void power_on_projector();
void power_off_projector();
void power_toggle_soundbar();
void volume_up_soundbar();
void volume_down_soundbar();
void mute_toggle_soundbar();
void update_HDMI_state(int _HDMI);
void set_HDMI1_multiplexer();
void set_HDMI2_multiplexer();
void set_HDMI3_multiplexer();
void update_LEDS();
void send_signal(int counter_val, int pin_num);


void update_LEDS(){
  if (power_state == 1){ // If system is ON, shine HDMI indicator LEDS accordingly
    if (HDMI == 1) {
      digitalWrite(9, HIGH); //HDMI 1 LED
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
    }
    if (HDMI == 2) {
      digitalWrite(9,  LOW);
      digitalWrite(10, HIGH); //HDMI 2 LED
      digitalWrite(11, LOW);
    }
    if (HDMI == 3) {
      digitalWrite(9,  LOW);
      digitalWrite(10, LOW);
      digitalWrite(11, HIGH); //HDMI 3 LED
    }
  }
  if (power_state == 0){  // If system is off, do not shine any LEDS
    digitalWrite(9,  LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  }
  if (mute_RLED == 1) {
    digitalWrite(12,  HIGH);  // Turn on mute led if mute is toggled on
  }
  if (mute_RLED == 0) {
    digitalWrite(12,  LOW);  // Turn off mute led if mute is toggled off
  }
}

//Poll pins
void poll_pins_and_send() {
  for (int i = 0; i < 5; i += 1){
    raw_pins[i] = digitalRead(input_pin_nums[i]); //polls raw pins
  }
  for (int i = 0; i < 5; i += 1){
    if (raw_pins[i] != _pins[i]) { //if polled pin state is not saved pin state
        delay(10); //debouncing delay

        if (raw_pins[i] != _pins[i]) { //if polled pin state is still not saved pin state
          _pins[i] = raw_pins[i]; //sets new saved value
          counters[i] += 1; //increments counter
          send_signal(counters[i], input_pin_nums[i]);  //calls signal sender function
        }
    }
  }
}

//function for sending out signals
void send_signal(int counter_val, int pin_num){
  if (counter_val % 2 == 0){
    switch (pin_num) {
        case 4:  // Toggle Power ON/OFF
            if (power_state == 0){
              signal_counters[0] += 1; //increments signal counter
              power_on_projector(); // sends IR signal to power on projector
              power_toggle_soundbar(); // toggles soundbar power
              EEPROM.write(0, 1); // stores power toggle in address 0 (to ON) in non-volatile memory
            }
            if (power_state == 1){
              signal_counters[0] += 1; //increments signal counter
              power_off_projector(); // sends IR signal to power off projector
              power_toggle_soundbar();// toggles soundbar power
              delay(1000);
              power_off_projector(); // sends IR signal to power off projector, projector requires two of these inputs
              EEPROM.write(0, 0); // stores power toggle in address 0 (to OFF) in non-volatile memory
            }
            power_state = !power_state; //toggles power state
            break;
        case 5:  // Increment HDMI Input
            signal_counters[1] += 1;
            if (HDMI == 0){  //This is for the first initialization loop, because non volatile memory starts with a value of 0
              HDMI = 1; }
            HDMI += 1;
            if (HDMI == 4){  //Ensures if HDMI increments at 3 it goes to 1
              HDMI = 1; }
            update_HDMI_state(HDMI);  //sends IR signal to multiplexer
            EEPROM.write(1, HDMI); // stores power toggle in address 0 (to ON) in non-volatile memory
            break;
        case 6:  // Volume Up
            signal_counters[2] += 1;
            volume_up_soundbar();  //  sends IR signal to soundbar
            break;
        case 7:  // Volume Down
            signal_counters[3] += 1;
            volume_down_soundbar();  //  sends IR signal to soundbar
            break;
        case 8:  // Mute toggle
            signal_counters[4] += 1;
            mute_toggle_soundbar(); // sends IR signal to mute soundbar
            mute_RLED = !mute_RLED; //toggles mute LED
            EEPROM.write(2, mute_RLED); // stores power toggle in address 0 (to ON) in non-volatile memory
            break;
    }
  }
}

//Update HDMI state
void update_HDMI_state(int _HDMI) {
  switch(_HDMI) {
    case 1:
        set_HDMI1_multiplexer();
        BLED = 1; // LED for HDMI 1
        GLED = 0;
        YLED = 0;
        break;
    case 2 :
        set_HDMI2_multiplexer();
        BLED = 0;
        GLED = 1; // LED for HDMI 2
        YLED = 0;
        break;
    case 3:
        set_HDMI3_multiplexer();
        BLED = 0;
        GLED = 0;
        YLED = 1; // LED for HDMI 3
        break;
    default:
        error_led = 1;  // LED for ERROR
  }
}


//IR Send Functions
//IrSender.sendNEC(Device address, command, repeat number);
//All addresses were found using TSOP384 reciever with IRREMOTE library using RecieveDump();
void set_HDMI1_multiplexer() {
  IrSender.sendNEC(0x80, 0x2, 3);
//  HDMI = 1;
}

void set_HDMI2_multiplexer() {
  IrSender.sendNEC(0x80, 0x4, 3);
//  HDMI = 2;
}

void set_HDMI3_multiplexer() {
  IrSender.sendNEC(0x80, 0x6, 3);
//  HDMI = 3;
}

void power_toggle_soundbar() {
  IrSender.sendNEC(0x0, 0x40, 6);
}

void volume_up_soundbar() {
  IrSender.sendNEC(0x0, 0x41, 3);
}

void volume_down_soundbar() {
  IrSender.sendNEC(0x0, 0x45, 3);
}

void mute_toggle_soundbar() {
  IrSender.sendNEC(0x0, 0x48, 3);
}

void power_on_projector() {
  IrSender.sendNEC(0x32, 0x2, 3);
}
void power_off_projector() {
  IrSender.sendNEC(0x32, 0x2E, 3);
}

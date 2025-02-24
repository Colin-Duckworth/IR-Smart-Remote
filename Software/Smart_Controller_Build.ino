//I am the author of this file//

#include <Arduino.h>
#include <IRremote.hpp>
#include "PinDefinitionsAndMore.h" // Sets I/O pins for IR communication
#include "Smart_Controller_Project_myheaders.h" // my header file
#include <EEPROM.h>


int HDMI = EEPROM.read(1); // remembers HDMI state (stored in address 1) from last use
int soundbar = 0; //soundbar var
int soundbar_mute = 0; //mute var
int projector = 0; //projector var
int power_state = EEPROM.read(0); // remembers power state (stored in address 0) from last turn on
int mute_RLED = EEPROM.read(2); // remembers mute LED state (stored in address 2) from last turn on
int BLED; //BLUE HDMI1 LED
int GLED; //GREEN HDMI2 LED
int YLED; //YELLOW HDMI3 LED
int error_led = 0; //ERROR LED
int input_pin_nums[] = {4, 5, 6, 7, 8}; //input pin numbers
int raw_pins[] = {1, 1, 1, 1, 1}; //input pin states start pulled up
int _pins[] = {1, 1, 1, 1, 1}; //input pin states start pulled up
int counters[] = {0, 0, 0, 0, 0};  //counters initialized to 0
int signal_counters[] = {0, 0, 0, 0, 0};  //signal counter for serial monitor tracking
String str;

void print_serial_stuff(){  //uncomment when using serial monitor window
  //prints button counter values
  //Serial.print(str + "button Counter values " + counters[0] + " " + counters[1] + " " + counters[2] +  " " + counters[3] +  " " + counters[4] + "\n");
  //prints signal send counter
  //Serial.print(str + "signal send Counter values " + signal_counters[0] + " " + signal_counters[1] + " " + signal_counters[2] +  " " + signal_counters[3] +  " " + signal_counters[4] + "\n");
  //prints Power and HDMI values
  //Serial.print(str + "Power state " + power_state + "  HDMI Value " + HDMI + "\n\n ");
  //prints Non-volatile values
  //Serial.print(str + "Power state " + power_state + "  HDMI Value " + HDMI + "\n\n ");
}
void eeprom_setup(){
  if (power_state == 255) {
      power_state = 0;  // Assuming 0 means OFF
      EEPROM.write(0, power_state); // Store the default value in EEPROM
    }
  if (HDMI == 255) {
      HDMI = 1;  // starting HDMI of 1
      EEPROM.write(1, HDMI); // Store the default value in EEPROM
    }
}


void setup() {
  eeprom_setup(); // sets up the eeprom for original use
  Serial.begin(9600);
  IrSender.begin(3, false, 0); 
  pinMode(4,  INPUT); //sets input pins
  pinMode(5,  INPUT);
  pinMode(6,  INPUT);
  pinMode(7,  INPUT);
  pinMode(8,  INPUT);
  pinMode(9,  OUTPUT); //sets output pins
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  poll_pins_and_send(); //checks for button presses,updates variables, and sends corresponding IR signals
  update_LEDS(); //updates indicator LEDs based on updated states
  print_serial_stuff(); //prints info to the serial monitor
  delay(10);
}

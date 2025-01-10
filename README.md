# IR-Smart-Remote
IR Smart Remote using an Arduino Uno to control HDMI Multiplexer, Optoma Projector, and wireless soundbar using various protocols (including NEC).

# Overview
The IR Smart Remote utilizes an Arduino Uno board to interface with a variety of devices via infrared signals. The remote allows users to control multiple devices with a simple button interface. Key features of the design include the ability to store device states in EEPROM for persistent memory, support for a 9V battery for portability, indicator LEDs to display system states, and a modular setup to easily modify or add new IR signals.

# Components Used
- Arduino Uno microcontroller
- IR LED: TSAL6200
- IR Reciever: TSOP384
- 2N2222 BJT Transistor: To drive the IR LED
- Breadboard: For prototyping and circuit connections
- Press buttons: For user input and controlling the Arduino
- 9V Battery: To power the system
- Indicator LEDs: To show the system status (Power, HDMI, Mute, Volume)

# Libraries Used
- IRREMOTE library: To send and receive IR signals using various protocols

# Key Features
- Modular Control: The remote can control an HDMI Multiplexer, Optoma Projector, and wireless soundbar using IR signals sent via an Arduino.
- EEPROM Memory: When powered off, the system saves the on/off states of the devices in EEPROM memory, so the last known state is restored when the system is powered back on.
- Power Efficient: The system runs on a 9V battery, making it portable for use in various environments. The use of pull-up resistors ensures that the circuit consumes minimal power when the buttons are not being pressed, preserving battery life.
- Customizable Signals: The IRREMOTE library makes it easy to overwrite or modify the IR signals that the smart remote sends out, allowing for flexible control and future expansion to other devices or protocols.
- Button Interface: Simple press buttons are used for user interaction with the system.
- Indicator LEDs: Visual feedback is provided with four indicator LEDs that show the current state of the system: -- POWER: Indicates whether the system is powered on. -- HDMI: Indicates the status of the HDMI device (on/off). -- MUTE: Indicates the mute status of the soundbar or other audio device. -- VOLUME: Indicates the volume status of the soundbar.

# Troubleshooting

Problem 1: LEDs Not Triggering Properly
- The system was not responding consistently to the button interfaces, causing the LEDs to trigger incorrectly.
- Added functionality to monitor the counters through the serial monitor while interfacing with the buttons.
- Diagnosed the issue as button bouncing, which led to inconsistent counter updates.
- Implemented debouncing delays to stabilize the button presses and ensure functional counters.
- The issue was resolved, resulting in consistent LED triggering with stable button interactions.

Problem 2: Loss of Saved State Data on Power Loss

- The microcontroller lost track of the current state whenever it lost power, which was critical since the IR commands for power on and off were unique and not toggles.
- Observed that the state data was lost upon power cycling, affecting the system’s ability to maintain its state.
- Integrated EEPROM storage to save the necessary state variables, ensuring data retention even after power loss.
- The system now retains its state after power loss, allowing it to boot up with the correct information.

Problem 3: Inconsistent IR LED Communication
- The IR LED was not consistently communicating with devices, especially at a distance.
- Used an oscilloscope to verify the IR LED was sending the correct signal, but observed the signal strength was insufficient.
- Determined that the microcontroller's output current (~20mA) was inadequate for the desired range, which required closer to 200mA.
- Ordered and implemented the TSAL6200 IR LED capable of handling higher current.
- Switched the microcontroller’s IR LED output pin to the base of a transistor switch, allowing 235mA to flow through the LED.
- The updated configuration resulted in functional and reliable IR communication over the desired range.

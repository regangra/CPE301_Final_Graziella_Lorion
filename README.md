# CPE301_Final
Graziella Lorion

# Evaporation Cooler
LAST REVISION:
12/11/2023, 09:14 AM

# Introduction
In this project, we will:

• Monitor the water levels in a reservoir and print an alert when the level is too low

• Monitor and display the current air temperature and humidity on an LCD screen

• Start and stop a fan motor as needed when the temperature falls out of a specified range (high
or low)

• Allow the user to use a control to adjust the angle of an output vent from the system

• Allow the user to enable or disable the system using the on/off button

• Record the time and date every time the motor is turned on or off. This information should be
transmitted to a host computer (over USB).

# Materials
Below you can find the list of the materials used in this project. 

• 1 Arduino ATMega 2560

• 1 Macbook Pro with the Arduino IDE

• 1 USB programming cable

• 2 Potentiometer

• 1 LCD (16x2) display

• 1 830 Tie-points breadboard

• 1 extension breadboard
• 1 push button

• 1 LED Red

• 1 LED Blue

• 1 LED Green

• 1 LED Yellow

• 1 1KΩ resistor

• 5 330Ω resistor

• 1 Real Time Clock DS1307 module v3

• 1 Water Sensor

• 1 Temperature and Humidity Sensor DHT11

• 1 Step Motor 28BYJ-48

• 1 ULN2003 Stepper Motor Driver Module

• 1 Power supply module for Arduino

• 1 LCD 1602 module (with pin header)

• 1 Fan blade

• 1 Mini DC Motor 3 to 6V

• 1 L293D H-Bridge for rotary motor

• 1 9V1A Power Adapter

• Jumper kit


# Libraries
For this project, the libraries used are described below:

• LiquidCrystal.h - for the LCD

• dht.h - For DH11 Temperature and Humidity sensor

• Stepper.h - for the Step Motor

• Wire.h - for Real Time Clock

• DS1307RTC.h - for Real Time Clock

• TimeLib.h - for Real Time Clock


# Functions
# void setup()

– Initializes Serial Port at 9600 baudrate

– Starts LCD SCreen

– Initializes ADC

– Sets up Stepper Motor

– Defines PK0 as input for push button

– Enables pull-up resistor on PK0

– Sets up as output PH4for the water sensor, and PB0, PB1, PB2, and PB3 as for LEDs

– Reads Water Sensor, and Temperature and Humidity Sensor once

– Sets up RTC Date and Time


# void loop()

– Sets up push button to toggle between ON, OFF, and Reset

– Controls all states of the cooler (enable, disable, running, idle and error)

– Controls the angles of Stepper Motor

– Controls the Water Sensor and Temperature and Humidity Sensors reading for each 60
seconds

– Checks Water Level

– Check Temperature and Humidity

– Output on Serial Monitor all changes of states


# void U0init (int U0baud)

– Function to Initialize Serial Port


# void U0putchar(unsigned char U0pdata)

– Function to print on Serial Monitor


# void adc init()

– Function to Initialize Analog to Digital Converter


# int adc read(unsigned int adc channel num)

– Function to Read Analog Data and convert it to Digital


# ISR(PCINT0 vect)

– Interrupt Routine Service to monitor push button on PK0


# void printTime()

– Function to print the time stamp on states changes


# void displayTemperature()

– Function to display Temperature and Humidity on the LCD Screen


# unsigned int readWater()

– Function to read the water level


# void running()

– Function to Run the DC Motor and Turn the Blue LED ON


# void idle()

– Function to Stop the DC Motor and Turn the Green LED ON


# void error()

– Function to Low Water Level and Red LED ON


# void disable()

– Function to Disable the System and Yellow LED ON

# Circuit

![circuiDesing](https://github.com/regangra/CPE301_Final_Graziella_Lorion/assets/26099207/c471a010-ae8c-4dfc-9e30-2bdefafdfe4d)

# Cooler States

The cooler continuously cycles through several states, with the state transitions triggered by the user
or by events such as temperature changes.

State Descriptions

Some states include specific implementation requirements such as the use of ISR.

All States 

- The Real Time Clock is used to report via Serial port the time of each state transition,
and any changes to the stepper motor position for the vent.

All states except disabled

• Humidity and temperature are continuously monitored and reported on the LCD screen. Updates
occur once per minute.

• System responds to change in vent position control

• The push button controls the system.

– If the fan motor is off, it turns on, going to state IDLE or RUNNING.

– If the fan motor is on, it turns off, going to the state DISABLED.

Disabled

• Yellow LED turns on, all other LEDs are off.

• No monitoring of temperature, humidity, or water level.

• The Start button is monitored with the Interrupt Service Routine.

Idle

• Green LED turns on, all other LEDs are off.

• The exact time stamp records the transition between the states.

• The water level, temperature, and humidity are continuously monitored and shown on the LCD
screen

– If the water level is less than 63 (on a scale from 0 to 255), the state will change to ERROR.

– If the water level is more or equal to 63, and the temperature is above 20 degrees Celsius,
the state will change to RUNNING.

Error

• Red LED turns on, all other LEDs are off.

• The motor turns off and does not start regardless of the temperature.

• The LCD screen displays the message: ’Error: Low Water’.

- If the water level is less than 63 (on a scale from 0 to 255) and we press the RESET button
(On/ OFF button), the state will NOT change.

- If the water level is more or equal to 63, and we press the RESET button (On/OFF button),
the state will change to IDLE.

Running

• Blue LED turns on, all other LEDs are off.

• The motor turns on

• The water level, temperature, and humidity are continuously monitored.

  – If the water level is less than 63 (on a scale from 0 to 255) the state will change to ERROR
  – If the temperature is less than 20 degrees Celsius, the state will change to IDLE.

# Serial Output
For each state, the Serial Monitor will output the code below followed by the date and time

R - Running

X - Error

D - Disabled

I - Idle

'+' - Rotate Clockwise
    
'-' - Rotate Counterclockwise
 
Example:
![Screenshot 2023-12-10 at 18 04 37](https://github.com/regangra/CPE301_Final_Graziella_Lorion/assets/26099207/0387fa8e-b852-4685-8884-2987ca1ba6bd)


# Video

[Test Idle, Running, and Disable](https://youtu.be/Y5z2uhWGDNg)

[Test Running, Error, and Disable](https://youtu.be/8PdY1p2rPDE)

[Test Idle, Error, and Disable](https://youtu.be/eEYD23jZKs4)

# Report
[Report](https://www.overleaf.com/read/vhhmvnbhzzxf#6dfd8b)

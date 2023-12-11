# CPE301_Final
Graziella Lorion

# Evaporation Cooler
LAST REVISION:
12/10/2023, 04:33 PM

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
void setup()

void loop()

void U0init (int U0baud)

void U0putchar(unsigned char U0pdata)

void adc_init()

int adc_read(unsigned int adc_channel_num)

ISR(TIMER1_0VF_vect) motiro port PK0

void printTime()

void displayTemperature()

unsigned int readWater()

void running()

void idle()

void error()

void disable()

# Circuit

![circuiDesing](https://github.com/regangra/CPE301_Final_Graziella_Lorion/assets/26099207/c471a010-ae8c-4dfc-9e30-2bdefafdfe4d)


# Video




# CPE301_Final
Graziella Lorion

# Evaporation Cooler
Turn an LED on and off every second.
LAST REVISION:
12/05/2023, 06:33 AM
This example shows the simplest thing you can do with an Arduino to see physical output: it blinks the on-board LED.

Hardware Required:

Arduino Board
1 830 Tie-points breadboard
1 extension breadboard
1 push button
1 LED Red
1 LED Blue
1 LED Green
1 LED Yellow
2 potentiometer
1 1K ohm resistor
5 330 ohm resistor
1 Real Time Clock DS1307 module v3
1 Water Sensor
1 Temperature and Humidity Sensor DHT11
1 Step Motor 28BYJ-48
1 ULN2003 Stepper Motor Driver Module
1 Power supply module for Arduino
1 LCD 1602 module (with pin header)
1 Fan blade
1 Rotary Motor 3 to 6 V
1 9V1A power Adapter
 wires to connect components



Circuit
This example uses the built-in LED that most Arduino boards have. This LED is connected to a digital pin and its number may vary from board type to board type. To make your life easier, we have a constant that is specified in every board descriptor file. This constant is LED_BUILTIN and allows you to control the built-in LED easily. Here is the correspondence between the constant and the digital pin.

D13 - 101

D13 - Due

D1 - Gemma

D13 - Intel Edison

D13 - Intel Galileo Gen2

D13 - Leonardo and Micro

D13 - LilyPad

D13 - LilyPad USB

D13 - MEGA2560

D13 - Mini

D6 - MKR1000

D13 - Nano

D13 - Pro

D13 - Pro Mini

D13 - UNO

D13 - Yún

D13 - Zero

If you want to light an external LED with this sketch, you need to build this circuit, where you connect one end of the resistor to the digital pin correspondent to the LED_BUILTIN constant. Connect the long leg of the LED (the positive leg, called the anode) to the other end of the resistor. Connect the short leg of the LED (the negative leg, called the cathode) to the GND. In the diagram below we show an UNO board that has D13 as the LED_BUILTIN value.

The value of the resistor in series with the LED may be of a different value than 220 ohms; the LED will light up also with values up to 1K ohm.

circuit

Schematic
schematic

Code
After you build the circuit plug your Arduino board into your computer, start the Arduino Software (IDE) and enter the code below. You may also load it from the menu File/Examples/01.Basics/Blink . The first thing you do is to initialize LED_BUILTIN pin as an output pin with the line

pinMode(LED_BUILTIN, OUTPUT);

In the main loop, you turn the LED on with the line:

digitalWrite(LED_BUILTIN, HIGH);

This supplies 5 volts to the LED anode. That creates a voltage difference across the pins of the LED, and lights it up. Then you turn it off with the line:

digitalWrite(LED_BUILTIN, LOW);

That takes the LED_BUILTIN pin back to 0 volts, and turns the LED off. In between the on and the off, you want enough time for a person to see the change, so the delay() commands tell the board to do nothing for 1000 milliseconds, or one second. When you use the delay() command, nothing else happens for that amount of time. Once you've understood the basic examples, check out the BlinkWithoutDelay example to learn how to create a delay while doing other things.

Once you've understood this example, check out the DigitalReadSerial example to learn how read a switch connected to the board.


See Also

Learn more
You can find more basic tutorials in the built-in examples section.

You can also explore the language reference, a detailed collection of the Arduino programming language.

Last revision 2015/07/28 by SM

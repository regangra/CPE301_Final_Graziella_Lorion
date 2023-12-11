/*
 *  Name: Graziella Lorion
 *  Assignment: Final Project
 *  Class: CPE301 Fall 2023
 *  Date: 11/20/2023
 */
// Libraries
#include <LiquidCrystal.h>
#include <dht.h>
#include <Stepper.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <TimeLib.h>

#define RDA 0x80
#define TBE 0x20

// Temperature and Humidity Sensor Pin
#define DHT11_PIN 6
// Analog Pins
// Water Sensor
unsigned int analogPin0 = 0x00;
// Stepper Motor Potentiometer
unsigned int analogPin7 = 0x07;
// Serial addresses
volatile unsigned char *myUCSR0A = (unsigned char *)0xC0;
volatile unsigned char *myUCSR0B = (unsigned char *)0xC1;
volatile unsigned char *myUCSR0C = (unsigned char *)0xC2;
volatile unsigned int  *myUBRR0  = (unsigned int *) 0xC4;
volatile unsigned char *myUDR0   = (unsigned char *)0xC6;
// Delay Registers
volatile unsigned char *myTCCR1A  = (unsigned char *) 0x80; // Timer/Counter 1 Control registers
volatile unsigned char *myTCCR1B  = (unsigned char *) 0x81; // Timer/Counter 1 Control registers
volatile unsigned char *myTCCR1C  = (unsigned char *) 0x82; // Timer/Counter 1 Control registers
volatile unsigned char *myTIMSK1  = (unsigned char *) 0x6F; // Timer Counter 1 Interrupt Mask
volatile unsigned int  *myTCNT1   = (unsigned  int *) 0x84; // Timer Counter 1
volatile unsigned char *myTIFR1   = (unsigned char *) 0x36; // Timer Interrupt Flag Register
// ADC addresses 
volatile unsigned char* my_ADMUX  = (unsigned char*) 0x7C;
volatile unsigned char* my_ADCSRB = (unsigned char*) 0x7B;
volatile unsigned char* my_ADCSRA = (unsigned char*) 0x7A;
volatile unsigned int* my_ADC_DATA = (unsigned int*) 0x78;
// Define Port B Register Pointers
volatile unsigned char* port_b = (unsigned char *) 0x25;
volatile unsigned char* ddr_b  = (unsigned char *) 0x24;
volatile unsigned char* pin_b  = (unsigned char *) 0x23;
// Define Port G Register Pointers
volatile unsigned char* port_g = (unsigned char*) 0x34;
volatile unsigned char* ddr_g  = (unsigned char*) 0x33;
volatile unsigned char* pin_g  = (unsigned char*) 0x32;
// Define Port H Register Pointers
volatile unsigned char* port_h = (unsigned char*) 0x102;
volatile unsigned char* ddr_h  = (unsigned char*) 0x101;
volatile unsigned char* pin_h  = (unsigned char*) 0x100;
// Define Port K Register Pointers
volatile unsigned char* port_k = (unsigned char*) 0x108; 
volatile unsigned char* ddr_k  = (unsigned char*) 0x107; 
volatile unsigned char* pin_k  = (unsigned char*) 0x106;
// Defines Stepper Motor Steps
const int stepsPerRevolution = 32;
// LCD pins
const int RS = 13, EN = 12, D4 = 11, D5 = 10, D6 = 9, D7 = 8;
// Interval to Update Sensors in milisseconds variables
const long interval = 60000;
unsigned long previousMillis = 0;
unsigned long currentMillis;
// RTC Months List
const char *monthName[12] = 
{
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
// Running / Idle Mode Variables
unsigned int enable = 0;
unsigned int mode = 0;
unsigned int newMode = 0;
unsigned int water;

// Stepper Motor Variables
unsigned int potentiometerRead = 0;
int potentiometerValue = 0;
int newPotentiometerRead = 0;
// Temperature Sensor Variable
int temperature = 1;

// Classes
// Temperature and Humidity Sensor
dht DHT;
// Step Motor
Stepper myStepper = Stepper(stepsPerRevolution, 2, 4, 3, 5);
// LCD
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
// RTC
tmElements_t timeStamp;

void setup() {
  // Initialize Serial
  U0init(9600);
  // Start LCD
  lcd.begin(16, 2);
  // Initialize ADC
  adc_init();
  // Set Stepper Speed
  myStepper.setSpeed(200);
  // Clear PK0 to INPUT
  *ddr_k &= 0xFE;
  // Enable the pullup resistor on PK0
  *port_k |= 0x01;
  // Set PH4 to OUTPUT
  *ddr_h |= 0x10;
  // Set LEDs PB0, PB1, PB2, and PB3 as OUTPUT
  *ddr_b |= 0x0F;
  // Read sensors once
  water = readWater();
  temperature = DHT.read11(DHT11_PIN);

  // RTC Date and Time Configuration
  int hour, minute, second, Day, Year;
  uint8_t monthIndex;
  char Month[12];
  sscanf(__DATE__, "%s %d %d", Month, &Day, &Year);  
  sscanf(__TIME__, "%d:%d:%d", &hour, &minute, &second);
  for (monthIndex = 0; monthIndex < 12; monthIndex++) 
  {
    if (strcmp(Month, monthName[monthIndex]) == 0)
    {
      break;
    }
  }
  timeStamp.Day = Day;
  timeStamp.Month = monthIndex + 1;
  timeStamp.Year = CalendarYrToTm(Year);
  timeStamp.Hour = hour;
  timeStamp.Minute = minute;
  timeStamp.Second = second;
  RTC.write(timeStamp);
}

void loop() {
  // Button Toggle between ON and OFF. Start as OFF.
  if(*pin_k & 0x01)
  {
    // generate delay to get just one value
    while(*pin_k & 0x01)
    {
      delay(20);
    }
    enable++;
    // Print Event
    if(enable % 2 == 0){
      U0putchar('D');
      printTime();
    }
  }

  // ENABLE
  if(enable % 2 == 1){
    // Display Temperature LCD
    displayTemperature();
    currentMillis = millis();
    // Delay 60 seconds between readings
    if (currentMillis - previousMillis >= interval) {
      // save the last reading
      previousMillis = currentMillis;
      // Water Sensor
      water = readWater();
      // Temperature and Humidity Sensor
      temperature = DHT.read11(DHT11_PIN);
    }
    // Stepper Motor direction control
    potentiometerRead = adc_read(analogPin7);
    // Map potentiometer value between -1 and 1
    newPotentiometerRead = map(potentiometerRead, 0, 1024, -1, 1);
    // Zero means no movement
    if (newPotentiometerRead - potentiometerValue != 0)
    {
      // One means rotate clockwise
      if (newPotentiometerRead > potentiometerValue)
      {
        U0putchar('+');
        printTime();
        myStepper.step(200);
      }
      // Minus one means rotate counterclockwise
      else if (newPotentiometerRead < potentiometerValue)
      {
        U0putchar('-');
        printTime();
        myStepper.step(-200);
      }
    }
    // Store the last scaled potenciometer value (between -1 and 1)
    potentiometerValue = newPotentiometerRead;

    // Check Water Level. Water threshold 63
    if(water >= 63)
    {
      // Temperature Threshold 20 Degrees Celsius
      if (int(DHT.temperature) >= 20)
      {
        // RUNNING MODE
        running();
        newMode = 1;
      }
      else
      {
        // IDLE MODE
        idle();
        newMode = 2;
      }
      // Print Event
      if(newMode - mode != 0)
      {
        if (int(DHT.temperature) >= 20)
        {
          U0putchar('R');
        }
        else
        {
          U0putchar('I');
        }
        printTime();
      }
      mode = newMode;
    } 
    else
    {
      // Print Event
      U0putchar('X');
      printTime();
      // While Water Level not Back Up
      while (water < 63)
      {
        // ERROR
        error();
        // If User Press RESET, Check the Water Level
        if(*pin_k & 0x01)
        {
          while(*pin_k & 0x01)
          {
            delay(10);
          }
          water = readWater();
        }
      }
      // When Water Level Back Up, Check Temperature
      if (int(DHT.temperature) >= 20)
      {
        // Back to RUNNING
        U0putchar('R');
      }
      else
      {
        // Back to IDLE
        U0putchar('I');
      }
      printTime();
    }
  }
  // DISABLE
  else
  {
    disable();
    mode = 0;
  }
}
// Function to Begin Serial
void U0init(int U0baud){
  unsigned long FCPU = 16000000;
  unsigned int tbaud;
  tbaud = (FCPU / 16 / U0baud - 1);
  *myUCSR0A = 0x20;
  *myUCSR0B = 0x08;
  *myUCSR0C = 0x06;
  *myUBRR0  = tbaud;
}
// Function to Print on Serial Monitor
void U0putchar(unsigned char U0pdata)
{
  while((*myUCSR0A & TBE)==0);
    *myUDR0 = U0pdata;
}

// Function to Initialize Analog to Digital Converter
void adc_init()
{
  // setup the A register
  *my_ADCSRA |= 0x80;// set   bit 7 to enable the ADC
  *my_ADCSRA &= 0xBF;// clear bit 6 It will become 1 in read function
  *my_ADCSRA &= 0xDF;// clear bit 5 to disable trigger.
  //*my_ADCSRA &= 0xF7;// clear bit 3 to disable interrupt
  *my_ADCSRA &= 0xF8;// clear bit 0-2 to 0 to set prescaler selection to slow reading
  // setup the B register
  *my_ADCSRB &= 0xBF;// clear bit 6 to reset the channel and gain bits
  *my_ADCSRB &= 0xF7;// clear bit 3 to set Mux to 0 (single mode) ADC0 
  *my_ADCSRB &= 0xF8;// clear bit 2-0 to 0 to free running mode
  // setup the MUX Register
  *my_ADMUX  |= 0x80;// REFS1 set bit 7 for Internal 1.1V reference
  *my_ADMUX  |= 0x40;// REFS0 set bit 7 for Internal 1.1V reference
  *my_ADMUX  &= 0xDF;// clear bit 5 for right adjust result
  *my_ADMUX  &= 0xF0;// clear bit 4-0 to reset the channel and gain bits
}
// Function to Read Analog Data
int adc_read(unsigned int adc_channel_num)
{
  // clear the channel selection bits (MUX 4:0)
  *my_ADMUX  &= 0xE0;
  // clear the channel selection bits (MUX 5)
  *my_ADCSRB &= 0xF7;
  // set the channel number
  if(adc_channel_num > 7)
  {
    // set the channel selection bits, but remove the most significant bit (bit 3)
    adc_channel_num -= 8;
    // set MUX bit 5
    *my_ADCSRB |= 0x08;
  }
  // set the channel selection bits
  *my_ADMUX  += adc_channel_num;
  // set bit 6 of ADCSRA to 1 to start a conversion
  *my_ADCSRA |= 0x40;
  // wait for the conversion to complete
  while((*my_ADCSRA & 0x40) != 0);
  // return the result in the ADC data register
  return *my_ADC_DATA;
}
// Interrupt Routine to Monitor Push Button
ISR(TIMER1_OVF_vect){
  TCNT1 = (-1600);
  // calculate period
  double period = 1.0/double(1000);
  // 100% duty cycle
  double half_period = period;
  // clock period for Arduino 16MHz clock
  double clk_period = 0.0000000625;
  // calculate ticks
  unsigned int ticks = half_period / clk_period;
  // Stop the Timer
  *myTCCR1B &= 0xF8;
  // Load the Count
  *myTCNT1 = (unsigned int) (65535 - ticks);
  // Start the Timer
  *myTCCR1B |= 0x01;
  // if it's not the STOP amount
  if(ticks != 65535)
  {
    // XOR to toggle PK0
    *port_k ^= 0x01;
  }
}
// Function to Print Time of Events
void printTime(){
    unsigned char Event[] = {' ','e','v','e','n','t',' ','=',' '};
    int i;
    unsigned char mod;
    unsigned char div;
    int length = 9;
    for(i=0;i<length;i++)
      {
        U0putchar(Event[i]);
      }
    RTC.read(timeStamp);
    unsigned char timeStampMonth = timeStamp.Month;
    div = (timeStampMonth / 10) + 48;
    mod = (timeStampMonth % 10) + 48;
    U0putchar(div);
    U0putchar(mod);
    U0putchar('/'); 
    unsigned char timeStampDay = timeStamp.Day;
    div = (timeStampDay / 10) + 48;
    mod = (timeStampDay % 10) + 48;
    U0putchar(div);
    U0putchar(mod);
    U0putchar('/');
    unsigned int timeStampYear = tmYearToCalendar(timeStamp.Year);
    timeStampYear -= 2000;
    div = (timeStampYear / 10) + 48;
    mod = (timeStampYear % 10) + 48;
    U0putchar(div);
    U0putchar(mod);
    U0putchar(' ');
    U0putchar('-');
    U0putchar(' ');
    unsigned int timeStampHour = timeStamp.Hour;
    div = (timeStampHour / 10) + 48;
    mod = (timeStampHour % 10) + 48;
    U0putchar(div);
    U0putchar(mod);
    U0putchar('h');
    unsigned int timeStampMinute = timeStamp.Minute;
    div = (timeStampMinute / 10) + 48;
    mod = (timeStampMinute % 10) + 48;
    U0putchar(div);
    U0putchar(mod);
    U0putchar('m');
    unsigned int timeStampSecond = timeStamp.Second;
    div = (timeStampSecond / 10) + 48;
    mod = (timeStampSecond % 10) + 48;
    U0putchar(div);
    U0putchar(mod);
    U0putchar('s');
    U0putchar('\n'); //new line
}
// Fucntion to Display Temperature on LCD Screen
void displayTemperature(){
    // Display Temperature
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("Temp: ");
    lcd.print(int(DHT.temperature));
    lcd.print((char)223);
    lcd.print("C");
    // Display Humidity
    lcd.setCursor(0,1);
    lcd.print("Humi: ");
    lcd.print(int(DHT.humidity));
    lcd.print("%");
}
// Function to Read Water Level
unsigned int readWater(){
  unsigned int water;
  // PH4 is HIGH
  *port_h |= 0x10;
  // Read Analog Pin A0
  water = adc_read(analogPin0);
  // PH4 is LOW
  *port_h &= 0xEF;
  return water;
}
// Function to Run the DC Motor and Turn the Blue LED ON
void running(){
  // blue led ON
  *port_b |= 0x02;
  // green, red, yellow led OFF
  *port_b &= 0xF2;
  // motor ON
  *port_g &= 0xFE;
  *port_g |= 0x02;
}
// Function to Stop the DC Motor and Turn the Green LED ON
void idle(){
  // yellow, blue, red led OFF
  *port_b &= 0xF1;
  // green led ON
  *port_b |= 0x01;
  // motor OFF
  *port_g &= 0xFC;
}
// Function to Low Water Level and Red LED ON
void error(){
  // blue, gree, yellow led OFF
  *port_b &= 0xF8;
  // red led ON
  *port_b |= 0x08;
  // Motor OFF
  *port_g &= 0xFC;
  // display
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Error");
  lcd.setCursor(0,1);
  lcd.print("Low Water");
}
// Function to Disable the System and Yellow LED ON
void disable(){
  lcd.clear();
  *port_k &= 0xFE;
  // blue, green, red led OFF
  *port_b &= 0xF4;
  // yellow led ON
  *port_b |= 0x04;
  // motor OFF
  *port_g &= 0xFC;
}
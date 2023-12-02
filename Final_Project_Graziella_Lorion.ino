/*
 *  Name: Graziella Lorion
 *  Assignment: Final Project
 *  Class: CPE301 Fall 2023
 *  Date: 11/20/2023
 */
 // Libraries
#include <LiquidCrystal.h>
#include <dht.h>
#include <RTClib.h>
#include <Stepper.h>

// Pins
#define RDA 0x80
#define TBE 0x20  
#define POWER_PIN 7
#define DHT11_PIN 6
#define SIGNAL_PIN A5

// Defines the number of steps per rotation
const int stepsPerRevolution = 2038;

//int value = 0; // store the sensor value
// LCD pins <--> Arduino pins
const int RS = 13, EN = 12, D4 = 11, D5 = 10, D6 = 9, D7 = 8;
// Serial addresses
volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;
volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;
volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;
volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4;
volatile unsigned char *myUDR0   = (unsigned char *)0x00C6;

// ADC addresses 
volatile unsigned char* my_ADMUX  = (unsigned char*) 0x7C;
volatile unsigned char* my_ADCSRB = (unsigned char*) 0x7B;
volatile unsigned char* my_ADCSRA = (unsigned char*) 0x7A;
volatile unsigned int* my_ADC_DATA = (unsigned int*) 0x78;

unsigned char analogPin = 0x00; 

// Define Port B Register Pointers
volatile unsigned char* port_b = (unsigned char *) 0x25;
volatile unsigned char* ddr_b  = (unsigned char *) 0x24;
volatile unsigned char* pin_b  = (unsigned char *) 0x23;
// Define Port D Register Pointers
volatile unsigned char* port_d = (unsigned char*) 0x2B;
volatile unsigned char* ddr_d  = (unsigned char*) 0x2A;
volatile unsigned char* pin_d  = (unsigned char*) 0x29;
// Define Port E Register Pointers
volatile unsigned char* port_e = (unsigned char*) 0x2E;
volatile unsigned char* ddr_e  = (unsigned char*) 0x2D;
volatile unsigned char* pin_e  = (unsigned char*) 0x2C;
// Define Port G
volatile unsigned char* port_g = (unsigned char*) 0x34;
volatile unsigned char* ddr_g  = (unsigned char*) 0x33;
volatile unsigned char* pin_g  = (unsigned char*) 0x32;
// Define Port H
volatile unsigned char* port_h = (unsigned char*) 0x102;
volatile unsigned char* ddr_h  = (unsigned char*) 0x101;
volatile unsigned char* pin_h  = (unsigned char*) 0x100;
// Define Port K Register Pointers
volatile unsigned char* port_k = (unsigned char*) 0x108; 
volatile unsigned char* ddr_k  = (unsigned char*) 0x107; 
volatile unsigned char* pin_k  = (unsigned char*) 0x106;


// Classes
dht DHT;
RTC_DS1307 rtc;
// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 2, 4, 3, 5);
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);


/*char daysOfTheWeek[7][12] = {
  "Sunday",
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday"
};*/

void setup() {
  U0init(9600);
  adc_init();
  //set PH4 to OUTPUT
  *ddr_h |= 0x10;

  // enable the pullup resistor on PK2
  //*port_k |= 0x04;

  // PH4 is LOW
  *port_h &= 0xEF;

  lcd.begin(16, 2); // set up number of columns and rows
  /*if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1);
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));*/
}

void loop() {
  int value;  // variable to store the value read
  value = adc_read(analogPin);
/*
  DateTime now = rtc.now();
  Serial.print("Date & Time: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
*/
  
  delay(1000); // delay 1 seconds
  // PH4 is HIGH
  *port_h |= 0x10;
  //digitalWrite (POWER_PIN, HIGH);
  delay(1000);
  //value = analogRead (SIGNAL_PIN);
  // PH4 is LOW
  *port_h &= 0xEF;
  //digitalWrite(POWER_PIN, LOW);

  // Rotate CW slowly at 5 RPM
	myStepper.setSpeed(5);
	myStepper.step(stepsPerRevolution);
	delay(1000);
	
	// Rotate CCW quickly at 10 RPM
	myStepper.setSpeed(10);
	myStepper.step(-stepsPerRevolution);
	delay(1000);

  int chk = DHT.read11(DHT11_PIN);
  unsigned char Temp[] = {'T', 'e', 'm', 'p', 'e', 'r', 'a', 't', 'u', 'r', 'e', ':',' '};
  
  int i;
  int length = 13;
    for(i=0;i<length;i++)
      {
        U0putchar(Temp[i]);
      }
  
  Serial.println(DHT.temperature);
  Serial.print("Humidity: ");
  Serial.println(DHT.humidity);
  Serial.print("Water level: ");
  Serial.println(value);
  delay(1000);

  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Temp: ");
  lcd.print(DHT.temperature);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Humi: ");
  lcd.print(DHT.humidity);
  lcd.print("%");
  delay(1000);
}

void U0init(int U0baud)
{
 unsigned long FCPU = 16000000;
 unsigned int tbaud;
 tbaud = (FCPU / 16 / U0baud - 1);
 // Same as (FCPU / (16 * U0baud)) - 1;
 *myUCSR0A = 0x20;
 *myUCSR0B = 0x18;
 *myUCSR0C = 0x06;
 *myUBRR0  = tbaud;
}
unsigned char U0kbhit()
{
  return *myUCSR0A & RDA;
}
unsigned char U0getchar()
{
  return *myUDR0;
}
void U0putchar(unsigned char U0pdata)
{
  while((*myUCSR0A & TBE)==0);
  *myUDR0 = U0pdata;
}

void adc_init()
{
  // setup the A register
  /*
  ADEN  1 means ADC enable
  ADSC  ADC start conversion. Single conversion 1 to start each conversion. Free mode 1 to start first conversion
  ADATE ADC auto trigger enable. Clear to disable auto trigger
  ADIF Interrupt flag - cleared by hardware
  ADIE Interrupt enable
  ADPS2 - 0 for prescaler /2
  ADPS1 - 0 for prescaler /2
  ADPS0 - 0 for prescaler /2
  */
  *my_ADCSRA |= 0b10000000; // set   bit 7 to enable the ADC
  *my_ADCSRA &= 0b10111111; // clear bit 6 It will become 1 in read function
  *my_ADCSRA &= 0b11011111; // clear bit 5 to disable trigger.
  *my_ADCSRA &= 0b11110111; // clear bit 3 to disable interrupt
  *my_ADCSRA &= 0b11111000; // clear bit 0-2 to 0 to set prescaler selection to slow reading
  // setup the B register
  /*
  -
  ACME
  -
  -
  MUX5  used with MUX4:0 in ADMUX. For single ended clear
  ADTS2 0 - free running mode
  ADTS1 0 - free running mode
  ADTS0 0 - free running mode
  */
  *my_ADCSRB &= 0b10111111; // clear bit 6 to reset the channel and gain bits
  *my_ADCSRB &= 0b11110111; // clear bit 3 to set Mux to 0 (single mode) ADC0 
  *my_ADCSRB &= 0b11111000; // clear bit 2-0 to 0 to free running mode
  // setup the MUX Register
  /*
  REFS1 - 1 for internal reference
  REFS2 - 1 for internal reference
  ADLAR - 0 for right adjusted
  -
  MUX3 0 - ADC0
  MUX2 0 - ADC0
  MUX1 0 - ADC0
  MUX0 0 - ADC0
  */
  *my_ADMUX  |= 0b10000000; // REFS1 set bit 7 for Internal 1.1V reference
  *my_ADMUX  |= 0b01000000; // REFS0 set bit 7 for Internal 1.1V reference
  *my_ADMUX  &= 0b11011111; // clear bit 5 for right adjust result
  *my_ADMUX  &= 0b11110000; // clear bit 4-0 to reset the channel and gain bits
}
int adc_read(unsigned char adc_channel_num)
{
  // clear the channel selection bits (MUX 4:0)
  *my_ADMUX  &= 0b11100000;
  // clear the channel selection bits (MUX 5)
  *my_ADCSRB &= 0b11110111;
  // set the channel number
  if(adc_channel_num > 7)
  {
    // set the channel selection bits, but remove the most significant bit (bit 3)
    adc_channel_num -= 8;
    // set MUX bit 5
    *my_ADCSRB |= 0b00001000;
  }
  // set the channel selection bits
  *my_ADMUX  += adc_channel_num;
  // set bit 6 of ADCSRA to 1 to start a conversion
  *my_ADCSRA |= 0b01000000;
  // wait for the conversion to complete
  while((*my_ADCSRA & 0b01000000) != 0);
  // return the result in the ADC data register
  return *my_ADC_DATA;
}

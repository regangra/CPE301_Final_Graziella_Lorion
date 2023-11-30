/*
 *  Name: Graziella Lorion
 *  Assignment: Final Project
 *  Class: CPE301 Fall 2023
 *  Date: 11/20/2023
 */
#include <LiquidCrystal.h>

#define POWER_PIN 7
#define SIGNAL_PIN A5
int value = 0; // store the sensor value
// LCD pins <--> Arduino pins
const int RS = 13, EN = 12, D4 = 11, D5 = 10, D6 = 9, D7 = 8;

int right=0,up=0;
int dir1=0,dir2=0;
// Create pacman character
byte pacman[8] = {
  0b01110,
  0b11101,
  0b11111,
  0b11100,
  0b11000,
  0b11100,
  0b11111,
  0b01110
};
// Create ghost character
byte ghost[8] = {
  0b01110,
  0b11111,
  0b10101,
  0b11111,
  0b11111,
  0b11111,
  0b10101,
  0b10101
};
// Create food character
byte food[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b01110,
  0b00000,
  0b00000,
  0b00000
};

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup() {
  Serial.begin(9600);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW);
  lcd.begin(16, 2); // set up number of columns and rows
  lcd.createChar(1, pacman); // create a new custom character
  lcd.createChar(2, food); // create a new custom character
  lcd.createChar(3, ghost); // create a new custom character
}

void loop() {
  digitalWrite (POWER_PIN, HIGH);
  delay(10);
  value = analogRead (SIGNAL_PIN);
  digitalWrite(POWER_PIN, LOW);

  Serial.print("Sensor value: ");
  Serial.println(value);
  delay(1000);

  lcd.clear();
  lcd.setCursor(right, up); // move cursor to (0, 0)
  lcd.print("Humidity = ");
  lcd.setCursor(11,0);
  lcd.print(value);  // print pacman at (0, 0)

  lcd.setCursor(5, 1); // move cursor to (5, 1)
  lcd.write((byte)2);  // print food char at (5, 1)

  lcd.setCursor(15, 1); // move cursor to (15, 1)
  lcd.write((byte)2);  // print food at (15, 1)

  lcd.setCursor(9, 1); // move cursor to (9, 1)
  lcd.write((byte)3);  // print ghost at (9, 1)
  //lcd.clear();
}
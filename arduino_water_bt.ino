/*

*/



//Include the module so we don't
//have to use the default Serial
//so the Arduino can be plugged in
//to a computer and still use bluetooth
#include <SoftwareSerial.h>

//Define the pins used for receiving
//and transmitting information via Bluetooth
const int rxpin = 8;
const int txpin = 9;

//Variable to store input value
//initialized with arbitrary value
char k = 'h';
int dur = 8;
//Connect the Bluetooth module
SoftwareSerial bluetooth(rxpin, txpin);

// define the pin that control the relay
#define RELAY1  7

//initializing the OLED display
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
B00000001, B11000000,
B00000001, B11000000,
B00000011, B11100000,
B11110011, B11100000,
B11111110, B11111000,
B01111110, B11111111,
B00110011, B10011111,
B00011111, B11111100,
B00001101, B01110000,
B00011011, B10100000,
B00111111, B11100000,
B00111111, B11110000,
B01111100, B11110000,
B01110000, B01110000,
B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//Initializig DS3231 clock
#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
return( (val/16*10) + (val%16) );
}


void setup() {
//INITIALIZE OLED SCREEN
Serial.begin(9600);
// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
display.display();
delay(2000);
display.clearDisplay();

//INITIALIZE RELAY
pinMode(RELAY1, OUTPUT);
digitalWrite(RELAY1,1);

//INITIALIZE BLUETOOTH
bluetooth.begin(9600);
bluetooth.println("Bluetooth ready");

//INITIALIZE CLOCK
// set the initial time here:
// DS3231 seconds, minutes, hours, day, date, month, year
//setDS3231time(00,42,17,2,28,05,18);

}

void loop() {

Serial.println("\n");
 // display the real-time clock data on the Serial Monitor,

  //Check for new data
if(bluetooth.available()){
  //Remember new data
  k = bluetooth.read();
  //Print the data for debugging purposes
  //lcd.clear();
  //lcd.print(k);
}

//water the plants if received data is w
if( k == 'w' ){
    // text display tests
  displayTime("Water");
  water_plants(dur);
  k= 'h';
  // text display tests
    displayTime("Waiting");
}
//wait if received data is h
else if( k == 'h') {
  // text display tests
    display.clearDisplay();
   displayTime("Waiting");
}
//change watering duration
else if (isDigit(k)) {
    dur = k - '0';
    // text display tests
displayTime("Dur.set");
  }
else {
  //lcd.clear();
  ///lcd.print(k);

}
//Wait 100 milliseconds to decrease unnecessary hardware strain
 delay(100);

}

// define watering function that waters for "dur" seconds
// how long to water is set by variable "dur", default is 8 seconds
void water_plants(int dur){
 digitalWrite(RELAY1,0);          // Turns ON Relays 1
 delay(dur*1000);                     // Wait 2 seconds

 digitalWrite(RELAY1,1);          // Turns Relay Off
 delay(2000);
}

//Functions for the clock
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
// sets time and date data to DS3231
Wire.beginTransmission(DS3231_I2C_ADDRESS);
Wire.write(0); // set next input to start at the seconds register
Wire.write(decToBcd(second)); // set seconds
Wire.write(decToBcd(minute)); // set minutes
Wire.write(decToBcd(hour)); // set hours
Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
Wire.write(decToBcd(month)); // set month
Wire.write(decToBcd(year)); // set year (0 to 99)
Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
Wire.beginTransmission(DS3231_I2C_ADDRESS);
Wire.write(0); // set DS3231 register pointer to 00h
Wire.endTransmission();
Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
// request seven bytes of data from DS3231 starting from register 00h
*second = bcdToDec(Wire.read() & 0x7f);
*minute = bcdToDec(Wire.read());
*hour = bcdToDec(Wire.read() & 0x3f);
*dayOfWeek = bcdToDec(Wire.read());
*dayOfMonth = bcdToDec(Wire.read());
*month = bcdToDec(Wire.read());
*year = bcdToDec(Wire.read());
}
void displayTime(String mystr)
{
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
// retrieve data from DS3231
readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
&year);

display.clearDisplay();
display.setTextSize(3);
display.setTextColor(WHITE);
display.setCursor(0,40);
display.print(mystr);
display.setTextSize(2);
display.setCursor(0,00);
display.print(hour, DEC);
// convert the byte variable to a decimal number when displayed
display.print(":");
if (minute<10)
{
  display.print("0");
}
display.print(minute, DEC);
display.print(" ");
switch(dayOfWeek){
case 1:
  display.print("Sun");
  break;
case 2:
  display.print("Mon");
  break;
case 3:
  display.print("Tue");
  break;
case 4:
  display.print("Wed");
  break;
case 5:
  display.print("Thu");
  break;
case 6:
  display.print("Fr");
  break;
case 7:
  display.print("Sat");
  break;
}
display.setCursor(0,20);
display.print(dayOfMonth, DEC);
display.print("/");
display.print(month, DEC);
display.print("/");
display.print(year, DEC);
display.display();



}

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

// initializing the OLED display
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


}

void loop() {


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
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Watering");
    display.display();
  water_plants(dur);
  k= 'h';
  // text display tests
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Waiting ");
    display.display();
}
//wait if received data is h
else if( k == 'h') {
  // text display tests
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Waiting");
    display.display();
}
//change watering duration
else if (isDigit(k)) {
    dur = k - '0';
    // text display tests
    display.clearDisplay();
    display.setTextSize(2);
    display. setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Watering duration:"+ String(dur));
    display.display();
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

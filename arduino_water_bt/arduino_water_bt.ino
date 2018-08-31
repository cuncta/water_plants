  /*
  
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

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


void setup() {
  //INITIALIZE LCD SCREEN
  // set up the number of columns and rows on the LCD
  lcd.begin(16, 2);
  // set the cursor to column 0, line 1
  // line 1 is the second row, since counting begins with 0
  lcd.setCursor(0, 1);
  // print to the second line
  //lcd.print("Dio Porco!");

  //INITIALIZE RELAY 
  pinMode(RELAY1, OUTPUT); 
  digitalWrite(RELAY1,1);

  //INITIALIZE BLUETOOTH
  bluetooth.begin(9600);
  bluetooth.println("Bluetooth ready");

 Serial.begin(9600);
  
}

void loop() {
    
    //Check for new data
  if(bluetooth.available()){
    //Remember new data
    k = bluetooth.read();
    //Print the data for debugging purposes
    lcd.clear();
    //lcd.print(k);
  }
  
  //water the plants if received data is w
  if( k == 'w' ){
    lcd.clear();
    lcd.print("watering");    
    water_plants(dur);
    k= 'h';
    lcd.clear();
    lcd.print("waiting");
  }
  //wait if received data is h
  else if( k == 'h') {
    lcd.clear();
    lcd.print("waiting");
  }
  //change watering duration
  else if (isDigit(k)) {
      dur = k - '0';
      lcd.clear();
      lcd.print(dur);
    }
  // send sensor data  
  else if( k == 's') { 
    lcd.clear();
    lcd.print("soil sensor");
    int Si0 = analogRead(A0);
    String S0 = String(Si0);
    bluetooth.print("#{'S0': "+S0+"}");
    Serial.print("#{'S0': "+S0+"}");
  }
  else {
    lcd.clear();
    lcd.print(k);

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


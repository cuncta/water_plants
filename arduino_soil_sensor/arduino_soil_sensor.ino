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

//Connect the Bluetooth module
SoftwareSerial bluetooth(rxpin, txpin);



void setup() {

 //INITIALIZE BLUETOOTH
 bluetooth.begin(9600);
 bluetooth.println("Bluetooth ready");
 Serial.begin(9600);


}

void loop() {


 //Wait 100 milliseconds to decrease unnecessary hardware strain
 // read the input on analog pin 0:
 int Si0 = analogRead(A0);
 int Si1 = analogRead(A1);
 int Si2 = analogRead(A2);
 int Si3 = analogRead(A3);
 int Si4 = analogRead(A4);
 // print out the value you read:
 String S0 = String(Si0);
 String S1 = String(Si1);
 String S2 = String(Si2);
 String S3 = String(Si3);
 String S4 = String(Si4);
 //bluetooth.print("#"+ thisString);
 bluetooth.print("#{'S0': "+S0+", 'S1': "+S1+", 'S2': "+S2+", 'S3': "+S3+", 'S4': "+S4+"}");
  delay(10);

}

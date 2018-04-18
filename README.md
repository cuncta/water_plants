# Raspberry Pi and Arduino Automated Plant Watering with Website

Set up your Pi and Arduino to automatically take care of house plants!
In this project Pizero W is used to make a website where you can set up set the watering parameters. I set it to water the plants twice a day at desired time for a certain number of seconds [1-9].


## Hardware components

* Raspberry Pi Zero W

* Arduino Uno
* Bluetooth module H5-05
* 16x2 LCD display (compatible with the Hitachi HD44780 driver)
* Relay module
* Submersible Pump

### Wiring 

* LCD screen:
  * LCD RS pin to digital pin 12
  * LCD Enable pin to digital pin 11
  * LCD D4 pin to digital pin 5
  * LCD D5 pin to digital pin 4
  * LCD D6 pin to digital pin 3
  * LCD D7 pin to digital pin 2
  * Additionally, wire a 10k pot to +5V and GND, with it's wiper (output) to LCD screens VO pin (pin3). A 220 ohm resistor is used to power the backlight of the display, usually on pin 15 and 16 of the LCD connector

* Bluetooth Module:
  * bluetooth rx pin to digital pin 8
  * bluetooth tx pin to digital pin 9

* Relay:
  * relay pin to digital pin 7
  * the power supply of the submersible pump must be connected to this relay


## Prerequisites

* Raspberrypi zero w with [Jessie](https://www.raspberrypi.org/blog/raspbian-jessie-is-here/).


* Python 2.7
  * datetime
  * time
  * bluetooth
  * apscheduler
  * json
  * flask
  * psutil
  * os
  * json
  * time


## Software

There are three parts to this setup. One file is the arduino sketch, and the other runs a local web server, and some files are used to store data. 
* Arduino 
    * *arduino_water_bt.ino* this is the arduino sketch. Arduino will check wether there is an incoming data from bluetooth. If it receives something there are four possibilities:
        * "w" it activates the pump for a certain amount of seconds, default is 8.
        * "h" it waits for the next command
        * "0-9" if it receives a number between 0 and 9 it changes the watering time to that value
        * if any other string or value is received it does nothing
* Webserver
    * *auto_water.py* this file, when executed, it starts the auto watering process
    * *water.py* contains the functions called by the webserver.
    * *web_plants.py* this is the python script that runs the webserver.
    * *templates/main.html* the template for the webserver.
* Data Storage
    * *last_watered.txt* contains the list of dates and times when the plants were watered.
    * *next_water.txt* contains the current watering schedule.
    * *watering_parameters.txt* contains the watering parameters saved as a dictionary

### Flask Webserver
The template for the Webserver is defined in *main.html*, that must be in a subfolder called templates. Before starting the webserver change the bluetooth address with your corresponding address in *water.py*. The port should not be changed.
```
#bluetooth address of arduino
bd_addr = "00:21:13:02:C2:54"
port = 1
```
To start the webserver run the following command:
```
sudo python web_plants.py
```
Now if you navigate to the ip address of your Pi on port 12345 you should see a web dashboard. To change the port modifiy the port number on the last line of *web_plants.py*
```
if __name__ == "__main__":
    app.run(host='0.0.0.0', port=12345, debug=True)
```

### Run Webserver automatically
To auto start the webserver when Pi gets turned on a tool called cronjob can be used:
```
sudo crontab -e
```
In the text editor add the following line and make sure to leave an empty line below
```
@reboot cd <your path to web_plants>; sudo python web_plants.py


```


## License


## Acknowledgments

The project was inspired by this post in [hackster.io](https://www.hackster.io/ben-eagan/raspberry-pi-automated-plant-watering-with-website-8af2dc)

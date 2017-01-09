# THIS TUTORIAL IS STILL A WORK IN PROGRESS. SEE THE BLYNK COMMUNITY THREAD BELOW FOR ALL INFO AVAILABLE # 


# ESP8266-WS2811B-Blynk
An Ardiuno sketch to connect an ESP8266 to WS28xx LED strips and control them using the Blynk mobile app. 
This small tutorial will help you build the controller which you can hook up to any number of LEDs you like. (max 2048 I think?) 

Official Blynk Community forum thread: http://community.blynk.cc/t/esp8266-fastled-blynk-smart-shelf-video/7333

##  Installation

#### Hardware

* ESP Dev Board. This could be any size or shape as long as it is ***not*** the ESP-01 > ESP-07 varients. 
* WS2811/WS2812/WS2812B LEDs.
* High current power supply. (About 1A / meter).
* Small project box.

![Smart LED Controller hardware](http://i.imgur.com/jamXAIA.png)

#### Libraries required:

* Blynk - http://www.blynk.cc/
* SimpleTimer - Included in Blynk Library
* FastLED - http://fastled.io/
* ArduinoOTA (from Arduino IDE Managed Libraries) 

#### Software

* Download the project INO file and required libraies.
* Download and install the Blynk Mobile App for iOS or Android. 
* Create a new project in the Blynk app and email yourself the auto generated auth token.
 * Use the QR Code below to clone my Blynk project dashboard. It will require about $4 worth of in-app energy or use a local server where you can give yourself free energy.
* Paste the auth token in to the project file along with your Wifi SSID and PASSWORD. 
```cpp
char auth[] = "...";
char ssid[] = "...";
char pass[] = "...";
```
* Define the number of LEDs you are using. 
```cpp
#define NUM_LEDS 100
```
* If you plan on using more than 1 controller, you can set a nickname for each
```cpp
char nickname[] = "OFFICE1";
```
* And select it's zone. 
 * 0 will cause a fatal error
 * 1 reserved for ALL zones
 * 2 = Menu Widget Item 1 (Modify zone name in Blynk Dash)
 * 3 = Menu Widget Item 2
 * 4 = Menu Widget Item 3
 * 5 = etc etc.. you could make as many zones as the Blynk Menu widget allows.
```cpp
int HardwareZone = 2; // OFFICE 
```

* Upload sketch to your ESP hardware. 
___If you are using ArduinoOTA to update over the network then also set a device hostname and power cycle your hardware after first upload otherwise OTA won't work___
```cpp
ArduinoOTA.setHostname("LED-Office1");
```

![Blynk Clone Project QR Code](http://i.imgur.com/FW4eFXy.jpg)
 
## Gallery

#### Video

[![Smart LED Controller Video](https://img.youtube.com/vi/0lBiOeScbh4/0.jpg)](https://www.youtube.com/watch?v=0lBiOeScbh4)

#### LED Module 

Here is a little LED light module I buult to put behind items around the house. 
I also use this to control strips of LEDs by adding a 3 pin JST connector to the end of the internal LED wiring.

![Smart LED Controller running on Blynk](http://community.blynk.cc/uploads/default/optimized/2X/3/3f2e471f2898263a4ee39f464b856228008e9604_1_666x500.jpg)
![Smart LED Controller running on Blynk](http://community.blynk.cc/uploads/default/optimized/2X/8/86fa4f07ea23d2684375fabe75922a8ec0c000a6_1_375x500.jpg)
![Smart LED Controller running on Blynk](http://community.blynk.cc/uploads/default/original/2X/4/493706c4a0123c67f99489ad7148b58b17109be2.jpg)
![Smart LED Controller running on Blynk](http://community.blynk.cc/uploads/default/optimized/2X/5/5f0616484f97dc5e4e3219aff4e08d3272430e8b_1_666x500.jpg)
![Smart LED Controller running on Blynk](http://community.blynk.cc/uploads/default/optimized/2X/6/6300d2c97a66e285e8275637572b022e97e916c4_1_375x500.jpg)
![Smart LED Controller running on Blynk](http://community.blynk.cc/uploads/default/optimized/2X/d/d5a41fea209ca08cef209a7a83d4eb8d5dfdb772_1_666x500.jpg)

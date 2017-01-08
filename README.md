# THIS TUTORIAL IS STILL A WORK IN PROGRESS. SEE THE BLYNK COMMUNITY THREAD BELOW FOR ALL INFO AVAILABLE # 


# ESP8266-WS2811B-Blynk
An Ardiuno sketch to connect an ESP (all varients) to WS28xx LED strips and control them using the Blynk mobile app. 

- - - -
Official Blynk Community forum thread: http://community.blynk.cc/t/esp8266-fastled-blynk-smart-shelf-video/7333

Video:

[![Smart LED Controller Video](https://img.youtube.com/vi/0lBiOeScbh4/0.jpg)](https://www.youtube.com/watch?v=0lBiOeScbh4)

![Smart LED Controller running on Blynk](http://community.blynk.cc/uploads/default/optimized/2X/3/3f2e471f2898263a4ee39f464b856228008e9604_1_666x500.jpg)
![Smart LED Controller running on Blynk](http://community.blynk.cc/uploads/default/optimized/2X/8/86fa4f07ea23d2684375fabe75922a8ec0c000a6_1_375x500.jpg)
![Smart LED Controller running on Blynk](http://community.blynk.cc/uploads/default/original/2X/4/493706c4a0123c67f99489ad7148b58b17109be2.jpg)
![Smart LED Controller running on Blynk](http://community.blynk.cc/uploads/default/optimized/2X/5/5f0616484f97dc5e4e3219aff4e08d3272430e8b_1_666x500.jpg)
![Smart LED Controller running on Blynk](http://community.blynk.cc/uploads/default/optimized/2X/6/6300d2c97a66e285e8275637572b022e97e916c4_1_375x500.jpg)

## Hardware

* ESP Dev Board. This could be any size or shape as long as it is ***not*** the ESP-01 > ESP-07 varients. 
* WS2811/WS2812/WS2812B LEDs.
* High current power supply. (About 1A / meter).
* Small prject box.

## Installation

* Download the project INO file. 
* Download and install the Blynk Mpbile App for iOS or Android. 
* Create a new project in the Blynk app and email yourself the auto generated auth token. 
* Paste the auth token in to the project file along with your Wifi SSID and PASS. 
 ___If you are using ArduinoOTA to update over the network then also set a device hostname___
 
#### Libraries required:

* Blynk - http://www.blynk.cc/
* SimpleTimer - Included in Blynk Library
* FastLED - http://fastled.io/
* ArduinoOTA (from Arduino IDE Managed Libraries) 


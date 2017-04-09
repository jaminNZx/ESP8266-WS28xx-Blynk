/**************************************************************

                            Settings

 **************************************************************/
/*
   Blynk Auth Code
*/
#define AUTH                        "8e8bf31a58d843e4bc9daaaa4e7abdfd"
/*
   WS28xx Config
*/

#define LED_TYPE                    WS2811
#define LED_ARRANGE                 GRB
#define LED_NUMBER                  100
#define LED_LIMIT_MILLIAMPS         2000 // Limit current in mA (Must be using FastLED v3.1.1+)
/*
    Zone - Assign a zone to your hardware
    --------------------------------------
      In the Blynk App, fill out the Menu Widget at the top with the different
      zones you want to control.
      Typically this is each room or area so you can control the entire
      zone at once, and exclude other zones. Example below:
        Zone 0 = reserved (will return nothing as its a false)
        Zone 1 = reserved (to control "all" zones at once)
        Zone 2 = Office  (Menu Widget Item 1)
        Zone 3 = Lounge  (Menu Widget Item 2)
        Zone 4 = Outside (Menu Widget Item 3)
        etc.. you can go until your menu widget doesnt let you add more.
    --------------------------------------
*/
#define ZONE                        2
/*
    Nickname - Unique name for each node
    --------------------------------------
      Typically this would be the zone name, followed by the node number.
      In my home, I have 2 nodes in each room. So I call them OFFICE1, OFFICE2, LOUNGE1, LOUNGE2.
    --------------------------------------
*/
#define NICKNAME                    "OFFICE1"
/*
   --------------------------------------
   Over The Air Hostname

     I try to use the NICKNAME (above) as the name to identify it in the IDE.
     Althought you can use what ever you like.
   --------------------------------------
*/
#define OTA_HOSTNAME                "LED-OFFICE1"
/*
   Local Server Settings (uncomment to use local server)
*/
#define USE_LOCAL_SERVER
#define SERVER                      IPAddress(192, 168, 1, 2)
#define BLYNK_MSG_LIMIT             400
/*
   Hardware Pins
*/
#define DATA_PIN                    12 // GPIO12 / D6 on WeMos/NodeMCU ESP8266
/*
   Virtual Pins
*/
#define vPIN_HUE                    V0
#define vPIN_SATURATION             V1
#define vPIN_BRIGHTNESS             V2

#define vPIN_PRESET                 V3

#define vPIN_FPS                    V5

#define vPIN_COLOUR_BLUE            V4
#define vPIN_COLOUR_RED             V6
#define vPIN_COLOUR_GREEN           V7
#define vPIN_COLOUR_WHITE           V8

#define vPIN_TERMINAL               V9
#define vPIN_MANUAL                 V10
#define vPIN_SYNC_GHUE              V11

#define vPIN_ALERT                  V13
#define vPIN_OFF                    V14

#define vPIN_RAINBOWSPEED           V22
#define vPIN_ZONE_SELECT            V23
#define vPIN_TESTMODE               V24

#define vPIN_COLOUR_MEM1            V25
#define vPIN_COLOUR_MEM2            V26
#define vPIN_COLOUR_MEM3            V27
#define vPIN_COLOUR_MEMSAVE         V28

#define vPIN_NIGHTMODE              V29
/*

*/



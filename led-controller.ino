/*
  Virtual Ports:
  V0 = Hue (Slider 0-255)
  V1 = Saturation (Slider 0-255)
  V2 = Brightness (Slider 0-255)
  V3 = Cycle Pre-set (Momentary Button)
  V4 = Colour: Blue (Momentary Button)
  V5 = Updates Per Sec (Slider 10-500)
  V6 = Colour: Red (Momentary Button)
  V7 = Colour: Green (Momentary Button)
  V8 = Colour: White (Momentary Button)
  V9 = Terminal Widget
  V10 = Manual Mode (Switch Button)
  V11 = Sync gHUE (Momentary Button)
  V12 = Colour: Yellow (Momentary Button)
  V13 = Alert Mode (Momentary Button)
  V14 = OFF (Momentary Button)
  V15 = Colour: Light Blue (Momentary Button)
  V16 = Zone1 Up Time (Value Widget)
  V17 = Zone1 Wifi Signal (Value Widget)
  V18 = Zone2 Up Time (Value Widget)
  V19 = Zone2 Wifi Signal (Value Widget)
  V20 = Zone3 Up Time (Value Widget)
  V21 = Zone3 Wifi Signal (Value Widget)
  V25 = Memory1 (Momentary Button)
  V26 = Memory2 (Momentary Button)
  V29 = Night Mode
*/
/****************************************************************************/
//#define BLYNK_PRINT Serial
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <FastLED.h>
#include <SimpleTimer.h>
#include <elapsedMillis.h>
/****************************************************************************/
#define BLYNK_MSG_LIMIT    200
#define DATA_PIN           12
/************************ CHANGE SETTINGS HERE ONLY *************************/
#define NUM_LEDS 100
char nickname[] = "OFFICE1";
int HardwareZone = 2; // OUTDOOR // DONT USE 0 or 1 ... CAUSES ERRORS WITH LOGIC
char auth[] = "xxxxxxxx";
char ssid[] = "xxxxxxxx";
char pass[] = "xxxxxxxx";
/****************************************************************************/
int varManualMode, varHue, varSaturation, varBrightness, varHuePrev, varHueNew;
int varNextColour, varNextColourPrev, varBlendingMode, varUpdatesPerSec;
int varAlertMode, varZone, varRainbowSpeed;
long HexRGB;
int varNight, curMode, curSaturation, curBrightness, curHue;
int varMemSave, varMem1, varMem1_Brightness, varMem1_Saturation, varMem1_Hue, varMem1_ManualMode;
int varMem2, varMem2_Brightness, varMem2_Saturation, varMem2_Hue, varMem2_ManualMode;
/****************************************************************************/
WidgetTerminal terminal(V9);
SimpleTimer timer;
CRGB leds[NUM_LEDS];
/****************************************************************************/
void sendUptime() {
  Blynk.virtualWrite(V16, millis() / 1000);
  Blynk.virtualWrite(V17, map(WiFi.RSSI(), -105, -40, 0, 100) );
}
/****************************************************************************/
void setup()
{
  WiFi.mode(WIFI_STA);
  // CHOOSE CLOUD OR LOCAL SERVER CONFIG
  Blynk.begin(auth, ssid, pass);
  // Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 1, 2));
  while (Blynk.connect() == false) {}
  /*********** OTA *************/
  ArduinoOTA.setHostname("LED-Controller-1");
  ArduinoOTA.begin();
  /******** BOOT VARS **********/
  varHue = 190;           // Start on a Blue Hue
  varSaturation = 255;    // Start Full Colour
  varBrightness = 255;    // Start Full Brightness
  varManualMode = 0;      // Start in preset mode
  varNextColour = 0;      //
  varNextColourPrev = 0;  // Set Button State
  varBlendingMode = 1;    // Start LINEARBLEND
  varUpdatesPerSec = 100; // Start on 100 fps
  varAlertMode = 0;       // Start Alert Mode OFF
  varZone = 1;
  varRainbowSpeed = 0;    // Start stationary
  /******** FASTLED ************/
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  /******** READY **************/
  terminal.print(F("# Blynk v" BLYNK_VERSION ": "));
  terminal.print(nickname);
  terminal.println(F(" Device started"));
  terminal.flush();
  timer.setInterval(1000L, sendUptime);
}
/****************************************************************************/
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, whitescan };
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
/****************************************************************************/
BLYNK_WRITE(V0) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varHue = param.asInt();
    //HexRGB = ((long)leds[0].r << 16) | ((long)leds[0].g << 8 ) | (long)leds[0].b;
    //Blynk.setProperty(V0, "color", "#" + String(HexRGB, HEX));
  }
}
BLYNK_WRITE(V1) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varSaturation = param.asInt();
  }
}
BLYNK_WRITE(V2) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varBrightness = param.asInt();
  }
}
BLYNK_WRITE(V3) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varNextColour = param.asInt();
    if (varNextColour == 1 && varNextColourPrev == 0) {
      nextPattern();
      delay(10);
    }
    varNextColourPrev = varNextColour;
  }
}
BLYNK_WRITE(V4) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varManualMode = 1;
    varBrightness = (int)255;
    varSaturation = (int)255;
    varHue        = (int)152;
    Blynk.virtualWrite(V10, varManualMode);
    Blynk.virtualWrite(V2, 255);
    Blynk.virtualWrite(V1, 255);
    Blynk.virtualWrite(V0, 152);
    delay(10);
  }
}
BLYNK_WRITE(V5) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varUpdatesPerSec = param.asInt();
  }
}
BLYNK_WRITE(V6) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varManualMode = 1;
    varBrightness = (int)255;
    varSaturation = (int)255;
    varHue        = (int)0;
    Blynk.virtualWrite(V10, varManualMode);
    Blynk.virtualWrite(V2, 255);
    Blynk.virtualWrite(V1, 255);
    Blynk.virtualWrite(V0, 0);
    delay(10);
  }
}
BLYNK_WRITE(V7) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varManualMode = 1;
    varBrightness = (int)255;
    varSaturation = (int)255;
    varHue        = (int)80;
    Blynk.virtualWrite(V10, varManualMode);
    Blynk.virtualWrite(V2, 255);
    Blynk.virtualWrite(V1, 255);
    Blynk.virtualWrite(V0, 80);
    delay(10);
  }
}
BLYNK_WRITE(V8) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varManualMode = 1;
    varBrightness = (int)255;
    varSaturation = (int)0;
    Blynk.virtualWrite(V10, varManualMode);
    Blynk.virtualWrite(V2, 255);
    Blynk.virtualWrite(V1, 0);
    delay(10);
  }
}
BLYNK_WRITE(V10) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varManualMode = param.asInt();
  }
}
BLYNK_WRITE(V11) {
  gHue = 0;
  terminal.print(nickname);
  terminal.println(" | Sync'd gHUE ");
  terminal.flush();
}

BLYNK_WRITE(V15) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varManualMode = 1;
    Blynk.virtualWrite(V10, varManualMode);
    varBrightness = (int)255;
    varSaturation = (int)255;
    varHue        = (int)27;
    Blynk.virtualWrite(V2, 255);
    Blynk.virtualWrite(V1, 255);
    Blynk.virtualWrite(V0, 27);
    delay(10);
  }
}

BLYNK_WRITE(V12) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varManualMode = 1;
    varBrightness = (int)255;
    varSaturation = (int)255;
    varHue        = (int)64;
    Blynk.virtualWrite(V10, varManualMode);
    Blynk.virtualWrite(V2, 255);
    Blynk.virtualWrite(V1, 255);
    Blynk.virtualWrite(V0, 64);
    delay(10);
  }
}

BLYNK_WRITE(V13) {
  varAlertMode = param.asInt();
  terminal.print(nickname);
  terminal.println(" | ALERT!");
  terminal.flush();
}

BLYNK_WRITE(V14) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varManualMode = 1;
    varBrightness = (int)0;
    Blynk.virtualWrite(V10, varManualMode);
    Blynk.virtualWrite(V2, 0);
    delay(10);
  }
}

BLYNK_WRITE(V22) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varRainbowSpeed = param.asInt();
    Blynk.virtualWrite(V22, varRainbowSpeed);
  }
}

BLYNK_WRITE(V23) {
  varZone = param.asInt();
  if (varZone == HardwareZone) {
    terminal.print(nickname);
    terminal.println(" | Zone Selected!");
    terminal.flush();
    Blynk.virtualWrite(V0,  varHue);
    Blynk.virtualWrite(V1,  varSaturation);
    Blynk.virtualWrite(V2,  varBrightness);
    Blynk.virtualWrite(V5,  varUpdatesPerSec);
    Blynk.virtualWrite(V10, varManualMode);
    Blynk.virtualWrite(V22, varRainbowSpeed);
  }
}
BLYNK_WRITE(V25) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varMem1 = param.asInt();
    if (varMemSave && varMem1) {
      varMem1_ManualMode = varManualMode;
      varMem1_Hue        = varHue;
      varMem1_Saturation = varSaturation;
      varMem1_Brightness = varBrightness;
    } else if (!varMemSave && varMem1) {
      varManualMode = varMem1_ManualMode;
      varBrightness = varMem1_Brightness;
      varSaturation = varMem1_Saturation;
      varHue        = varMem1_Hue;
      Blynk.virtualWrite(V10, varManualMode);
      Blynk.virtualWrite(V2, varBrightness);
      Blynk.virtualWrite(V1, varSaturation);
      Blynk.virtualWrite(V0, varHue);
    }
  }
  delay(10);
}
BLYNK_WRITE(V26) {
  if ( (varZone == HardwareZone) || (varZone == 1)) {
    varMem2 = param.asInt();
    if (varMemSave && varMem2) {
      varMem2_ManualMode = varManualMode;
      varMem2_Hue        = varHue;
      varMem2_Saturation = varSaturation;
      varMem2_Brightness = varBrightness;
    } else if (!varMemSave && varMem2) {
      varManualMode = varMem2_ManualMode;
      varBrightness = varMem2_Brightness;
      varSaturation = varMem2_Saturation;
      varHue        = varMem2_Hue;
      Blynk.virtualWrite(V10, varManualMode);
      Blynk.virtualWrite(V2, varBrightness);
      Blynk.virtualWrite(V1, varSaturation);
      Blynk.virtualWrite(V0, varHue);
    }
  }
  delay(10);
}
BLYNK_WRITE(V27) {
  varMemSave = param.asInt();
}
BLYNK_WRITE(V29) {
  varNight = param.asInt();
  if (varNight) {
    curMode = varManualMode;
    curSaturation = varSaturation;
    curBrightness = varBrightness;
    curHue = varHue;
    varManualMode = 1;
    varBrightness = (int)100;
    varSaturation = (int)255;
    varHue        = (int)152;
    Blynk.virtualWrite(V10, varManualMode);
    Blynk.virtualWrite(V2, 100);
    Blynk.virtualWrite(V1, 255);
    Blynk.virtualWrite(V0, 152);
    delay(10);
  } else if (!varNight) {
    varManualMode = curMode;
    varBrightness = curBrightness;
    varSaturation = curSaturation;
    varHue        = curHue;
    Blynk.virtualWrite(V10, curMode);
    Blynk.virtualWrite(V2, curBrightness);
    Blynk.virtualWrite(V1, curSaturation);
    Blynk.virtualWrite(V0, curHue);
    delay(10);
  }

}
/****************************************************************************/
void loop()
{
  Blynk.run();
  ArduinoOTA.handle();
  timer.run(); // Initiates SimpleTimer

  if (varAlertMode == 1) {
    for (int i = 0; i < 20; i++) {
      fill_solid(leds, NUM_LEDS, CRGB::White);
      FastLED.show();
      FastLED.delay(50);
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      FastLED.delay(50);
    }
    varAlertMode = 0;
  }

  if (varManualMode == 1) { // Manual Control
    fill_solid(leds, NUM_LEDS, CHSV(varHue, varSaturation, varBrightness));
  }

  if (varManualMode == 0) { // Pallette Mode
    gPatterns[gCurrentPatternNumber]();
    FastLED.show();
    FastLED.delay(1000 / varUpdatesPerSec);
    EVERY_N_MILLISECONDS( 20 ) {
      gHue++;  // slowly cycle the "base color" through the rainbow
    }
  }
  FastLED.show();
}
/****************************************************************************/
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, varRainbowSpeed);
}

void rainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) {
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void whitescan() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
    FastLED.show();
    FastLED.delay(1000 / varUpdatesPerSec);
    leds[i] = CRGB::Black;

  }
}

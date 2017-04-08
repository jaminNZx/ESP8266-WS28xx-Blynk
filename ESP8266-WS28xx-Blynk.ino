#define BLYNK_PRINT Serial
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <FastLED.h>
#include <SimpleTimer.h>
#include <wifi_credentials.h>
#include "settings.h"

int varManualMode, varHue, varSaturation, varBrightness, varHuePrev, varHueNew;
int varNextColour, varNextColourPrev, varBlendingMode, varUpdatesPerSec;
int varAlertMode, varZone, varRainbowSpeed;
long HexRGB;
String FormattedRGB, FormattedRGBprev;
int varNight, curMode, curSaturation, curBrightness, curHue;
int varMemSave, varMem1, varMem1_Brightness, varMem1_Saturation, varMem1_Hue, varMem1_ManualMode;
int varMem2, varMem2_Brightness, varMem2_Saturation, varMem2_Hue, varMem2_ManualMode;
int testLEDnumber;

WidgetTerminal terminal(vPIN_TERMINAL);
SimpleTimer timer;
CRGB leds[LED_NUMBER];

void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
#if defined(USE_LOCAL_SERVER)
  Blynk.begin(AUTH, WIFI_SSID, WIFI_PASS, SERVER);
#else
  Blynk.begin(AUTH, WIFI_SSID, WIFI_PASS);
#endif
  while (Blynk.connect() == false) {}
  /*********** OTA *************/
  ArduinoOTA.setHostname(OTA_HOSTNAME);
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
#ifdef LED_LIMIT_MILLIAMPS
  FastLED.setMaxPowerInVoltsAndMilliamps(5, LED_LIMIT_MILLIAMPS);
#endif
  FastLED.addLeds<LED_TYPE, DATA_PIN, LED_ARRANGE>(leds, LED_NUMBER);
  /******** READY **************/
  terminal.print(F("# Blynk v" BLYNK_VERSION ": "));
  terminal.print(NICKNAME);
  terminal.println(F(" Device started"));
  terminal.flush();

  timer.setInterval(500, updateColours);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle};
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void updateColours() {

}
BLYNK_WRITE(vPIN_HUE) {
  if ( (varZone == ZONE) || (varZone == 1)) {
    varHue = param.asInt();
    if (varManualMode) {
      HexRGB = ((long)leds[0].r << 16) | ((long)leds[0].g << 8 ) | (long)leds[0].b;
      FormattedRGB = "#" + String(HexRGB, HEX);
      Blynk.setProperty(vPIN_HUE, "color", FormattedRGB);
    }
  }
}
BLYNK_WRITE(vPIN_SATURATION) {
  if ( (varZone == ZONE) || (varZone == 1)) {
    varSaturation = param.asInt();
    if (varManualMode) {
      HexRGB = ((long)leds[0].r << 16) | ((long)leds[0].g << 8 ) | (long)leds[0].b;
      FormattedRGB = "#" + String(HexRGB, HEX);
      Blynk.setProperty(vPIN_HUE, "color", FormattedRGB);
    }
  }
}
BLYNK_WRITE(vPIN_BRIGHTNESS) {
  if ( (varZone == ZONE) || (varZone == 1)) {
    varBrightness = param.asInt();
    if (varManualMode) {
      HexRGB = ((long)leds[0].r << 16) | ((long)leds[0].g << 8 ) | (long)leds[0].b;
      FormattedRGB = "#" + String(HexRGB, HEX);
      Blynk.setProperty(vPIN_HUE, "color", FormattedRGB);
    }
  }
}
BLYNK_WRITE(vPIN_PRESET) {
  if ( (varZone == ZONE) || (varZone == 1)) {
    varNextColour = param.asInt();
    if (varNextColour == 1 && varNextColourPrev == 0) {
      nextPattern();
      delay(10);
    }
    varNextColourPrev = varNextColour;
  }
}
BLYNK_WRITE(vPIN_COLOUR_BLUE) {
  if ( (varZone == ZONE) || (varZone == 1)) {
    varManualMode = 1;
    varBrightness = (int)255;
    varSaturation = (int)255;
    varHue        = (int)152;
    Blynk.virtualWrite(vPIN_MANUAL, varManualMode);
    Blynk.virtualWrite(vPIN_BRIGHTNESS, 255);
    Blynk.virtualWrite(vPIN_SATURATION, 255);
    Blynk.virtualWrite(vPIN_HUE, 152);
    delay(10);
  }
}
BLYNK_WRITE(vPIN_FPS) {
  if ( (varZone == ZONE) || (varZone == 1)) varUpdatesPerSec = param.asInt();
}
BLYNK_WRITE(vPIN_COLOUR_RED) {
  if ( (varZone == ZONE) || (varZone == 1)) {
    varManualMode = 1;
    varBrightness = (int)255;
    varSaturation = (int)255;
    varHue        = (int)0;
    Blynk.virtualWrite(vPIN_MANUAL, varManualMode);
    Blynk.virtualWrite(vPIN_BRIGHTNESS, 255);
    Blynk.virtualWrite(vPIN_SATURATION, 255);
    Blynk.virtualWrite(vPIN_HUE, 0);
    delay(10);
  }
}
BLYNK_WRITE(vPIN_COLOUR_GREEN) {
  if ( (varZone == ZONE) || (varZone == 1)) {
    varManualMode = 1;
    varBrightness = (int)255;
    varSaturation = (int)255;
    varHue        = (int)80;
    Blynk.virtualWrite(vPIN_MANUAL, varManualMode);
    Blynk.virtualWrite(vPIN_BRIGHTNESS, 255);
    Blynk.virtualWrite(vPIN_SATURATION, 255);
    Blynk.virtualWrite(vPIN_HUE, 80);
    delay(10);
  }
}
BLYNK_WRITE(vPIN_COLOUR_WHITE) {
  if ( (varZone == ZONE) || (varZone == 1)) {
    varManualMode = 1;
    varBrightness = (int)255;
    varSaturation = (int)0;
    Blynk.virtualWrite(vPIN_MANUAL, varManualMode);
    Blynk.virtualWrite(vPIN_BRIGHTNESS, 255);
    Blynk.virtualWrite(vPIN_SATURATION, 0);
    delay(10);
  }
}
BLYNK_WRITE(vPIN_MANUAL) {
  if ( (varZone == ZONE) || (varZone == 1)) varManualMode = param.asInt();
  if (param.asInt()) {
    HexRGB = ((long)leds[0].r << 16) | ((long)leds[0].g << 8 ) | (long)leds[0].b;
    FormattedRGB = "#" + String(HexRGB, HEX);
    Blynk.setProperty(vPIN_HUE, "color", FormattedRGB);
  }
}
BLYNK_WRITE(vPIN_SYNC_GHUE) {
  gHue = 0;
  varUpdatesPerSec = 15;
  terminal.print(NICKNAME);
  terminal.println(" | Sync'd gHUE ");
  terminal.flush();
}
BLYNK_WRITE(vPIN_COLOUR_LIGHTBLUE) {
  if ( (varZone == ZONE) || (varZone == 1)) {
    varManualMode = 1;
    Blynk.virtualWrite(vPIN_MANUAL, varManualMode);
    varBrightness = (int)255;
    varSaturation = (int)255;
    varHue        = (int)27;
    Blynk.virtualWrite(vPIN_BRIGHTNESS, 255);
    Blynk.virtualWrite(vPIN_SATURATION, 255);
    Blynk.virtualWrite(vPIN_HUE, 27);
    delay(10);
  }
}
BLYNK_WRITE(vPIN_COLOUR_YELLOW) {
  if ( (varZone == ZONE) || (varZone == 1)) {
    varManualMode = 1;
    varBrightness = (int)255;
    varSaturation = (int)255;
    varHue        = (int)64;
    Blynk.virtualWrite(vPIN_MANUAL, varManualMode);
    Blynk.virtualWrite(vPIN_BRIGHTNESS, 255);
    Blynk.virtualWrite(vPIN_SATURATION, 255);
    Blynk.virtualWrite(vPIN_HUE, 64);
    delay(10);
  }
}
BLYNK_WRITE(vPIN_ALERT) {
  varAlertMode = param.asInt();
}
BLYNK_WRITE(vPIN_OFF) {
  if ( (varZone == ZONE) || (varZone == 1)) {
    varManualMode = 1;
    varBrightness = (int)0;
    Blynk.virtualWrite(vPIN_MANUAL, varManualMode);
    Blynk.virtualWrite(vPIN_BRIGHTNESS, 0);
    delay(10);
  }
}
BLYNK_WRITE(vPIN_RAINBOWSPEED) {
  if ( (varZone == ZONE) || (varZone == 1)) {
    varRainbowSpeed = param.asInt();
    Blynk.virtualWrite(vPIN_RAINBOWSPEED, varRainbowSpeed);
  }
}
BLYNK_WRITE(vPIN_ZONE_SELECT) {
  varZone = param.asInt();
  if (varZone == ZONE) {
    terminal.print(NICKNAME);
    terminal.println(" | Zone Selected!");
    terminal.flush();
    Blynk.virtualWrite(vPIN_HUE,  varHue);
    Blynk.virtualWrite(vPIN_SATURATION,  varSaturation);
    Blynk.virtualWrite(vPIN_BRIGHTNESS,  varBrightness);
    Blynk.virtualWrite(vPIN_FPS,  varUpdatesPerSec);
    Blynk.virtualWrite(vPIN_MANUAL, varManualMode);
    Blynk.virtualWrite(vPIN_RAINBOWSPEED, varRainbowSpeed);
  }
}
BLYNK_WRITE(vPIN_COLOUR_MEM1) {
  if ( (varZone == ZONE) || (varZone == 1)) {
    varMem1 = param.asInt();
    if (varMemSave && varMem1) {
      varMem1_ManualMode = varManualMode;
      varMem1_Hue        = varHue;
      varMem1_Saturation = varSaturation;
      varMem1_Brightness = varBrightness;
      HexRGB = ((long)leds[0].r << 16) | ((long)leds[0].g << 8 ) | (long)leds[0].b;
      FormattedRGB = "#" + String(HexRGB, HEX);
      Blynk.setProperty(vPIN_COLOUR_MEM1, "color", FormattedRGB);
    } else if (!varMemSave && varMem1) {
      varManualMode = varMem1_ManualMode;
      varBrightness = varMem1_Brightness;
      varSaturation = varMem1_Saturation;
      varHue        = varMem1_Hue;
      Blynk.virtualWrite(vPIN_MANUAL, varManualMode);
      Blynk.virtualWrite(vPIN_BRIGHTNESS, varBrightness);
      Blynk.virtualWrite(vPIN_SATURATION, varSaturation);
      Blynk.virtualWrite(vPIN_HUE, varHue);
      HexRGB = ((long)leds[0].r << 16) | ((long)leds[0].g << 8 ) | (long)leds[0].b;
      FormattedRGB = "#" + String(HexRGB, HEX);
      Blynk.setProperty(vPIN_COLOUR_MEM1, "color", FormattedRGB);
    }
  }
  delay(10);
}
BLYNK_WRITE(vPIN_COLOUR_MEM2) {
  if ( (varZone == ZONE) || (varZone == 1)) {
    varMem2 = param.asInt();
    if (varMemSave && varMem2) {
      varMem2_ManualMode = varManualMode;
      varMem2_Hue        = varHue;
      varMem2_Saturation = varSaturation;
      varMem2_Brightness = varBrightness;
      HexRGB = ((long)leds[0].r << 16) | ((long)leds[0].g << 8 ) | (long)leds[0].b;
      FormattedRGB = "#" + String(HexRGB, HEX);
      Blynk.setProperty(vPIN_COLOUR_MEM2, "color", FormattedRGB);
    } else if (!varMemSave && varMem2) {
      varManualMode = varMem2_ManualMode;
      varBrightness = varMem2_Brightness;
      varSaturation = varMem2_Saturation;
      varHue        = varMem2_Hue;
      Blynk.virtualWrite(vPIN_MANUAL, varManualMode);
      Blynk.virtualWrite(vPIN_BRIGHTNESS, varBrightness);
      Blynk.virtualWrite(vPIN_SATURATION, varSaturation);
      Blynk.virtualWrite(vPIN_HUE, varHue);
      HexRGB = ((long)leds[0].r << 16) | ((long)leds[0].g << 8 ) | (long)leds[0].b;
      FormattedRGB = "#" + String(HexRGB, HEX);
      Blynk.setProperty(vPIN_COLOUR_MEM2, "color", FormattedRGB);
    }
  }
  delay(10);
}
BLYNK_WRITE(vPIN_COLOUR_MEMSAVE) {
  varMemSave = param.asInt();
}
BLYNK_WRITE(vPIN_TESTMODE) {
  if (param.asInt()) {
    varManualMode = 2;
    testLEDnumber = param.asInt();
  } else {
    varManualMode = 0;
  }
}
BLYNK_WRITE(vPIN_NIGHTMODE) {
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
    Blynk.virtualWrite(vPIN_MANUAL, varManualMode);
    Blynk.virtualWrite(vPIN_BRIGHTNESS, 100);
    Blynk.virtualWrite(vPIN_SATURATION, 255);
    Blynk.virtualWrite(vPIN_HUE, 152);
    delay(10);
  } else if (!varNight) {
    varManualMode = curMode;
    varBrightness = curBrightness;
    varSaturation = curSaturation;
    varHue        = curHue;
    Blynk.virtualWrite(vPIN_MANUAL, curMode);
    Blynk.virtualWrite(vPIN_BRIGHTNESS, curBrightness);
    Blynk.virtualWrite(vPIN_SATURATION, curSaturation);
    Blynk.virtualWrite(vPIN_HUE, curHue);
    delay(10);
  }

}
/****************************************************************************/
void loop()
{
  Blynk.run();
  ArduinoOTA.handle();
  timer.run(); // Initiates SimpleTimer

  switch (varAlertMode) {
    case 1:
      for (int i = 0; i < 20; i++) {
        fill_solid(leds, LED_NUMBER, CRGB::White);
        FastLED.show();
        FastLED.delay(50);
        fill_solid(leds, LED_NUMBER, CRGB::Black);
        FastLED.show();
        FastLED.delay(50);
      }
      varAlertMode = 0;
      break;
    case 2:
      for (int i = 0; i < 10; i++) {
        fill_solid(leds, LED_NUMBER, CRGB::White);
        FastLED.show();
        FastLED.delay(250);
        fill_solid(leds, LED_NUMBER, CRGB::Black);
        FastLED.show();
        FastLED.delay(50);
        fill_solid(leds, LED_NUMBER, CRGB::White);
        FastLED.show();
        FastLED.delay(50);
        fill_solid(leds, LED_NUMBER, CRGB::Black);
        FastLED.show();
        FastLED.delay(50);
      }
      varAlertMode = 0;
      break;
    case 3:
      for (int i = 0; i < 20; i++) {
        fill_solid(leds, LED_NUMBER, CRGB::Blue);
        FastLED.show();
        FastLED.delay(50);
        fill_solid(leds, LED_NUMBER, CRGB::Red);
        FastLED.show();
        FastLED.delay(50);
      }
      varAlertMode = 0;
      break;
    case 4:
      for (int i = 0; i < 20; i++) {
        fill_solid(leds, LED_NUMBER, CRGB::Red);
        FastLED.show();
        FastLED.delay(50);
        fill_solid(leds, LED_NUMBER, CRGB::Black);
        FastLED.show();
        FastLED.delay(50);
      }
      varAlertMode = 0;
      break;
  }

  switch (varManualMode) {
    case 1:
      fill_solid(leds, LED_NUMBER, CHSV(varHue, varSaturation, varBrightness));
      break;
    case 2:
      //int val = analogRead(2);
      //int numLedsToLight = map(val, 0, 1023, 0, NUM_LEDS);

      FastLED.clear();
      for (int led = 0; led < testLEDnumber; led++) leds[led] = CRGB::Blue;
      break;
    default:
      gPatterns[gCurrentPatternNumber]();
      EVERY_N_MILLISECONDS( 20 ) gHue++;  // slowly cycle the "base color" through the rainbow
      break;

  }
  FastLED.show();
}
/****************************************************************************/
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern() {
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, LED_NUMBER, gHue, varRainbowSpeed);
  FastLED.delay(1000 / varUpdatesPerSec);
}

void rainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) {
  if ( random8() < chanceOfGlitter) leds[ random16(LED_NUMBER) ] += CRGB::White;
  FastLED.delay(1000 / varUpdatesPerSec);
}

void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, LED_NUMBER, 10);
  int pos = random16(LED_NUMBER);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
  FastLED.delay(1000 / varUpdatesPerSec);
}

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, LED_NUMBER, 20);
  int pos = beatsin16(13, 0, LED_NUMBER);
  leds[pos] += CHSV( gHue, 255, 192);
  FastLED.delay(1000 / varUpdatesPerSec);
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, LED_NUMBER, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, LED_NUMBER)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.delay(1000 / varUpdatesPerSec);
}

#define BLYNK_PRINT Serial
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <FastLED.h>
#include <SimpleTimer.h>
#include <wifi_credentials.h>
#include "settings.h"

int varSpeed, varRainbowSpeed, varAlertMode, varZone;
int testLEDnumber, varMemSave;
int arrayCurrent[5], arrayMemory1[5], arrayMemory2[5], arrayMemory3[5], arrayNightMemory[5];

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
  arrayCurrent[0, 255, 255, 255]; // Set starting sequence as rainbow mode
  varSpeed = 100; // Start on 100 fps
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
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle};
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

String CurrentHexRGB() {
  return String("#") + String(((long)leds[0].r << 16) | ((long)leds[0].g << 8 ) | (long)leds[0].b, HEX);
}

BLYNK_WRITE(vPIN_HUE) {
  if (varZone == ZONE || varZone == 1) {
    updateColours(arrayCurrent[0], param.asInt(), arrayCurrent[2], arrayCurrent[3]);
    //Blynk.setProperty(vPIN_HUE, "color", CurrentHexRGB());
  }
}
BLYNK_WRITE(vPIN_SATURATION) {
  if (varZone == ZONE || varZone == 1) {
    updateColours(arrayCurrent[0], arrayCurrent[1], param.asInt(), arrayCurrent[3]);
    //Blynk.setProperty(vPIN_HUE, "color", CurrentHexRGB());
  }
}
BLYNK_WRITE(vPIN_BRIGHTNESS) {
  if (varZone == ZONE || varZone == 1) {
    updateColours(arrayCurrent[0], arrayCurrent[1], arrayCurrent[2], param.asInt());
    //Blynk.setProperty(vPIN_HUE, "color", CurrentHexRGB());
  }
}
BLYNK_WRITE(vPIN_FPS) {
  if (varZone == ZONE || varZone == 1) varSpeed = param.asInt();
}
BLYNK_WRITE(vPIN_PRESET) {
  if (varZone == ZONE || varZone == 1 && param.asInt()) nextPattern();
}
BLYNK_WRITE(vPIN_COLOUR_RED) {
  if (varZone == ZONE || varZone == 1 && param.asInt()) {
    updateColours(1, 0, 255, 255);
    updateWidgets();
  }
}
BLYNK_WRITE(vPIN_COLOUR_GREEN) {
  if (varZone == ZONE || varZone == 1 && param.asInt()) {
    updateColours(1, 80, 255, 255);
    updateWidgets();
  }
}
BLYNK_WRITE(vPIN_COLOUR_BLUE) {
  if (varZone == ZONE || varZone == 1 && param.asInt()) {
    updateColours(1, 152, 255, 255);
    updateWidgets();
  }
}
BLYNK_WRITE(vPIN_COLOUR_WHITE) {
  if (varZone == ZONE || varZone == 1 && param.asInt()) {
    updateColours(1, 255, 0, 255);
    updateWidgets();
  }
}
BLYNK_WRITE(vPIN_MANUAL) {
  if (varZone == ZONE || varZone == 1) {
    updateColours(param.asInt(), arrayCurrent[1], arrayCurrent[2], arrayCurrent[3]);
    updateWidgets();
  }
}
BLYNK_WRITE(vPIN_SYNC_GHUE) {
  gHue = 0;
  varSpeed = 100;
  terminal.print(NICKNAME);
  terminal.println(" | Sync'd gHUE ");
  terminal.flush();
}
BLYNK_WRITE(vPIN_ALERT) {
  varAlertMode = param.asInt();
}
BLYNK_WRITE(vPIN_OFF) {
  if ( varZone == ZONE || varZone == 1 && param.asInt()) {
    updateColours(1, 255, 255, 0);
    updateWidgets();
  }
}
BLYNK_WRITE(vPIN_RAINBOWSPEED) {
  if ( varZone == ZONE || varZone == 1) {
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
    updateColours(arrayCurrent[0], arrayCurrent[1], arrayCurrent[2], arrayCurrent[3]);
    Blynk.virtualWrite(vPIN_FPS,  varSpeed);
    Blynk.virtualWrite(vPIN_RAINBOWSPEED, varRainbowSpeed);
  }
}
BLYNK_WRITE(vPIN_COLOUR_MEM1) {
  if (varZone == ZONE || varZone == 1 && param.asInt()) {
    if (varMemSave) {
      arrayMemory1[arrayCurrent[0], arrayCurrent[1], arrayCurrent[2], arrayCurrent[3]];
      Blynk.setProperty(vPIN_COLOUR_MEM1, "color", CurrentHexRGB());
    } else {
      updateColours(arrayMemory1[0], arrayMemory1[1], arrayMemory1[2], arrayMemory1[3]);
      updateWidgets();
    }
  }
}
BLYNK_WRITE(vPIN_COLOUR_MEM2) {
  if (varZone == ZONE || varZone == 1 && param.asInt()) {
    if (varMemSave) {
      arrayMemory2[arrayCurrent[0], arrayCurrent[1], arrayCurrent[2], arrayCurrent[3]];
      Blynk.setProperty(vPIN_COLOUR_MEM2, "color", CurrentHexRGB());
    } else {
      updateColours(arrayMemory2[0], arrayMemory2[1], arrayMemory2[2], arrayMemory2[3]);
      updateWidgets();
    }
  }
}
BLYNK_WRITE(vPIN_COLOUR_MEM3) {
  if ( varZone == ZONE || varZone == 1 && param.asInt()) {
    if (varMemSave) {
      arrayMemory3[arrayCurrent[0], arrayCurrent[1], arrayCurrent[2], arrayCurrent[3]];
      Blynk.setProperty(vPIN_COLOUR_MEM3, "color", CurrentHexRGB());
    } else {
      updateColours(arrayMemory3[0], arrayMemory3[1], arrayMemory3[2], arrayMemory3[3]);
      updateWidgets();
    }
  }
}
BLYNK_WRITE(vPIN_COLOUR_MEMSAVE) {
  varMemSave = param.asInt();
}
BLYNK_WRITE(vPIN_TESTMODE) {
  if (param.asInt()) {
    updateColours(2, arrayCurrent[1], arrayCurrent[2], arrayCurrent[3]);
    testLEDnumber = param.asInt();
  } else {
    updateColours(0, arrayCurrent[1], arrayCurrent[2], arrayCurrent[3]);
  }
}
BLYNK_WRITE(vPIN_NIGHTMODE) {
  if (param.asInt()) {
    arrayNightMemory[arrayCurrent[0], arrayCurrent[1], arrayCurrent[2], arrayCurrent[3]];
    updateColours(1, 152, 255, 100);
    updateWidgets();
  } else {
    updateColours(arrayNightMemory[0], arrayNightMemory[1], arrayNightMemory[2], arrayNightMemory[3]);
    updateWidgets();
  }
}
void updateColours(int m, int h, int s, int b) {
  arrayCurrent[0] = m;
  arrayCurrent[1] = h;
  arrayCurrent[2] = s;
  arrayCurrent[3] = b;
}
void updateWidgets() {
  Blynk.virtualWrite(vPIN_MANUAL,     arrayCurrent[0]);
  Blynk.virtualWrite(vPIN_HUE,        arrayCurrent[1]);
  Blynk.virtualWrite(vPIN_SATURATION, arrayCurrent[2]);
  Blynk.virtualWrite(vPIN_BRIGHTNESS, arrayCurrent[3]);
  //Blynk.setProperty(vPIN_HUE, "color", CurrentHexRGB());
}

/****************************************************************************/
void loop(){
  Blynk.run();
  ArduinoOTA.handle();
  timer.run();

  switch (varAlertMode) {
    case 1:
      for (int i = 0; i < 10; i++) {
        fill_solid(leds, LED_NUMBER, CRGB::White);
        FastLED.show();
        delay(50);
        fill_solid(leds, LED_NUMBER, CRGB::Black);
        FastLED.show();
        delay(50);
      }
      varAlertMode = 0;
      break;
    case 2:
      for (int i = 0; i < 5; i++) {
        fill_solid(leds, LED_NUMBER, CRGB::White);
        FastLED.show();
        delay(250);
        fill_solid(leds, LED_NUMBER, CRGB::Black);
        FastLED.show();
        delay(50);
        fill_solid(leds, LED_NUMBER, CRGB::White);
        FastLED.show();
        delay(50);
        fill_solid(leds, LED_NUMBER, CRGB::Black);
        FastLED.show();
        delay(250);
      }
      varAlertMode = 0;
      break;
    case 3:
      for (int i = 0; i < 10; i++) {
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
      for (int i = 0; i < 10; i++) {
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

  switch (arrayCurrent[0]) {
    case 1:
      fill_solid(leds, LED_NUMBER, CHSV(arrayCurrent[1], arrayCurrent[2], arrayCurrent[3]));
      break;
    case 2:
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
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, LED_NUMBER, gHue, varRainbowSpeed);
  FastLED.delay(1000 / varSpeed);
}

void rainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) {
  if ( random8() < chanceOfGlitter) leds[ random16(LED_NUMBER) ] += CRGB::White;
  FastLED.delay(1000 / varSpeed);
}

void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, LED_NUMBER, 10);
  int pos = random16(LED_NUMBER);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
  FastLED.delay(1000 / varSpeed);
}

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, LED_NUMBER, 20);
  int pos = beatsin16(13, 0, LED_NUMBER);
  leds[pos] += CHSV( gHue, 255, 192);
  FastLED.delay(1000 / varSpeed);
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, LED_NUMBER, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, LED_NUMBER)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.delay(1000 / varSpeed);
}

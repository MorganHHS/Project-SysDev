#include <FastLED.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

#define NUM_LEDS 1
#ifndef WIFISSID
#define WIFISSID "banaantje"         //SSID of the WIFI Accesspoint
#define WIFIPASS "BananenZijnGoed!"  //Password of the WIFI Accesspoint
#endif


const char* ssid = WIFISSID;
const char* password = WIFIPASS;
const char* host = "192.168.4.1";  //Server Host IP
const uint16_t port = 6969;        // Socket Port number


// How many leds in your strip?

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN D5

void ledOn();
void ledOff();
int leesBeweging();

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  Wire.begin();
  // Uncomment/edit one of the following lines for your leds arrangement.

  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
}



void loop(void) {

  ledOn();
  delay(500);
  ledOff();
  delay(500);

  while (1) {

    int beweging = leesBeweging();

    delay(500);

    if(beweging == 1) {
      ledOn();
      delay(1000);
      ledOff();
      delay(500);
    }
  }

}

void ledOn() {

  leds[0] = CRGB::Red;
  FastLED.show();
}

void ledOff() {

  leds[0] = CRGB::Black;
  FastLED.show();
}

int leesBeweging() {

  Wire.beginTransmission(0x38);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);
  unsigned int inputs = Wire.read();
  Serial.print("Digital in: ");
  Serial.println(inputs & 0x01);
  return inputs & 0x01;



}

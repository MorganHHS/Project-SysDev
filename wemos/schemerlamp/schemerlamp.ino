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
const uint16_t port = 8080;        // Socket Port number


// How many leds in your strip?

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN D5

void ledOn();        // Led aan
void ledOff();       //Led uit
int leesBeweging();  // Deze functie zal letten op beweging.
volatile bool brand;

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  Wire.begin();
  wifiVerbinden();
  ledOff();
  // Uncomment/edit one of the following lines for your leds arrangement.

  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS); // Gebruik maken van de library FastLED, uitlezen hoeveel leds we gebruiken.
}



void loop(void) {
  WiFiClient client = verbindenPi();
  brand =false;
  client.print("setSchemerlamp");

  int vorigeBeweging = 0;
  while (1) {
    
    int beweging = leesBeweging();  // LeesBeweging() checkt of er beweging plaats vindt bij de bewegingssensor
    Serial.print(beweging);
    if (beweging != vorigeBeweging) {
      if (beweging == 1) {
        Serial.print("Beweging: ");
        Serial.println(beweging); // print de waarde in de seriele monitor
        client.print("schemerlamp mary leesBeweging");
      }      
    }
  vorigeBeweging = beweging;    


    if (client.available()) {
      char ch = static_cast<const char>(client.read());
      Serial.print("pi zegt:");
      Serial.print(ch);
      Serial.print("\n");
      if (ch == '4') {
        ledOn();
      }
      if (ch == '8') {
        brand = true; // Pi heeft 8 gestuurd, er is brand.
        ledOn();
      }
      if (ch == '9'){
        brand = false; // Pi heeft 9 gestuurd, er is dus geen brand.
        ledOn();
      }
      ch = '0';
    }

    if (!client.connected()) {
      Serial.println("Verbinding met server verbroken");
      break;
    }
    if ((WiFi.status() != WL_CONNECTED)) {
      Serial.println("Verbinding met WiFi verbroken");
      setup();
    }
  }
}

void ledOn() {
  leds[0] = CRGB::Red;  // Hiermee zetten we het ledje op een bepaalde kleur
  FastLED.show();       // Hiermee wordt de led gezegd te laten zien wat er zojuist is ingesteld. Hij gaat in dit geval dus aan
  for (int i = 0; i < 1000; i++) {
    int beweging = leesBeweging();
    if (beweging == 1) {
      delay(1000);
      Serial.println(i);      
      ledOn();
      //delay(1000);
    }
    delay(10);
  }
  if (!brand){
    Serial.println("ledje uit plis" );  
    ledOff();
  }
  
}

void ledOff() {
  Serial.println("led uit");
  leds[0] = CRGB::Black;
  FastLED.show();
}

int leesBeweging() {

  Wire.beginTransmission(0x38);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);
  unsigned int inputs = Wire.read();
  return inputs & 0x01;
}

void wifiVerbinden() {                     // verbinding maken met Accespoint van de Pi.
  WiFi.mode(WIFI_STA);                     //
  WiFi.begin(ssid, password);              // arduino library doet het moeilijke werk
  while (WiFi.status() != WL_CONNECTED) {  // print .. terwijl hij aan t verbinden is.
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

WiFiClient verbindenPi() {  // verbinden met de pi
  Serial.print("connecting to: ");
  Serial.print(host);
  Serial.print(" :");
  Serial.println(port);

  // Hiermee zet je de Wificlient class in TCP connectie modus.
  WiFiClient client;  // client aanmaken
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
  }
  return client;
}

#include <Servo.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FastLED.h>

#ifndef WIFISSID
#define WIFISSID "banaantje"         //SSID of the WIFI Accesspoint
#define WIFIPASS "BananenZijnGoed!"  //Password of the WIFI Accesspoint
#endif

const char* ssid = WIFISSID;
const char* password = WIFIPASS;
const char* host = "192.168.4.1";  //Server Host IP
const uint16_t port = 8080;        // Socket Port number

#define LED_PIN D5 // Defines voor de LED strip.
#define NUM_LEDS 3
CRGB leds[NUM_LEDS];

bool brand;

void setup() 
{
  Serial.begin(115200);  // baudrate instellen en serieel starten
  Wire.begin();          // I2C communicatie starten
  wifiVerbinden();       // wifi verbinden functie
  ledSetup();
}

void loop() 
{
  brand = false;
  twiSetup();
  WiFiClient client = verbindenPi();
  int prevPotVal = 0;
  int prevLdrVal = 0;
  while (1) 
  {
    unsigned int potValue = PotRead();
    unsigned int LdrValue = LdrRead();
    //Serial.println("lalala");

    if (client.available())
    { //Reads if server as sent WEMOS a data
      char ch = static_cast<char>(client.read());
      Serial.print("Server zegt: ");
      Serial.println(ch);
      if (ch == '1')
      {
        RegelLcdPanel();
        delay(100);
      }
      if (ch == '2')
      {
        RegelVenster();
        delay(100);
      }
      if (ch == '8')
      {
        brand = true;
        RegelVenster();
        RegelLcdPanel();
        delay(100);
      }
      if (ch == '9')
      {
        brand = false;
        RegelVenster();
        RegelLcdPanel();
        delay(100);
      }
      ch = '0';
    }

    if (potValue != prevPotVal && abs(potValue - prevPotVal) >= 20) // print alleen als verschil meer dan 20 is
    { 
      Serial.print("potwaarde: ");
      Serial.println(prevPotVal);
      client.print("vensterPot");
      prevPotVal = potValue;
    }
    
    if (LdrValue != prevLdrVal && abs(LdrValue - prevLdrVal) >= 200) // Als de LDR waarde verandert && Nieuwe waarde - oude waarde heeft een verschil van 300, zendt een bericht naar de Pi.
    {
      Serial.print("LDR-waarde: ");
      Serial.println(prevLdrVal);
      client.print("vensterLDR");
      prevLdrVal = LdrValue;
    }
    
    if (!client.connected()) 
    {
      break;
    }
    if ((WiFi.status() != WL_CONNECTED)) 
    {
      Serial.println("Verbinding met WiFi verbroken");
      setup();
    }
  }
}

void RegelLcdPanel() 
{
  if (!brand) 
  {
    for (int i = 0; i < NUM_LEDS; i++ ) // Deze for loop is voor het regelen van de brightness, de CRGB combinatie staat nu op oranje
    {
      leds[i] = CRGB(252, 120, 6);
      FastLED.setBrightness(( PotRead() / 50 ));
      FastLED.show();
    }
  } 
  if (brand) 
  {
    for (int i = 0; i < NUM_LEDS; i++ ) // Deze for loop is voor het regelen van de brightness, de CRGB combinatie staat nu op oranje
    {
      leds[i] = CRGB(255, 0, 0);
      FastLED.setBrightness((50));
      FastLED.show();
    }
  }
}

unsigned int LdrRead()
{
  Wire.requestFrom(0x36, 2);
  unsigned int anin0 = Wire.read() & 0x03;
  anin0 = anin0 << 8; // Volgens mij als die 8 verlaagt wordt krijg je lage output waardes.
  anin0 = anin0 | Wire.read();
  return anin0;
}

unsigned int PotRead()
{
  Wire.requestFrom(0x36, 4);
  unsigned int anin1 = Wire.read() & 0x03;
  anin1 = anin1 << 8;
  anin1 = anin1 | Wire.read();
  anin1 = Wire.read() & 0x03; // Door deze twee keer uit te lezen schrijf je de LDR over met de waarde van de potmeter.
  anin1 = anin1 << 8;
  anin1 = anin1 | Wire.read();
  return anin1;
}

void wifiVerbinden() 
{                     // verbinding maken met Accespoint van de Pi.
  WiFi.mode(WIFI_STA);                     //
  WiFi.begin(ssid, password);              // arduino library doet het moeilijke werk
  while (WiFi.status() != WL_CONNECTED) {  // print . terwijl hij aan t verbinden is.
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

WiFiClient verbindenPi() 
{  // verbinden met de pi
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

void RegelVenster()
{
  if (!brand) {
    if (LdrRead() > 500)
      VensterOmlaag();
    else
      VensterOmhoog();
  } if (brand) {
    VensterOmhoog();
  }
}
void VensterOmlaag()
{
  //Set PCA9554 outputs (IO44-IO7)
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(1 << 4));
  Wire.endTransmission();
  //Serial.print("Digital out: ");
  //Serial.println(outputs & 0x0F);
}

void VensterOmhoog()
{
  //Set PCA9554 outputs (IO44-IO7)
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(0 << 4));
  Wire.endTransmission();
  //Serial.print("Digital out: ");
  //Serial.println(outputs & 0x0F);
}

void twiSetup(void) 
{
  //Config PCA9554
  //Inside loop for debugging purpose (hot plugging wemos module into i/o board).
  //IO0-IO3 as input, IO4-IO7 as output.
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));
  Wire.write(byte(0x0F));
  Wire.endTransmission();

  //Config MAX11647
  //Inside loop for debugging purpose (hot plugging wemos module into i/o board).
  Wire.beginTransmission(0x36);
  Wire.write(byte(0xA2));
  Wire.write(byte(0x03));
  Wire.endTransmission();
}

void ledSetup() 
{
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS); // Setup voor de LED strip.
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();
}

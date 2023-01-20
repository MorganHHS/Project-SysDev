#include <Servo.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <String.h>

#ifndef WIFISSID
#define WIFISSID "banaantje" // "banaantje" - "Lab001"
#define WIFIPASS "BananenZijnGoed!" // "BananenZijnGoed!" - "Lab001WiFi"
#endif

const char* ssid     = WIFISSID;
const char* password = WIFIPASS;
const char* host = "192.168.4.1"; // 145.52.126.192
const uint16_t port = 5050;

Servo myservo;

int deurdicht = 80; // hoek van 80 graden staat de deur perfect dicht.
int deuropen = 170; // hoek van 170 graden staat de deur mooi open

void setup()
{
  bool brand = false;
  deurSluiten(); // misschien gaat doet dit ding daarom raar?
  myservo.attach(D5); // hiermee zeg je dat servo op PIN D5 zit
  Serial.begin(115200); // baudrate instellen en serieel starten
  Wire.begin(); // I2C communicatie starten
  wifiVerbinden(); // wifi verbinden functie
}

void loop() {
  bool brand = false;
  bool hulp = true;
  WiFiClient client = verbindenPi(); // verbinding maken met de host/server Pi
  int timer5ms = 0;

  while (1) {
    int knopjes = knopjesLezen();
    Serial.print("Waarde knopjes:");
    Serial.println(knopjes);

    if (client.connected()) {
      if (knopjes == 1 ) {
        client.print("1knop");
        delay(100);
      }
      if (knopjes == 2 ) {
        client.print("2knop");
        delay(100);
      }
      knopjes = 0;
    }

    if (client.available()) { // Lezen wat de server verstuurd en printen.
      char ch = static_cast<const char>(client.read());
      Serial.print("pi zegt:");
      Serial.print(ch);
      Serial.print("\n");

      if (ch == '1') {
        brand = false;
        deurOpenen(brand);
        Serial.println("Geen brand");
      }
      if (ch == '2') {
        brand = true;
        deurOpenen(brand);
        Serial.println("Brand");
      }
      if (ch == '3') {
        brand = false;
        deurOpenen(brand);
        Serial.println("Geen brand");
      }
      ch = '0'; // zodat deur niet dubbel commando's gaat doen.
    }

    if (!client.connected()) {
      break;
    }
    if ((WiFi.status() != WL_CONNECTED)) {
      Serial.println("Verbinding met WiFi verbroken");
      setup();
    }
    delay(1);
    if (!brand && hulp) {
      if (timer5ms < 1000) {
        timer5ms++;
      }
      if (timer5ms == 499) {
        lichtAan();
      }
      if (timer5ms == 999) {
        timer5ms = 0;
        lichtUit();
      }
    }
  }
}

bool deurOpenen(bool brand) { // pi kan deze functie aanroepen om deur te openen
  lichtAan();
  myservo.write(deuropen);
  int knop;
  for (int i = 0; i < 500; i++) {
    int knop = knopjesLezen();
    if (knop == 1) {
      brand = true;
      break;
    }
    if (knop == 2) {
      brand = true;
      break;
    }
    delay(10);
  }
  //delay(50);
  if (!brand) {
    deurSluiten(); // deur blijft 5 seconden open en gaat dan weer dicht.
  }
}

void deurSluiten() { // later moet brandvariabele meegegeven worden.
  lichtUit();
  myservo.write(deurdicht); // servo schrijven naar deur dicht hoek, 80 graden
}

void wifiVerbinden() { // verbinding maken met Accespoint van de Pi.
  WiFi.mode(WIFI_STA); //
  WiFi.begin(ssid, password); // arduino library doet het moeilijke werk
  while (WiFi.status() != WL_CONNECTED) { // print .. terwijl hij aan t verbinden is.
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int knopjesLezen() {
  //Config PCA9554
  //Inside loop for debugging purpose (hot plugging wemos module into i/o board).
  //IO0-IO3 as input, IO4-IO7 as output.
  Wire.beginTransmission(0x38); // slave adres meegeven
  Wire.write(byte(0x03));
  Wire.write(byte(0x0F));
  Wire.endTransmission();

  //Read PCA9554 outputs (IO40-IO3)
  Wire.beginTransmission(0x38); // slave adres meegeven
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);
  unsigned int inputs = Wire.read(); // hier lees je waarde van knopje met I2C
  return inputs & 0x03; // knopje waarden teruggeven aan de main.
}

WiFiClient verbindenPi() { // verbinden met de pi
  Serial.print("connecting to: ");
  Serial.print(host);
  Serial.print(" :");
  Serial.println(port);

  // Hiermee zet je de Wificlient class in TCP connectie modus.
  WiFiClient client; // client aanmaken
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
  }
  return client;
}

void lichtAan() {
  //Set PCA9554 outputs (IO44-IO7)
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(3 << 4));
  Wire.endTransmission();
}

void lichtUit() {
  //Set PCA9554 outputs (IO44-IO7)
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(0));
  Wire.endTransmission();
}

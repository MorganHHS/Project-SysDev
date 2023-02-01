#include <Wire.h>
#include <ESP8266WiFi.h>

#define I2C_SDL    D1
#define I2C_SDA    D2

#ifndef WIFISSID
#define WIFISSID  "banaantje" //"Vernetwerk" //"Lab001" 
#define WIFIPASS  "BananenZijnGoed!" //"zachtwarensysteempje" //Lab001WiFi" 
#endif


const char* ssid     = WIFISSID;
const char* password = WIFIPASS;
const char* host = "192.168.4.1";
const uint16_t port = 8080;

int statusKnop = 0;
int laatsteStatusKnop = 0;
int beginIndrukking = 0;
int eindIndrukking = 0;
int tijdIngedrukt = 0;
bool hulp = false;
bool brand = false;
unsigned int vorigeDruk = 0;
unsigned int druk = 0;
int beginGeenDruk = 0;
int eindGeenDruk = 0;
int tijdGeenDruk = 0;

unsigned int inputs = 0;
unsigned int outputs = 0;

void lampAanzetten () { //zet de lamp/led uit
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(1 << 4));
  Wire.endTransmission();
}
void lampUitzetten () { //zet de lamp/led uit
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(1 << 8));
  Wire.endTransmission();
}
void notificatie() { //lamp/led laten knipperen voor hulpsignaal
  if (hulp) {
    lampAanzetten();
    delay(500);
    lampUitzetten();
  }
}
void indrukTijd(WiFiClient* client) { //bepaald/berekend de tijd hoelang de knop is ingedrukt en stuurt dat naar de server
  statusKnop = (inputs & 0x01);
  if (statusKnop != laatsteStatusKnop) {
    //De knop is ingedrukt
    if (statusKnop == HIGH) {
      beginIndrukking = millis();
    }
    //De knop is losgelaten
    else {
      eindIndrukking = millis();
      tijdIngedrukt = eindIndrukking - beginIndrukking;
      if (tijdIngedrukt >= 100UL && tijdIngedrukt < 3900UL) {
        // Verstuurt een string naar de server.
        Serial.println("data versturen naar raspberry pi");
        if (client->connected()) {
          client->print("bed mary regelLed");
          delay(100);
        }
      }
      if (tijdIngedrukt >= 4000UL) {
        // Verstuurt een string naar de server.
        Serial.println("data versturen naar raspberry pi");
        if (client->connected()) {
          client->print("bed mary hulpNodig");
          delay(100);
        }
      }
    }
  }
  laatsteStatusKnop = statusKnop;
}

void uitvoerenActie(WiFiClient* client) {
  // Lezen wat de server verstuurd en juiste actie uitvoeren.
  while (client->available()) {
    char ch = static_cast<char>(client->read());
    if (ch == '1' && brand == false) {
      lampAanzetten();
    }
    if (ch == '2' && brand == false) {
      lampUitzetten();
    }
    if (ch == '3') {
      hulp = !hulp;
    }
    if (ch == '8') {
      lampAanzetten();
      brand = true;
    }
    if (ch == '9') {
      lampUitzetten();
      brand = false;
    }
  }
}
unsigned int leesInputs() {
  //Read PCA9554 inputs (IO40-IO3)
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);
  unsigned int invoer = Wire.read();
  return invoer;
}
void uitBedGevallen(WiFiClient* client) {
  //Leest de analoge waarde van de druksensor
  //En bepaald hoelang iemand niet op het bed heeft opgeligd als er iemand is op gaan liggen.
  Wire.requestFrom(0x36, 4);
  unsigned int anin0 = Wire.read() & 0x03;
  anin0 = anin0 << 8;
  anin0 = anin0 | Wire.read();
  unsigned int analoog = anin0;
  if (analoog != vorigeDruk && analoog - vorigeDruk >= abs(50)) {
    druk = analoog;
    //Druk is te laag nadat de druk hoog was.
    if (druk < 400 && vorigeDruk >= 400) {
      beginGeenDruk = millis();
    }
    //Druk is weer hoog nadat het laag was als het ooit hoog geweest is.
    else if (druk >= 400 && vorigeDruk < 400 && beginGeenDruk > 0) {
      eindGeenDruk = millis();
      tijdGeenDruk = eindGeenDruk - beginGeenDruk;
      if (tijdGeenDruk >= 10000UL) {
        // Verstuurt een string naar de server.
        Serial.println("data versturen naar raspberry pi");
        if (client->connected()) {
          client->print("bed mary afwezigheidBed");
          delay(100);
        }
      }
      beginGeenDruk = 0;
    }
    vorigeDruk = druk;
  }
}
void setup()
{
  Serial.begin(115200);
  Wire.begin();
  // wifi verbinden enzo
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //IO0-IO3 as input, IO4-IO7 as output.
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));
  Wire.write(byte(0x0F));
  Wire.endTransmission();
  lampUitzetten();
}
void loop() {
  // wifi verbinden
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);
  // Hiermee zet je de Wificlient class in TCP connectie modus.
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }
  if (client.connected()) {
    client.print("setBed");
  }
  while (1) {
    inputs = leesInputs();
    uitBedGevallen(&client);
    notificatie();
    indrukTijd(&client);
    uitvoerenActie(&client);
    delay(100);
    //client.stop();
    //break;

    // Close the connection
    Serial.println();
    //Serial.println("connectie afsluiten");
    if (!client.connected()){
      break;
    }
    if ((WiFi.status() != WL_CONNECTED)){
      setup();
    }
  }
}

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <String.h>

#define I2C_SDL D1
#define I2C_SDA D2

#ifndef WIFISSID
#define WIFISSID "banaantje"         // "banaantje" - "Lab001"
#define WIFIPASS "BananenZijnGoed!"  // "BananenZijnGoed!" - "Lab001WiFi"
#endif

const char* ssid = WIFISSID;
const char* password = WIFIPASS;
const char* host = "192.168.4.1";  // 145.52.126.192 // 192.168.4.1
const uint16_t port = 8080;

volatile int rookmelderGrens = 470;


void setup(void) {
  Wire.begin();
  Serial.begin(115200);
  wifiVerbinden();
}

void loop(void) {
  bool brandFlag = false;
  bool brand = false;
  int drempelwaarde = rookmelderGrens;
  WiFiClient client = verbindenPi();
  if(client.connected()){
    client.print("setZuil");
  }
  twiSetup();

  while (1) {
    buzzerUit();
    int vorigeWaarde;
    int knopje = leesKnop();


    if (client.available()) {
      char ch = static_cast<const char>(client.read());
      Serial.print("pi zegt:");
      Serial.print(ch);
      Serial.print("\n");
      if (ch == '1'){
        deurbel(); 
      }
      ch = '0';
    }


    int rookWaarde = leesRookSensor();
    if (rookWaarde != vorigeWaarde){
      Serial.println("Rookwaarde: ");
      Serial.println(rookWaarde);
    }
    vorigeWaarde = rookWaarde;
    if (rookWaarde > drempelwaarde) {
      //Serial.println("brand!!");
      brand = true;
    }
    if (brand) {
      brandAlarm();
      if (knopje == 1) {
        drempelwaarde = rookWaarde + 20;
        brand = false;
        client.print("zuilKnop");
        delay(500);
        knopje = 0;
      }

    } else if (knopje == 1) {
      drempelwaarde = rookmelderGrens;
    }
    if (!client.connected()) {
      break;
    }
    if ((WiFi.status() != WL_CONNECTED)) {
      Serial.println("Verbinding met WiFi verbroken");
      setup();
    }
    if (brandFlag != brand) {  // zodat het niet te vaak print
      if (client.connected()) {
        if (brand) {
          client.print("zuilBrand");
        }
      }
    }
    brandFlag = brand;
    delay(5);
  }
}

void twiSetup(void) {
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

void brandAlarm() {
  //Set PCA9554 outputs (IO44-IO7)
  for (int i = 0; i < 2; i++) {
    buzzerAan();
    //lichtAan();
    delay(50);
    buzzerUit();
    //lichtUit();
    delay(50);
  }
  for (int i = 0; i < 100; i++) {
    int knopje = leesKnop();
    if (knopje == 1) {
      break;
    }
    delay(5);
  }
}

void deurbel() {
  Serial.println("Iemand heeft aangebeld!");
  for (int i = 0; i < 30; i++) {
    buzzerAan();
    delay(50);
    int rook = leesRookSensor();
    if (rook > rookmelderGrens){
      buzzerUit();
      return;
    }
  }
  buzzerUit();
}

int leesKnop() {
  //Read PCA9554 outputs (IO40-IO3)
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);
  unsigned int inputs = Wire.read();
  /*Serial.print("Digital in: ");
  Serial.println(inputs & 0x01);*/
  return inputs & 0x01;
}

int leesRookSensor() {

  //Read analog 10bit inputs 0&1
  Wire.requestFrom(0x36, 4);
  unsigned int anin0 = Wire.read() & 0x03;
  anin0 = anin0 << 8;
  anin0 = anin0 | Wire.read();
  return anin0;
}

void buzzerAan() {
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(3 << 4));
  Wire.endTransmission();
}

void buzzerUit() {
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(0));
  Wire.endTransmission();
}

void lichtAan() {
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(1 << 5));
  Wire.endTransmission();
}

void lichtUit() {
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(0 << 5));
  Wire.endTransmission();
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

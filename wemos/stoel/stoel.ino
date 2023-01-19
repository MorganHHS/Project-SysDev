#include <Servo.h>
#include <Wire.h>
#include <ESP8266WiFi.h>


#ifndef WIFISSID
#define WIFISSID "banaantje"         //SSID of the WIFI Accesspoint
#define WIFIPASS "BananenZijnGoed!"  //Password of the WIFI Accesspoint
#endif


const char* ssid = WIFISSID;
const char* password = WIFIPASS;
const char* host = "192.168.4.1";  //Server Host IP
const uint16_t port = 6969;        // Socket Port number

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long periode = 5000;


void setup() {
  Serial.begin(115200);
  Wire.begin();
  // wifi verbinden enzo
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  startMillis = millis();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void LedAan() {  //Function to turn led ON
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(1 << 4));
  Wire.endTransmission();
}

void TrillenAan() {  //Function to turn vibration motor ON
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(2 << 4));
  Wire.endTransmission();
}

void TrillenUit() {  //Function to turn vibration motor OFF
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(0 << 4));
  Wire.endTransmission();
}

unsigned int DrukSensor() {  //Function to read force sensor output
  Wire.requestFrom(0x36, 4);
  unsigned int anin0 = Wire.read() & 0x03;
  anin0 = anin0 << 8;
  anin0 = anin0 | Wire.read();
  unsigned int b = anin0;
  return b;
}

unsigned int inputs() {  //Function to read button value
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);
  unsigned int a = Wire.read() & 0x01;
  return a;
}




void loop() {


  //Wifi Connection printing
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

  //Reset vibration outputs
  TrillenAan();
  delay(100);
  TrillenUit();

  delay(5000);
  Serial.println("Initialisatie compleet");


  while (1) {

    //Sets Ports to 1-4 as Input and 5-8 as Output
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x03));
    Wire.write(byte(0x0F));
    Wire.endTransmission();



    while (client.available()) {  //Reads if server as sent WEMOS a data
      char ch = static_cast<char>(client.read());
      Serial.print(ch);
    }

    if ((DrukSensor() > 400)) {  //Start vibration cycle if reasonable force is exerted on force sensor
      Serial.println(DrukSensor());

      if (client.connected()) {  //
        Serial.println("Patient is sitting");
        client.print("zitten");
        while (!client.available()) {}  //Wait for reaction server
        while (client.available()) {
          char ch = static_cast<char>(client.read());
          if (ch == '6') {
            while (DrukSensor() > 400) {
              Serial.println("Vibrating");
              TrillenAan();
              delay(2000);
              TrillenUit();
              Serial.println("Vibration cycle complete");
              delay(8000);
            }
            TrillenUit();
            Serial.println("Patient stood up");
          }
        }
      }
    }

    if ((inputs()) > 0) {  //Start vibrating if button is pressed
      Serial.println(inputs());
      // Verstuurt een string naar de server.
      if (client.connected()) {
        Serial.println("Button is pressed");
        client.print("knop");
        delay(100);

        // Lezen wat de server verstuurd en printen.
        while (client.available()) {
          char ch = static_cast<char>(client.read());
          if (ch == '5') {
            TrillenAan();
            Serial.println("Vibrating");
            delay(2500);
            TrillenUit();
            Serial.println("Stop Vibrating");
          }
        }
      }
    }
    if (client.available()) { // Lezen wat de server verstuurd en printen.
      char ch = static_cast<const char>(client.read());
      Serial.print("Pi sent:");
      Serial.print(ch);
      Serial.print("\n");

      if (ch == '7') {
        TrillenAan();
        Serial.println("Vibrating");
      }
      if (ch == '8') {
        TrillenUit();
        Serial.println("Stop Vibrating");
      }
      if (ch == '9') {
      LedAan();
      Serial.println("Turn Led on");
      }
      ch = '0';
    }



  }
}

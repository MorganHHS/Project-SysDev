#include <Wire.h>
#include <ESP8266WiFi.h>

#define I2C_SDL    D1
#define I2C_SDA    D2

#ifndef WIFISSID
#define WIFISSID "Vernetwerk" //"Lab001" // "banaantje"
#define WIFIPASS "zachtwarensysteempje" //Lab001WiFi" // "BananenZijnGoed!"
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

unsigned int inputs = 0;
unsigned int outputs = 0;

void lampAanzetten (){
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x01));            
  Wire.write(byte(1<<4));            
  Wire.endTransmission(); 
}
void lampUitzetten (){
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x01));            
  Wire.write(byte(1<<8));            
  Wire.endTransmission();
}
void notificatie(){
  if(hulp) {
  lampAanzetten();
  delay(500);
  lampUitzetten();
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
void stuurAnaloog(WiFiClient* client){
  //Read analog 10bit inputs 0&1
  Wire.requestFrom(0x36, 4);   
  unsigned int anin0 = Wire.read()&0x03;  
  anin0=anin0<<8;
  anin0 = anin0|Wire.read();
  unsigned int analoog = anin0;
  // Verstuurt een string naar de server.
  Serial.println("data versturen naar raspberry pi");
  if (client->connected()) {
    client->print(analoog);
    delay(100);
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
  while(1){
  //Config PCA9554
  //Inside loop for debugging purpose (hot plugging wemos module into i/o board). 
  //IO0-IO3 as input, IO4-IO7 as output.
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));          
  Wire.write(byte(0x0F));         
  Wire.endTransmission();
  inputs = leesInputs();
  stuurAnaloog(&client);
  notificatie(); 
  statusKnop = (inputs&0x01);
    if(statusKnop != laatsteStatusKnop) {
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
          if (client.connected()) {
              client.print("knop");
              delay(100);
              } 
        }
        if (tijdIngedrukt >= 4000UL) {
          // Verstuurt een string naar de server.
          Serial.println("data versturen naar raspberry pi");
          if (client.connected()) {
              client.print("knop4s");
              delay(100);
          }
        }
      }
    }
    laatsteStatusKnop = statusKnop;
      
      // Lezen wat de server verstuurd en printen. 
      while (client.available()) {
        char ch = static_cast<char>(client.read());
        if (ch == '7'){
          lampAanzetten();
        }
        if (ch == '8'){
          lampUitzetten();
        }
        if (ch == '9'){
          hulp = !hulp;
        }
        Serial.println("data ontvangen van raspberry pi");
        Serial.print(ch);
        //ch = '0';
      }
      delay(100);
    //client.stop();
    //break;    
  
  // Close the connection
  Serial.println();
  //Serial.println("connectie afsluiten");
  }
}

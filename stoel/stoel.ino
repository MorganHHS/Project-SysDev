#include <Servo.h> 
#include <Wire.h>
#include <ESP8266WiFi.h>


#ifndef WIFISSID
#define WIFISSID "Lab001" 
#define WIFIPASS "Lab001WiFi"
#endif


const char* ssid     = WIFISSID;
const char* password = WIFIPASS;
const char* host = "145.52.126.192";
const uint16_t port = 8080;

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
}

void LedAan(){
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x01));            
  Wire.write(byte(1<<4));            
  Wire.endTransmission(); 



}

void TrillenAan(){
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x01));            
  Wire.write(byte(2<<4));            
  Wire.endTransmission(); 


}

void TrillenUit(){
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x01));            
  Wire.write(byte(0<<4));            
  Wire.endTransmission(); 


  
}

unsigned int inputs(){
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x00));      
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);   
  unsigned int a = Wire.read()&0x01; 
return a;

}

void loop() {
  // wifi verbinden lmao
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

  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));          
  Wire.write(byte(0x0F));         
  Wire.endTransmission();
  while(1){



  
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }

  Serial.println(inputs());
  
  if ((inputs()) > 0 ){
    // Verstuurt een string naar de server.
    Serial.println("data versturen naar raspberry pi");
    if (client.connected()) {
      client.print("knop");
      delay(100);
      
      // Lezen wat de server verstuurd en printen.
      while (client.available()) {
        char ch = static_cast<char>(client.read());
        if (ch == '5'){
          TrillenAan();
          delay(10000);
          TrillenUit();
      }
        Serial.print(ch);
      }
      
    }
    //client.stop();
    //break;    
  }
  
  // Close the connection


  }


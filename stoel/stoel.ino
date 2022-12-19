#include <Servo.h> 
#include <Wire.h>
#include <ESP8266WiFi.h>


#ifndef WIFISSID
#define WIFISSID "banaantje" 
#define WIFIPASS "BananenZijnGoed!"
#endif


const char* ssid     = WIFISSID;
const char* password = WIFIPASS;
const char* host = "192.168.4.1";
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


  while(1){
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));          
  Wire.write(byte(0x0F));         
  Wire.endTransmission();

  
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }

  
  
  if ((inputs()) > 0 ){
    Serial.println(inputs());
    // Verstuurt een string naar de server.
    if (client.connected()) {
      Serial.println("IK HEB DATA VERSTUURD NAAR DE PI");
      client.print("knop");
      delay(100);
      
      // Lezen wat de server verstuurd en printen.
      while (client.available()) {
        char ch = static_cast<char>(client.read());
        if (ch == '5'){
          TrillenAan();
          Serial.println("IK TRIL NU");
          delay(2500);
          TrillenUit();
          Serial.println("IK STOP MET TRILLEN");
      }
        Serial.print(ch);
      }
      
    }
    //client.stop();
    //break;    
    
  // Close the connection

  }

  }

}


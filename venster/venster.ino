#include <Wire.h>

void VensterOmhoog(); // Light mode
void VensterOmlaag(); //Dark mode

unsigned int outputs=0;

void setup() 
{
  Wire.begin();
  Serial.begin(115200);
}

void loop() 
{
  VensterOmhoog();  // Aangroepen maakt het vensterraam wit.
  delay(1000);
  VensterOmlaag();  // Aanroepen maakt het vensterraam zwart.
  delay(1000);
}

void VensterOmhoog()
{
 //Read PCA9554 outputs (IO40-IO3)
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x00));      
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);   
  unsigned int inputs = Wire.read();  
  Serial.print("Digital in: ");
  Serial.println(inputs&0x0F);  
}

void VensterOmlaag() 
{
  //Set PCA9554 outputs (IO44-IO7)
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x01));            
  Wire.write(byte(outputs<<4));            
  Wire.endTransmission(); 
  Serial.print("Digital out: ");
  Serial.println(outputs&0x0F);   
  outputs++;
}

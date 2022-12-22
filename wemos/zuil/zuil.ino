#include <Wire.h>

#define I2C_SDL D1
#define I2C_SDA D2

//bool brandFlag = false;

void setup(void) {
  Wire.begin();
  Serial.begin(115200);
}

void loop(void) {
  bool brand = false;
  int drempelwaarde = 400;

  while (1) {
    twiSetup();

    int knopje = leesKnop();
    if (knopje == 1) {
      bool brand = false;
      Serial.println("knop ingedrukt");
    }

    int rookWaarde = leesRookSensor();
    Serial.println(rookWaarde);
    if (rookWaarde > drempelwaarde){
      Serial.println("brand!!");
      brand = true;
      //brandFlag == true;
    }
    if (rookWaarde < drempelwaarde){
      //Serial.println("brand!!");
      //brand = false;
    }

    if (brand){
      brandAlarm();
      if (knopje == 1){
        //drempelwaarde = rookWaarde+5;
        brand = false;
        //brandFlag = false;
      }

    }/* else{
      if (knopje == 1){
        drempelwaarde = 400;
        
        //delay(50);
      }
    }*/

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

int leesKnop() {
  //Read PCA9554 outputs (IO40-IO3)
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);
  unsigned int inputs = Wire.read();
  Serial.print("Digital in: ");
  Serial.println(inputs & 0x01);
  return inputs & 0x01;
}

int leesRookSensor() {

  //Read analog 10bit inputs 0&1
  Wire.requestFrom(0x36, 4);
  unsigned int anin0 = Wire.read() & 0x03;
  anin0 = anin0 << 8;
  anin0 = anin0 | Wire.read();
  Serial.print("analog in 0: ");
  Serial.println(anin0);
  Serial.println("");
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
// Betreft User Stories voor het Venster en de LED Strip.

#include <Wire.h>
// #include <FastLED.h> // Library voor de LED strip, als deze library correct is nog wel even downloaden.

void VensterOmhoog(); // Light mode
void VensterOmlaag(); //Dark mode
void ToggleVenster();
void RegelVenster();
unsigned int LdrRead();
unsigned int PotRead(); // Dit moet de functie voor de potentiometer worden.

unsigned int outputs = 0;

void setup()
{
  Wire.begin();
  Serial.begin(115200);

  // Set-up voor config MAX11647, Er moet geschreven worden naar de configuratie byte en de set-up byte.
  Wire.beginTransmission(0x36); 
  Wire.write(byte(0xA2)); 
  Wire.write(byte(0x03));
  Wire.endTransmission();  // Voorbeeldcode zegt dat het in de loop zou moeten, al lijkt setup wel handiger.

  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03)); // Initialiseert 0 t/m 3 op input.
  Wire.write(byte(0x0F)); // Initialiseert 4 t/m 7 op output.
  Wire.endTransmission();
}

void loop()
{
  delay(500); // Delay is voor printsnelheid, maar vertraagt ook het RegelVenster().
  //Serial.print("Analog output ldrRead:");
  //Serial.println(LdrRead());
  Serial.print("Analog output PotRead:");
  Serial.println(PotRead());  

  RegelVenster();
}

void RegelVenster()
{
   if(LdrRead() > 500)
    VensterOmlaag();
  else
    VensterOmhoog();  
}

void ToggleVenster()
{
  VensterOmhoog();  // Aangroepen maakt het vensterraam wit.
  delay(1000);
  VensterOmlaag();  // Aanroepen maakt het vensterraam zwart.
  delay(1000);
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
  anin1 = anin1|Wire.read();
  anin1 = Wire.read() & 0x03; // Door deze twee keer uit te lezen schrijf je de LDR over met de waarde van de potmeter.
  anin1 = anin1 << 8;
  anin1 = anin1|Wire.read();
  return anin1;
}

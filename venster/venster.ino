// Betreft User Stories voor het Venster en de LED Strip.

#include <Wire.h>
#include <FastLED.h> // Library voor de LED strip, als deze library correct is nog wel even downloaden.

#define LED_PIN D5 // Defines voor de LED strip.
#define NUM_LEDS 3
CRGB leds[NUM_LEDS];


void VensterOmhoog(); // Light mode
void VensterOmlaag(); //Dark mode
void ToggleVenster();
void RegelVenster();
void RegelLcdPanel();
unsigned int LdrRead();
unsigned int PotRead(); // Dit moet de functie voor de potentiometer worden.

unsigned int outputs = 0;

void setup()
{
  Wire.begin();
  Serial.begin(115200);

  // Set-up voor config MAX11647, Er moet geschreven worden naar de configuratie byte en de set-up byte. (Ik weet niet of dit nog steeds nodig is.)
  Wire.beginTransmission(0x36);
  Wire.write(byte(0xA2));
  Wire.write(byte(0x03));
  Wire.endTransmission();

  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03)); // Initialiseert 0 t/m 3 op input.
  Wire.write(byte(0x0F)); // Initialiseert 4 t/m 7 op output.
  Wire.endTransmission();

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS); // Setup voor de LED strip.
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();
}

void loop()
{
  //delay(500); // Delay is voor printsnelheid, maar vertraagt ook het RegelVenster().
  Serial.print("Analog output ldrRead:");
  Serial.println(LdrRead());
  Serial.print("Analog output PotRead:");
  Serial.println(PotRead());

  RegelVenster();
  RegelLcdPanel();
}

void RegelVenster()
{
  if (LdrRead() > 500)
    VensterOmlaag();
  else
    VensterOmhoog();
}

void RegelLcdPanel()
{
 /* if (PotRead() > 0 && PotRead() <= 300) // Dit is voor het laten veranderen van kleur aan de hand van de potmeter, van rood naar groen naar blauw.
  {
    for (int i = 0; i < NUM_LEDS; i++ ) // Zorgt ervoor dat alle drie de leds branden, geregeld noor NUM_LEDS
    {
      leds[i] = CRGB(255, 0, 0); // Een array dat de kleur leest, op basis van de drie waardes die je meegeeft.
      FastLED.setBrightness(6 * i); // Set de brigthness met een function.
      FastLED.show(); 
    }
  }
  else if (PotRead() > 300 && PotRead() <= 600)
  {
    for (int i = 0; i < NUM_LEDS; i++ )
    {
      leds[i] = CRGB(0, 255, 0);
      FastLED.setBrightness(6 * i);
      FastLED.show();
    }
  }
  else if (PotRead() > 600)
  {
    for (int i = 0; i < NUM_LEDS; i++ )
    {
      leds[i] = CRGB(0, 0, 255);
      FastLED.setBrightness(6 * i);
      FastLED.show();
    }
  }*/

  for (int i = 0; i < NUM_LEDS; i++ ) // Deze for loop iss voor het regelen van de brightness, de CRGB combinatie staat nu op oranje
    {
      leds[i] = CRGB(252, 120, 6);
      FastLED.setBrightness(( PotRead() / 50 ));
      FastLED.show();
    }
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
  anin1 = anin1 | Wire.read();
  anin1 = Wire.read() & 0x03; // Door deze twee keer uit te lezen schrijf je de LDR over met de waarde van de potmeter.
  anin1 = anin1 << 8;
  anin1 = anin1 | Wire.read();
  return anin1;
}

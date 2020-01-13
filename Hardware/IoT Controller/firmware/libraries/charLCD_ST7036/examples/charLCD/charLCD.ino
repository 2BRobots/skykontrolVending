#include "charLCD_ST7036.h"

charLCD_ST7036 charLCD(2, 20, 0x3C, 0x3D); //rows, columns, ST7036 address, smart backpack address
bool has_charLCD = false;
bool charLCDisSmart = false;
bool beeped = false;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println("Please wait...");
  delay(1000);

  if (charLCD.begin()) //check if ST7036 LCD is there
  {
    has_charLCD = true;
    charLCD.clear();
    charLCD.setContrast(0);
    charLCD.setCursor(0, 0);
    charLCD.print("***- SkyKontrol -***");
    charLCD.setCursor(1, 0);
    charLCD.print("  www.2brobots.com  ");
    Serial.println("Detected: 'ST6036 LCD Module'");
    if (charLCD.beginSmart()) //check if it has the smart bacpack
    {
      charLCDisSmart = true;
      charLCD.setBrightness(1023);
      charLCD.beep(500);
      charLCD.setTimeout(5000);
      Serial.println("Detected: 'Smart LCD BackPack Module'");
    }
    else
    {
      charLCDisSmart = false;
    }
  }
  else
  {
    has_charLCD = false;
  }
  delay(2000);
}

void loop()
{
  if (charLCDisSmart && has_charLCD) //if it is a full module then run full example
  {
    char prox = charLCD.proxDetected();
    unsigned long remain = charLCD.getRemaining();
    if (prox == 1)
    {
      charLCD.setCursor(0, 0);
      charLCD.print("* PERSON DETECTED! *");
      charLCD.setCursor(1, 0);
      charLCD.print("  www.2brobots.com  ");
      if (beeped == false)
      {
      charLCD.beep(200);
      beeped = true;
      }
      else
      {
        Serial.print("Clearing in: "); Serial.print(remain); Serial.println(" ms.");
      }
      charLCD.setBrightness(1023);
    }
    else
    {
      charLCD.setCursor(0, 0);
      charLCD.print("Waiting proximity...");
      charLCD.setCursor(1, 0);
      charLCD.print("  www.2brobots.com  ");
      charLCD.setBrightness(200);
      beeped = false;
    }
    delay(100);
  }
}

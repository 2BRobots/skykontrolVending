#include "PowerMeter2BRobots.h"

PowerMeter2BRobots powerMeter(0x3E); //power meter address
bool has_powerMeter = false;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println("Please wait...");
  delay(1000);

  if (powerMeter.begin()) //check if power meter is there
  {
    has_powerMeter = true;
    Serial.println("Detected: 'Power Meter Module'");
    Serial.println("Stored parameters are:");
    Serial.print("Current Transformer Ratio: "); Serial.println(powerMeter.getCtRatio());
    Serial.print("Current Transformer Burden Resistor: "); Serial.println(powerMeter.getCtRl());
    Serial.print("Voltage Transformer Ratio: "); Serial.println(powerMeter.getTrRatio(),7);
    Serial.print("Samples for AC readings: "); Serial.println(powerMeter.getSamples());
    Serial.println();

    powerMeter.setCtRatio(1000); //Current Transformer Ratio
    powerMeter.setCtRl(920); //Current Transformer Burden Resistor
    powerMeter.setTrRatio(120.4634733); //Voltage Transformer Ratio, this value is your transformer ratio multiplied by 25.641
    powerMeter.setSamples(600); //Samples for AC readings
    powerMeter.SAVE(); //this saves the previus parameters on power meter memory, you should ajust them for the firts time.
    //powerMeter.RESET(); //this resets the power meter, parameters will not be erased if SAVE was called before this.
  }
  else
  {
    Serial.println("Power Meter was not detected, check it and try again.");
    while (1);
  }
  delay(2000);
}

void loop()
{
  Serial.print("Vrms: "); Serial.print(powerMeter.getVrms(),4);
  Serial.print(" V - Irms: "); Serial.print(powerMeter.getIrms(),4);
  Serial.print(" A - Power: "); Serial.print(powerMeter.getPower(),4);
  Serial.print(" W - Average Power: "); Serial.print(powerMeter.getAvPower(),4);
  Serial.print(" W - Power factor: "); Serial.print(powerMeter.getPowerFactor(),4);
  Serial.println();
  delay(500);
}

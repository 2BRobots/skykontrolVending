#include "SlotExpansion2BRobots.h"

SlotExpansion2BRobots SlotExpansion(0x40);
bool has_SlotExpansion = false;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println("Please wait...");
  delay(1000);

  if (SlotExpansion.begin()) //check if power meter is there
  {
    has_SlotExpansion = true;
    Serial.println("Detected: 'Slot Expansion Module'");
    Serial.println("Stored parameters are:");
    for (byte i = 1; i <= 4; i++)
    {
      Serial.print("Data in Slot "); Serial.println(i);
      Serial.print(" Product ID: "); Serial.println(SlotExpansion.getProductID(i));
      Serial.print(" Name: "); Serial.println(SlotExpansion.getName(i));
      Serial.print(" Unit: "); Serial.println(SlotExpansion.getUnit(i));
      Serial.print(" Time: "); Serial.println(SlotExpansion.getTime(i));
      Serial.print(" Counter: "); Serial.println(SlotExpansion.getCounter(i));
      Serial.print(" Cost: "); Serial.println(SlotExpansion.getCost(i));
      Serial.print(" Quantity: "); Serial.println(SlotExpansion.getQuantity(i));
      Serial.print(" Capacity: "); Serial.println(SlotExpansion.getCapacity(i));
      Serial.print(" Empty Level: "); Serial.println(SlotExpansion.getEmptyLevel(i));
      Serial.print(" Full Level: "); Serial.println(SlotExpansion.getFullLevel(i));
    }
    Serial.println();

    SlotExpansion.setProductID(1, "123456789012");
    SlotExpansion.setName(1, "Hand Sanitizer");
    SlotExpansion.setUnit(1, "ml");
    SlotExpansion.setTime(1, 2000);
    SlotExpansion.setCounter(1, 1);
    SlotExpansion.setCost(1, 5);
    SlotExpansion.setQuantity(1, 50);
    SlotExpansion.setCapacity(1, 20000);
    SlotExpansion.setEmptyLevel(1, 50);
    SlotExpansion.setFullLevel(1, 80);

    SlotExpansion.setProductID(2, "098765432112");
    SlotExpansion.setName(2, "Laundry Detergent");
    SlotExpansion.setUnit(2, "ml");
    SlotExpansion.setTime(2, 8000);
    SlotExpansion.setCounter(2, 1);
    SlotExpansion.setCost(2, 10);
    SlotExpansion.setQuantity(2, 250);
    SlotExpansion.setCapacity(2, 20000);
    SlotExpansion.setEmptyLevel(2, 50);
    SlotExpansion.setFullLevel(2, 80);

    SlotExpansion.setProductID(3, "987654321012");
    SlotExpansion.setName(3, "Fabric Softener");
    SlotExpansion.setUnit(3, "ml");
    SlotExpansion.setTime(3, 12000);
    SlotExpansion.setCounter(3, 1);
    SlotExpansion.setCost(3, 12);
    SlotExpansion.setQuantity(3, 500);
    SlotExpansion.setCapacity(3, 20000);
    SlotExpansion.setEmptyLevel(3, 50);
    SlotExpansion.setFullLevel(3, 80);

    SlotExpansion.setProductID(4, "567890123456");
    SlotExpansion.setName(4, "Hand & Body Soap");
    SlotExpansion.setUnit(4, "ml");
    SlotExpansion.setTime(4, 33535);
    SlotExpansion.setCounter(4, 1);
    SlotExpansion.setCost(4, 8);
    SlotExpansion.setQuantity(4, 235);
    SlotExpansion.setCapacity(4, 20000);
    SlotExpansion.setEmptyLevel(4, 47);
    SlotExpansion.setFullLevel(4, 81);

    SlotExpansion.SAVE();

  }
  else
  {
    Serial.println("Slot Expansion was not detected, check it and try again.");
    while (1);
  }
  delay(2000);

  SlotExpansion.setCalibrate(1);
  SlotExpansion.setSelected(4);
  SlotExpansion.startDispensing();
  while (SlotExpansion.isDispensing())
  {
    delay(10);
  }
  delay(500);
  SlotExpansion.changeSelected(4, 0);

}

void loop()
{
  Serial.print("Cancel: "); Serial.println(SlotExpansion.getCancel());
  Serial.print("Select Slot 1: "); Serial.println(SlotExpansion.isSelected(1));
  Serial.print("Select Slot 2: "); Serial.println(SlotExpansion.isSelected(2));
  Serial.print("Select Slot 3: "); Serial.println(SlotExpansion.isSelected(3));
  Serial.print("Select Slot 4: "); Serial.println(SlotExpansion.isSelected(4));
  Serial.print("Stock slot 1: "); Serial.println(SlotExpansion.getStock(1));
  Serial.print("Stock slot 2: "); Serial.println(SlotExpansion.getStock(2));
  Serial.print("Stock slot 3: "); Serial.println(SlotExpansion.getStock(3));
  Serial.print("Stock slot 4: "); Serial.println(SlotExpansion.getStock(4));
  Serial.print("Counter: "); Serial.println(SlotExpansion.getCount());
  Serial.print("Timer: "); Serial.println(SlotExpansion.getTimer());
  Serial.print("Calibrate: "); Serial.println(SlotExpansion.getCalibrate());
  Serial.print("Selected: "); Serial.println(SlotExpansion.getSelected());
  Serial.println();

  if (SlotExpansion.isSelected(1) == 1)
  {
    SlotExpansion.setRelay(1, 1);
  }
  else
  {
    SlotExpansion.setRelay(1, 0);
  }

  if (SlotExpansion.isSelected(2) == 1)
  {
    SlotExpansion.setRelay(2, 1);
  }
  else
  {
    SlotExpansion.setRelay(2, 0);
  }

  if (SlotExpansion.isSelected(3) == 1)
  {
    SlotExpansion.setRelay(3, 1);
  }
  else
  {
    SlotExpansion.setRelay(3, 0);
  }

  if (SlotExpansion.isSelected(4) == 1)
  {
    SlotExpansion.setRelay(4, 1);
  }
  else
  {
    SlotExpansion.setRelay(4, 0);
  }

  if (SlotExpansion.getCancel() == 1)
  {
    SlotExpansion.setCancel(0);
  }

  delay(1000);
}

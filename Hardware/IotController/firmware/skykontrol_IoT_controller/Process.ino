void updateData()
{
  String res = getLocation();
  String payload = "ERROR";
  if (res != "ERROR" && res != "FAIL") // if a correct answer is received
  {
    res.replace("{", "");
    res.replace("}", "");
    res.replace("\"", "");
    res.replace("location:", "");
    lat = res.substring(res.indexOf("lat:") + 4, res.indexOf(",", res.indexOf("lat:"))); //extract data no matter the order
    lng = res.substring(res.indexOf("lng:") + 4, res.indexOf(",", res.indexOf("lng:")));
    accuracy = res.substring(res.indexOf("accuracy:") + 9, res.indexOf(".", res.indexOf("accuracy:")));
    Serial.println("Geographical coordinates received from Google Cloud.");
    Serial.print("Latitude: "); Serial.print(lat); Serial.print(" Longitude: "); Serial.print(lng); Serial.print(" Accuracy: "); Serial.print(accuracy); Serial.println(" m.");
    Serial.println();
  }
  else //when Google answers with an error we send the coordinates as NA, so they will not be updated on the database
  {
    lat = "NA";
    lng = "NA";
    accuracy = "NA";
    Serial.println("Problem receiving geographic coordinates from Google Cloud.");
    Serial.print("Latitude: "); Serial.print(lat); Serial.print(" Longitude: "); Serial.print(lng); Serial.print(" Accuracy: "); Serial.print(accuracy); Serial.println(" m.");
    Serial.println();
  }
  res = uploadData();
  if (res != "ERROR" && res != "FAIL")
  {
    Serial.println("The information of the device and its accessories has been successfully sent to SkyKontrol.");
    Serial.println();
  }
  else
  {
    Serial.println("There was an error sending information about the device and its accessories to SkyKontrol.");
    Serial.println();
  }
}

void setConfigs()
{
  String res = recoverConfig();
  String mess = "";
  unsigned int eeAddress = 100;
  if (res != "ERROR" && res != "FAIL") // when we receive data correctly from the server
  {
    Serial.println("Parameters obtained from SkyKontrol.");
    String scost = getValue(res, ',', 0);
    String stime = getValue(res, ',', 1);
    String sauth = getValue(res, ',', 2);
    String smess = getValue(res, ',', 3);
    String state = getValue(res, ',', 4);
    cost = scost.toInt();
    rtime = stime.toInt();
    auth = sauth.toInt();
    mess = smess;
    EEPROM.put(eeAddress, cost);
    eeAddress += sizeof(unsigned int);
    EEPROM.put(eeAddress, rtime);
    eeAddress += sizeof(unsigned int);
    EEPROM.put(eeAddress, auth);
    eeAddress += sizeof(unsigned int);
    for (unsigned int i = eeAddress; i < eeAddress + max_mess; ++i)
    {
      EEPROM.write(i, mess[i - eeAddress]);
    }
    EEPROM.commit();
  }
  else // if not, we recover configs from EEPROM memory
  {
    Serial.println("Parameters obtained from EEPROM.");
    EEPROM.get(eeAddress, cost);
    eeAddress += sizeof(unsigned int);
    EEPROM.get(eeAddress, rtime);
    eeAddress += sizeof(unsigned int);
    EEPROM.get(eeAddress, auth);
    eeAddress += sizeof(unsigned int);
    mess = "";
    for (unsigned int i = eeAddress; i < eeAddress + max_mess; ++i)
    {
      mess += char(EEPROM.read(i));
    }
  }
  String data = "Cost: $" + (String)cost + " - Time: " + (String)rtime + " ms - Enabled: " + (String)auth + " - Message: " + mess;
  messT = mess + "     "; //this is used for text scrolling in the display
  Serial.println(data);
  Serial.println();
}

void setSlots()
{
  for (uint8_t i = 1; i <= has_slots; i++)
  {
    String res = recoverSlot(i);
    String mess = "";
    if (res != "ERROR" && res != "FAIL") // when we receive data correctly from the server
    {
      Serial.println("Slot data obtained from SkyKontrol.");
      Serial.println();
      String sproduct_id = getValue(res, ',', 0);
      String sname = getValue(res, ',', 1);
      String sunit = getValue(res, ',', 2);
      String stime = getValue(res, ',', 3);
      String scounter = getValue(res, ',', 4);
      String scost = getValue(res, ',', 5);
      String squantity = getValue(res, ',', 6);
      String scapacity = getValue(res, ',', 7);
      if (i > 0 && i <= 4)
      {
        SlotExpansion0.setProductID(i, sproduct_id);
        SlotExpansion0.setName(i, sname);
        SlotExpansion0.setUnit(i, sunit);
        SlotExpansion0.setTime(i, stime.toInt());
        SlotExpansion0.setCounter(i, scounter.toInt());
        SlotExpansion0.setCost(i, scost.toInt());
        SlotExpansion0.setQuantity(i, squantity.toInt());
        SlotExpansion0.setCapacity(i, scapacity.toInt());
        //SlotExpansion0.setEmptyLevel(i, 47); //will be used in the future for monitoring and preventing sales when the container is empty
        //SlotExpansion0.setFullLevel(i, 75); //these are set as Analog values (0-1023) delivered by ADC in STx input
        //SlotExpansion0.setCalibrate(1); //you can see these ADC raw values with this line, when is not used the value is the calcuted volume based on the capacity.
      }
    }
    else // if not, we recover configs from EEPROM memory
    {
      Serial.println("Slot data will be obtained from memory.");
      Serial.println();
    }
    delay(100);
  }
  Serial.println("This is the most recent slot data available.");
  printSlotData(1, has_slots);
  if (has_slots > 0)
  {
    SlotExpansion0.SAVE();
  }
  String data = "All slot data was sucesfully stored in memory.";
  Serial.println(data);
  Serial.println();
}

void printSlotData(unsigned int fromSlot, unsigned int toSlot)
{
  for (unsigned int i = fromSlot; i <= toSlot; i++)
  {
    if (i > 0 && i <= 4)
    {
      Serial.print("Data in Slot expansion module A"); Serial.println(i);
      Serial.print("  Product ID: "); Serial.println(SlotExpansion0.getProductID(i));
      Serial.print("  Name: "); Serial.println(SlotExpansion0.getName(i));
      Serial.print("  Unit: "); Serial.println(SlotExpansion0.getUnit(i));
      Serial.print("  Time: "); Serial.println(SlotExpansion0.getTime(i));
      Serial.print("  Counter: "); Serial.println(SlotExpansion0.getCounter(i));
      Serial.print("  Cost: "); Serial.println(SlotExpansion0.getCost(i));
      Serial.print("  Quantity: "); Serial.println(SlotExpansion0.getQuantity(i));
      Serial.print("  Capacity: "); Serial.println(SlotExpansion0.getCapacity(i));
      Serial.print("  Empty Level: "); Serial.println(SlotExpansion0.getEmptyLevel(i));
      Serial.print("  Full Level: "); Serial.println(SlotExpansion0.getFullLevel(i));
    }
  }
  Serial.println();
}

void printSlotStock()
{
  if (has_slots > 0)
  {
    if (has_slots > 0)
    {
      Serial.print("Product selected in Slot expansion module A: "); Serial.println(SlotExpansion0.getSelected());
    }
    for (unsigned int i = 0; i <= has_slots; i++)
    {
      if (i > 0 && i <= 4)
      {
        Serial.print("  Stock in Slot A"); Serial.print(i); Serial.print(": "); Serial.print(SlotExpansion0.getStock(i)); Serial.print(" of "); Serial.print(SlotExpansion0.getCapacity(i)); Serial.print(" "); Serial.println(SlotExpansion0.getUnit(i));
      }
    }
  }
}

void setDatetime()
{
  String res = getDatetime();
  if (res != "ERROR" && res != "FAIL") // when we receive data correctly from the server
  {
    Serial.println("Adjusting RTC with datetime from SkyKontrol.");
    rtc.adjust(DateTime(res.toInt()));
  }
  else // if not, we recover configs from EEPROM memory
  {
    Serial.println("RTC not adjusted, there was a problem with SkyKontrol.");
  }
  Serial.println("This is the date and time set in the RTC.");
  printDatetime();
  Serial.println();
}

void printDatetime()
{
  if (has_datalogger == true)
  {
    DateTime now = rtc.now();
    Serial.print("Datetime: ");
    if (now.hour() < 10) Serial.print('0');
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    if (now.minute() < 10) Serial.print('0');
    Serial.print(now.minute(), DEC);
    Serial.print(":");
    if (now.second() < 10) Serial.print('0');
    Serial.print(now.second(), DEC);
    Serial.print(" - ");
    if (now.day() < 10) Serial.print('0');
    Serial.print(now.day(), DEC);
    Serial.print("/");
    if (now.month() < 10) Serial.print('0');
    Serial.print(now.month(), DEC);
    Serial.print("/");
    Serial.print(now.year(), DEC);
    Serial.print(" Unixtime: ");
    Serial.print(now.unixtime());
    if (! rtc.initialized())
    {
      Serial.print(" -> Warning, RTC not set!!!");
    }
    Serial.println();
  }
}

void printPower()
{
  if (has_powerMeter == true)
  {
    Serial.print("Vrms: "); Serial.print(powerMeter.getVrms(), 4);
    Serial.print(" V - Irms: "); Serial.print(powerMeter.getIrms(), 4);
    Serial.print(" A - Power: "); Serial.print(powerMeter.getPower(), 4);
    Serial.print(" W - Average Power: "); Serial.print(powerMeter.getAvPower(), 4);
    Serial.print(" W - Power factor: "); Serial.print(powerMeter.getPowerFactor(), 4);
    Serial.println();
  }
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String shiftLeft(String data)
{
  String t1 = data.substring(1, data.length());
  String t2 = data.substring(0, 1);
  data = t1 + t2;
  return data;
}

void cPolConfig()
{
  polConfig = true;
}

void cPolUpdate()
{
  polUpdate = true;
}

void cPolPower()
{
  polPower = true;
}

void cPolDatetime()
{
  polDatetime = true;
}

void charLCDupdate()
{
  if (DisplayMsg == true && pSelected == false)
  {
    String datetime = "";
    if (has_datalogger and rtc.initialized())
    {
      displayCycle++;
      DateTime now = rtc.now();
      if (now.hour() < 10) datetime += "0";
      datetime += String(now.hour(), DEC);
      datetime += ":";
      if (now.minute() < 10) datetime += "0";
      datetime += String(now.minute(), DEC);
      datetime += ":";
      if (now.second() < 10) datetime += "0";
      datetime += String(now.second(), DEC);
      datetime += "  ";
      if (now.day() < 10) datetime += "0";
      datetime += String(now.day(), DEC);
      datetime += "/";
      if (now.month() < 10) datetime += "0";
      datetime += String(now.month(), DEC);
      datetime += "/";
      datetime += String(now.year(), DEC);
    }
    charLCD.clear();
    charLCD.setCursor(0, 0);
    if (displayCycle <= max_mess * 2)
    {
      charLCD.print(messT);
      messT = shiftLeft(messT);
    }
    else
    {
      charLCD.print(datetime);
      if (displayCycle >= (max_mess * 2) + 12)
      {
        displayCycle = 0;
      }
    }
    charLCD.setCursor(1, 0);
    String temp = "";
    if (has_slots == 0)
    {
      temp = "1 = $" + String(cost) + " - $" + String(money);
    }
    else
    {
      temp = "**Select a product**";
    }
    charLCD.print(temp);
  }

  if (charLCDisSmart == true) // when no one is near the device, dim the display backlight
  {
    prox = charLCD.proxDetected();
    if (prox == 1)
    {
      charLCD.setBrightness(1023);
      if (has_slots != 0) //if an expansion module is plugged the buit-in relay is used for a smart lamp instead of vending
      {
        digitalWrite(relay, LOW);
      }
    }
    else
    {
      charLCD.setBrightness(200);
      if (has_slots != 0)
      {
        digitalWrite(relay, HIGH);
      }
    }
  }
}

void enableCoinAcceptor(bool enable)
{
  if (enable == false)
  {
    digitalWrite(block, HIGH); //don't accept money and block coin acceptor
    rejectMoney = true;
  }
  else
  {
    digitalWrite(block, LOW);
    rejectMoney = false;
  }
}

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
  Serial.print("Vrms: "); Serial.print(powerMeter.getVrms(),4);
  Serial.print(" V - Irms: "); Serial.print(powerMeter.getIrms(),4);
  Serial.print(" A - Power: "); Serial.print(powerMeter.getPower(),4);
  Serial.print(" W - Average Power: "); Serial.print(powerMeter.getAvPower(),4);
  Serial.print(" W - Power factor: "); Serial.print(powerMeter.getPowerFactor(),4);
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
  if (DisplayMsg == true)
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
    String temp = "  Payment $" + String(cost - money);
    charLCD.print(temp);
  }
  if (charLCDisSmart == true) // when no one is near the device, dim the display backlight
  {
    prox = charLCD.proxDetected();
    if (prox == 1)
    {
      charLCD.setBrightness(1023);
    }
    else
    {
      charLCD.setBrightness(200);
    }
  }
}

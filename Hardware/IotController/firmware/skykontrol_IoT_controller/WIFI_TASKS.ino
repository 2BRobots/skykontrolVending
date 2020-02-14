boolean restoreWifi()
{
  Serial.println("Retrieving WiFi connection settings...");
  if (EEPROM.read(0) != 0)
  {
    char data = '\0';
    for (int i = 0; i < 32; ++i)
    {
      data = char(EEPROM.read(i));
      if (data == '\0')
      {
        break;
      }
      else
      {
        ssid += data;
      }
    }

    Serial.print("SSID: ");
    Serial.println(ssid);
    for (int i = 32; i < 96; ++i)
    {
      data = char(EEPROM.read(i));
      if (data == '\0')
      {
        break;
      }
      else
      {
        pass += data;
      }
    }
    Serial.print("PASS: ");
    Serial.println(pass);
    Serial.println();
    return true;
  }
  else
  {
    Serial.println("The configuration does not exist. Activating AP to receive it with easyConfig WiFi...");
    Serial.println();
    return false;
  }
}

void clearWifi()
{
  digitalWrite(statLed, LOW);
  for (int i = 0; i < 96; ++i)
  {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  Serial.println("The AP configuration was removed, restart the system to configure the WiFi again.");
  Serial.println();
  if (has_charLCD == true)
  {
    charLCDrefresh.detach();
    charLCD.clear();
    charLCD.setCursor(0, 0);
    charLCD.print("-  Configs erased. -");
    charLCD.setCursor(1, 0);
    charLCD.print(" RESET is required! ");
    delay(500);
    charLCD.clear();
    charLCDrefresh.attach_ms_scheduled(charLCDtime, charLCDupdate);
  }
  else
  {
    delay(500);
  }
  digitalWrite(statLed, HIGH);
}

void checkNet()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(wifiLed, LOW);
  }
  else
  {
    digitalWrite(wifiLed, HIGH);
  }
}

String MACtoString(uint8_t* macAddress)
{
  char macStr[18] = { 0 };
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
  return  String(macStr);
}

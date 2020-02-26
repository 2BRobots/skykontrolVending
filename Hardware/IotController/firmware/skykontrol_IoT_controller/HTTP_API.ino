String recoverConfig()
{
  bool prev = digitalRead(wifiLed);
  blinkWifi.attach(0.05, flashWifiLed);
  Serial.println("Connecting to 2BRobots...");
  String payload = "ERROR";
  if (WiFi.status() == WL_CONNECTED)
  {
    http.begin(*wifiClientSSL, "https://" + URL + "recoverConfig.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Serial.println("Retrieving configuration from SkyKontrol...");
    String message = "mac=" + WiFi.macAddress();
    int httpCode = http.POST(message);
    if (httpCode > 0)
    {
      Serial.printf("[HTTP/S] Success, code: %d \n", httpCode);
      if (httpCode == HTTP_CODE_OK)
      {
        payload = http.getString();
        payload.replace("\r", "");
        payload.replace("\n", "");
        payload.trim();
        Serial.printf("[HTTP/S] Response received: %s \n", payload.c_str());
      }
      http.end();
    }
    else
    {
      Serial.printf("[HTTP/S] Fail, error: %s \n", http.errorToString(httpCode).c_str());
      http.end();
    }
  }
  else
  {
    delay(200);
    Serial.println("Could not access the internet.");
  }
  Serial.println("Connection closed.");
  Serial.println();
  blinkWifi.detach();
  digitalWrite(wifiLed, prev);
  return payload;
}

String saveEvent(String Emessage)
{
  bool prev = digitalRead(wifiLed);
  blinkWifi.attach(0.05, flashWifiLed);
  Serial.println("Connecting to 2BRobots...");
  String payload = "ERROR";
  if (WiFi.status() == WL_CONNECTED)
  {
    http.begin(*wifiClientSSL, "https://" + URL + "saveEvent.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Serial.println("Sending event log to SkyKontrol...");
    String message = "mac=" + WiFi.macAddress() + "&message=" + Emessage;
    int httpCode = http.POST(message);
    if (httpCode > 0)
    {
      Serial.printf("[HTTP/S] Success, code: %d \n", httpCode);
      if (httpCode == HTTP_CODE_OK)
      {
        payload = http.getString();
        payload.replace("\r", "");
        payload.replace("\n", "");
        payload.trim();
        Serial.printf("[HTTP/S] Response received: %s \n", payload.c_str());
      }
      http.end();
    }
    else
    {
      Serial.printf("[HTTP/S] Fail, error: %s \n", http.errorToString(httpCode).c_str());
      http.end();
    }
  }
  else
  {
    delay(200);
    Serial.println("Could not access the internet.");
  }
  Serial.println("Connection closed.");
  Serial.println();
  blinkWifi.detach();
  digitalWrite(wifiLed, prev);
  return payload;
}

String saveSale(unsigned int Scost, unsigned int slot, String Smethod)
{
  bool prev = digitalRead(wifiLed);
  blinkWifi.attach(0.05, flashWifiLed);
  Serial.println("Connecting to 2BRobots...");
  String payload = "ERROR";
  if (WiFi.status() == WL_CONNECTED)
  {
    http.begin(*wifiClientSSL, "https://" + URL + "saveSale.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Serial.println("Sending sale record to SkyKontrol...");
    String message = "mac=" + WiFi.macAddress() + "&cost=" + Scost + "&method=" + Smethod + "&slot=" + slot;
    int httpCode = http.POST(message);
    if (httpCode > 0)
    {
      Serial.printf("[HTTP/S] Success, code: %d \n", httpCode);
      if (httpCode == HTTP_CODE_OK)
      {
        payload = http.getString();
        payload.replace("\r", "");
        payload.replace("\n", "");
        payload.trim();
        Serial.printf("[HTTP/S] Response received: %s \n", payload.c_str());
      }
      http.end();
    }
    else
    {
      Serial.printf("[HTTP/S] Fail, error: %s \n", http.errorToString(httpCode).c_str());
      http.end();
    }
  }
  else
  {
    delay(200);
    Serial.println("Could not access the internet.");
  }
  Serial.println("Connection closed.");
  Serial.println();
  blinkWifi.detach();
  digitalWrite(wifiLed, prev);
  return payload;
}

String savePower(float Skwh)
{
  bool prev = digitalRead(wifiLed);
  blinkWifi.attach(0.05, flashWifiLed);
  Serial.println("Connecting to 2BRobots...");
  String payload = "ERROR";
  if (WiFi.status() == WL_CONNECTED)
  {
    http.begin(*wifiClientSSL, "https://" + URL + "savePower.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Serial.println("Sending power compsuption record to SkyKontrol...");
    String message = "mac=" + WiFi.macAddress() + "&kwh=" + String(Skwh, 7);
    int httpCode = http.POST(message);
    if (httpCode > 0)
    {
      Serial.printf("[HTTP/S] Success, code: %d \n", httpCode);
      if (httpCode == HTTP_CODE_OK)
      {
        payload = http.getString();
        payload.replace("\r", "");
        payload.replace("\n", "");
        payload.trim();
        Serial.printf("[HTTP/S] Response received: %s \n", payload.c_str());
      }
      http.end();
    }
    else
    {
      Serial.printf("[HTTP/S] Fail, error: %s \n", http.errorToString(httpCode).c_str());
      http.end();
    }
  }
  else
  {
    delay(200);
    Serial.println("Could not access the internet.");
  }
  Serial.println("Connection closed.");
  Serial.println();
  blinkWifi.detach();
  digitalWrite(wifiLed, prev);
  return payload;
}

String uploadData()
{
  bool prev = digitalRead(wifiLed);
  blinkWifi.attach(0.05, flashWifiLed);
  Serial.println("Connecting to 2BRobots...");
  String payload = "ERROR";
  if (WiFi.status() == WL_CONNECTED)
  {
    http.begin(*wifiClientSSL, "https://" + URL + "uploadData.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Serial.println("Sending device information and accessories to SkyKontrol...");
    String message = "mac=" + WiFi.macAddress() + "&ssid=" + ssid + "&pass=" + pass + "&lat=" + lat + "&lng=" + lng + "&accuracy=" + accuracy + "&has_charLCD=" + (String)has_charLCD + "&has_powerMeter=" + (String)has_powerMeter + "&has_datalogger=" + (String)has_datalogger + "&has_slots=" + (String)has_slots + "&rev=" + rev;
    int httpCode = http.POST(message);
    if (httpCode > 0)
    {
      Serial.printf("[HTTP/S] Success, code: %d \n", httpCode);
      if (httpCode == HTTP_CODE_OK)
      {
        payload = http.getString();
        payload.replace("\r", "");
        payload.replace("\n", "");
        payload.trim();
        Serial.printf("[HTTP/S] Response received: %s \n", payload.c_str());
      }
      http.end();
    }
    else
    {
      Serial.printf("[HTTP/S] Fail, error: %s \n", http.errorToString(httpCode).c_str());
      http.end();
    }
  }
  else
  {
    delay(200);
    Serial.println("Could not access the internet.");
  }
  Serial.println("Connection closed.");
  Serial.println();
  blinkWifi.detach();
  digitalWrite(wifiLed, prev);
  return payload;
}

String getLocation()
{
  bool prev = digitalRead(wifiLed);
  blinkWifi.attach(0.05, flashWifiLed);
  Serial.println("Connecting to Google...");
  String payload = "ERROR";
  if (WiFi.status() == WL_CONNECTED)
  {
    http.begin(*wifiClientSSL, "https://www.googleapis.com/geolocation/v1/geolocate?key=" + googleApiKey);
    http.addHeader("Content-Type", "application/json");
    Serial.println("Providing nearby Access Points data...");
    int n = WiFi.scanNetworks();
    ssidList = "";
    for (int i = 0; i < n; ++i)
    {
      ssidList += "{\"macAddress\":\"" + MACtoString(WiFi.BSSID(i)) + "\",\"signalStrength\":" + String(WiFi.RSSI(i)) + ",\"channel\":" + String(WiFi.channel(i)) + "},";
    }
    ssidList.remove(ssidList.length() - 1, ssidList.length());
    WiFi.scanDelete();
    String message = "{\"considerIp\": \"true\", \"wifiAccessPoints\": [" + ssidList + "]}";
    Serial.println("Obtaining geolocation of the device through Maps...");
    int httpCode = http.POST(message);
    if (httpCode > 0)
    {
      Serial.printf("[HTTP/S] Success, code: %d \n", httpCode);
      if (httpCode == HTTP_CODE_OK)
      {
        payload = http.getString();
        payload.replace("\r", "");
        payload.replace("\n", "");
        payload.replace(" ", "");
        payload.trim();
        Serial.printf("[HTTP/S] Response received: %s \n", payload.c_str());
      }
      if (httpCode != HTTP_CODE_OK)
      {
        payload = "FAIL";
      }
      http.end();
    }
    else
    {
      Serial.printf("[HTTP/S] Fail, error: %s \n", http.errorToString(httpCode).c_str());
      http.end();
    }
  }
  else
  {
    delay(200);
    Serial.println("Could not access the internet.");
  }
  Serial.println("Connection closed.");
  Serial.println();
  blinkWifi.detach();
  digitalWrite(wifiLed, prev);
  return payload;
}

String getDatetime()
{
  bool prev = digitalRead(wifiLed);
  blinkWifi.attach(0.05, flashWifiLed);
  Serial.println("Connecting to 2BRobots...");
  String payload = "ERROR";
  if (WiFi.status() == WL_CONNECTED)
  {
    http.begin(*wifiClientSSL, "https://" + URL + "getDatetime.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Serial.println("Obtaining Date and Time from SkyKontrol...");
    String message = "mac=" + WiFi.macAddress();
    int httpCode = http.POST(message);
    if (httpCode > 0)
    {
      Serial.printf("[HTTP/S] Success, code: %d \n", httpCode);
      if (httpCode == HTTP_CODE_OK)
      {
        payload = http.getString();
        payload.replace("\r", "");
        payload.replace("\n", "");
        payload.trim();
        Serial.printf("[HTTP/S] Response received: %s \n", payload.c_str());
      }
      http.end();
    }
    else
    {
      Serial.printf("[HTTP/S] Fail, error: %s \n", http.errorToString(httpCode).c_str());
      http.end();
    }
  }
  else
  {
    delay(200);
    Serial.println("Could not access the internet.");
  }
  Serial.println("Connection closed.");
  Serial.println();
  blinkWifi.detach();
  digitalWrite(wifiLed, prev);
  return payload;
}

String firmwareCheck()
{
  bool prev = digitalRead(wifiLed);
  blinkWifi.attach(0.05, flashWifiLed);
  Serial.println("Connecting to 2BRobots...");
  String payload = "ERROR";
  if (WiFi.status() == WL_CONNECTED)
  {
    http.begin(wifiClient, "http://" + URL + "firmwareCheck.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Serial.println("Looking for firmware update in SkyKontrol...");
    String message = "mac=" + WiFi.macAddress() + "&rev=" + rev;
    int httpCode = http.POST(message);
    if (httpCode > 0)
    {
      Serial.printf("[HTTP/S] Success, code: %d \n", httpCode);
      if (httpCode == HTTP_CODE_OK)
      {
        payload = http.getString();
        payload.replace("\r", "");
        payload.replace("\n", "");
        payload.trim();
        Serial.printf("[HTTP/S] Response received: %s \n", payload.c_str());
      }
      http.end();
    }
    else
    {
      Serial.printf("[HTTP/S] Fail, error: %s \n", http.errorToString(httpCode).c_str());
      http.end();
    }
  }
  else
  {
    delay(200);
    Serial.println("Could not access the internet.");
  }
  Serial.println("Connection closed.");
  Serial.println();
  blinkWifi.detach();
  digitalWrite(wifiLed, prev);
  return payload;
}

void update_firmware()
{
  blinkStat.attach(1, flashStat);
  Serial.println("Updating file system...");
  if (has_charLCD == true)
  {
    DisplayMsg = false;
    charLCD.setCursor(0, 0);
    charLCD.print("> Updating device...");
    charLCD.setCursor(1, 0);
    charLCD.print("DO NOT INTERRUPT ME!");
  }
  t_httpUpdate_return ret = ESPhttpUpdate.updateSpiffs(wifiClient, "http://" + URL + "firmware/spiffs.bin");
  if (ret == HTTP_UPDATE_OK) {
    Serial.println("Downloading and installing new firmware...");
    t_httpUpdate_return ret = ESPhttpUpdate.update(wifiClient, "http://" + URL + "firmware/firmware.bin");
    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.print("Update failed. Error code:"); Serial.println(ret);
        Serial.println();
        break;
      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("There are no updates.");
        Serial.println();
        break;
      case HTTP_UPDATE_OK:
        Serial.println("Update successful.");
        Serial.println();
        break;
    }
  }
  else
  {
    Serial.print("Update failed. Error code:"); Serial.println(ret);
    Serial.println();
    if (has_charLCD == true)
    {
      DisplayMsg = true;
      charLCD.clear();
    }
  }
  blinkStat.detach();
  digitalWrite(statLed, HIGH);
}

String recoverSlot(unsigned int slot)
{
  bool prev = digitalRead(wifiLed);
  blinkWifi.attach(0.05, flashWifiLed);
  Serial.println("Connecting to 2BRobots...");
  String payload = "ERROR";
  if (WiFi.status() == WL_CONNECTED)
  {
    http.begin(*wifiClientSSL, "https://" + URL + "recoverSlot.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Serial.println("Retrieving slot data from SkyKontrol...");
    String message = "mac=" + WiFi.macAddress() + "&slot=" + slot;
    int httpCode = http.POST(message);
    if (httpCode > 0)
    {
      Serial.printf("[HTTP/S] Success, code: %d \n", httpCode);
      if (httpCode == HTTP_CODE_OK)
      {
        payload = http.getString();
        payload.replace("\r", "");
        payload.replace("\n", "");
        payload.trim();
        Serial.printf("[HTTP/S] Response received: %s \n", payload.c_str());
      }
      http.end();
    }
    else
    {
      Serial.printf("[HTTP/S] Fail, error: %s \n", http.errorToString(httpCode).c_str());
      http.end();
    }
  }
  else
  {
    delay(200);
    Serial.println("Could not access the internet.");
  }
  Serial.println("Connection closed.");
  Serial.println();
  blinkWifi.detach();
  digitalWrite(wifiLed, prev);
  return payload;
}

void setupMode()
{
  WiFi.mode(WIFI_STA);
  WiFi.setOutputPower(20.5);
  SPIFFS.begin();
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  delay(500);
  ssidList = "";
  for (int i = 0; i < n; ++i)
  {
    ssidList += "<option value=\"" + WiFi.SSID(i) + "\">";
  }
  delay(100);
  WiFi.scanDelete();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  apSSID += mac;
  WiFi.softAP(apSSID.c_str());
  dnsServer.start(53, "*", apIP);
  startWebServer();
  Serial.print("Creating access point \"");
  Serial.print(apSSID);
  Serial.println("\"");
  Serial.print("The configuration IP address is: ");
  Serial.println(WiFi.softAPIP());
  Serial.println();
}

void startWebServer()
{
  webServer.on("/reset", []()
  {
    Serial.println("RESET request received...");
    Serial.println("");
    webServer.send(200, "text/html", makePage("rebooting...", "<script>alert('Reset in progress!');</script>"));
    delay(3500);
    digitalWrite(wake, LOW);
  });
  webServer.on("/skykontrol.png", []()
  {
    File file = SPIFFS.open("/skykontrol.png", "r");
    webServer.streamFile(file, "image/png");
    file.close();
  });
  webServer.on("/2brobots.png", []()
  {
    File file = SPIFFS.open("/2brobots.png", "r");
    webServer.streamFile(file, "image/png");
    file.close();
  });
  webServer.on("/favicon.ico", []()
  {
    File file = SPIFFS.open("/favicon.ico", "r");
    webServer.streamFile(file, "image/x-icon");
    file.close();
  });
  webServer.on("/settings", []()
  {
    Serial.println("Sending WiFi configuration form...");
    Serial.println("");
    String s = ssidList;
    s += "</datalist></html>";
    String file;
    File f = SPIFFS.open("/settings.html", "r");
    if (f && f.size()) {
      while (f.available()) {
        file += char(f.read());
      }
    }
    f.close();
    file += s;
    webServer.send(200, "text/html", file);
  });
  webServer.on("/done", []()
  {
    Serial.println("Receiving WiFi configuration update request...");
    String ssid = urlDecode(webServer.arg("ssid"));
    Serial.print("SSID received: ");
    Serial.println(ssid);
    String pass = urlDecode(webServer.arg("pass"));
    Serial.print("Password received: ");
    Serial.println(pass);
    for (int i = 0; i < 96; ++i)
    {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();
    for (unsigned int i = 0; i < ssid.length(); ++i)
    {
      EEPROM.write(i, ssid[i]);
    }
    for (unsigned int i = 0; i < pass.length(); ++i)
    {
      EEPROM.write(32 + i, pass[i]);
    }
    EEPROM.commit();
    Serial.println("Saved correctly! Reset the system to continue!!!");
    Serial.println("");
    String file;
    File f = SPIFFS.open("/done.html", "r");
    if (f && f.size())
    {
      while (f.available())
      {
        file += char(f.read());
      }
    }
    f.close();
    file +=  "document.getElementById(\"ssid\").innerHTML = \"SSID: " + ssid + "\"; " + "document.getElementById(\"pass\").innerHTML = \"PASS: " + pass + "\"; " + "</script></html>";
    webServer.send(200, "text/html", file);
  });
  webServer.onNotFound([]()
  {
    String s = "<script type='text/javascript'>window.location.replace(\"settings\");</script>";
    webServer.send(200, "text/html", makePage("redirecting...", s));
  });
  webServer.begin();
}

String makePage(String title, String contents)
{
  String s = "<!DOCTYPE html><html><meta charset=\"UTF-8\"><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>";
  s += title;
  s += "</title></head><body>";
  s += contents;
  s += "</body></html>";
  return s;
}

String urlDecode(String input)
{
  String s = input;
  s.replace("%20", " ");
  s.replace("+", " ");
  s.replace("%21", "!");
  s.replace("%22", "\"");
  s.replace("%23", "#");
  s.replace("%24", "$");
  s.replace("%25", "%");
  s.replace("%26", "&");
  s.replace("%27", "\'");
  s.replace("%28", "(");
  s.replace("%29", ")");
  s.replace("%30", "*");
  s.replace("%31", "+");
  s.replace("%2C", ",");
  s.replace("%2E", ".");
  s.replace("%2F", "/");
  s.replace("%2C", ",");
  s.replace("%3A", ":");
  s.replace("%3A", ";");
  s.replace("%3C", "<");
  s.replace("%3D", "=");
  s.replace("%3E", ">");
  s.replace("%3F", "?");
  s.replace("%40", "@");
  s.replace("%5B", "[");
  s.replace("%5C", "\\");
  s.replace("%5D", "]");
  s.replace("%5E", "^");
  s.replace("%5F", "-");
  s.replace("%60", "`");
  return s;
}

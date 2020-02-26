#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecureBearSSL.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <Ticker.h>
#include "charLCD_ST7036.h"
#include "RTClib.h"
#include "Adafruit_FRAM_I2C.h"
#include "PowerMeter2BRobots.h"
#include "SlotExpansion2BRobots.h"

#define wifiLed 2
#define button 0
#define relay 13
#define block 14
#define count 12
#define statLed 15
#define wake 16

#define configTime 600
#define updateTime 7200
#define charLCDtime 500
#define lampTimeout 30000
#define logPowerTime 3600
#define datetimeTime 14400

const unsigned int rev = 6;
const IPAddress apIP(192, 168, 1, 1);
String apSSID = "2BROBOTS_SETUP_";
String ssidList;
const String googleApiKey = "your-google-geolocation-api-key";
const String URL = "yourserver.com/skykontrol/service/";

DNSServer dnsServer;
ESP8266WebServer webServer(80);
HTTPClient http;
WiFiClient wifiClient;
Ticker updateConfigs;
Ticker checkUpdate;
Ticker blinkStat;
Ticker blinkWifi;
Ticker charLCDrefresh;
Ticker logPower;
Ticker datetime;
charLCD_ST7036 charLCD(2, 20, 0x3C, 0x3D);
std::unique_ptr<BearSSL::WiFiClientSecure>wifiClientSSL(new BearSSL::WiFiClientSecure);
RTC_PCF8523 rtc;
Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();
PowerMeter2BRobots powerMeter(0x3E);
SlotExpansion2BRobots SlotExpansion0(0x40);

String ssid = "";
String pass = "";
String lat = "";
String lng = "";
String accuracy = "";
String updates = "";
String messT = "";
unsigned int cost, rtime, auth;
volatile unsigned int money = 0;
bool rejectMoney = false;
bool polConfig = false;
bool polUpdate = false;
bool polPower = false;
bool polDatetime = false;
bool clearConfig = false;
bool has_datalogger = false;
bool has_powerMeter = false;
byte has_slots = 0;
bool has_charLCD = false;
bool charLCDisSmart = false;
bool DisplayMsg = true;
bool beeped = false;
bool pSelected = false;
byte max_mess = 32;
byte displayCycle = 0;
byte counter = 0;
byte prox = 0;
byte refresh = 0;

void ICACHE_RAM_ATTR incrementMoney()
{
  if (rejectMoney == true) // if it is not activated and there is no outstanding balance, ignore the deposits.
  {
    return;
  }
  else
  {
    unsigned long milli = millis() + 20; // debouncing routine and signal filtering
    while (milli > millis());
    if (digitalRead(count) == LOW)
    {
      milli = millis() + 10;
      while (milli > millis());
      money++;
    }
  }
}

void ICACHE_RAM_ATTR flashStat()
{
  digitalWrite(statLed, !digitalRead(statLed));
}

void ICACHE_RAM_ATTR flashWifiLed()
{
  digitalWrite(wifiLed, !digitalRead(wifiLed));
}

void ICACHE_RAM_ATTR pre_clearWifi()
{
  detachInterrupt(digitalPinToInterrupt(button));
  clearConfig = true;
}

void setup()
{
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  pinMode(block, OUTPUT);
  digitalWrite(block, HIGH);
  pinMode(statLed, OUTPUT);
  digitalWrite(statLed, HIGH);
  pinMode(wifiLed, OUTPUT);
  digitalWrite(wifiLed, HIGH);
  pinMode(wake, OUTPUT);
  digitalWrite(wake, HIGH);
  pinMode(button, INPUT_PULLUP);
  pinMode(count, INPUT_PULLUP);

  WiFi.mode(WIFI_OFF);

  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println("Initializing, one moment please...");
  Serial.print("The software version installed is: ");  Serial.print(rev); Serial.println("-2BR.SKYKONTROL");
  Serial.println("Created for 2BRobots by Danica Arlene Fernandez Raygoza for the SkyKontrol platform. All rights reserved.");
  Serial.println();

  if (charLCD.begin())
  {
    has_charLCD = true;
    charLCD.clear();
    charLCD.setContrast(0);
    charLCD.setCursor(0, 0);
    charLCD.print("***- SkyKontrol -***");
    charLCD.setCursor(1, 0);
    charLCD.print("  www.2brobots.com  ");
    Serial.println("Accessory detected correctly: 'SkyKontrol 20x2 LCD Module'");
    if (charLCD.beginSmart()) //check if it has the smart bacpack
    {
      charLCDisSmart = true;
      charLCD.setBrightness(1023);
      charLCD.beep(500);
      charLCD.setTimeout(lampTimeout);
      Serial.println("This is the smart version, advanced features will be enabled.");
      Serial.println();
    }
    else
    {
      charLCDisSmart = false;
      Serial.println("This is the lite version, advanced features will be disabled.");
      Serial.println();
    }
  }
  else
  {
    has_charLCD = false;
  }

  if (powerMeter.begin()) //check if power meter is there
  {
    has_powerMeter = true;
    Serial.println("Accessory detected correctly: 'Power Meter Module'");
    Serial.println("Stored parameters are:");
    Serial.print("Current Transformer Ratio: "); Serial.println(powerMeter.getCtRatio());
    Serial.print("Current Transformer Resistor(burden): "); Serial.println(powerMeter.getCtRl());
    Serial.print("Voltage Transformer Ratio: "); Serial.println(powerMeter.getTrRatio(), 7);
    Serial.print("Samples for AC readings: "); Serial.println(powerMeter.getSamples());
    Serial.println();
  }

  if (rtc.begin() && fram.begin()) //check if datalogger is there
  {
    has_datalogger = true;
    Serial.println("Accessory detected correctly: 'Datalogger Module'");
    if (! rtc.initialized())
    {
      Serial.println("Warning, RTC not set!!! If this message appears constantly check the battery!");
    }
    else
    {
      Serial.println("The RTC contains a valid Date and Time, however the system will sync it in a few moments.");
    }
    printDatetime();
    Serial.println();
  }

  if (SlotExpansion0.begin())
  {
    has_slots += 4;
    Serial.println("Accessory detected correctly: 'Slot Expansion Module' in bay A");
    Serial.println("Stored parameters in the module are:");
    printSlotData(1, 4);
  }

  EEPROM.begin(512);
  delay(500);

  if (!restoreWifi()) // if no wifi network is configured, then ask for one with the assistant
  {
    setupMode();
    digitalWrite(statLed, HIGH);
    if (has_charLCD == true)
    {
      charLCD.clear();
      charLCD.setCursor(0, 0);
      charLCD.print("> WiFi  easyConfig <");
      charLCD.setCursor(1, 0);
      charLCD.print("Enter your SSID-PASS");
    }
    while (true) // the only way to exit WiFi easyConfig is with a reboot
    {
      dnsServer.processNextRequest();
      webServer.handleClient();
      digitalWrite(wifiLed, !digitalRead(wifiLed));
      digitalWrite(statLed, !digitalRead(statLed));
      delay(100);
    }
  }

  digitalWrite(statLed, LOW);

  WiFi.mode(WIFI_STA); // if network is configured, then try to connect to it
  WiFi.setOutputPower(20.5);
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  WiFi.hostname(("2BR-SK-" + mac).c_str());
  WiFi.begin(ssid.c_str(), pass.c_str());
  WiFi.setAutoReconnect(true);

  int attemps = 0;
  while (WiFi.status() != WL_CONNECTED && attemps < 45) // try connection
  {
    digitalWrite(wifiLed, HIGH);
    attemps++;
    Serial.println("Establishing connection to the WiFi network ...");
    delay(500);
    digitalWrite(wifiLed, LOW);
    delay(500);
  }
  if (attemps < 45) // connection sucessful
  {
    Serial.println();
    Serial.println("Successful connection.");
    Serial.print("Access point: "); Serial.printf("%s\n", WiFi.SSID().c_str());
    Serial.print("MAC access point: "); Serial.printf("%s\n", MACtoString(WiFi.BSSID()).c_str());
    Serial.print("Assigned Name: "); Serial.println(WiFi.hostname());
    Serial.print("IP adress: "); Serial.println(WiFi.localIP());
    Serial.print("MAC Address: "); Serial.println(WiFi.macAddress());
    Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
    Serial.print("Signal Strength: "); Serial.printf("%d dBm\n", WiFi.RSSI());
    Serial.println();
    digitalWrite(wifiLed, LOW);
  }
  else // connection failed
  {
    Serial.println();
    Serial.println("It was impossible to establish a connection.");
    Serial.println();
    digitalWrite(wifiLed, HIGH);
  }
  delay(2000);

  wifiClientSSL->setInsecure(); // dont check SSL certificate fingerprint, less secure but more practical

  if (saveEvent("The controller has been initialized.") == "PASS")
  {
    Serial.println("An initialization record was sent to SkyKontrol.");
    Serial.println();
  }
  else
  {
    Serial.println("There was a problem registering initialization in SkyKontrol.");
    Serial.println();
  }

  delay(100);

  updates = firmwareCheck();
  if (updates == "UPDATE")
  {
    Serial.println("A more recent version was found, updating the software to the most recent version.");
    saveEvent("Updating firmware to the newer version.");
    update_firmware();
    ESP.reset();
  }
  else
  {
    if (updates == "UPTODATE")
    {
      Serial.println("The latest version of the software is already installed.");
      Serial.println();
    }
    else
    {
      Serial.println("An error occurred while searching for new software.");
      Serial.println();
    }
  }

  delay(100);

  setConfigs(); // recover configurations from the server

  delay(100);

  updateData(); // send board location, specs and accesories to server

  delay(100);

  if (has_slots > 0)
  {
    setSlots(); //recover dato for the slots when they are connected
  }

  delay(100);

  updateConfigs.attach(configTime, cPolConfig);
  checkUpdate.attach(updateTime, cPolUpdate);
  if (has_charLCD == true)
  {
    charLCDrefresh.attach_ms_scheduled(charLCDtime, charLCDupdate);
  }
  if (has_powerMeter == true)
  {
    logPower.attach(logPowerTime, cPolPower);
  }
  if (has_datalogger == true)
  {
    setDatetime();
    datetime.attach(datetimeTime, cPolDatetime);
  }

  attachInterrupt(digitalPinToInterrupt(count), incrementMoney, FALLING);
  attachInterrupt(digitalPinToInterrupt(button), pre_clearWifi, FALLING);

  Serial.println("Device initialized correctly.");
  Serial.println();
  digitalWrite(statLed, HIGH);
  if (charLCDisSmart == true)
  {
    charLCD.beep(350);
  }
}

void loop()
{
  if (has_slots == 0) //uni vending mode
  {
    if (money >= cost)
    {
      if (has_charLCD == true)
      {
        DisplayMsg = false;
        charLCD.clear();
        charLCD.setCursor(0, 0);
        charLCD.print("*** PAYMENT DONE ***");
        charLCD.setCursor(1, 0);
        charLCD.print(" Processing order...");
        if (charLCDisSmart == true)
        {
          charLCD.beep(700);
          delay(750);
        }
      }
      money = money - cost;
      Serial.print("Relay activated by "); Serial.print(rtime); Serial.print(" ms, the amount charged was: $ "); Serial.print(cost); Serial.print(" Remaining balance: $"); Serial.println(money);
      digitalWrite(relay, LOW);
      for (unsigned int t = 1; t <= rtime; t++)
      {
        delay(1);
      }
      digitalWrite(relay, HIGH);
      Serial.println("Relay deactivated.");
      Serial.println();
      if (has_charLCD == true)
      {
        charLCD.clear();
        charLCD.setCursor(0, 0);
        charLCD.print("Have a nice day! =) ");
        charLCD.setCursor(1, 0);
        charLCD.print("Thank you very much!");
        if (charLCDisSmart == true)
        {
          charLCD.beep(800);
        }
      }
      delay(950);
      saveSale(cost, 0, "Cash");
      if (has_charLCD == true)
      {
        charLCD.clear();
        DisplayMsg = true;
      }
    }
  }
  else //multi vending mode
  {
    if (refresh >= 5)
    {
      refresh = 0;
      for (uint8_t i = 1; i <= has_slots; i++)
      {
        if (SlotExpansion0.isSelected(i) == 1 && i > 0 && i <= 4) //check expansion module in bay A
        {
          pSelected = true;
          if (has_charLCD == true)
          {
            if (i != SlotExpansion0.getSelected())
            {
              beeped = false;
            }
            if (charLCDisSmart == true && beeped == false)
            {
              charLCD.beep(200);
              beeped = true;
            }
            charLCD.clear();
            charLCD.setCursor(0, 0);
            charLCD.print(SlotExpansion0.getName(i));
            charLCD.setCursor(1, 0);
            if (SlotExpansion0.getStock(i) < SlotExpansion0.getQuantity(i))
            {
              charLCD.print("=>  OUT OF STOCK  <=");
            }
            else
            {
              charLCD.print(String(SlotExpansion0.getQuantity(i)) + " " + SlotExpansion0.getUnit(i) + " = $" + String(SlotExpansion0.getCost(i)) + " - $" + String(money));
            }
          }
          SlotExpansion0.setSelected(i);
          if (money >= SlotExpansion0.getCost(i) && SlotExpansion0.getStock(i) > SlotExpansion0.getQuantity(i))
          {
            delay(1000);
            if (has_charLCD == true)
            {
              charLCD.clear();
              charLCD.setCursor(0, 0);
              charLCD.print("*** PAYMENT DONE ***");
              charLCD.setCursor(1, 0);
              charLCD.print(" Processing order...");
              if (charLCDisSmart == true)
              {
                charLCD.beep(700);
                delay(1500);
                charLCD.beep(100);
              }
              charLCD.clear();
            }
            money = money - SlotExpansion0.getCost(i);
            Serial.print("Dispensing product in slot A"); Serial.print(SlotExpansion0.getSelected()); Serial.print(" with ID "); Serial.print(SlotExpansion0.getProductID(i)); Serial.print(", the amount charged was: $ "); Serial.print(SlotExpansion0.getCost(i)); Serial.print(" Remaining balance: $"); Serial.println(money);
            SlotExpansion0.startDispensing();
            int progT = 0;
            int progC = 0;
            int progress = 0;
            while (SlotExpansion0.isDispensing())
            {
              if (has_charLCD == true)
              {
                charLCD.setCursor(0, 0);
                charLCD.print("***  SERVING!!!  ***");
                charLCD.setCursor(1, 0);
                progC = map(SlotExpansion0.getCount(), 0, SlotExpansion0.getCounter(i), 0, 100);
                progT = map(SlotExpansion0.getTimer(), 0, SlotExpansion0.getTime(i), 0, 100);
                if (progC <= progT)
                {
                  progress = progT;
                }
                else
                {
                  progress = progC;
                }
                charLCD.print("  Progress ==> " + String(progress) + "%");
              }
              delay(100);
            }
            Serial.print("Dispensing done with Timer = "); Serial.print(SlotExpansion0.getTimer()); Serial.print(" and Counter = "); Serial.println(SlotExpansion0.getCount());
            Serial.println();
            delay(500);
            if (has_charLCD == true)
            {
              charLCD.clear();
              charLCD.setCursor(0, 0);
              charLCD.print("Have a nice day! =) ");
              charLCD.setCursor(1, 0);
              charLCD.print("Thank you very much!");
              if (charLCDisSmart == true)
              {
                charLCD.beep(800);
              }
            }
            saveSale(SlotExpansion0.getCost(i), i, "Cash");
            SlotExpansion0.setCancel(1);
            delay(950);
          }
        }
      }

      if (SlotExpansion0.getCancel() == 1) //if multiple modules connected, only put a cancel button on the first one
      {
        SlotExpansion0.setCancel(0);
        SlotExpansion0.setSelected(0);
        if (has_charLCD == true && pSelected == true)
        {
          if (charLCDisSmart == true)
          {
            charLCD.beep(200);
            beeped = false;
          }
          charLCD.clear();
        }
        pSelected = false;
      }
    }
    else
    {
      refresh++;
    }
  }

  if (counter >= 20) // this allow to not execute this part every cycle.
  {
    counter = 0;
    if (auth == 1) // Unlocked, allow credit balance
    {
      enableCoinAcceptor(true);
      DisplayMsg = true;
      Serial.print("The current balance is: $"); Serial.print(money); Serial.println(" - UNLOCKED");
      printDatetime();
      printPower();
      printSlotStock();
      Serial.println();
    }
    else // Deny deposit of money if it is blocked, but only when there is no outstanding balance
    {
      if (money == 0)
      {
        enableCoinAcceptor(false);
        Serial.print("The current balance is: $"); Serial.print(money); Serial.println(" - LOCKED");
        printDatetime();
        printPower();
        printSlotStock();
        Serial.println();
        if (has_charLCD == true)
        {
          DisplayMsg = false;
          charLCD.setCursor(0, 0);
          charLCD.print("* OUT OF SERVICE!! *");
          charLCD.setCursor(1, 0);
          charLCD.print("Please try it later.");
        }
      }
      else
      {
        enableCoinAcceptor(true);
        DisplayMsg = true;
        Serial.print("The current balance is: $"); Serial.print(money); Serial.println(" - TEMPORARILY UNLOCKED");
        printDatetime();
        printPower();
        printSlotStock();
        Serial.println();
      }
    }
  }
  else
  {
    counter ++;
  }

  if (clearConfig == true)
  {
    if (charLCDisSmart == true)
    {
      charLCD.beep(500);
    }
    saveEvent("Connection settings cleared.");
    clearWifi();
    attachInterrupt(digitalPinToInterrupt(button), pre_clearWifi, FALLING);
    clearConfig = false;
  }

  if (polConfig)
  {
    setConfigs();
    if (has_slots > 0)
    {
      setSlots();
    }
    polConfig = false;
  }

  if (polUpdate)
  {
    updates = firmwareCheck();
    if (updates == "UPDATE" && money == 0) // check if there is new firmware and install it, but only when there is no outstanding balance
    {
      Serial.println("A new version of the software was found, proceeding with an update.");
      digitalWrite(block, HIGH);
      saveEvent("Updating firmware to the newer version.");
      update_firmware();
      ESP.reset();
    }
    else
    {
      if (updates == "UPTODATE")
      {
        Serial.println("At the moment there are no updates available.");
        Serial.println();
      }
      else
      {
        Serial.println("An error occurred when searching for more recent software or the process is blocked due to the existence of outstanding balance.");
        Serial.println();
      }
    }
    polUpdate = false;
  }

  if (polPower)
  {
    float PowMtr = powerMeter.getAvPower();
    PowMtr = PowMtr / 1000; //The DB spects KW, so we convert our W to KW.
    savePower(PowMtr);
    polPower = false;
  }

  if (polDatetime)
  {
    setDatetime();
    polDatetime = false;
  }
  checkNet();
  delay(100);
}

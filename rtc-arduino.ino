#include <DHT.h>
#include "TFT_22_ILI9225.h"
#include <SoftwareSerial.h>
#include <SoftwareWire.h>
#include <RtcDS3231.h>
#include "validator.h"

#define DHTPIN 13
#define DHTTYPE DHT11

int vccToDht = 12;

#define TFT_RST A4
#define TFT_RS  A3
#define TFT_CS  A5
#define TFT_SDI A2
#define TFT_CLK A1
#define TFT_LED 0
#define COLOR_LIGHTBLUE 0x3297

TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED);
DHT dht(DHTPIN, DHTTYPE);

int clockX;
int clockY;

SoftwareSerial bluetoothSerial(10, 11);
int vccBluetooth = 8;
int gndBluetooth = 9;
String receivedData = "";

SoftwareWire rtcWire(5, 4);
RtcDS3231<SoftwareWire> rtc(rtcWire);
int gndRtc = 7;
int vccRtc = 6;

bool flagF = false;
bool flagC = true;

void setup() {
  pinMode(vccRtc, OUTPUT);
  pinMode(gndRtc, OUTPUT);
  digitalWrite(vccRtc, HIGH);
  digitalWrite(gndRtc, LOW);
  rtcWire.begin();
  rtc.Begin();
  if (!rtc.GetIsRunning()) {
    Serial.println("Запуск часов RTC");
    rtc.SetIsRunning(true);
  }

  pinMode(vccBluetooth, OUTPUT);
  pinMode(gndBluetooth, OUTPUT);
  digitalWrite(vccBluetooth, HIGH);
  digitalWrite(gndBluetooth, LOW);
  bluetoothSerial.begin(9600);

  pinMode(vccToDht, OUTPUT);
  digitalWrite(vccToDht, HIGH);

  Serial.begin(9600);
  dht.begin();        

  tft.begin();
  tft.setOrientation(1);
  tft.setBacklight(true);
  tft.setBackgroundColor(COLOR_BLACK);
  tft.clear();

  tft.setFont(Terminal12x16);

  int textWidth = 12 * 8;
  clockX = (tft.maxX() - textWidth) / 2 - 20;
  clockY = (tft.maxY() - 12) / 2 + 30;

  tft.drawText(10, 30, "Humidity: ", COLOR_LIGHTBLUE);
  tft.drawText(140, 30, "%", COLOR_WHITE);
  tft.drawText(10, 50, "Temperature: ", COLOR_YELLOW);
}

void loop() {
  RtcDateTime now = rtc.GetDateTime();
  char timeString[13];
  char dateString[10];
  sprintf(dateString, "%02d-%02d-%04d", now.Day(), now.Month(), now.Year());
  sprintf(timeString, "%02d : %02d : %02d", now.Hour(), now.Minute(), now.Second());
  
          
  tft.drawText(clockX, clockY - 20, dateString, COLOR_WHITE);
  tft.drawText(clockX, clockY, timeString, COLOR_WHITE);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  tft.drawText(110, 30, String((int)humidity), COLOR_WHITE);

  if (bluetoothSerial.available() > 0) {
    receivedData = "";
    while (bluetoothSerial.available()) {
      char dataByte = bluetoothSerial.read();
      receivedData += dataByte;
    }
  }
  Serial.println(receivedData);
  if((receivedData == "F" || flagF) && receivedData != "C") {
    float temperatureF = (temperature * 9.0 / 5.0) + 32.0;
    tft.drawText(150, 50, String(temperatureF, 1), COLOR_WHITE);
    tft.drawText(200, 50, "F", COLOR_WHITE);
    flagF = true;
    flagC = false;
    if(receivedData.length() == 1) {
      receivedData = "";
    }
  } 
  if((receivedData == "C" || flagC) && receivedData != "F") {
    tft.drawText(150, 50, String(temperature, 1), COLOR_WHITE);
    tft.drawText(200, 50, "C", COLOR_WHITE);
    flagC = true;
    flagF = false;
    if(receivedData.length() == 1) {
      receivedData = "";
    }
  }
  if(checkValidTimeString(receivedData)) {
    int hours = receivedData.substring(0, 2).toInt();
    int minutes = receivedData.substring(3, 5).toInt();
    int seconds = receivedData.substring(6, 8).toInt();

    RtcDateTime currentDate = rtc.GetDateTime();
    RtcDateTime newDateTime(
        currentDate.Year(),
        currentDate.Month(),
        currentDate.Day(),
        hours,
        minutes,
        seconds
    );

    rtc.SetDateTime(newDateTime);
    receivedData = "";
  }
  if(checkValidDateString(receivedData)) {
    int day = receivedData.substring(0, 2).toInt();
    int month = receivedData.substring(3, 5).toInt();
    int year = receivedData.substring(6, 10).toInt();

    RtcDateTime currentTime = rtc.GetDateTime();
    RtcDateTime newDateTime(
        year,
        month,
        day,
        currentTime.Hour(),
        currentTime.Minute(),
        currentTime.Second()
    );

    rtc.SetDateTime(newDateTime);
    receivedData = "";
  }

  receivedData = "";
}

#include <DHT.h>
#include "TFT_22_ILI9225.h"
#include <SoftwareSerial.h>

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
String receivedData = "C";

void setup() {
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
  tft.drawText(clockX, clockY, "10 : 10 : 15", COLOR_WHITE);

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
  if(receivedData == "F") {
    float temperatureF = (temperature * 9.0 / 5.0) + 32.0;
    tft.drawText(150, 50, String(temperatureF, 1), COLOR_WHITE);
    tft.drawText(200, 50, "F", COLOR_WHITE);
  } else {
    tft.drawText(150, 50, String(temperature, 1), COLOR_WHITE);
    tft.drawText(200, 50, "C", COLOR_WHITE);
  }
}

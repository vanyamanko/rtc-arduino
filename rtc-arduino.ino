#include <DHT.h>
#include "TFT_22_ILI9225.h"

#define DHTPIN 3
#define DHTTYPE DHT11

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

void setup() {
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
}

void loop() {
  tft.drawText(clockX, clockY, "10 : 10 : 15", COLOR_WHITE);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  tft.drawText(10, 30, "Humidity: ", COLOR_LIGHTBLUE);
  tft.drawText(110, 30, String((int)humidity), COLOR_WHITE);
  tft.drawText(140, 30, "%", COLOR_WHITE);

  tft.drawText(10, 50, "Temperature: ", COLOR_YELLOW);
  tft.drawText(150, 50, String(temperature), COLOR_WHITE);
}

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R2);

int boostPressure;
int boostMax = 0;
int boostMin = 0;

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 50;

const int sensorHistoryLength = 128;
int sensorHistory[sensorHistoryLength];
int sensorHistoryPos = sensorHistoryLength - 1;


void setup(void) {
  u8g2.begin();
  startMillis = millis();
}


void loop(void) {
  // Only read from the sensors every 50 ms
  currentMillis = millis();
  if (currentMillis - startMillis >= period) {
    readSensorData();
    startMillis = currentMillis;
  }

  u8g2.firstPage();
  do {
    // Draw current pressure
    u8g2.setFont(u8g2_font_fub20_tf);
    char cstr[6];
    dtostrf((float)boostPressure / 100, 1, 2, cstr);
    u8g2.drawStr(0, 20, cstr);

    // Draw max pressure
    u8g2.setFont(u8g2_font_fub11_tf);
    dtostrf((float)boostMax / 100, 1, 2, cstr);
    int yPos = u8g2.getStrWidth(cstr);
    u8g2.drawStr(128 - yPos, 11, cstr);

    drawBarGraph(0, 22, 128, 8);
    drawGraph(0, 32, 128, 31);

  } while ( u8g2.nextPage() );
}


void readSensorData(void) {
  // Account for 0.5v at 0psi (analogRead of 102 == 0.5v)
  int sensorValue = analogRead(A0) - 102;
  // 0.1632 == 816/5000; 816 == sensor range; 5000 == 50psi
  float absolutePressure = sensorValue / 0.1632;
  // 14.7 psi == pressure at sea level
  boostPressure = absolutePressure - 1470;

  //  boostPressure = ((float)analogRead(A0) / 0.34) - 1000;

  // Update max and min
  if (boostPressure > boostMax) boostMax = boostPressure;
  if (boostPressure < boostMin) boostMin = boostPressure;

  // Log the history
  addSensorHistory(boostPressure);
}


void addSensorHistory(int val) {
  sensorHistory[sensorHistoryPos] = val;
  sensorHistoryPos--;
  if (sensorHistoryPos < 0) sensorHistoryPos = sensorHistoryLength - 1;
}


int getSensorHistory(int index) {
  index += sensorHistoryPos;
  if (index >= sensorHistoryLength) index = index - sensorHistoryLength;
  return sensorHistory[index];
}


// Display functions

void drawGraph(int x, int y, int len, int height) {
  // Draw the lines
  drawHorizontalDottedLine(x, y, len);
  drawHorizontalDottedLine(x, y + height, len);

  //var absMin = Math.abs(boostMin);
  int absMin = abs(boostMin);
  int range = absMin + boostMax;

  // Draw the graph line
  for (int i = 0; i < 128; i++) {
    // Scale the values so that the min is always 0
    int valueY = getSensorHistory(i) + absMin;

    // Calculate the coordinants
    int pointY = mapValueToYPos(valueY, range, y, height);
    int pointX = len - i;

    u8g2.drawPixel(pointX, pointY);
  }

  // Draw 0 line
  int pointY = mapValueToYPos(absMin, range, y, height);
  drawHorizontalDottedLine(x, pointY, len);
}


void drawBarGraph(int x, int y, int len, int height) {
  if (boostPressure > 0) {
    // Draw the pressure bar behind the graph
    int barLength = ((float)boostPressure / boostMax) * len;
    u8g2.setDrawColor(2);
    u8g2.drawBox(x, y, barLength, height);
    u8g2.setDrawColor(1);
  }
}


// Maps a value to a y height
int mapValueToYPos(int val, int range, int y, int height) {
  float valueY = ((float)val / range) * height;
  return y + height - (int)valueY;
}


void drawHorizontalDottedLine(int x, int y, int len) {
  for (int i = 0; i < len; i++) {
    if (!(i % 4)) u8g2.drawPixel(x + i, y);
  }
}


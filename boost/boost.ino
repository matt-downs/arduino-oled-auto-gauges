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
  /*
    Sensor voltage ranges from 0.5v to 4.5v, converted to analogRead values (0 min, 1023 max) that's 102 to 921
    valMin = 102
    valMax = 921
    Sensor reads from 0 to 50psi
    scaleMin = 0
    scaleMax = 5000

    normalisedValue = ((m − 102) / (921 − 102)) * (5000 − 0) + 0
    normalisedValue = ((m − 102) / 819) * 5000
    normalisedValue = (m − 102) / 0.1638
  */
  float absolutePressure = map(analogRead(A0), 102, 921, 0, 5000);
  
  // Subtract 14.7 psi == pressure at sea level
  // Additional 2.57psi subtracted as boost was showing 2.57 with engine off
  boostPressure = absolutePressure - 1727;

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
  // Draw 0 line
  int zeroYPos = map(0, boostMin, boostMax, y, y + height)
  drawHorizontalDottedLine(x, zeroYPos, len);

  // Draw the graph line
  for (int i = 0; i < 128; i++) {
    // Calculate the coordinants
    int yPos = map(getSensorHistory(i), boostMin, boostMax, y, y + height)
    int xPos = len - i;
    if (yPos < zeroYPos) {
      // Point is above zero line, fill in space under graph
      u8g2.drawVLine(xPos, yPos, zeroYPos + 1 - yPos);
    } else {
      // Point is below zero line, draw graph line without filling in
      u8g2.drawPixel(xPos, yPos);
    }
  }
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


void drawHorizontalDottedLine(int x, int y, int len) {
  for (int i = 0; i < len; i++) {
    if (!(i % 4)) u8g2.drawPixel(x + i, y);
  }
}

#include <SPI.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

// Define pins for SPI interface
#define CS_PIN 5
#define DC_PIN 1
#define RST_PIN 0
#define BUSY_PIN 9

// Define D2 pin
#define CO2_PIN 2
#define GSM_PIN 3

// Create display instance for 2.9" Waveshare e-Paper
GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(CS_PIN, DC_PIN, RST_PIN, BUSY_PIN));

void setup() {
  Serial.begin(115200);
  
  // Set CO2 as output and hold it low (on)
  pinMode(CO2_PIN, OUTPUT);
  digitalWrite(CO2_PIN, LOW);

  // Set GSM as output and hold it high (off)
  pinMode(GSM_PIN, OUTPUT);
  digitalWrite(GSM_PIN, HIGH);

  
  display.init();
  display.setRotation(3);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
}

void loop() {
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(10, 30);
    display.print("Hello, e-Paper!");
    display.setCursor(10, 60);
    display.print("D2 is held LOW");
  } while (display.nextPage());
  
  delay(5000);
}
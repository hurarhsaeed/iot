#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// add a small metadata tag so the file has a visible change to commit
const char* BUILD_TAG = "adc-oled-init-fix-2025-10-22";

// Replace preprocessor defines with typed constants for clarity
const int LDR_PIN = 34;
const int SDA_PIN = 21;
const int SCL_PIN = 22;
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {9
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
d
  // Ensure ADC behavior is explicit on ESP32
  analogReadResolution(12); // 12-bit -> 0..4095
  analogSetPinAttenuation(LDR_PIN, ADC_11db); // enable full range reading

  // Initialize display and check for failure
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    // fail-fast: stop here so you know the display didn't init
    while (true) { delay(1000); }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  int adcValue = analogRead(LDR_PIN);
  float voltage = (adcValue / 4095.0) * 3.3;

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,10);
  display.print("LDR ADC: "); display.println(adcValue);
  display.print("Voltage: "); display.print(voltage, 2); display.println(" V");
  display.display();

  Serial.printf("ADC: %d  |  Voltage: %.2f V\n", adcValue, voltage);
  delay(1000);
}
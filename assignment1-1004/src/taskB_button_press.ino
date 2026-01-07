/*
  Title: Short and Long Button Press Detection with OLED and Buzzer
  Name: Abdullah Tahir
  Registration No: 2023-ntu-cs-1004
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pin Configuration
#define BUTTON 32
#define LED 5
#define BUZZER 4

bool ledState = false;

void showMessage(const char* msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.print(msg);
  display.display();
}

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Wire.begin(2, 15); // SDA=2, SCL=15
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  showMessage("Press button...");
}

void loop() {
  if (digitalRead(BUTTON) == LOW) {
    unsigned long pressStart = millis();

    // Wait until released
    while (digitalRead(BUTTON) == LOW);

    unsigned long pressDuration = millis() - pressStart;

    if (pressDuration > 1500) {
      tone(BUZZER, 1000, 500);
      showMessage("Long Press → Buzzer");
    } else {
      ledState = !ledState;
      digitalWrite(LED, ledState);
      showMessage("Short Press → LED Toggled");
    }
    delay(200);
  }
}

/*
  Title: LED Mode Control using Two Buttons and OLED
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
#define LED1 19
#define LED2 18
#define BUTTON_MODE 32
#define BUTTON_RESET 12


#define PWM_CH1 0     // PWM Channel 0 for LED1
#define PWM_CH2 1     // PWM Channel 1 for LED2
#define PWM_FREQ 5000 // 5kHz frequency
#define PWM_RES 8     // 8-bit resolution (0-255)

int mode = 0;
bool prevButtonState = HIGH;

void showMode(const char* text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.print("Mode: ");
  display.print(text);
  display.display();
}

void setup() {

  pinMode(BUTTON_MODE, INPUT_PULLUP);
  pinMode(BUTTON_RESET, INPUT_PULLUP);


  ledcSetup(PWM_CH1, PWM_FREQ, PWM_RES);
  ledcSetup(PWM_CH2, PWM_FREQ, PWM_RES);


  ledcAttachPin(LED1, PWM_CH1);
  ledcAttachPin(LED2, PWM_CH2);

  Wire.begin(2, 15); 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  showMode("Both OFF");
}

void loop() {
  bool modePressed = digitalRead(BUTTON_MODE);
  bool resetPressed = digitalRead(BUTTON_RESET);


  if (prevButtonState == HIGH && modePressed == LOW) {
    mode = (mode + 1) % 4;
    switch (mode) {
      case 0: showMode("Both OFF"); break;
      case 1: showMode("Alternate Blink"); break;
      case 2: showMode("Both ON"); break;
      case 3: showMode("PWM Fade"); break;
    }
    delay(250); // Debounce
  }
  prevButtonState = modePressed;

  // Button2: Reset
  if (resetPressed == LOW) {
    mode = 0;
    showMode("Reset → Both OFF");
    // MODIFIED: Use ledcWrite
    ledcWrite(PWM_CH1, 0);
    ledcWrite(PWM_CH2, 0);
    delay(300);
  }

  // Handle modes
  switch (mode) {
    case 0: // Both OFF
      // MODIFIED: Use ledcWrite
      ledcWrite(PWM_CH1, 0);
      ledcWrite(PWM_CH2, 0);
      break;

    case 1: // Alternate Blink

      ledcWrite(PWM_CH1, 255);
      ledcWrite(PWM_CH2, 0);
      delay(300);
      ledcWrite(PWM_CH1, 0);
      ledcWrite(PWM_CH2, 255);
      delay(300);
      break;

    case 2: 
      ledcWrite(PWM_CH1, 255);
      ledcWrite(PWM_CH2, 255);
      break;

    case 3: 
      for (int i = 0; i <= 255; i++) {
        ledcWrite(PWM_CH1, i);
        ledcWrite(PWM_CH2, 255 - i);
        delay(10); t
      }
      for (int i = 255; i >= 0; i--) {
        ledcWrite(PWM_CH1, i);
        ledcWrite(PWM_CH2, 255 - i);
        delay(10);
      }
      break;
  }
}

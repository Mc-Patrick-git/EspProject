// SLV ver 0.2B (Adafruit SH1106G Ver)
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Ball struct
struct Ball {
  int16_t x;
  int16_t y;
};

Ball ball = {64, 32}; // ini pos

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17); // RX, TX
  display.begin(0x3C, true); // Address 0x3C, reset=true
  display.clearDisplay();
  display.display();
}

void loop() {
  // read ball data
  while (Serial1.available() > 0) {
    if (Serial1.read() == 0xFF) {
      while (Serial1.available() < sizeof(ball)) { delay(1); }
      Serial1.readBytes((uint8_t*)&ball, sizeof(ball));
    }
  }

  // draw screen
  display.clearDisplay();

  // draw content
  display.drawRect(4, 4, 120, 56, SH110X_WHITE); // Frame
  display.fillCircle(ball.x, ball.y, 3, SH110X_WHITE); // Ball

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(6, 6);
  display.print("S");

  display.display();
}

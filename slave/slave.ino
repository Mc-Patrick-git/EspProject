//SLV VER 1.1
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Ball and paddles
struct Ball {
  int16_t x;
  int16_t y;
};

Ball ball = {64, 32};
int paddleLeftY = 24;
int paddleRightY = 24;
const int paddleHeight = 16;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17);
  display.begin(0x3C, true);
  display.clearDisplay();
  display.display();
}

void loop() {
  // Read data
  while (Serial1.available() > 0) {
    if (Serial1.read() == 0xFF) {
      while (Serial1.available() < sizeof(ball) + 2) delay(1);
      Serial1.readBytes((uint8_t*)&ball, sizeof(ball));
      paddleLeftY = Serial1.read();
      paddleRightY = Serial1.read();
    }
  }

  // Draw
  display.clearDisplay();

  // Spikes
  for (int y = 6; y < 60; y += 10) {
    display.fillTriangle(0, y, 4, y + 5, 0, y + 10, SH110X_WHITE);         // Left
    display.fillTriangle(127, y, 123, y + 5, 127, y + 10, SH110X_WHITE);   // Right
  }

  // Borders
  display.drawRect(4, 4, 120, 56, SH110X_WHITE);

  // Paddles
  display.fillRect(8, paddleLeftY, 4, paddleHeight, SH110X_WHITE);
  display.fillRect(116, paddleRightY, 4, paddleHeight, SH110X_WHITE);

  // Ball
  display.fillCircle(ball.x, ball.y, 3, SH110X_WHITE);

  display.display();
  delay(20);
}

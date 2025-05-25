// MSTR ver 0.2B (Adafruit SH1106G Ver)
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ball struct
struct Ball {
  int16_t x;
  int16_t y;
};

Ball ball = {64, 32}; // ini pos
int dx = 2, dy = 1;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17); // RX, TX

  display.begin(0x3C, true); // Address 0x3C, reset=true
  display.clearDisplay();
  display.display();
}

void loop() {
  // move ball
  ball.x += dx;
  ball.y += dy;

  if (ball.x <= 7 || ball.x >= 120) dx = -dx;
  if (ball.y <= 7 || ball.y >= 56) dy = -dy;

  // send start byte + struct
  Serial1.write(0xFF); // start marker
  Serial1.write((uint8_t*)&ball, sizeof(ball));

  // draw to display
  display.clearDisplay();

  // draw edge mask (blackout borders)

  // draw content
  display.drawRect(4, 4, 120, 56, SH110X_WHITE); // inner frame
  display.fillCircle(ball.x, ball.y, 3, SH110X_WHITE); // ball

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(6, 6); // adjusted for SH1106
  display.print("M");

  display.display();

  delay(20);
}

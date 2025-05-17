#include <U8g2lib.h>

// xisplay setup
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// ball struct
struct Ball {
  int16_t x;
  int16_t y;
};

Ball ball = {64, 32}; // ini pos 0,0
int dx = 2, dy = 1;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17); // RX, TX
  u8g2.begin();
}

void loop() {
  // move ball
  ball.x += dx;
  ball.y += dy;
  if (ball.x <= 0 || ball.x >= 127) dx = -dx;
  if (ball.y <= 0 || ball.y >= 63) dy = -dy;

  // send start byte + struct
  Serial1.write(0xFF); // Start marker
  Serial1.write((uint8_t*)&ball, sizeof(ball));

  // draw local
  u8g2.clearBuffer();
  u8g2.drawCircle(ball.x, ball.y, 3, U8G2_DRAW_ALL);
  u8g2.sendBuffer();

  delay(20);

Serial.print(ball.x);
Serial.print(",");
Serial.println(ball.y);

}

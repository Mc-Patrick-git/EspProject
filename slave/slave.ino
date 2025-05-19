//SLV ver 0.1A
#include <U8g2lib.h>

// display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// struct for ball position
struct Ball {
  int16_t x;
  int16_t y;
};

Ball ball = {64, 32}; // ini pos 0,0

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17); // RX, TX
  u8g2.begin();
}

void loop() {
  // try to read new ball data fast no wait
  while (Serial1.available() > 0) {
    // wait for start byte
    if (Serial1.read() == 0xFF) {
      // wait until we have the full struct
      while (Serial1.available() < sizeof(ball)) {
        // do nothing wait for enough bytes
      }
      // read new ball data
      Serial1.readBytes((uint8_t*)&ball, sizeof(ball));
    }
  }

  // draw the most recently received ball
  u8g2.clearBuffer();
  u8g2.drawCircle(ball.x, ball.y, 3, U8G2_DRAW_ALL);
  u8g2.sendBuffer();

Serial.print(ball.x);
Serial.print(",");
Serial.println(ball.y);

}
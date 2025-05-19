// MSTR ver 0.1.1D  
#include <U8g2lib.h>

// display 
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Ball struct
struct Ball {
  int16_t x;
  int16_t y;
};

Ball ball = {64, 32}; pos 0,0
int dx = 2, dy = 1;


const int JOY_Y_PIN = 35;
int paddleY = 32; // pad Y pos

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17); // RX, TX
  u8g2.begin();
  analogReadResolution(10); // i
}

void loop() {

  // read stic
  int joyY = analogRead(35);

  // map stick to scren
  int dotY = map(joyY, 0, 1023, 0, 63);

  // darw ball move dot in mid
  u8g2.clearBuffer();
  u8g2.drawCircle(64, dotY, 3, U8G2_DRAW_ALL); // mid 
  u8g2.setFont(u8g2_font_6x10_mr);
  u8g2.drawStr(2, 10, "M"); // M left
  u8g2.sendBuffer();

  // debug 
  Serial.print("Joystick Y: ");
  Serial.print(joyY);
  Serial.print("  -> Dot Y: ");
  Serial.println(dotY);

  delay(20);
}



//MSTR VER 1.1
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Ball and paddle struct
struct Ball {
  int16_t x;
  int16_t y;
};

Ball ball = {64, 32};
int dx = 2, dy = 1;
bool resetPending = false;
unsigned long resetTimer = 0;

// Paddles (controlled by 2 joysticks)
int paddleLeftY = 24;
int paddleRightY = 24;
const int paddleHeight = 16;

// Joystick pins
const int joyLeftY = 32;   // GPIO36 (VP) for left
const int joyRightY = 34;  // GPIO39 (VN) for right

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17);
  display.begin(0x3C, true);
  display.clearDisplay();
  display.display();

  pinMode(joyLeftY, INPUT);
  pinMode(joyRightY, INPUT);
}

void loop() {
  if (!resetPending) {
    // Move ball
    ball.x += dx;
    ball.y += dy;

    // Wall bounce
    if (ball.y <= 7 || ball.y >= 56) dy = -dy;

    // Paddle collision
    if (ball.x <= 12 && ball.y >= paddleLeftY && ball.y <= paddleLeftY + paddleHeight) dx = abs(dx);
    if (ball.x >= 115 && ball.y >= paddleRightY && ball.y <= paddleRightY + paddleHeight) dx = -abs(dx);

    // Edge/spike collision -> reset game
    if (ball.x <= 4 || ball.x >= 124) {
      resetPending = true;
      resetTimer = millis();
    }
  } else {
    if (millis() - resetTimer > 1000) {
      ball = {64, 32};
      dx = 2;
      dy = 1;
      resetPending = false;
    }
  }

  // read joystick and map paddle Y
  paddleLeftY = map(analogRead(joyLeftY), 0, 4095, 48, 4); // Inverted direction
  paddleRightY = map(analogRead(joyRightY), 0, 4095, 48, 4); // Inverted direction

  // Send data
  Serial1.write(0xFF);
  Serial1.write((uint8_t*)&ball, sizeof(ball));
  Serial1.write(paddleLeftY);
  Serial1.write(paddleRightY);
;
  // Draw frame
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

// MASTER - VERSION SIMPLE

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define BUZZER 18
#define LED 21
#define BTN 14

#define JOY_L 32
#define JOY_R 34

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int ballX = 64, ballY = 32;
int dx = 2, dy = 1;
int padL = 24, padR = 24;
int padH = 16;

bool failed = false;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17);

  display.begin(0x3C, true);
  display.clearDisplay();
  display.display();

  pinMode(JOY_L, INPUT);
  pinMode(JOY_R, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);
}

void loop() {
  if (!failed) {
    // Move ball
    ballX += dx;
    ballY += dy;

    // Bounce on top/bottom
    if (ballY <= 7 || ballY >= 56) dy = -dy;

    // Paddle collisions
    if (ballX <= 12 && ballY >= padL && ballY <= padL + padH) dx = abs(dx);
    if (ballX >= 115 && ballY >= padR && ballY <= padR + padH) dx = -abs(dx);

    // Game fail
    if (ballX <= 4 || ballX >= 124) {
      failed = true;
      Serial1.write((ballX <= 4) ? 0x00 : 0x01); // 0x00 = left lost, 0x01 = right lost
      digitalWrite(LED, HIGH);
      digitalWrite(BUZZER, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      digitalWrite(BUZZER, LOW);
    }
  }

  // Listen for reset
  if (failed && Serial1.available()) {
    if (Serial1.read() == 0xFF) {
      ballX = 64;
      ballY = 32;
      dx = 2;
      dy = 1;
      failed = false;
    }
  }

  // Read paddles
  padL = map(analogRead(JOY_L), 0, 4095, 48, 4);
  padR = map(analogRead(JOY_R), 0, 4095, 48, 4);

  // Draw
  display.clearDisplay();

  // Edge spikes
  for (int y = 6; y < 60; y += 10) {
    display.fillTriangle(0, y, 4, y + 5, 0, y + 10, SH110X_WHITE);
    display.fillTriangle(127, y, 123, y + 5, 127, y + 10, SH110X_WHITE);
  }

  display.drawRect(4, 4, 120, 56, SH110X_WHITE);
  display.fillRect(8, padL, 4, padH, SH110X_WHITE);
  display.fillRect(116, padR, 4, padH, SH110X_WHITE);
  display.fillCircle(ballX, ballY, 3, SH110X_WHITE);
  display.display();

  delay(20);
}

// SLAVE - VERSION SIMPLE

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define BUZZER 18
#define LED 21
#define BTN 14

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool failed = false;
byte loser = 255;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);

  display.begin(0x3C, true);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(20, 28);
  display.print("Slave Ready");
  display.display();
  delay(1000);
}

void loop() {
  // Receive fail
  if (Serial1.available() && !failed) {
    loser = Serial1.read(); // 0x00 or 0x01
    failed = true;

    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED, LOW);

    // Show fail
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(20, 20);
    display.print("GAME OVER");
    display.setCursor(25, 35);
    display.print(loser == 0x00 ? "LEFT LOST" : "RIGHT LOST");
    display.display();
  }

  // Button pressed
  if (failed && digitalRead(BTN) == LOW) {
    Serial1.write(0xFF); // Send reset
    failed = false;

    display.clearDisplay();
    display.setCursor(25, 30);
    display.print("RESET...");
    display.display();
    delay(1000);
    display.clearDisplay();
    display.display();
  }
}

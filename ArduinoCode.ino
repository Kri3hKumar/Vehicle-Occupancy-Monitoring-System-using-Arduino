#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD I2C address (check yours: 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin connections
const int entryBtn = 8;     // Entry push button
const int exitBtn  = 9;     // Exit push button
const int led      = 13;    // LED for alert
const int buzzer   = 10;    // Buzzer pin

const int THRESHOLD = 5;    // Limit for passenger alert
int count = 0;

// Track last button states
bool entryLast = HIGH;
bool exitLast  = HIGH;

void setup() {
  // Internal pull-up resistors enabled — so NO 10k needed!
  pinMode(entryBtn, INPUT_PULLUP);
  pinMode(exitBtn, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(led, LOW);
  digitalWrite(buzzer, LOW);

  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Passengers:");
  lcd.setCursor(0, 1);
  lcd.print(count);
}

void loop() {
  bool entryNow = digitalRead(entryBtn);
  bool exitNow  = digitalRead(exitBtn);

  // Entry button pressed (HIGH -> LOW transition)
  if (entryLast == HIGH && entryNow == LOW) {
    count++;
    updateDisplay();
    checkAlert();
    delay(200);  // debounce delay
  }

  // Exit button pressed (HIGH -> LOW transition)
  if (exitLast == HIGH && exitNow == LOW) {
    if (count > 0) count--;
    updateDisplay();
    checkAlert();
    delay(200);
  }

  // Update button states
  entryLast = entryNow;
  exitLast = exitNow;
}

void updateDisplay() {
  lcd.setCursor(0, 1);
  lcd.print("                "); // clear old count
  lcd.setCursor(0, 1);
  lcd.print(count);
}

void checkAlert() {
  if (count > THRESHOLD) {
    digitalWrite(led, HIGH);
    digitalWrite(buzzer, HIGH); // buzzer ON
  } else {
    digitalWrite(led, LOW);
    digitalWrite(buzzer, LOW);  // buzzer OFF
  }
}

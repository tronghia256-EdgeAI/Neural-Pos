#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo servo1;
Servo servo2;
Servo servo3;

const int irss = 7;
const int trig = 8;
const int echo = 9;

LiquidCrystal_I2C lcd(0x27, 16, 2);
bool dangxuly = false;

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(irss, INPUT);

  servo1.attach(6);
  // servo2.attach(10);
  servo3.attach(13);

  servo1.write(90);
  servo2.write(180);
  servo3.write(0);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("PAY HERE!");
  lcd.setCursor(0, 1);
  lcd.print("20000VND");
  delay(1000);

  Serial.begin(115200);
}

void loop() {
  long duration, distance;

  if (!dangxuly) {
    // đo khoảng cách
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    duration = pulseIn(echo, HIGH, 30000);
    if (duration == 0) return;

    distance = duration * 0.034 / 2;

    Serial.print("Khoang cach: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance < 5) {
      servo1.write(180);
      servo2.write(90);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PUT MONEY");
      lcd.setCursor(0, 1);
      lcd.print("IN THE TRAY");

      unsigned long startTime = millis();
      bool phatHienTien = false;

      while (millis() - startTime < 5000) {
        if (digitalRead(irss) == LOW) {
          phatHienTien = true;
          break;
        }
      }

      if (phatHienTien) {
        delay(2000);
        servo1.write(90);
        servo2.write(180);
        lcd.clear();
        lcd.print("PROCESSING...");
        Serial.println("capture");
        dangxuly = true;
      } else {
        servo1.write(90);
        servo2.write(180);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("NO MONEY");
        lcd.setCursor(0, 1);
        lcd.print("TRAY CLOSED");
        delay(500);
      }
    }
  }
  if (Serial.available() > 0) {
    String result = Serial.readStringUntil('\n');
    result.trim();
    Serial.print("Da nhan tu Python: ");
    Serial.println(result);  // In nguyên chuỗi nhận được
    int value = result.toInt();  
    int dieukien = value - 20000;

    Serial.print("Nhan gia tri: ");
    Serial.println(value);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RECEIVE");
    lcd.setCursor(0, 1);
    lcd.print(String(value) + "VND");
    delay(1000);

    if (dieukien == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SUCCESSFUL");
      lcd.setCursor(0, 1);
      lcd.print("PAYMENT");
      servo3.write(90);
      delay(1000);
      servo3.write(0);
    } else if (dieukien > 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CHANGE");
      lcd.setCursor(0, 1);
      lcd.print(String(dieukien) + "VND");
      servo3.write(90);
      delay(1000);
      servo3.write(0);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("NOT ENOUGH");
      delay(1000);
      servo1.write(90);
      servo2.write(90);
      delay(1000);
      if (digitalRead(irss) == HIGH) {
        servo1.write(0);
        servo2.write(180);
      }
    }

    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PAY HERE!");
    lcd.setCursor(0, 1);
    lcd.print("20000VND");
    dangxuly = false;
  }
}

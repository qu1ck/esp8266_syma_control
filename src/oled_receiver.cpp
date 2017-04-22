#include <Arduino.h>
#include <SSD1306Brzo.h>
#include <brzo_i2c.h>

#define SSD1306_ADDRESS 0x3C

SSD1306Brzo display(SSD1306_ADDRESS, D1, D2);

volatile uint32_t lastImpulseUs = 0;
volatile uint32_t dataBuf = 0;
volatile uint32_t data = 0;
volatile uint32_t lastDataReadMs = 0;
volatile uint8_t bitCount = 0;

void isr() {
  uint32_t currentUs = micros();
  uint32_t timeElapsed = currentUs - lastImpulseUs;
  lastImpulseUs = currentUs;
  if (timeElapsed > 1200) {
    // This is either start or end of preambula bit
    bitCount = 0;
    dataBuf = 0;
  } else {
    if (timeElapsed > 800) {
      // This is a 1
      dataBuf += dataBuf + 1;
    } else {
      // This is a 0
      dataBuf += dataBuf;
    }
    bitCount++;
    if (bitCount == 32) {
      data = dataBuf;
      lastDataReadMs = millis();
    }
  }
}

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println();
  attachInterrupt(D3, &isr, FALLING);
  display.init();
}

char s[100];
int throttle, pitch, yaw, trim;

void decode(uint32_t data, int &yaw, int &pitch, int &throttle, int &trim) {
  uint32_t buf = data;
  trim = buf & 0xFF;
  buf >>= 8;
  throttle = buf & 0xFF;
  buf >>= 8;
  pitch = buf & 0xFF;
  buf >>= 8;
  yaw = buf & 0xFF;
}

void loop() {
  display.clear();
  bool haveData = true;
  if (millis() - lastDataReadMs < 3000) {
    decode(data, yaw, pitch, throttle, trim);
  } else {
    haveData = false;
  }
  // print the info
  if (haveData) {
    sprintf(s, "Y: %03d\nP: %03d\nT: %03d\nR: %03d", yaw, pitch, throttle,
            trim);
  }
  display.drawString(5, 5, (haveData) ? s : "Y: ---\nP: ---\nT: ---\nR: ---");
  display.drawLine(70, 10, 70, 54);
  display.drawLine(80, 32, 124, 32);
  display.drawLine(102, 10, 102, 54);
  if (haveData) {
    if (throttle > 127) {
      throttle -= 127;
    }
    display.drawCircle(70, 54 - throttle * 44 / 127, 3);
    display.drawCircle(102 + (63 - yaw) * 44 / 127,
                       32 + (pitch - 63) * 44 / 127, 3);
  }
  display.display();
  delay(30);
}

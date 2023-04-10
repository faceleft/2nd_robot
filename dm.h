#ifndef DM_H
#define DM_H
#include <Arduino.h>

namespace dm {
// prefs
const int SENSORS_NUM = 1;
const int ECHO_PINS[SENSORS_NUM] = {3};
const int TRIG_PINS[SENSORS_NUM] = {2};

volatile bool pulseFlag[SENSORS_NUM];
volatile bool changeFlag;
volatile bool readyFlag[SENSORS_NUM];

volatile unsigned long timer[SENSORS_NUM];
volatile unsigned long pulseTimeUs[SENSORS_NUM];

unsigned long timeout[SENSORS_NUM];
volatile int con = 0;

bool digitalReadFast(uint8_t pin) {
  if (pin < 8) {
    return bitRead(PIND, pin);
  } else if (pin < 14) {
    return bitRead(PINB, pin - 8);
  } else if (pin < 20) {
    return bitRead(PINC, pin - 14);    // Return pin state
  }
}

template <int i>
void echo() {
  bool highOrLow = digitalReadFast(ECHO_PINS[0]);
  con++;
  if (pulseFlag[i] && !highOrLow) {
    pulseTimeUs[i] = micros() - timer[i];
    pulseFlag[i] = false;
    changeFlag = true;
    readyFlag[i] = true;
  }
  else if(highOrLow){
    timer[i] = micros();
    pulseFlag[i] = true;
  }
}

void sendingPulses() {
  for (int i = 0; i < SENSORS_NUM; i++) {
    if (readyFlag[i] || millis() - timeout[i] > 50) {
      digitalWrite(TRIG_PINS[i], 1);
      delayMicroseconds(10);
      digitalWrite(TRIG_PINS[i], 0);
      readyFlag[i] = false;
      timeout[i] = millis();
    }
  }
}

void init() {
  changeFlag = false;
  for (int i = 0; i < SENSORS_NUM; i++) {
    pulseFlag[i] = false;
    readyFlag[i] = true;
    timeout[i] = millis();
    pinMode(TRIG_PINS[i], OUTPUT);
    pinMode(ECHO_PINS[i], INPUT);
    digitalWrite(TRIG_PINS[i], 0);
  }
  attachInterrupt(digitalPinToInterrupt(ECHO_PINS[0]), echo<0>, CHANGE);
  sendingPulses();
}
}

#endif

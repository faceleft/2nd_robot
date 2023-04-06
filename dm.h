#ifndef DM_H
#define DM_H
#include <Arduino.h>
#define DM_SENSORS_NUM 3
namespace dm {
    // prefs
    const int ECHO_PINS[DM_SENSORS_NUM] = {0, 1, 2};
    const int TRIG_PINS[DM_SENSORS_NUM] = {3, 4, 5};

    volatile bool pulseFlag[DM_SENSORS_NUM];
    volatile bool changeFlag;
    volatile bool readyFlag[DM_SENSORS_NUM];

    volatile unsigned long timer[DM_SENSORS_NUM];
    volatile unsigned long pulseTimeUs[DM_SENSORS_NUM];

    unsigned long timeout[DM_SENSORS_NUM];

    template <int i>
    void echoHigh() {
    timer[i] = micros();
    pulseFlag[i] = true;
    }

    template <int i>
    void echoLow() {
    if (pulseFlag[i]) {
        pulseTimeUs[i] = micros() - timer[i];
        pulseFlag[i] = false;
        changeFlag = true;
        readyFlag[i] = true;
    }
    }

    void pulse() {
    for (int i = 0; i < DM_SENSORS_NUM; i++)
        digitalWrite(TRIG_PINS[i], 0);
    for (int i = 0; i < DM_SENSORS_NUM; i++) {
        if (readyFlag[i] || micros() - timeout[i] > 500) {
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
    for (int i = 0; i < DM_SENSORS_NUM; i++) {
        pulseFlag[i] = false;
        readyFlag[i] = true;
        timeout[i] = millis();
    }
    
    attachInterrupt(ECHO_PINS[0], echoHigh<0>, RISING);
    attachInterrupt(ECHO_PINS[1], echoHigh<1>, RISING);
    attachInterrupt(ECHO_PINS[2], echoHigh<2>, RISING);

    attachInterrupt(ECHO_PINS[0], echoLow<0>, FALLING);
    attachInterrupt(ECHO_PINS[1], echoLow<1>, FALLING);
    attachInterrupt(ECHO_PINS[2], echoLow<2>, FALLING);

    pulse();
    }
}

#endif
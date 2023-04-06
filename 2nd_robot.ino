#include "dm.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("Start program");
  dm::init();
}

void loop()
{
  if (dm::changeFlag) {
    dm::changeFlag = false;
    Serial.print("Sensors: ");
    for (int i = 0; i < dm::DM_SENSORS_NUM; i++) {
      Serial.print(dm::pulseTimeUs[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
}

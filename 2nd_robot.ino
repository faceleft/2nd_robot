#include "dm.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("Start program");
  dm::init();
  Serial.println("End of setup");
}

void loop()
{
  /*
  Serial.print(dm::pulseFlag[0]);
  Serial.print(" ");
  Serial.print(dm::changeFlag);
  Serial.print(" ");
  Serial.print(dm::readyFlag[0]);
  Serial.print(" ");
  Serial.print(dm::timer[0]);
  Serial.print(" ");
  Serial.print(dm::pulseTimeUs[0]);
  Serial.print(" ");
  Serial.print(dm::con);
  Serial.print(" ");
  Serial.println();
  */
  if (dm::changeFlag) {
    dm::changeFlag = false;
    //Serial.print("Sensors: ");
    for (int i = 0; i < dm::SENSORS_NUM; i++) {
      Serial.print(dm::pulseTimeUs[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
  dm::sendingPulses();

}

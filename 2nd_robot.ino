#define SR_COUNT 3

volatile bool flag[SR_COUNT];
volatile bool change_flag;
volatile bool ready_flag[SR_COUNT];

volatile unsigned long timer[SR_COUNT];
volatile unsigned long pulse_time_us[SR_COUNT];

unsigned long timeout[SR_COUNT];

const uint8_t echo_pins[SR_COUNT] = {0,1,2};
const uint8_t trig_pins[SR_COUNT] = {3,4,5};

template<uint8_t i>
void echo_high(){
  timer[i] = micros();   
  flag[i] = true;
}

template<uint8_t i>
void echo_low(){
  if(flag[i]){
    pulse_time_us[i] = millis()-timer[i];
    flag[i] = false;
    change_flag = true;
    ready_flag = true;
  }    
}


inline void send_pulse(){
    for(int i=0;i<SR_COUNT;i++){
      if(ready_flag[i] ||micros()-timeout[i] > 100){
        digitalWrite(trig_pins[i], 0);
        delayMicroseconds(2);
        digitalWrite(trig_pins[i], 1);
        delayMicroseconds(10);
        digitalWrite(trig_pins[i], 0);
        ready_flag[i]=false;
        timeout[i]=millis();
      }
    }
}

void setup(){
    Serial.begin(115200);
    Serial.println("Start program");
    for(int i = 0; i < SR_COUNT; i++) {
      flag[i] = false;   
      change_flag[i] = false;
      ready_flag[i] = true;
      timeout[i] = millis();
    }
    attachInterrupt(echo_pins[0], echo_high<0>, RISING);
    attachInterrupt(echo_pins[1], echo_high<1>, RISING);
    attachInterrupt(echo_pins[2], echo_high<2>, RISING);

    attachInterrupt(echo_pins[0], echo_low<0>, FALLING);
    attachInterrupt(echo_pins[1], echo_low<1>, FALLING);
    attachInterrupt(echo_pins[2], echo_low<2>, FALLING);
  send_pulse();
}

void loop(){
    if(change_flag){
      change_flag=false;
      Serial.print("Sensors: ");
      for(int i = 0; i < SR_COUNT; i++){
        Serial.print(pulse_time_us[i]);
        Serial.print(" ");
      }
      Serial.println();
    }
    
    
}
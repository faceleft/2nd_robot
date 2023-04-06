#define SR_COUNT 3

bool flag[SR_COUNT];
bool change_flag[SR_COUNT];
unsigned long timer[SR_COUNT];
unsigned long pulse_time_us[SR_COUNT];

const uint8_t echo_pins[SR_COUNT] = {0,1,2};
const uint8_t trig_pins[SR_COUNT] = {3,4,5};


void echo0_high(){
  timer[0] = micros();   
  flag[0] = true;
}
void echo1_high(){
  timer[1] = micros();   
  flag[1] = true;
}
void echo2_high(){
  timer[2] = micros();   
  flag[2] = true;
}

void echo0_low(){
  if(flag[0]){
    pulse_time_us[0] = micros()-timer[0];
    flag[0] = false;
    change_flag[0] = true;
  }    
}
void echo1_low(){
  if(flag[1]){
    pulse_time_us[1] = micros()-timer[1];
    flag[1] = false;
    change_flag[1] = true;
  } 
}
void echo2_low(){
  if(flag[2]){
    pulse_time_us[2] = micros()-timer[2];
    flag[2] = false;
    change_flag[2] = true;
  } 
}

void send_pulse(int i){
    digitalWrite(trig_pins[i], 0);
    delayMicroseconds(2);
    digitalWrite(trig_pins[i], 1);
    delayMicroseconds(10);
    digitalWrite(trig_pins[i], 0);
}

void setup(){
    Serial.begin(115200);
    Serial.println("Start program");
    for(int i = 0; i < SR_COUNT; i++) {
      flag[i] = false;   
      change_flag[i] = false;
    }
    attachInterrupt(echo_pins[0], echo0_high, RISING);
    attachInterrupt(echo_pins[1], echo1_high, RISING);
    attachInterrupt(echo_pins[2], echo2_high, RISING);

    attachInterrupt(echo_pins[0], echo0_low, FALLING);
    attachInterrupt(echo_pins[1], echo1_low, FALLING);
    attachInterrupt(echo_pins[2], echo2_low, FALLING);
}

void loop(){
    for(int i = 0; i < SR_COUNT; i++){
      if(change_flag[i]){
        change_flag[i]=false;
        Serial.print(i);
        Serial.print(" sensor ");
        Serial.println(pulse_time_us[i]);
      }
      if(!flag[i]) send_pulse(i);
    }
    
}
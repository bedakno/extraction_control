#include <SerialComm.h>
#include "FastPID.h"

SerialComm SCom;
float Hz=160000;
float Kp=1.2, Ki=0*Hz, Kd=0/Hz;
int output_bits=11;
bool output_signed=true;
FastPID myPID(Kp, Ki, Kd, Hz, output_bits, output_signed);

void checkPID(){
  if(myPID.err()){
    Serial.println("PID config error");
    for(;;){
      delay(100);
    }
  }
}


void setup() {
  Serial.begin(115200);
}

void loop() {
  if(Serial.available()){
    SCom.process(myPID);
  }
}

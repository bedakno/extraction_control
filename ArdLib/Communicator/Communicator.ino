#include <SerialComm.h>
#include <FastPID.h>
#include <Fast_IO_Due.h>
Fast_IO_Due fastIO;
SerialComm SCom;
float Hz=160000;
float Kp=1, Ki=2, Kd=3;
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
  fastIO.initialize_adc({0,1,11});
  fastIO.initialize_dac();
}

void loop() {
  if(Serial.available()){
    SCom.process(myPID);
  }
}

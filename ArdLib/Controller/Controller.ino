#include <FastPID.h>
#include <Fast_IO_Due.h> 
#include <SerialComm.h>


Fast_IO_Due fastIO;
SerialComm SCom;

float Hz=130000;
float Kp=0, Ki=0, Kd=0;
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
  pinMode(LED_BUILTIN, OUTPUT);
  fastIO.initialize_adc({0,1,2,3,4,5});
  fastIO.initialize_dac();
  checkPID();
  myPID.setOutputRange(-2047,2047);
  delay(1000);
}

int16_t output, setpoint=0;
bool control;

void controlADD(){
  control = fastIO.calc_norm(5);
  if(control){
    output=myPID.step(setpoint, fastIO.IOnorm); //If PID-Controller is turned on (control == true), calculate new output.
  }
  fastIO.write_dac(int16_t(2047+output)); //return output (can be old held one or new calculated one).
}

void loop() {
  //int start = micros();
  //for(size_t t = 0; t<1000000; t++){
  if(Serial.available()){
    SCom.process(myPID, fastIO);
  }
  controlADD();
  //}
  //Serial.println(micros()-start);
}

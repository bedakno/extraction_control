#include <FastPID.h>
#include <SPI.h>
#include <Fast_IO_Due.h>
#include <SerialComm.h>


Fast_IO_Due fastIO;
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
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(100); // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(100); // wait for a second
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  fastIO.initialize_adc({0,1,11});
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
    output=myPID.step(setpoint, IOnorm); //If PID-Controller is turned on (control == true), calculate new output.
  }
  fastIO.write_dac(output+2047); //return output (can be old held one or new calculated one).
}


void getcommand(){
  char cmdraw[16];
  size_t cmdlen = Serial.readBytesUntil('\n', cmdraw, 16);
  char cmd[cmdlen];
  for(size_t i =0; i<cmdlen; i++){
     cmd[i]=cmdraw[i];
  }
  char* ans = (char*) malloc((16)*sizeof(char));
  ans = SCom.process(cmd, cmdlen);
  Serial.print(ans);
  Serial.flush();
  SCom.resetInputBuffer();
  free(ans);
}

int start;
void loop() {    
  if(Serial.available()){
    getcommand();
  }
}

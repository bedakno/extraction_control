#include "Fast_IO_Due.h"
#include "FastPID.h"
#include "SerialComm.h"


Fast_IO_Due fastIO;
SerialComm SCom;
float Hz=220000;
float Kp=1, Ki=0, Kd=0;
int output_bits=11;
bool output_signed=true;
FastPID myPID(Kp, Ki, Kd, Hz, output_bits, output_signed);
uint32_t start;


void setup(){
  Serial.begin(115200);
  fastIO.initialize_dac();
  //Input adc channels as {[pin1,pin2..]} with pinx being 0 -> A0, 1 -> A1 ... invalid entries will be ignored
  fastIO.initialize_adc({0,5});
  fastIO.initialize_dac();
  start=micros();
}


void dactest(){
  //fastIO.write_dac(1024);
  //fastIO.write_dac(2048);
}

void dacnoise(){
  while(!DACC_ISR_TXRDY);
  DACC->DACC_CDR = 1024;
}

int temp;
void temperature(){
  while((ADC->ADC_ISR&0x80)==0);
  temp=ADC->ADC_CDR[15]; //set voltage value from adc 15 (connected to A0)
  delay(10);
  Serial.println(temp);
  Serial.flush();
  delay(10); 
}

const unsigned int res=2000; //number of points to measure before sending to computer
int val, val2; //save value
unsigned int i=0; //counter
int values[res]; //arrays for measurements
int times[res];

void adctest(){ //save values in array and print them later to not too much of a delay due to transmitting the data
  for(i=0; i<res;i++){
   times[i]=micros()-start; //fill times array
  values[i]=fastIO.read_adc();
  }
  for(size_t j=0; j<res;j++){
      Serial.println(String(times[j])+';'+String(values[j])); //transmit all values to computer
  }
}

int timediff;
void adcspeed(){
  start=micros();
  for(i=0; i<100000;i++){
    val2=fastIO.read_adc();
    val=fastIO.read_anyadc(5);
  }
 timediff=micros()-start;
 Serial.println(1./timediff*100000);
 Serial.println(val2);
}

void linear(){
  Serial.println(fastIO.med_anyadc(0));
}

void adcdactest(){
  //val=fastIO.read_adc();
  //fastIO.write_dac(val);
}

uint32_t med=0;
void adctest2(){
  for(size_t i=0; i<100; i++){
    med+=fastIO.read_adc();
  }
  med/=100;
  Serial.println(med);
  med=0;
}

void loop() {
 SCom.process(myPID, fastIO);
}

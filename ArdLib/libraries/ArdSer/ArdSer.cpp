#include "ArdSer.h"

void readData(){
    uint8_t pos1, pos2, pos3;

    message = Serial.readStringUntil('\n');
    pos1 = message.indexOf(':',0);
    pos2 = message.indexOf(':', pos1+1);
    pos3 = message.indexOf(':', pos2+1);

    arg1 = message.substring(0, pos1);
    arg2 = message.substring(pos1+1, pos2);
    arg3 = message.substring(pos2+1, pos3);

    resetInputBuffer();
}

void writeData(String data){
    Serial.println(data);
}

void resetInputBuffer(void){
    while(Serial.available()){
        Serial.read();
    }
}


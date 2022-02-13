#include "ArdSer.h"

void interface::receive(){
    uint8_t pos1, pos2, pos3;

    String message = Serial.readStringUntil('\n');
    pos1 = message.indexOf(':',0);
    pos2 = message.indexOf(':', pos1+1);
    pos3 = message.indexOf(':', pos2+1);

    arg1 = message.substring(0, pos1);
    arg2 = message.substring(pos1+1, pos2);
    arg3 = message.substring(pos2+1, pos3);

    resetInputBuffer();
}

void interface::transmit(String data){
    Serial.println(data);
}

void interface::resetInputBuffer(void){
    while(Serial.available()){
        Serial.read();
    }
}


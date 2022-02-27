//
//  SerialComm.cpp
//  
//
//  Created by Béla Knopp on 24.01.22.
//
#include "SerialComm.h"


SerialComm::SerialComm(){
}
SerialComm::~SerialComm(){
}

bool SerialComm::process(FastPID &pid, Fast_IO_Due &IO){
    receive();
    switch(arg[0]){
        case READ:
            receive();
            Serial.println(read(pid, IO, arg[0]));
            break;
        case WRITE:
            receive();
            write(pid, IO, arg[0]); //set value
            break;
        case RESET:
            NVIC_SystemReset();
            break;
        case CHECK:
            Serial.println(CHECK);
            break;
        default:
            return true;
    }
    return false;
}

uint32_t SerialComm::read(FastPID &pid, Fast_IO_Due &IO, char con){
    switch (con){
    case P:
        return pid._p;
    case I:
        return pid._i;//*pid._hz;
    case D:
        return pid._d;///pid._hz;
    case F:
        return pid._hz;
    case N:
        return IO.IOnorm;
    case A:
        receive();
        return med_anyadc(fast_atoi(arg));
    default:
        return 0;
    }
}

void SerialComm::write(FastPID &pid, Fast_IO_Due &IO, char con){
    receive();
    uint32_t value = fast_atoi(arg);
    switch (con){
    case P:
        pid._p = value;
        break;
    case I:
        pid._i = value;///pid._hz;
        break;
    case D:
        pid._d = value;//*pid._hz;
        break;
    case F:
        pid._hz = value;
        break;
    }
}

void SerialComm::receive(){
   /*
   * reads data from serial buffer and seperates at given _DELIM delimiter.
   * halts reading when _END character is found or args cant fit any more data (argnum)
   * empties serial buffer at the end
   */
    size_t curarglen;
    if(Serial.peek() == _END){
        Serial.read();
        arg[0] = _NULL_TERM;
    }
    else{
        curarglen = Serial.readBytesUntil(_DELIM, arg, BUF_SIZE);
        arg[curarglen] = _NULL_TERM;
        Serial.read();
    }
}

void SerialComm::resetInputBuffer(void){
  /*
   * reads all data serial buffer and discharges them
   */
    while(Serial.available()){
        Serial.read();
    }
}

uint32_t SerialComm::fast_atoi(char* str){
    int val = 0;
    while( *str ) {
        val = val*10 + (*str++ - '0');
    }
    return val;
}


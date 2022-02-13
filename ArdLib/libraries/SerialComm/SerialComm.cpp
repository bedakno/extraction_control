//
//  SerialComm.cpp
//  
//
//  Created by Béla Knopp on 24.01.22.
//
#include "SerialComm.h"


SerialComm::SerialComm(){
    ArdSer intf();
}
SerialComm::~SerialComm(){
}

bool SerialComm::process(const PID& pid){
    bool error = true;
    String cmd;
    String con;
    uint32_t data;

    intf.receive();

    cmd = intf.arg1;
    con = intf.arg2;
    data = intf.arg3.toInt();
    
    if(cmd == READ){
        if(cmdlen>2){
            intf.transmit(ERROR);
        }
        else{
            rdata = read(con);
            intf.transmit(String(rdata));
            error = false;
        }
    }
    if(cmd == WRITE){
        if(cmdlen==4){ //set should have more than 2 chars
            intf.transmit(READ);
        }
        else{
            write(con, data); //set value
            error = false;
        }
    }
    if(cmd == RESET){
        NVIC_SystemReset();
    }
    if(cmd == CHECK){ //Check
        intf.transmit(CHECK)
        error = false;
    }
    return error;
}

void SerialComm::resetInputBuffer(){
    while(Serial.available()){
        Serial.read();
      }
}

String SerialComm::createCommand(char _cmd, char _con, uint32_t _data, size_t _datalen){
    char* cmd = (char*) malloc((_numlen+4)*sizeof(char)); //allocate memory for pointers
    char* data_char = (char*) malloc(_numlen*sizeof(char));
    cmd[0] = _cmd; //1st char is command
    cmd[1] = DELIM;
    cmd[2] = _con; //2nd char is constant
    cmd[3] = DELIM
    if(_datalen>0){
        itoa(_data, num_char, 10); //transform given int _val in base 10 to array of chars num_char
        for(size_t c = 0; c<_datalen; c++){
            cmd[c+4]=num_char[c]; //write numchar into cmd at according places
        }
    }
    cmd[_datalen+4] = DELIM;
    cmd[_datalen+4+1] = NL; //newline
    cmd[_datalen+4+2] = TER; //carriage return
    return String(cmd);
}

uint32_t SerialComm::read(const &PID pid, char cmd){
    if(cmd=='P'){
        return pid._p;
    }
    if(cmd=='I'){
        return pid._i;
    }
    if(cmd=='D'){
        return pid._d;
    }
    if(cmd=='F'){
        return pid._hz;
    }
    if(cmd=='N'){
        return IOnorm;
    }
    return 0;
}

void SerialComm::write(char cmd, uint32_t value){
    _p = _p*(cmd!='P')+value*(cmd=='P');
    _i = _i*(cmd!='I')+value*(cmd=='I');
    _d = _d*(cmd!='D')+value*(cmd=='D');
    _hz = _hz*(cmd!='F')+value*(cmd=='F');
    
}




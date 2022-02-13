//
//  SerialComm.cpp
//  
//
//  Created by Béla Knopp on 24.01.22.
//
#include "SerialComm.h"
#include "FastPID.h"
#include "Fast_IO_Due.h"



SerialComm::~SerialComm(){
}
SerialComm::SerialComm(){
}

char* SerialComm::process(char* cmd, size_t cmdlen){
    char cmd[1];
    char con[1];
    uint32_t data;

    if(cmdlen<4){ //command has to consist of more than 4 chars
        return createCommand(ERR,ERR,1,1);
    }
    /*
    * 
    */
    uint8_t pos1, pos2, pos3;
    String arg1, arg2, arg3;
    message = Serial.readStringUntil('\n');
    pos1 = message.indexOf(':',0);
    pos2 = message.indexOf(':', pos1+1);
    pos3 = message.indexOf(':', pos2+1);

    message.substring(0, pos1).toCharArray(cmd, 1);
    message.substring(pos1+1, pos2).toCharArray(con, 1);
    data = message.substring(pos2+1, pos3).toInt();
    
    if(cmd == READ){
        if(cmdlen>2){
            return createCommand(ERR,ERR,2,1);
        }
        rdata = read(con);
        size_t rdatalen = ((bool) val * (size_t) log10(rdata) + 1); //get number of digits from val
        return createCommand(WRITE, con, val, rdatalen);
    }
    if(cmd == WRITE){
        if(cmdlen==4){ //set should have more than 2 chars
            return createCommand(READ, con, 0, 0);
        }
        //uint32_t intvalue = getValue(cmd, cmdlen); //get the uint32_t from passed char[]; use atoi instead?
        write(con, data); //set value
        val = read(con); //get new value
        size_t rdatalen = ((bool) val * (size_t) log10(rdata) + 1);
        return createCommand(WRITE, con, val, rdatalen); //output new value
    }
    if(cmd[0] == RESET){
        NVIC_SystemReset();
    }
    if(cmd[0] == CHECK){ //Check
        return createCommand(CHECK,CHECK,42,2);
    }
    return createCommand(ERR,ERR,0,1); //error
}

void SerialComm::resetInputBuffer(){
    while(Serial.available()){
        Serial.read();
      }
}

char* SerialComm::createCommand(char _cmd, char _con, uint32_t _data, size_t _datalen){
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
    return cmd;
}

uint32_t SerialComm::read(char cmd){
    if(cmd=='P'){
        return _p;
    }
    if(cmd=='I'){
        return _i;
    }
    if(cmd=='D'){
        return _d;
    }
    if(cmd=='F'){
        return _hz;
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




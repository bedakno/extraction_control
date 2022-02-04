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
    uint32_t val=0;
    if(cmdlen<2){ //command has to consist of more than 1 char
        return createCommand(ERR,ERR,1,1);
    }
    if(cmd[1]==GET){
        if(cmdlen>2){
            return createCommand(ERR,ERR,2,1);
        }
        val = getter(cmd[0]);
        size_t numlen = ((bool) val * (size_t) log10(val) + 1); //get number of digits from val
        return createCommand(cmd[0], SET, val, numlen);
    }
    
    if(cmd[1]==SET){
        if(cmdlen==2){ //set should have more than 2 chars
            return createCommand(cmd[0], GET, 0, 0);
        }
        uint32_t intvalue = getValue(cmd, cmdlen); //get the uint32_t from passed char[]
        setter(cmd[0], intvalue); //set value
        val = getter(cmd[0]); //get new value
        size_t numlen = ((bool) val * (size_t) log10(val) + 1);
        return createCommand(cmd[0], SET, val, numlen); //output new value
    }
    
    if(cmd[1]==EXEC){
        if(cmd[0]=='R'){ //Reset
            NVIC_SystemReset();
        }
        if(cmd[0]=='C'){ //Check
            return createCommand(cmd[0],EXEC,42,2);
        }
        return createCommand(ERR,EXEC,0,1);
    }
    return createCommand(ERR,ERR,0,1); //error
}

void SerialComm::reset_input_buffer(){
    while(Serial.available()){
        Serial.read();
      }
}

char* SerialComm::createCommand(char _con, char _delim, uint32_t _val, size_t _numlen){
    char* cmd = (char*) malloc((_numlen+4)*sizeof(char)); //allocate memory for pointers
    char* num_char = (char*) malloc(_numlen*sizeof(char));
    cmd[0]=_con; //1st char is constant
    cmd[1]=_delim; //2nd char is delimiter
    if(_numlen>0){
        itoa(_val, num_char, 10); //transform given int _val in base 10 to array of chars num_char
        for(uint8_t c = 0; c<_numlen; c++){
            cmd[c+2]=num_char[c]; //write numchar into cmd at according places
        }
    }
    cmd[_numlen+2]=NL; //newline
    cmd[_numlen+3]=TER; //carriage return
    return cmd;
}

uint32_t SerialComm::getter(char cmd){
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

void SerialComm::setter(char cmd, uint32_t value){
    _p = _p*(cmd!='P')+value*(cmd=='P');
    _i = _i*(cmd!='I')+value*(cmd=='I');
    _d = _d*(cmd!='D')+value*(cmd=='D');
    _hz = _hz*(cmd!='F')+value*(cmd=='F');
    
}

uint32_t SerialComm::getValue(char cmd[], uint8_t cmdlen){
    uint32_t value=0;
    for(uint8_t nc=cmdlen-1; nc>1; nc--){
        value+=((cmd[nc]-48)*pow(10,(cmdlen-1)-nc)); //cmd[nc]-48 converts ascii code numbers (from 48 = 0 to 57 = 9); multiplies with 10^0 for nc = cmdlen-1 (last char) and 10^cmdlen-2 (third char) 1st and 2nd char are constant and delimiter
    }
    return value;
}




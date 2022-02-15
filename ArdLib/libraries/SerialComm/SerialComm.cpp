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

bool SerialComm::process(FastPID &pid){
    bool error = true;
    String cmd;
    String con;
    uint32_t data;
    uint32_t rdata;

    receive();

    cmd = arg1;
    con = arg2;
    data = arg3.toInt();
    
    if(cmd == READ){
        if(con.length()==0){
            error = true;
        }
        else{
            rdata = read(pid, con, data);
            transmit(String(rdata));
            error = false;
        }
    }
    if(cmd == WRITE){
        if(arg3.length()==0){ //set should have more than 2 chars
            transmit(READ);
        }
        else{
            write(pid, con, data); //set value
            error = false;
        }
    }
    if(cmd == RESET){
        NVIC_SystemReset();
    }
    if(cmd == CHECK){ //Check
        transmit(CHECK);
        error = false;
    }
    if(error == true){
        transmit(ERR);
    }
    return error;
}

void SerialComm::resetInputBuffer(){
    while(Serial.available()){
        Serial.read();
      }
}

// String SerialComm::createCommand(char _cmd, char _con, uint32_t _data, size_t _datalen){
//     char* cmd = (char*) malloc((_numlen+4)*sizeof(char)); //allocate memory for pointers
//     char* data_char = (char*) malloc(_numlen*sizeof(char));
//     cmd[0] = _cmd; //1st char is command
//     cmd[1] = DELIM;
//     cmd[2] = _con; //2nd char is constant
//     cmd[3] = DELIM
//     if(_datalen>0){
//         itoa(_data, num_char, 10); //transform given int _val in base 10 to array of chars num_char
//         for(size_t c = 0; c<_datalen; c++){
//             cmd[c+4]=num_char[c]; //write numchar into cmd at according places
//         }
//     }
//     cmd[_datalen+4] = DELIM;
//     cmd[_datalen+4+1] = NL; //newline
//     cmd[_datalen+4+2] = TER; //carriage return
//     return String(cmd);
// }

uint32_t SerialComm::read(FastPID &pid, String con, uint32_t data = 0){
    if(con==P){
        return pid._p;
    }
    if(con==I){
        return pid._i;
    }
    if(con==D){
        return pid._d;
    }
    if(con==F){
        return pid._hz;
    }
    if(con==N){
        return IOnorm;
    }
    if(con == A){
        return med_anyadc(data);
    }
    return 0;
}

void SerialComm::write(FastPID &pid, String con, uint32_t value){
    if(con==P){
        return pid.setP(value);
    }
    if(con==I){
        return pid.setI(value);
    }
    if(con==D){
        return pid.setD(value);
    }
    if(con==F){
        return pid.setHZ(value);
    }
}

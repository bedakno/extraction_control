//
//  SerialComm.h
//  
//
//  Created by Béla Knopp on 24.01.22.
//

#ifndef SerialComm_h
#define SerialComm_h

#include <Arduino.h>
#include "FastPID.h"
#include "Fast_IO_Due.h"
#include "ArdSer.h"

class SerialComm:ArdSer,FastPID,Fast_IO_Due{
public:
    SerialComm(){};
    ~SerialComm();
    bool process();
    void resetInputBuffer();
    
private:
    static const char READ = "R";
    static const char WRITE = "W";
    static const char RESET = "X";
    static const char CHECK = "C";
    static const char ERR = "E";
    static const char DELIM = ":";
    static const char NL = "\n";
    static const char TER = "\r";
    
private:
    void write(char, uint32_t);
    uint32_t read(char);
    uint32_t readValue(char[], uint8_t);
    char* createCommand(char, char, uint32_t, size_t);
};

#endif /* SerialComm_h */

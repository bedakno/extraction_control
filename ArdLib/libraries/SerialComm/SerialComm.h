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


class SerialComm: public interface, public FastPID, public Fast_IO_Due{
public:
    SerialComm();
    ~SerialComm();
    bool process(FastPID&);
    void resetInputBuffer();
    
private:
    String READ = "R";
    String WRITE = "W";
    String RESET = "X";
    String CHECK = "C";
    String ERR = "E";
    String A = "A";
    String P = "P";
    String I = "I";
    String D = "D";
    String F = "F";
    String N = "N";
    String DELIM = ":";
    String NL = "\n";
    String TER = "\r";
    
private:
    void write(FastPID&, String, uint32_t);
    uint32_t read(FastPID&, String, uint32_t);
    char* createCommand(char, char, uint32_t, size_t);
};

#endif /* SerialComm_h */

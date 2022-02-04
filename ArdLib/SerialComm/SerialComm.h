//
//  SerialComm.h
//  
//
//  Created by Béla Knopp on 24.01.22.
//

#ifndef SerialComm_h
#define SerialComm_h

#include <Arduino.h>
#include <string>
using namespace std;

class SerialComm{
public:
    SerialComm();
    ~SerialComm();
    char* process(char*, size_t);
    void reset_input_buffer();
    
private:
    static const char GET = '?';
    static const char SET = '=';
    static const char EXEC = '!';
    static const char ERR = 'E';
    static const char NL = '\n';
    static const char TER = '\r';
    
private:
    void setter(char, uint32_t);
    uint32_t getter(char);
    uint32_t getValue(char[], uint8_t);
    char* createCommand(char, char, uint32_t, size_t);
};

#endif /* SerialComm_h */

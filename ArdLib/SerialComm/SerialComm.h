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
    void resetInputBuffer();
    
private:
    static const char READ = 'R';
    static const char WRITE = 'W';
    static const char RESET = 'X';
    static const char CHECK = 'C';
    static const char ERR = 'E';
<<<<<<< HEAD
    static const char DELIM = ':'
=======
>>>>>>> a86e73a9ab16359234022fcbf0a3561dc0629900
    static const char NL = '\n';
    static const char TER = '\r';
    
private:
    void write(char, uint32_t);
    uint32_t read(char);
    uint32_t readValue(char[], uint8_t);
    char* createCommand(char, char, uint32_t, size_t);
};

#endif /* SerialComm_h */

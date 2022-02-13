#ifndef ArdSer_h
#define ArdSer_h

#include <Arduino.h>

class interface{
public:
    void receive(void);
    void transmit(String data);

    String arg1;
    String arg2;
    String arg3;

private:
    void resetInputBuffer(void);
};


#endif
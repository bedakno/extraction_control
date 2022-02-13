#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import serial
import logging
import time


class SerC:
    #Delimiters
    READ_delim = 'R'
    WRITE_delim = 'W'
    CHECK_delim = 'C'
    RESET_delim = 'X'
    ERR_delim = 'E'
    #Terminator
    TERMI = '\n'
    #Letters for constants
    con_P = 'P' #proportional
    con_I = 'I' #integral
    con_D = 'D' #differential
    con_HZ = 'F' #frequency

    

    def query(self, msg):
        self.interface.reset_input_buffer()
        self.interface.reset_output_buffer()
        self.interface.write(msg)
        time.sleep(0.5)
        return self.interface.readline().decode().strip()
    
    def create_command(self, _delim, _let, _val=''): #letter, delimiter, value
        cmd = ''.join([_delim, _let, _val, self.TERMI]).encode()
        return cmd
    
    def __init__(self, port_ = "/dev/cu.usbmodem14401", baudrate_ = 115200, timeout_ = 1):
        
        #Init serial interface
        self.interface = serial.Serial(port = port_, baudrate = baudrate_, timeout = timeout_)
        time.sleep(2)
        
        #Check connection
        cmd = self.create_command(self.CHECK_delim, self.CHECK_delim)
        #_cmd=self.createCommand(self.con_P, self.GET_delim)
        ans = self.query(cmd)
        #self.interfaceSY2.write(_cmd)
        time.sleep(1)
        if ans == 'CC42':
            logging.debug("Serial connection to Arduino on port " + port_ +" established.")
            print("Connection working")
        else:
            logging.error("No reply on serial connection to Arduino on port " + port_ + ".")
        
    def help():
        return
        #return functions and variables to be used
    
    def check_con(self):
        cmd = self.create_command(self.CHECK_delim, self.CHECK_delim)
        ans = self.query(cmd)
        if ans == 'CC42':
            logging.debug("Serial connection to Arduino is working.")
            print("Connection working")
        else:
            logging.error("No reply from Arduino.")

    def reset_ard(self):
        cmd = self.create_command(self.RESET_delim, self.RESET_delim)
        self.interface.write(cmd)
    
    
    #function to get a value from arduino
    def read_data(self, con):
        #create command string
        cmd = self.create_command(self.READ_delim, con)
        #write data to arduino via serial
        ans = self.query(cmd)
        if ans == '':
            logging.error("no answer received")
        #get int from transmitted string
        valuestr = ''.join(x for x in ans if x.isdigit())
        if(len(valuestr)>0):
            valuein = int(valuestr)
        else:
            logging.error("Answer is empty")
            return
        #transform FastPID format to real value
        cmdhz = self.create_command(self.READ_delim, self.con_HZ)
        hz = self.interface.write(cmdhz)
        if con == 'P':
            value = (valuein/256)
        if con == 'I':
            value = (valuein*hz)/256
        if con == 'D':
            value = (valuein/hz)/256
        
        return value
    
    def write_data(self, con, valuein):
        #transform value to FastPID format
        cmdhz = self.create_command(self.READ_delim, self.con_HZ)
        hz = self.query(cmdhz)
        if con == 'P':
            value = int(valuein*256)
        if con == 'I':
            value = int((valuein/hz)*256)
        if con == 'D':
            value = int((valuein*hz*256))
        
        #create command string
        cmd = self.create_command(self.WRITE_delim, con, str(value))
        
        #write data to arduino via serial
        ans = self.query(cmd)
        if ans == '':
            logging.error("no answer received")
          
        valuestr = ''.join(x for x in ans if x.isdigit())
        if(len(valuestr)>0):
            valueout = int(valuestr)
        else:
            logging.error("Answer is empty")
            return
        #transform FastPID format to real value
        if con == 'P':
            value = (valueout/256)
        if con == 'I':
            value = (valueout*hz)/256
        if con == 'D':
            value = (valueout/hz)/256     
        return value

if __name__ == '__main__':
    # Make an instance of ArdDueComm class
    a = SerC()
    
    
    a.interfaceSX1.write('Test1\n'.encode())
    print(a.interfaceSX1.readline().strip())
    
    
    a.interfaceSX1.write('Test2\n'.encode())
    print(a.interfaceSX1.readline().strip())
    
    
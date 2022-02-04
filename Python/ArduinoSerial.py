#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import serial
import logging
import time
import sys #exit function
import re

class ArdDueSComm:
    #Delimiters
    GET_delim = '?'
    SET_delim = '='
    EXEC_delim = '!'
    #Terminator
    TERMI = '\n'
    #Arduino names
    ARD_SX1 = 'SX1'
    ARD_SY1 = 'SY1'
    ARD_SX2 = 'SX2'
    ARD_SY2 = 'SY2'
    #Letters for constants
    con_P = 'P' #proportional
    con_I = 'I' #integral
    con_D = 'D' #differential
    con_HZ = 'F' #frequency
    #Letters for executes
    exec_R = 'R' #reset
    exec_C = 'C' #check
    
    def setInterface(self,_ard):
        if _ard==self.ARD_SX1:
            return self.interfaceSX1
        elif _ard==self.ARD_SY1:
            return self.interfaceSY1
        elif _ard==self.ARD_SX2:
            return self.interfaceSX2
        elif _ard==self.ARD_SY2:
            return self.interfaceSY2
        return ''
    def WriteRead(self, _ardin, _msg):
        _interface = self.setInterface(_ardin)
        _interface.reset_input_buffer()
        _interface.reset_output_buffer()
        _interface.write(_msg)
        time.sleep(0.5)
        return _interface.readline().decode().strip()
    
    def createCommand(self,_let, _delim, _val=''): #letter, delimiter, value
        cmd = ''.join([_let,_delim,_val, self.TERMI]).encode()
        return cmd
    
    def __init__(self, _port0="/dev/cu.usbmodem14401", _port1="/dev/cu.usbmodem14301", _port2="/dev/cu.usbmodem14201", _port3="/dev/cu.usbmodem14101", _baudrate=115200, _timeout=1):
        
        #Init serial interface
        self.interfaceSX1 = serial.Serial(port=_port0, baudrate=_baudrate, timeout=_timeout)
        #self.interfaceSY1 = serial.Serial(port=_port1, baudrate=_baudrate, timeout=_timeout)
        #self.interfaceSX2 = serial.Serial(port=_port2, baudrate=_baudrate, timeout=_timeout)
        #self.interfaceSY2 = serial.Serial(port=_port3, baudrate=_baudrate, timeout=_timeout)
        time.sleep(2)
        
        #Check connection
        _cmd = self.createCommand(self.exec_C, self.EXEC_delim)
        #_cmd=self.createCommand(self.con_P, self.GET_delim)
        _ansSX1 = self.WriteRead('SX1', _cmd)
        #self.interfaceSX1.write('Test2'.encode())
        #self.interfaceSY1.write(_cmd)
        #self.interfaceSX2.write(_cmd)
        #self.interfaceSY2.write(_cmd)
        time.sleep(1)
        #_ansSX1 = self.interfaceSX1.read_until(b'\n').decode('utf-8').strip('\n\r')
        #testans = _ansSX1.encode()
        #print("test")
        #print(_cmd)
        #print("ans")
        #print(_ansSX1)
        #print("programm")
        #_ansSY1 = self.interfaceSY1.readline()
        #_ansSX2 = self.interfaceSX2.readline()
        #_ansSY2 = self.interfaceSY2.readline()
        #_ansSX1 = ''
        _ansSY1 = ''
        _ansSX2 = ''
        _ansSY2 = ''
        if _ansSX1 == 'C!42':
            logging.debug("Serial connection to ArduinoSX1 established.")
            print("yeey")
        else:
            logging.error("No reply on serial connection to ArduinoSX1.")
            
        if _ansSY1 == 'C!42':
            logging.debug("Serial connection to ArduinoSY1 established.")
        else:
            logging.error("No reply on serial connection to ArduinoSY1.")
            
        if _ansSX2 == 'C!42':
            logging.debug("Serial connection to ArduinoSX2 established.")
        else:
            logging.error("No reply on serial connection to ArduinoSX2.")
            
        if _ansSY2 == 'C!42':
            logging.debug("Serial connection to ArduinoSY2 established.")
        else:
            logging.error("No reply on serial connection to ArduinoSY2.")
        time.sleep(1)
        
    def help():
        return
        #return functions and variables to be used
        
    
    
    
        
    def resetArd(self,_ard):
        interface = self.setInterface(_ard)
        _cmd = self.createCommand(self.exec_R, self.EXEC_delim)
        interface.write(_cmd)
    
    
    #function to get a value from arduino
    def getValue(self,_ard, _con):
        try: 
            interface=self.setInterface(_ard)
        except:
            logging.error("Wrong arduinoname")
            print("This Arduino-adress does not exist")
            return
        #create command string
        _cmd = self.createCommand(_con, self.GET_delim)
        #write data to arduino via serial
        _ans = self.WriteRead(_ard, _cmd)
        if _ans == '':
            logging.error("no answer received")
        #get int from transmitted string
        _valuestr = ''.join(x for x in _ans if x.isdigit())
        if(len(_valuestr)>0):
            _valuein = int(_valuestr)
        else:
            logging.error("Answer is empty")
            return
        #transform FastPID format to real value
        _cmdhz = self.createCommand('F', self.GET_delim)
        _hz = interface.write(_cmdhz)
        if _con == 'P':
            _value = (_valuein/256)
        if _con == 'I':
            _value = (_valuein*_hz)/256
        if _con == 'D':
            _value = (_valuein/_hz)/256
        
        return _value
    
    def setValue(self,_ard, _con, _valuein):
        interface=self.setInterface(_ard)
        if interface == '':
            logging.error("Wrong arduinoname")
            print("This Arduino-adress does not exist")
            return
        
        #transform value to FastPID format
        _cmdhz = self.createCommand('F', self.GET_delim)
        _hz = self.WriteRead(_ard, _cmdhz)
        if _con == 'P':
            _value = _valuein*256
        if _con == 'I':
            _value = (_valuein/_hz)*256
        if _con == 'D':
            _value = (_valuein*_hz*256)
        
        #create command string
        _cmd = self.createCommand(_con, self.SET_delim, str(_value))
        
        #write data to arduino via serial
        _ans = self.WriteRead(_ard, _cmd)
        if _ans == '':
            logging.error("no answer received")
          
        _valuestr = ''.join(x for x in _ans if x.isdigit())
        if(len(_valuestr)>0):
            _valuein = int(_valuestr)
        else:
            logging.error("Answer is empty")
            return
        #transform FastPID format to real value
        if _con == 'P':
            _value = (_valuein/256)
        if _con == 'I':
            _value = (_valuein*_hz)/256
        if _con == 'D':
            _value = (_valuein/_hz)/256     
        return _value

if __name__ == '__main__':
    # Make an instance of ArdDueComm class
    a = ArdDueSComm()
    
    
    a.interfaceSX1.write('Test1\n'.encode())
    print(a.interfaceSX1.readline().strip())
    
    
    a.interfaceSX1.write('Test2\n'.encode())
    print(a.interfaceSX1.readline().strip())
    
    
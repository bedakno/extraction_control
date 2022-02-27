from gzip import READ
from irrad_control.devices.arduino import arduino_serial
import logging

class ArduinoController(arduino_serial.ArduinoSerial):
    PARAM_SHIFT = 2**8
    #Delimiters
    READ_delim = 'R'
    WRITE_delim = 'W'
    CHECK_delim = 'C'
    RESET_delim = 'X'
    ADC_delim = 'A'
    ERR_delim = 'E'
    #Terminator
    TERMI = '\n'
    #Letters for constants
    con_P = 'P' #proportional
    con_I = 'I' #integral
    con_D = 'D' #differential
    con_HZ = 'F' #frequency

    def __init__(self, port = "/dev/cu.usbmodem14301", baudrate = 115200, timeout = 1., P = 1, I = 0, D = 0, HZ = 130000):
        super().__init__(port=port, baudrate=baudrate, timeout=timeout)
        self.check_con()
        self.write_data(self.con_P, P)
        self.write_data(self.con_I, I)
        self.write_data(self.con_D, D)
        self.write_data(self.con_HZ, HZ)

    def check_con(self):
        """checks if the arduino is responding

        Raises:
            RuntimeError: One of two things happend: either the arduino is stuck in code and is not responding or the serial connection was not successful
        """
        cmd = self.create_command(self.CHECK_delim)
        ans = self.query(cmd)
        if ans != 'C':
           raise RuntimeError("Serial connection to arduino not established or arduino is not responding")
    
    def reset_ard(self):
        """resets the arduino via software. not as powerful as arduino must be able to respond\\
            but this restores the default settings
        """
        cmd = self.create_command(self.RESET_delim)
        self.write(cmd)
    
    def read_adc(self, adc):
        cmd = self.create_command(self.READ_delim, self.ADC_delim, adc)
        ans = self.query(cmd)
        if ans == '':
            logging.error("no answer received")
        return ans
    
    def read_data(self, con):
        """reads data/constants from the arduino.  <con> determines which

        Args:
            con (char): the constant to be read.\\
            P -> proportional;
            I -> integral;
            D -> differential;
            F -> frequecy;

        Returns:
            [float]: constant
        """
        #create read command
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
        if con == 'P':
            value = (valuein/(self.PARAM_SHIFT))
        elif con == 'I':
            value = (valuein)/(self.PARAM_SHIFT)
        elif con == 'D':
            value = (valuein)/(self.PARAM_SHIFT)
        else:
            value = valuein
        self.reset_buffers()
        return value
    
    def write_data(self, con, valuein):
        """writes data to arduino

        Args:
            con (char): constant to be set\\
            P -> proportional;
            I -> integral;
            D -> differential;
            F -> frequecy;
            valuein (float): new value
        """
        if any(con == i for i in ['P','I','D']):
            if (valuein > self.PARAM_SHIFT):
                raise ValueError("PID-constants must be smaller than "+ str(self.PARAM_SHIFT))
        #transform value to FastPID format (shift bits so no decimal place)
        if con == 'P':
            value = int(valuein*self.PARAM_SHIFT)
        elif con == 'I':
            value = int(valuein*self.PARAM_SHIFT)
        elif con == 'D':
            value = int(valuein*self.PARAM_SHIFT)
        else:
            value = valuein
        #create command string
        cmd = self.create_command(self.WRITE_delim, con, value)
        #write data to arduino via serial
        self.write(cmd)

    
    
    
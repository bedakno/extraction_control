from gzip import READ
from arduino_serial import ArduinoSerial
import logging

class ArduinoController(ArduinoSerial):
    PARAM_SHIFT = 2**8
    #Delimiters
    READ_delim = 'R'
    WRITE_delim = 'W'
    CHECK_delim = 'C'
    RESET_delim = 'X'
    ADC_delim = 'A'
    DAC_delim = 'D'
    ERR_delim = 'E'
    #Terminator
    TERMI = '\n'
    #Letters for constants
    con_P = 'p' #proportional
    con_I = 'i' #integral
    con_D = 'd' #differential
    con_HZ = 'f' #frequency

    def __init__(self, port = "/dev/cu.usbmodem14301", baudrate = 115200, timeout = 1., P = 1, I = 0, D = 0, HZ = 130000):
        super().__init__(port=port, baudrate=baudrate, timeout=timeout)
        self.check_con()
        self.set_frequency(value = 130000)
        self.set_p(value = P)
        self.set_i(value = I)
        self.set_d(value = D)

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
            raise RuntimeError("No answer received")
        return ans
    
    def set_frequency(self, value):
        """Sets the frequency used by PID-Controller. This does not change the actual sampling rate.
            Actual value is estimated to be around 130kHz.
        Args:
            value (int): new frequency
        """
        if(value>0):
            self._write_data(con = self.con_HZ, value = value)
        else:
            raise ValueError("Frequency must be greater than 0")
        
    def get_frequency(self):
        """gets frequency used by PID-Controller. The value does not represent the actual sampling rate
        """
        return self._read_data(con = self.con_HZ)

    def set_p(self, value):
        """sets P-value of PID controller

        Args:
            value (float): new value

        Raises:
            ValueError: input value must be smaller than 256 and greater or equal to 0 (Values smaller than 1/256 will be rounded to 0 or 1/256)
        """
        if (value > self.PARAM_SHIFT and value >=0):
                raise ValueError("PID-constants must be smaller than "+ str(self.PARAM_SHIFT) + " and greater or equal to 0")
        else:
            self._write_data(con = self.con_P, value = value*self.PARAM_SHIFT)

    def get_p(self):
        """gets P-value of PID-controller
        """
        return self._read_data(con = self.con_P)/self.PARAM_SHIFT

    def set_i(self, value):
        if (value > self.PARAM_SHIFT and value >=0):
                raise ValueError("PID-constants must be smaller than "+ str(self.PARAM_SHIFT) + " and greater or equal to 0")
        else:
            self._write_data(con = self.con_I, value = value*self.PARAM_SHIFT)
    
    def get_i(self):
        """gets I-value of PID-controller
        """
        return self._read_data(con = self.con_I)/self.PARAM_SHIFT

    def set_d(self, value):
        if (value > self.PARAM_SHIFT and value >=0):
                raise ValueError("PID-constants must be smaller than "+ str(self.PARAM_SHIFT) + " and greater or equal to 0")
        else:
            self._write_data(con = self.con_D, value = value*self.PARAM_SHIFT)

    def get_d(self):
        """gets D-value of PID-controller
        """
        return self._read_data(con = self.con_D)/self.PARAM_SHIFT

    def _read_data(self, con):
        """reads data/constants from the arduino.  <con> determines which

        Args:
            con (char): the constant to be read.\\
            p -> proportional;
            i -> integral;
            d -> differential;
            f -> frequecy;

        Returns:
            [float]: constant
        """
        cmd = self.create_command(self.READ_delim, con)
        ans = self.query(cmd)
        if ans == '':
           raise RuntimeError("No answer received")
        #get int from transmitted string
        valuestr = ''.join(x for x in ans if x.isdigit())
        if(len(valuestr)>0):
            value = int(valuestr)
        else:
            raise RuntimeError("Answer does not contain any digits")
        self.reset_buffers()
        return value
    
    def _write_data(self, con, value):
        """writes data to arduino
        Args:
            con (char): constant to be set\\
            p -> proportional;
            i -> integral;
            d -> differential;
            f -> frequecy;
            D -> DAC;
            value (float): new value
        """
        cmd = self.create_command(self.WRITE_delim, con, value)
        self.write(cmd)

    
    
    
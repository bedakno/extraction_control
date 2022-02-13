from irrad_control.devices.arduino import ardser

class SerC(ardser.ArdSer):
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

    def __init__(self, port = "/dev/cu.usbmodem14401", baudrate = 115200, timeout = 1):
        super().__init__(port=port, baudrate=baudrate, timeout=timeout)

    def check_con(self):
        """checks if the arduino is responding

        Raises:
            RuntimeError: One of two things happend: either the arduino is stuck in code and is not responding or the serial connection was not successful
        """
        cmd = self.create_command(self.CHECK_delim, self.CHECK_delim)
        ans = self.query(cmd)
        if ans != 'CC42':
           raise RuntimeError("Serial connection to arduino not established or arduino is not responding")
    def reset_ard(self):
        """resets the arduino via software. not as powerful as arduino must be able to respond\\
            but this restores the default settings
        """
        cmd = self.create_command(self.RESET_delim, self.RESET_delim)
        self.query(cmd)
    
    
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
            self.logging.error("no answer received")
        #get int from transmitted string
        valuestr = ''.join(x for x in ans if x.isdigit())
        if(len(valuestr)>0):
            valuein = int(valuestr)
        else:
            self.logging.error("Answer is empty")
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
        """writes data to arduino

        Args:
            con (char): constant to be set\\
            P -> proportional;
            I -> integral;
            D -> differential;
            F -> frequecy;
            valuein (float): new value
        """
        #transform value to FastPID format (shift 8 bit so no decimal place and multiply with unit)
        cmdhz = self.create_command(self.READ_delim, self.con_HZ)
        hz = self.query(cmdhz)
        if con == 'P':
            value = int(valuein*256)
        if con == 'I':
            value = int((valuein/hz)*256)
        if con == 'D':
            value = int((valuein*hz*256))
        
        #create command string
        cmd = self.create_command(self.WRITE_delim, con, value)
        
        #write data to arduino via serial
        ans = self.query(cmd)

        if ans.contains("R"):
            print("No value input received")
    
    
    
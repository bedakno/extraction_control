from time import time
from data_writer import DataWriter
from adc_board import ADCBoard

class ADCDataWriter(DataWriter, ADCBoard):

    columns = 'tstamp L R U D S_H S_V'.split()

    def __init__(self, outfile, comments = ''):
        """instance to read adc values continously and matching them to a timestamp

        Args:
            outfile (str): name of outputfile
            comments (str, optional): _description_. Defaults to ''.
        """
        ADCBoard.__init__()
        DataWriter.__init__(outfile=outfile, columns=self.columns, comments=comments)
        #setup adc channels 7,6,5,4 to common ground as well as 1,2 and 0,2 as differential inputs to arduino ground
        self.setup_channels([7,6,5,4,(1,2),(0,2)])
        self.time0 = time()

    def _get_values(self):
        """reads adc values matching the columns as well as the current timestamp
        
        Returns:
            _type_: dictionary with column name and matching value
        """
        values = self.read_channels(self.columns[1:])
        timestamp = {'tstamp': time()-self.time0}
        values.update(timestamp)
        return values

    def read_continously(self):
        """write values into file continously until KeyboardInterrupt happens
        """
        try:
            while True:
                self.write_row(**self._get_values())
        except:
            KeyboardInterrupt





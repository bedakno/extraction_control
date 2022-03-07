from time import time
from data_writer import DataWriter
from adc_board import ADCBoard

def read_continously(outputfile, channel_numbers, channel_names, **data_writer_kwargs):
    adc = ADCBoard()
    writer = DataWriter(outfile=outputfile, columns=channel_names, **data_writer_kwargs)
    adc.setup_channels(channel_numbers)
    with writer as w:
        try:
            while True:
                result = adc.read_channels(channel_names)
                w.write_row(tstamp=time(), **result)
        except KeyboardInterrupt:
            pass


if __name__ == "__main__":
    channel_names = 'tstamp L R U D S_H S_V'.split()
    channel_numbers = [7,6,5,4,(1,2),(0,2)]
    outputfile = 'test.csv'
    read_continously(channel_names=channel_names, channel_numbers=channel_numbers, outputfile=outputfile)
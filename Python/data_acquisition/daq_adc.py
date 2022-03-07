from time import time
from data_writer import DataWriter
from adc_board import ADCBoard
import logging
import argparse

# Load data from CSV
parser = argparse.ArgumentParser(
    formatter_class=argparse.RawDescriptionHelpFormatter,
    epilog="")

parser.add_argument('-hz', '--datarate', nargs=1,
                     help='sampling rate')
parser.add_argument('-o', '--output_file', nargs=1,
                    help='name of outputfile')

args = parser.parse_args()

datarateinput = args.datarate[0]
outputfileinput = args.output_file[0]

logging.getLogger().setLevel(logging.INFO)

def read_continously(outputfile, channel_numbers, channel_names, **data_writer_kwargs):
    adc = ADCBoard()
    adc.drate = datarateinput if datarateinput is not None else 100
    writer = DataWriter(outfile=outputfile, columns=channel_names, **data_writer_kwargs)
    adc.setup_channels(channel_numbers)
    start_time = time()
    with writer as w:
        try:
            while True:
                result = adc.read_channels(channel_names[1:])
                w.write_row(tstamp=time(), **result)
                if time() - start_time > 1:
                    logging.info(';' .join(f'{k}={v:.3f} V' for k,v in result.items()))
                    start_time = time()
        except KeyboardInterrupt:
            pass


if __name__ == "__main__":
    channel_names = 'tstamp L R U D S_H S_V'.split()
    channel_numbers = [7,6,5,4,(1,2),(0,2)]
    outputfile = outputfileinput if outputfileinput is not None else 'outputfile_daq_adc.csv' 
    read_continously(channel_names=channel_names, channel_numbers=channel_numbers, outputfile=outputfile, comments = '')
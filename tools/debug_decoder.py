import sys
import os
import re
import serial
import struct
import datetime

DEFAULT_PORT            = 'COM6'
DEFAULT_TIMEOUT         = 1
DEFAULT_BAUDRATE        = 9600
DICTIONARY_FILE_PATH    = "../bsw/services/debug/inc/debug_dictionary.h"

# Get the current working directory
current_dir = os.getcwd()
dictionary_file = os.path.join(current_dir, DICTIONARY_FILE_PATH)

def usage():
    print('Usage:')
    print('  python debug_decoder.py -p <PORT> -b <BAUDRATE> -t <TIMEOUT>')


def parse_dictionary_file(dictionary_file):
    levels = []
    modules = []
    messages = []

    with open(dictionary_file, 'r') as f:
        for line in f:
            # Match enum declarations
            match_enum = re.match(r'typedef enum', line)
            if match_enum:
                enum_values = []
                for line in f:
                    match_value = re.match(r'\s*(\w+)', line)
                    if match_value:
                        enum_values.append(match_value.group(1))
                    else:
                        if re.search(r'\bDebug_LevelType\b', line):
                            levels = enum_values
                            break
                        elif re.search(r'\bDebug_ModuleName\b', line):
                            modules = enum_values
                            break
                        elif re.search(r'\bDebug_MessageID\b', line):
                            messages = enum_values
                            break

    return levels, modules, messages


def decode_message(rawData):
    decoded_message = []

    if (len(rawData) == 8):
        format_string   = "<HHBB"   # 2 uint16, 2 uint8
    elif (len(rawData) == 10):
        format_string   = "<HHHBB"  # 3 uint16, 2 uint8
    else:
        return

    unpacked_data   = struct.unpack(format_string, rawData[:-2])

    decoded_message = list(unpacked_data)

    return decoded_message


def main():
    levels, modules, messages = parse_dictionary_file(dictionary_file)
    nums = []

    port        = DEFAULT_PORT
    baudrate    = DEFAULT_BAUDRATE
    timeout     = DEFAULT_TIMEOUT

    if (len(sys.argv) > 1):
        for i in range(1, len(sys.argv)):
            if (sys.argv[i] == '-p'):
                port     = sys.argv[i + 1].upper()
            if (sys.argv[i] == '-b'):
                baudrate = int(sys.argv[i + 1])
            if (sys.argv[i] == '-t'):
                timeout  = int(sys.argv[i + 1])
            if (sys.argv[i] == '-h'):
                usage()
                exit()

    ser = serial.Serial(port = port, baudrate = baudrate, timeout = timeout)

    with open('decode_message.log', 'w') as f:
        while True:
            nums = decode_message(ser.readline())

            now = datetime.datetime.now()
            if (len(nums) == 4):
                f.write("[%s] [%s][%s.c:%s][%s]\n" %(now.strftime("%Y-%m-%d %H:%M:%S:%f")[:-3], levels[nums[2]], modules[nums[3]].lower(), nums[1], messages[nums[0]]))
            elif (len(nums) == 5):
                f.write("[%s] [%s][%s.c:%s][%s][%s]\n" %(now.strftime("%Y-%m-%d %H:%M:%S:%f")[:-3], levels[nums[3]], modules[nums[4]].lower(), nums[2], messages[nums[1]], nums[0]))


main()

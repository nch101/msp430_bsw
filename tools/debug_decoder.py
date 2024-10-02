import sys
import os
import re
import serial

DEFAULT_PORT            = 'COM5'
DEFAULT_TIMEOUT         = 1
DEFAULT_BAUDRATE        = 19200
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
    integer_array = []

    data_list = rawData.decode().split()

    try:
        for value in data_list:
            integer_array.append(int(value))
    except ValueError:
        print("Invalid data received:", value)

    return integer_array


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

            if (len(nums) == 4):
                f.write("[%s][%s.c:%s][%s]\n" %(levels[nums[0]], modules[nums[1]].lower(), nums[2], messages[nums[3]]))
            elif (len(nums) == 5):
                f.write("[%s][%s.c:%s][%s][%s]\n" %(levels[nums[0]], modules[nums[1]].lower(), nums[2], messages[nums[3]], nums[4]))


main()

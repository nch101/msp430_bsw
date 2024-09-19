import sys
import os
import re
import serial

PORT                    = 'COM5'
TIMEOUT                 = 1
BAUDRATE                = 19200
DICTIONARY_FILE_PATH    = "../bsw/services/debug/inc/debug_dictionary.h"

# Get the current working directory
current_dir = os.getcwd()
dictionary_file = os.path.join(current_dir, DICTIONARY_FILE_PATH)

def usage():
    print('Usage:')
    print('  python debug_decoder.py <Path-to-debug-file> <Path-to-decoded-file>')


def is_arg_valid():
    if len(sys.argv) < 3:
        print('Missing one more args')
        usage()
        return False, "", ""

    # Get file path
    debug_file  = os.path.realpath(sys.argv[2])
    decode_file = os.path.realpath(sys.argv[3])

    if not os.path.exists(debug_file):
        print('ERROR: The debug file {} does not exists.'.format(debug_file))
        usage()
        return False, "", ""

    return True, debug_file, decode_file


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


def decode_message(line, levels, modules, messages):
    paths = line.split(":")
    nums = list(map(int, paths))

    if (len(nums) == 4):
        return ("[%s][%s.c:%s][%s]\n" %(levels[nums[0]], modules[nums[1]].lower(), nums[2], messages[nums[3]]))
    else:
        return ("[%s][%s.c:%s][%s][%s]\n" %(levels[nums[0]], modules[nums[1]].lower(), nums[2], messages[nums[3]], nums[4]))

def main():
    decode_data = []

    levels, modules, messages = parse_dictionary_file(dictionary_file)
    option = sys.argv[1]

    if (option == '-f'):
        status, debug_file, decode_file = is_arg_valid()

        if (status == False):
            exit(0)

        with open(debug_file, 'r') as file:
            for line in file:
                decode_data.append(decode_message(line, levels, modules, messages))

        with open(decode_file, "w") as file:
            file.writelines(decode_data)
    elif (option == "-s"):
        ser = serial.Serial(port = PORT, bytesize = 8, timeout = TIMEOUT)
        ser.baudrate = BAUDRATE 

        while (True):
            line = ser.readline()
            print(decode_message(line, levels, modules, messages))

    else:
        print("Unknown option")
        exit(0)

main()

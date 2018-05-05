#!/usr/bin/python
import time
import datetime
import sys
import getopt
import os
 
a_time = None
s_second = None
s_microsecone = None
abs_time = 0.0
inputfile = None
outputfile = None
 
def usage():
    print('''Help Information:
             -h, --help:        Show help information
             -i, --inputfile:   input file  to parse
             -o, --outputfile:  output fiel parsed
                        ''')


def calc_delta(stream):
    global s_second
    global s_microsecond
    global a_time
    global outfile
    a_time_op = None
    begin_index = None
    end_index = None
    delta_second = 0
    delta_mircosecond = 0
    dd_time = 0
    ss_time = 0
    d_time = None
    new_line = None
    if a_time ==None:
        print("Can't convert to android time")
        exit(-1)
    for line in stream:
        if line:
            a_time_op = line.find('android time')
            if a_time_op>=1:
                begin_index =  line.index('[')
                end_index = line[begin_index+1:].index(']')+begin_index+1
                date_string = line[a_time_op+13:].strip()
                abs_time = line[begin_index + 1 :end_index]
                [s_second,s_microsecond] = abs_time.split('.')
                a_time = datetime.datetime.strptime(date_string, "%Y-%m-%d %H:%M:%S.%f")
                a_time_op = 0
        if line:
            try:
                begin_index =  line.index('[')
                end_index = line[begin_index+1:].index(']')+begin_index+1
                time_string = line[begin_index + 1 :end_index]
                [d_second,d_microsecond] = time_string.split('.')
                dd_time = datetime.timedelta(seconds=int(d_second),microseconds=int(d_microsecond))
                ss_time = datetime.timedelta(seconds=int(s_second),microseconds=int(s_microsecond))
#                print "/////////////////"
#                print a_time
#                print dd_time 
#                print ss_time
                d_time = a_time + dd_time - ss_time
#                print d_time
                new_line = d_time.strftime("%m-%d %H:%M:%S.%f")+' ' + line
                outputfile.write(new_line)
            except:
                outputfile.write(line)


def get_atime(stream):
    global s_second
    global s_microsecond
    global a_time
    a_time_op = None
    begin_index = None
    end_index = None
    for line in stream:
        if line:
            a_time_op = line.find('android time')
            if a_time_op>=1:
                begin_index =  line.index('[')
                end_index = line[begin_index+1:].index(']')+begin_index+1
                date_string = line[a_time_op+13:].strip()
                abs_time = line[begin_index + 1 :end_index]
                [s_second,s_microsecond] = abs_time.split('.')
                a_time = datetime.datetime.strptime(date_string, "%Y-%m-%d %H:%M:%S.%f")
                a_time_op = 0
                break

def main(argv):
    global inputfile
    global outputfile
    inputpath = None
    outputpath = None
    try:
        opts, args = getopt.getopt(argv,"hi:o:",["help","inputfile=","outputfile="])
    except getopt.GetoptError:
        usage()
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
            sys.exit()
        if opt in ("-i", "--inputfile"):
            inputpath = arg
        if opt in ("-o", "outputfile"):
            outputpath = arg
    if inputpath == None:
        usage()
        sys.exit()
    if outputpath == None:
        outputpath = os.getcwd()+"/out.txt"

    inputfile = open(inputpath, 'r')
    outputfile = open(outputpath, 'w')
    get_atime(inputfile)
    inputfile.seek(0)
    calc_delta(inputfile)
    inputfile.close()
    outputfile.close()
if __name__ == "__main__":
    main(sys.argv[1:])

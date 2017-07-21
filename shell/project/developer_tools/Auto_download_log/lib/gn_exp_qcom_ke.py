#!/usr/bin/python
import struct
import sys
import time
import os
import shutil
import getopt
import zipfile

'''
if len(sys.argv) < 2:
    print('Arguments wrong!')
    sys.exit(1)
'''

# common global define
exp_info = ''
debug = 0

# KE global define
GN_DUMP_VAILD_MASK = 0x00000001
GN_DUMP_INSUFFICIENT_STORAGE_MASK = 0x00000002
MAX_DUMP_SIZE = 0x200000

db_header_size = 128
vm_info_size = 56
dump_header_size = 72
section_header_size = 48
contexts_offset = db_header_size + vm_info_size - 512 

dmesg_raw_file = 'DDRCS0.BIN'
last_kmsg_file = 'SYS_LAST_KMSG'
exp_main_file = '__exp_main.txt'
dump_head = ''
vminfo = ''


def create_dir(infile,outdir):
    #create dir
    if outdir[-1] != '/':
        outdir += '/'
    #time_str = outdir + time.strftime('%Y-%m-%d %H.%M.%S',time.localtime(time.time()))
    time_str = outdir
    if not os.path.exists(time_str): os.mkdir(time_str)
    new_db = time_str + '/' + os.path.split(infile)[1]
    print(new_db)
    shutil.copyfile(infile,new_db)
    #change current work dir
    return time_str
    #os.chdir(time_str)

def ke_exp_info():
    trace_pos_start = -1
    trace_pos_end = -1
    state = 'none'
    global exp_info
    fp = open(last_kmsg_file,"rb")
    while 1:
        lines = fp.readline()
        if lines == '':
            if trace_pos_end == -1:
                trace_pos_end = fp.tell()
            break
        if 'Internal error:' in lines and state == 'none':
            state = 'found KE'
        if 'PC is at' in lines and state == 'found KE':
            exp_info = lines[lines.find('PC'):]
            state = 'found PC'
        if 'Call trace' in lines and state == 'found PC':
            trace_pos_start = fp.tell()
            state = 'found Call trace'
        if 'Kernel panic - not syncing' in lines:
            if state == 'found Call trace':
                trace_pos_end = fp.tell()
                state = 'found Kernel panic'
                break
            elif 'Resetting the SoC' in lines:
                state = 'found subsys crash'
                exp_info = 'PC is at ' + lines[lines.find('Kernel'):]
                trace_pos_end = fp.tell()
                fp.seek(0)
            else:
                state = 'found other crash'
                exp_info = 'PC is at ' + lines[lines.find('Kernel'):]

        if 'subsystem failure reason' in lines and state == 'found subsys crash':
            trace_pos_start = fp.tell() - len(lines)
            break

    if trace_pos_start != -1 and trace_pos_end != 1 and trace_pos_start < trace_pos_end:
        fp.seek(trace_pos_start)
        trace_info = fp.read(trace_pos_end - trace_pos_start)
        #print trace_info
        exp_info = exp_info + '\r\nBacktrace:\r\n' + trace_info
    fp.close()

def unzip(zipfilename,unzipdirname):
    print 'Start to unzip file %s to folder %s ...' % (zipfilename, unzipdirname) 
    srcZip = zipfile.ZipFile(zipfilename, "r")  
    for eachfile in srcZip.namelist():  
        print "Unzip file %s ..." % eachfile  
        eachfilename = os.path.normpath(os.path.join(unzipdirname, eachfile))  
        #eachdirname = os.path.dirname(eachfilename)  
        #if not os.path.exists(eachdirname):  
        #    os.makedirs(eachdirname)  
        fd=open(eachfilename, "wb")  
        fd.write(srcZip.read(eachfile))  
        fd.close()  
    srcZip.close()

def JE_SWT(infile,outdir):
    unzip(infile,outdir)



def exp_main(header):
    #write version.txt
    fp_ver = open(exp_main_file,"wb")
    fatal = "$** *** *** *** *** *** *** *** Fatal *** *** *** *** *** *** *** **$\r\n\r\n"
    types,exptime,gnznvernumber,build_date,reverved = struct.unpack("8s32s32s32s24s",header)
    types = types.strip(chr(0))
    gnznvernumber = gnznvernumber.strip(chr(0))
    build_date = build_date.strip(chr(0))
    exptime = exptime.strip(chr(0))
    exptime = exptime[0:5] + '01' + exptime[7:]
    timesec = time.mktime(time.strptime(exptime,"%Y-%m-%d %H:%M:%S %Z"))
    exptime = time.strftime('%a %b %d %H:%M:%S %Z %Y',time.localtime(timesec))
    expsec = 0.000000
    version = "Build Info: '%s %s'\r\nException Log Time:[%s] [%f]\r\n\r\nException Class:  %s\r\n%s\r\n" \
    % (gnznvernumber,build_date,exptime,expsec,types,exp_info)
    fp_ver.write(fatal)
    fp_ver.write(version)
    fp_ver.write(fatal)
    fp_ver.close()

def KE(fp):
    #read vm_info
    global vminfo
    vminfo = fp.read(vm_info_size)
    '''
    dload_mode_phy,log_buf_phy,log_first_idx_phy,log_next_idx_phy,log_buf_len,struct_log_size,struct_log_ts_nsec_offset,\
    struct_log_len_offset,struct_log_text_len_offset,log_first_idx_bits,log_next_idx_bits,struct_log_ts_nsec_bits,\
    struct_log_len_bits,struct_log_text_len_bits,dload_mode_bits = struct.unpack("4Qi10H",vminfo)

    print(log_buf_len,struct_log_size,struct_log_ts_nsec_offset,struct_log_len_offset,struct_log_text_len_offset\
        ,struct_log_ts_nsec_bits,struct_log_len_bits,struct_log_text_len_bits)
    '''

    #read dump_header
    global dump_head
    dump_head = fp.read(dump_header_size)
    signature,dumpversion,validity_flag,dump_size,sections_count,log_buf_len2,log_first_idx,log_next_idx,reverved = \
        struct.unpack("8s4i3Q24s",dump_head)

    print(signature.strip(chr(0)),dumpversion,validity_flag,dump_size,sections_count,log_buf_len2,log_first_idx,log_next_idx)

    if not validity_flag & GN_DUMP_VAILD_MASK:
        print('invail dumper')
        fp.close()
        sys.exit(1)
    elif validity_flag & GN_DUMP_INSUFFICIENT_STORAGE_MASK:
        print('dump insufficant storage')
        fp.close()
        sys.exit(1)
    elif dump_size == 0 or dump_size >  MAX_DUMP_SIZE:
        print('dump_size( = {0}) invaild'.format(dump_size))
        fp.close()
        sys.exit(1)
    i = 0
    while i < sections_count:
        parse_sections(fp,contexts_offset)
        i += 1
    #fp.close()    
    #print 'parsing success'


#parse a section
def parse_sections(fp,offset):

    # backup cur file pos
    cur_pos = fp.tell()

    section = fp.read(section_header_size)
    section_name,section_offset,section_size,base_addr,validity_flag_s,reverved_u8 = \
        struct.unpack("16s3Qi4s",section)
    print(section_name.strip(chr(0)),section_offset,section_size,base_addr,validity_flag_s)


    # backup cur file pos
    cur_pos = fp.tell()

    #read the section
    fp.seek(section_offset+offset)
    contexts = fp.read(section_size)

    fp.seek(cur_pos)

    #get the section name
    file_name = section_name.strip(chr(0))
    #print file_name
    fpw = open(file_name,"wb")
    fpw.write(contexts)
    fpw.close()
    if dmesg_raw_file in file_name:
        dmesg(file_name)

def dmesg(raw_file):
    #print raw_file
    fp = open(raw_file,"rb")
    wrap_cnt = 0
    fpw = open(last_kmsg_file,"wb")

    dload_mode_phy,log_buf_phy,log_first_idx_phy,log_next_idx_phy,log_buf_len,struct_log_size,struct_log_ts_nsec_offset,\
    struct_log_len_offset,struct_log_text_len_offset,log_first_idx_bits,log_next_idx_bits,struct_log_ts_nsec_bits,\
    struct_log_len_bits,struct_log_text_len_bits,dload_mode_bits = struct.unpack("4Qi10H",vminfo)

    signature,dumpversion,validity_flag,dump_size,sections_count,log_buf_len2,log_first_idx,log_next_idx,reverved = \
        struct.unpack("8s4i3Q24s",dump_head)

    cur_idx = log_first_idx
    i = 0
    while cur_idx != log_next_idx and wrap_cnt < 2:
        #print cur_idx
        fp.seek(cur_idx)
        log_head = fp.read(struct_log_size)
        ts_nsec,length,text_len,dict_len,facility,flag = struct.unpack("Q3Hss",log_head)
        #print ts_nsec,length,text_len
        if wrap_cnt > 0 and cur_idx + length > log_first_idx:
            print('cur_idx = {0} is invaild'.format(cur_idx))
            break
        if length == 0:
            cur_idx = 0
            wrap_cnt += 1
            continue
        fp.seek(cur_idx + struct_log_size)
        text_str = fp.read(text_len)
        for partial in text_str.split('\n'):
            f = '[{0:>5}.{1:0>6d}] {2}\n'.format(
                ts_nsec / 1000000000,(ts_nsec % 1000000000) / 1000,partial)
            #print f
            fpw.write(f)
        cur_idx = cur_idx + length
    fpw.close()    
    fp.close()

def help():
    print('Example of acceptable Arguments:')
    print(sys.argv[0]+' -i <inputfile> -o <outputdir>')
    print(sys.argv[0]+' inputfile')


if __name__ == '__main__':

    inputfile = ''
    outputdir = './'
    '''
    try:
        inputfile = sys.argv[1]
        outputdir = './'
    except:
        print('Example of acceptable Arguments:')
        print(sys.argv[0]+' -i <inputfile> -o <outputdir>')
        print(sys.argv[0]+' inputfile')
        sys.exit(1)
        '''
    
    try:
        opts,args = getopt.getopt(sys.argv[1:],'hi:o:',['infile=','outdir=','help'])
    except getopt.GetoptError:
        help()
        sys.exit(2)

    if debug == 1:
        print opts,args

    for opt,arg in opts:
        if opt in ('-h','--help'):
            help()
            sys.exit(2)
        elif opt in ('-i','--infile'):
            inputfile = arg
        elif opt in ('-o','--outdir'):
            outputdir = arg
    if debug == 1:
        print('inputfile: ' + inputfile)
        print('outputdir: ' + outputdir)
    if inputfile == '':
        if len(sys.argv) == 2:
            inputfile = sys.argv[1]
        else:
            help()
            sys.exit(2)
    outdir = create_dir(inputfile,outputdir)
    os.chdir(outdir)
    #JE SWT
    ext = os.path.splitext(inputfile)[1]
    if ext in ('.JE','.SWT'):
        JE_SWT(inputfile,'.')
        sys.exit(0)

    #KE
    #read db header
    fp = open(os.path.split(inputfile)[1],"rb")
    header = fp.read(db_header_size)
    if 'KE' == header[0:2]:
        print('Exception Class: ' + header[0:2])
        KE(fp)
        ke_exp_info()
    exp_main(header)
    fp.close()





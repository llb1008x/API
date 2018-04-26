#!/usr/bin/python
#-*- coding: utf-8 -*-
import re
import os
import sys
import zipfile
import commands

err=[]
def split_android(mk,data):
    b_index=0
    e_index=0
    c_index=0
    begin_pattern=re.escape('include $(CLEAR_VARS)')
    end_pattern=re.escape('include $(BUILD_PREBUILT)')
    with open(mk,'r') as f :  lines = f.readlines()

    for i in range(len(lines)):
        str=lines[i].strip()
        if not str or str.startswith('#'): continue

        #find begin
        b_match=re.match(begin_pattern,str)
        if b_match:
            b_index = i
            continue

        #find end
        e_match=re.match(end_pattern,str)
        if e_match: 
            e_index = i

        if b_index < e_index:
            #print b_index, e_index+1
            data[c_index]=lines[b_index:e_index + 1]
            c_index += 1
            b_index = e_index
    return data

def expand_line(lines):
    l=[]
    special_line=''
    is_special_line=False
    for line in lines:
        line=line.strip()
        line=line.replace(":=","=")
        line=line.replace("+=","=")
        if line.endswith('\\'):
            special_line+="%s "%line.rstrip('\\').strip()
            is_special_line=True
        else:
            if is_special_line:
                special_line+="%s "%line.strip()
                l.append(special_line)
                is_special_line=False
            else:
                l.append(line)
    return l

def check_copy_files(list,local_path):
    global err
    for line in list: 
        key = line.split('=')[0].strip()
        value = line.split('=')[-1].strip()
        if key == "PRODUCT_COPY_FILES":
            if value.startswith('$(foreach'): continue
            for v in value.split():
                if v.startswith('#') : continue
                v=v.replace('$(LOCAL_PATH)',local_path)
                v=v.split(':')[0]
                if not os.path.exists(v):
                    err.append("CheckError: not config %s in %s\n"%(v,local_path))

def check_jni_files(list,local_path):
    global err
    local_prebuilt_jni_libs=''
    for line in list: 
        #print line
        key = line.split('=')[0].strip()
        value = line.split('=')[-1].strip()
        if key == "LOCAL_MODULE":
            local_module=value
        elif key == "LOCAL_SRC_FILES":
            local_src_files=value.replace("$(LOCAL_MODULE)",local_module)
        elif key == "LOCAL_PREBUILT_JNI_LIBS":
            local_prebuilt_jni_libs +="%s " %value

    #print local_module
    #print local_src_files
    #print local_prebuilt_jni_libs

    if local_prebuilt_jni_libs:
        for v in local_prebuilt_jni_libs.split():
            if v.startswith('#') : continue
            if v[0] == '@':
                #print local_src_files
                status, out = commands.getstatusoutput("zipinfo -1 %s/%s"%(local_path,local_src_files))
                if v[1:] not in out.split('\n'):
                    err.append("CheckError: not jni %s in %s/%s\n"%(v,local_path,local_src_files))
            else:
                if not os.path.exists("%s/%s"%(local_path,v)):
                    err.append("CheckError: not lib %s in %s\n"%(v,local_path))

def main(argv):
    global err
    data={} 
    android_file=argv[1]
    android_path=os.path.dirname(android_file)
   #not check GMS apk
    if 'packages/apps/3rd_GMS/' in android_path: return
    data = split_android(android_file,data)
    for n,l in data.items():
       l=expand_line(l)
       check_copy_files(l,android_path)
       check_jni_files(l,android_path)
    if err:
        sys.stdout.write('Sign to MakeFile\n')
        for e in err:
            sys.stderr.write(e)

if __name__ == '__main__':
    main(sys.argv)

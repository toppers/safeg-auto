#!/usr/bin/env python3
import sys
import os
import re

if len(sys.argv) != 2:
    print("Argment Error!")
    sys.exit(1)

#Read from Input File 
with open(sys.argv[1]) as f:
    s_lines = f.read()

#Convert

s_lines = re.sub(r'.section\s*\.(\w+)\.(\w+)', r'\1 .cseg \2', s_lines)

s_lines = re.sub(r'.section\s*\.(\w+)', r'.cseg \1', s_lines)

s_lines = re.sub(r'/\*', ';/*', s_lines)  

s_lines = re.sub(r'^(\s*)\*', r'\1;*', s_lines, flags=re.MULTILINE)

s_lines = re.sub('.global', '.extern', s_lines)    
           
s_lines = re.sub('~', '!', s_lines)
    
s_lines = re.sub(r'#include\s+[<"](.+)[>"]', r'$include (\1)', s_lines)

s_lines = re.sub('#ifdef', '$ifdef', s_lines)

s_lines = re.sub('#ifndef', '$ifndef', s_lines)    

s_lines = re.sub('#endif', '$endif', s_lines)

s_lines = re.sub('#else', '$else', s_lines)    

s_lines = re.sub(r'.macro\s+(\w+)', r'\1 .macro', s_lines)

s_lines = re.sub(r'.endr', '.endm', s_lines)

s_lines = re.sub(r'pushsp\s*(\w+)-(\w+)', r'pushsp \1, \2', s_lines)
s_lines = re.sub(r'popsp\s*(\w+)-(\w+)', r'popsp \1, \2', s_lines)

s_lines = re.sub(r'\#define[ \t](\w+)[ \t]+(.+)', r'    \1 .set \2', s_lines)

s_lines = re.sub(r'\#define[ \t](\w+)', r'    \1 .set 1', s_lines)

s_lines = re.sub('hv_cfg_asm.h', 'hv_cfg_asm.inc', s_lines)
s_lines = re.sub('hv_asm.h', 'hv_asm.inc', s_lines)

#Output File
file_extension = {'.S' : '.asm', '.h' : '.inc'}
path_w = os.path.splitext(sys.argv[1])[0] + file_extension[os.path.splitext(sys.argv[1])[1]]
with open(path_w, mode="w", encoding='utf-8') as f:
    f.write(s_lines)

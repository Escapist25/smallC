#!/bin/sh
./small $1
python splitcode.py
python gencode.py
cp finalcode.s MIPSCode.s
cp InterCode1.txt InterCode

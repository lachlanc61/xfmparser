#!/bin/bash

#dumb construction script for swig
#creates all files in default locations then moves to final locs
#	all locs relative, run from construction directory
#all origin files in /gfg
#final results go in .

python --version

echo $PWD

swig -python gfg/gfg.i
echo "swig fin"

gcc -c -fpic gfg/gfg.c gfg/gfg_wrap.c -I/home/lachlan/.pyenv/versions/3.10.6/include/python3.10
echo "gcc1 fin"


gcc -shared gfg.o gfg_wrap.o -o build/_gfg.so
echo "gcc2 complete"

mv gfg/gfg.py build
mv gfg/gfg_wrap.c gfg.o gfg_wrap.o build

#test:
#python
#>>import build.gfg as gfg
#>>gfg.cvar.myvar
#>> 3.4


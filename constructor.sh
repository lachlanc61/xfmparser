#!/bin/bash

python setup.py build_ext --build-lib=src 

return=$?

echo $return

if [[ $return == 0 ]];
then
    python main.py
fi
#--inplace

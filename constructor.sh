#!/bin/bash

python setup.py build_ext --build-lib=parsercore 

return=$?

echo $return

if [[ $return == 0 ]];
then
    python main.py
fi
#--inplace

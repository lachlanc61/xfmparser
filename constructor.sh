#!/bin/bash

result = $( python setup.py build_ext --build-lib=parsercore )

if [[ $result == 0 ]];
then
    python main.py
fi
#--inplace

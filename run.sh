#!/bin/bash

g++ -pthread -std=c++17 parallel_read.cpp -o parallel_read && \
	"/home/lachlan/CODEBASE/Xfmparser/"parallel_read datasets/yahtzee-upper-big2.txt

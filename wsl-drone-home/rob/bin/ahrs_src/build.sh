#! /bin/bash
g++ -o /home/rob/bin/ahrs_data main.cpp -I"/usr/share/c++-mscl/source/" -I"/usr/share/c++-mscl/Boost/include/" -L"/usr/share/c++-mscl" -lmosquitto -lmscl -lstdc++ -std=c++11 -fpermissive

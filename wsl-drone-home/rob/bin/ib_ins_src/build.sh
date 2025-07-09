#! /bin/bash
ldconfig
g++ -o /home/rob/bin/ib_ins udp_listener.cpp  -lmosquitto -lstdc++ -std=c++11 -L/usr/local/lib -lixblue_stdbin_decoder -lpthread -lboost_system

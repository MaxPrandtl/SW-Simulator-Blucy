
#! /bin/bash

#cd mqtt2sock-main
#python3 mqtt2sock.py & 
#cd ..

cd connecto-main
python3 connecto.py &
cd ..

cd sock2mqtt-main 
python3 sock2mqtt.py &
cd ..


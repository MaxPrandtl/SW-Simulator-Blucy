#!/bin/bash

cd /home/rob/bin
sudo ./sync

cd /home/rob/bin
sudo ./go &

#cd /home/rob/Roboworld/sushidrop
#sudo ./start_sushidrop.sh &

cd /home/rob/Demoni/Marco
sudo ./dvl.sh &

wait


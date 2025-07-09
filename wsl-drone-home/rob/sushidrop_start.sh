#!/bin/bash

cd /home/rob/sync
sudo ./sync


cd /home/rob/bin
sudo ./go.sh &

cd /home/rob/Roboworld/sushidrop
sudo ./start_roboworld.sh &

cd /home/rob/Demoni/Marco
sudo ./dvl.sh &


wait


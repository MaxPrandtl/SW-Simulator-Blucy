#!/bin/bash

cd /home/rob/bin
sudo ./go &

cd /home/rob/Roboworld/sushidrop
sudo ./start_sushidrop.sh &

wait


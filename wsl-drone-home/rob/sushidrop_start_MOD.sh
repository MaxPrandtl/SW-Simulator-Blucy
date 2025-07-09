#!/bin/bash
#Se tutto dovesse andare a Troie usa questo script per far girare tutto sulla prima scheda 

#cd /home/rob/sync
#sudo ./sync

cd /home/rob/bin
sudo ./go.sh &

cd /home/rob/Roboworld/sushidrop
sudo ./start_roboworld_MOD.sh &

#cd /home/rob/Demoni/Marco
#sudo ./dvl.sh &

wait


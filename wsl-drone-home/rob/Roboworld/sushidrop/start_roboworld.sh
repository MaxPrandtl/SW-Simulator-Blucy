#!/bin/bash

BASE_DIR="/home/rob"
OUTPUT_DIR="bin/x86/Sushidrop"

# clock
cd $BASE_DIR/Roboworld/sushidrop/Clock/$OUTPUT_DIR
sudo ./Clock.out $BASE_DIR/Roboworld/sushidrop/Clock/robots/sushidrop/config/ &


# drivers



# logger
# cd $BASE_DIR/Roboworld/sushidrop/Logger/$OUTPUT_DIR
# sudo ./Logger.out $BASE_DIR/Roboworld/sushidrop/Logger/robots/sushidrop/config/ &

wait



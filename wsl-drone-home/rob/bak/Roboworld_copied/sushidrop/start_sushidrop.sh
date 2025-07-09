#!/bin/bash

BASE_DIR="/home/rob"
OUTPUT_DIR="bin/x86/Debug"

# clock
cd $BASE_DIR/Roboworld/sushidrop/Clock/$OUTPUT_DIR
sudo ./Clock.out $BASE_DIR/Roboworld/sushidrop/Clock/robots/sushidrop/config/ &


# drivers


# thrust mapping
cd $BASE_DIR/Roboworld/sushidrop/ThrustMapping/$OUTPUT_DIR
sudo ./ThrustMapping.out $BASE_DIR/Roboworld/sushidrop/ThrustMapping/robots/sushidrop/config/ &


# measure routing
cd $BASE_DIR/Roboworld/sushidrop/MeasureRouting_Ang/$OUTPUT_DIR
sudo ./MeasureRouting_Ang.out $BASE_DIR/Roboworld/sushidrop/MeasureRouting_Ang/robots/sushidrop/config/ &

cd $BASE_DIR/Roboworld/sushidrop/MeasureRouting_HorPos/$OUTPUT_DIR
sudo ./MeasureRouting_HorPos.out $BASE_DIR/Roboworld/sushidrop/MeasureRouting_HorPos/robots/sushidrop/config/ &

cd $BASE_DIR/Roboworld/sushidrop/MeasureRouting_HorVel/$OUTPUT_DIR
sudo ./MeasureRouting_HorVel.out $BASE_DIR/Roboworld/sushidrop/MeasureRouting_HorVel/robots/sushidrop/config/ &

cd $BASE_DIR/Roboworld/sushidrop/MeasureRouting_VerPos/$OUTPUT_DIR
sudo ./MeasureRouting_VerPos.out $BASE_DIR/Roboworld/sushidrop/MeasureRouting_VerPos/robots/sushidrop/config/ &

cd $BASE_DIR/Roboworld/sushidrop/MeasureRouting_Alt/$OUTPUT_DIR
sudo ./MeasureRouting_Alt.out $BASE_DIR/Roboworld/sushidrop/MeasureRouting_Alt/robots/sushidrop/config/ &


# NGC
cd $BASE_DIR/Roboworld/sushidrop/NGC/$OUTPUT_DIR
sudo ./NGC.out $BASE_DIR/Roboworld/sushidrop/NGC/robots/sushidrop/config/ &


# filters
cd $BASE_DIR/Roboworld/sushidrop/Filter_Alt/$OUTPUT_DIR
sudo ./Filter_Alt.out $BASE_DIR/Roboworld/sushidrop/Filter_Alt/robots/sushidrop/config/ &


# controls
cd $BASE_DIR/Roboworld/sushidrop/Control_AngPos/$OUTPUT_DIR
sudo ./Control_AngPos.out $BASE_DIR/Roboworld/sushidrop/Control_AngPos/robots/sushidrop/config/ &

cd $BASE_DIR/Roboworld/sushidrop/Control_AngVel/$OUTPUT_DIR
sudo ./Control_AngVel.out $BASE_DIR/Roboworld/sushidrop/Control_AngVel/robots/sushidrop/config/ &

cd $BASE_DIR/Roboworld/sushidrop/Control_VerPos/$OUTPUT_DIR
sudo ./Control_VerPos.out $BASE_DIR/Roboworld/sushidrop/Control_VerPos/robots/sushidrop/config/ &


# guidance

# path planner


# logger
cd $BASE_DIR/Roboworld/sushidrop/Logger/$OUTPUT_DIR
sudo ./Logger.out $BASE_DIR/Roboworld/sushidrop/Logger/robots/sushidrop/config/ &

wait



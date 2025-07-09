#! /bin/bash


killall mqtt_clock

killall mqtt_ana_dig_io

killall mqtt_us_imu

killall mqtt_thrusters

killall mqtt_pax00

killall mqtt_mini_ct

killall mqtt_mini_svs

killall mqtt_ib_ins

killall python3

killall ib_ins

killall ahrs_data

killall go.sh


ssh 192.168.29.112 shutdown -t now



mosquitto_sub -t "us_imu/utcTimestampNanoseconds" -t "us_imu/latitude" -t "us_imu/longitude" -C 10 &#|cut -d ' ' -f 1 &
#echo $Pippo
wait
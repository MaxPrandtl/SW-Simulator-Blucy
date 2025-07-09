# FIRMWARE STREAMER

## PLATFORM

|     |   |
| :--- | :--- |
| **OS**   | Linux |
| **Kernel** | 4.15.0-136-generic |
| **Distributor** | Ubuntu |
| **Release** | 18.04 |
| **Codename** | bionic |
| **Architecture** | amd64 |


## BUILD DEMO
```
cd firmware_streamer/
mkdir build
cd build/
cmake ..
make
```

## RUN DEMO
#### Terminal 1: Start 
```
cd build/
./run_connection ../config/connections/DVL.json
```

## TEST DEMO
#### terminal 1: start borker mqtt 
```
cd script/mqtt/
mosquitto -c mosquitto.conf
```

#### terminal 2: start clock 
```
cd script/mqtt/
bash run_clock.sh
```

#### terminal 3: start udp server 
```
cd script/udp/
./udp_server_async
```

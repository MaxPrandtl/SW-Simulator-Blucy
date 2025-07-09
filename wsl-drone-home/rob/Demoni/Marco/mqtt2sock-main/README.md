# MQTT2SOCK

compile the config.json as follows: 


    {
        "id": <id>,
        "host": <mqtt host>,
        "connections": [{
            "incoming": [<mqtt topic>, <mqtt topic>, ...],
            "outgoing": [{
                "protocol": <protocol>, // can be tcp, udp, socket
                "host": <host>, // if tcp, udp
                "port": <port>, // if tcp, udp
                "socket": <socket>, //if socket
            }, ...]
        }, ...]
    }
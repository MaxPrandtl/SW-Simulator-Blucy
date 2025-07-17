from pathlib import Path

DEFAULT_CONFIG = {
    "broker-ip": "192.168.29.111",  # ip blucy
    "topics": [
        "us_imu/#",
        "ib_ins/#",
        "miniSVS/#",
        "miniCT/#",
        "HMI/#",
        "clock/#",
        "thrusters/#",
        "pa200/#",
        "NGC/#",
        "filters/#",
        "controls/#",
        "telemetry/#",
        "sensors/#",
    ],
    "default-export-format": "csv",
    "logger-main-path": "/Users/bryanvespignani/Documents/Coding/Python/Tesi/scripts/logger-python/main.py",
    "exporter-path": "/Users/bryanvespignani/Documents/Coding/Python/Tesi/scripts/logger-python/Exporter.py",
    "subscriber-run-script-path": "~/Desktop/run_sub",
    "subscriber-stop-script-path": "~/Desktop/stop_sub",
    "python-version": "python3",
    "max-line-file": "100000",
    "default-z-offset": "-0.77",
    "ground-machine-ip": "192.168.29.234",
    "color-theme": "dark",
    "danger-threshold": "2",
    "warning-threshold": "5",
    "svlogger-main-path": "",
    "svexporter-path": "",
    "aoi-endpoint": "https://tuo.server.com/api",
}

CONFIG_DIR = Path.home().joinpath(".blucy-logger")

CONFIG_FILE_PATH = CONFIG_DIR.joinpath("config.json")

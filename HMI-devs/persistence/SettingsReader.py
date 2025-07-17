from pathlib import Path
from json import load, dumps


class ConfigReader:
    def __init__(self, config_file_path=None) -> None:
        if config_file_path == None:
            raise ValueError("Configuration file path is none")
        elif not Path(config_file_path).is_file():
            raise FileNotFoundError("Path is not a file")

        self.file_path = config_file_path

    def read(self):
        with open(self.file_path, "r") as f:
            data = load(f)

        return data

    def update(self, new_config):
        if new_config == None:
            raise ValueError("New config is None")

        with open(self.file_path, "w") as f:
            f.write(dumps(new_config))

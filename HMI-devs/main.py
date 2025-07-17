import sys
from pathlib import Path
from json import dumps
from dotenv import load_dotenv
from PySide6 import QtWidgets
from controller.Controller import Controller
from view.MainWindow import MainWindow
from persistence.SettingsReader import ConfigReader
from persistence.DefaultConfig import DEFAULT_CONFIG, CONFIG_DIR, CONFIG_FILE_PATH

load_dotenv()

# Entry point
if __name__ == "__main__":
    # Creo la view
    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()

    # Controllo che il file di configurazione esista, altrimenti lo creo con i valori di default
    if not CONFIG_DIR.exists():
        print("creo cartella e file")
        Path.mkdir(CONFIG_DIR)
        Path.touch(CONFIG_FILE_PATH)
        with open(CONFIG_FILE_PATH, "w") as f:
            f.write(dumps(DEFAULT_CONFIG))

    # Creo controller
    config_reader = ConfigReader(CONFIG_FILE_PATH)
    controller = Controller(config_reader=config_reader, view=window)

    # Setto il controller della view e mostro la finestra principale
    window.set_controller(controller)
    window.show()

    sys.exit(app.exec())

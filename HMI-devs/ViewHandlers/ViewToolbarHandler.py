from EmbeddedBrowsers.WindowModel import WindowModel
from controller.Controller import Controller


class ToolBarHandler:
    def __init__(self, view, controller: Controller):
        self.view = view
        self.controller = controller

        self.windows_opened = {
            "webcam": False,
            "fog": False,
            "fog-data": False,
            "sinaps": False,
            "qinsy": False,
            "nikon": False,
            "mbes": False,
        }

        self.map_id_name = {
            "webcam": "WebcamWindow.py",
            "fog": "FogWindow.py",
            "fog-data": "FogDataWindow.py",
            "sinaps": "SinapsWindow.py",
            "qinsy": "QinsyWindow.py",
            "nikon": "DslrWindow.py",
            "mbes": "MultibeamWindow.py",
        }

        self.threadpool = self.controller.thread_pool

        self.python_version = controller.get_config("python-version")

        self.buttons = [
            self.view.tool_bar_webcam,
            self.view.tool_bar_fog,
            self.view.tool_bar_fog_data,
            self.view.tool_bar_sinaps,
            self.view.tool_bar_nikon,
            self.view.tool_bar_multibeam,
            self.view.tool_bar_qinsy
        ]

        self.__bind_button_events()

    def __get_window_model_instance(self, name, _id):
        return WindowModel(
            name=name,
            id=_id,
            threadpool=self.threadpool,
            python=self.python_version,
            finish_callback=self.__finish_callback,
        )

    def __finish_callback(self, _id):
        self.windows_opened[_id] = False

    def __handle_button(self, name):
        if self.windows_opened[name] is False:
            self.windows_opened[name] = True
        else:
            print(f"Window ({name}) is already opened")
            return

        self.__get_window_model_instance(self.map_id_name[name], name).start()

    def __bind_button_events(self):
        self.buttons[0].clicked.connect(lambda: self.__handle_button("webcam"))
        self.buttons[1].clicked.connect(lambda: self.__handle_button("fog"))
        self.buttons[2].clicked.connect(lambda: self.__handle_button("fog-data"))
        self.buttons[3].clicked.connect(lambda: self.__handle_button("sinaps"))
        self.buttons[4].clicked.connect(lambda: self.__handle_button("nikon"))
        self.buttons[5].clicked.connect(lambda: self.__handle_button("mbes"))
        self.buttons[6].clicked.connect(lambda: self.__handle_button("qinsy"))

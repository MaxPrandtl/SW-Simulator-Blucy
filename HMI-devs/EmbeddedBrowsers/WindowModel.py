from pathlib import Path
from PySide6.QtCore import QProcess, QRunnable, Slot


class WindowModel(QRunnable):
    def __init__(self, **kwargs) -> None:
        super(WindowModel, self).__init__()

        if kwargs["python"] is None:
            raise Exception("Python is not defined")

        self.name = kwargs["name"]
        self.w_id = kwargs["id"]
        self.threadpool = kwargs["threadpool"]
        self.python = kwargs["python"]
        self.finish_callback = kwargs["finish_callback"]

    def start(self):
        self.threadpool.start(self)

    @Slot()
    def run(self):
        path = Path.cwd().joinpath("EmbeddedBrowsers", self.name)

        self.process = QProcess()
        self.process.start(self.python, [str(path)])

        self.process.waitForStarted()
        print(self.name, "started")

        self.process.waitForFinished(-1)
        print(self.name, "stop")

        self.finish_callback(self.w_id)

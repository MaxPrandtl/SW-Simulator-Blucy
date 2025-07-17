from PySide6.QtCore import QRunnable
from time import sleep
from Signals import WorkerBottomCamSignals


SHUTTING_DOWN_TIME = 30


class BottomCamShutdownWorker(QRunnable):
    def __init__(self):
        super(BottomCamShutdownWorker, self).__init__()
        self.signals = WorkerBottomCamSignals()

    def run(self):
        for i in range(SHUTTING_DOWN_TIME, 0, -1):
            sleep(1)
            self.signals.progress.emit(i)
        sleep(1)
        self.signals.finished.emit()

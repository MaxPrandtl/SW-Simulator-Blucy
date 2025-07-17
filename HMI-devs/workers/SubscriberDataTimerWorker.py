from PySide6.QtCore import QRunnable
import time

from Signals import WorkerTimerSignals


class SubscriberDataTimerWorker(QRunnable):
    def __init__(self, *args, **kwargs):
        super(SubscriberDataTimerWorker, self).__init__()
        self.args = args
        self.kwargs = kwargs

        self.miniSVS_timestamp = kwargs.get("miniSVS_timestamp")
        self.us_imu_timestamp = kwargs.get("us_imu_timestamp")

        self.signals = WorkerTimerSignals()
        self.running = False

    def run(self):
        self.running = True
        while self.running:
            time.sleep(1)
            if self.running:
                self.signals.time_up.emit("time up")

    def stop(self):
        self.running = False

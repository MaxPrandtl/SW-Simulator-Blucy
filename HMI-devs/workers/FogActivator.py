from time import sleep
from PySide6.QtCore import QRunnable
import socket
from Signals import WorkerFogSignals
from utils.DataProcess import Data


STARTUP_TIME = 1 * 60 * 5  # (In secondi)


class FogActivator(QRunnable):
    def __init__(self, *args, **kwargs):
        super(FogActivator, self).__init__()
        self.args = args
        self.kwargs = kwargs
        self.signals = WorkerFogSignals()
        self.running = False


    def run(self):
        self.running = True

        while self.running:
            try:
                print("Avvio timer")
                for i in range(STARTUP_TIME, 0, -1):
                    self.signals.progress.emit(i)
                    sleep(1)
                self.signals.startup_completed.emit()
                

            except Exception as e:
                print("Errore udp fog:", e)
                continue


    def stop(self):
        self.sock.close()
        self.sock = None
        self.running = False

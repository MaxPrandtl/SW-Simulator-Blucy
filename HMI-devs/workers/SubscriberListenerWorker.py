from PySide6.QtCore import QRunnable
import socket
from Enum.SubscriberSensorStatus import SubscriberSensorStatus
from Signals import WorkerSubscriberSignals


class UDPSubscriberListenerWorker(QRunnable):
    def __init__(self, *args, **kwargs):
        super(UDPSubscriberListenerWorker, self).__init__()
        self.args = args
        self.kwargs = kwargs

        self.ground_ip = self.kwargs.get("ground_ip")
        self.port = self.kwargs.get("port", 8888)

        self.signals = WorkerSubscriberSignals()
        self.running = False

    def run(self):
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.sock.bind((self.ground_ip, self.port))
        except Exception as e:
            print("Errore di connessione col subscriber:", e)
            self.signals.error.emit(1)
            return

        print("sub udp start")
        self.running = True

        while self.running:
            try:
                topic, result_code = self.sock.recv(1024).decode("utf-8").split("$")
                if topic == "miniSVS":
                    name = "miniSVS"
                    status = SubscriberSensorStatus(int(result_code))
                elif topic == "us_imu":
                    name = "us_imu"
                    status = SubscriberSensorStatus(int(result_code))
                self.signals.on_data.emit((name, status))
            except Exception as e:
                print("udp off", e)
                self.sock = None
                # self.signals.error.emit(2)
                break

        self.signals.finished.emit("UDP fogger listener worker finished")
        print("data udp off")

    def stop(self):
        self.sock.close()
        self.sock = None
        self.running = False

from time import sleep
from PySide6.QtCore import QRunnable
import socket
from Signals import WorkerFogSignals
from utils.DataProcess import Data


STARTUP_TIME = 1 * 60 * 5  # (In secondi)


class UDPFogListenerWorker(QRunnable):
    def __init__(self, *args, **kwargs):
        super(UDPFogListenerWorker, self).__init__()
        self.args = args
        self.kwargs = kwargs

        self.ground_ip = self.kwargs.get("ground_ip")
        self.port = 49154
        self.view = self.kwargs.get("view")

        self.signals = WorkerFogSignals()
        self.running = False

        self.__valid_gpslike = False
        self.__startup_completed = False

    def run(self):
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.sock.bind((self.ground_ip, self.port))
        except Exception as e:
            print("Socket non connessa al fog. X e Y non disponibili.")
            self.signals.error.emit((e, "Socket non connessa al fog. X e Y non disponibili."))
            return

        print("data udp start")
        self.running = True

        while self.running:
            try:
                gps_like_strs = self.sock.recv(2048).decode("utf-8")

                gpgga = ""
                for s in gps_like_strs.split("\r\n"):
                    if s.startswith("$GPGGA"):
                        gpgga = s

                if gpgga == "":
                    continue

                # Tokens gpgga
                gpgga_tokens = gpgga.split(",")[1:]

                lat = str(Data.deg_min_to_deg(gpgga_tokens[1]))
                lat_pole = gpgga_tokens[2]

                lon = str(Data.deg_min_to_deg(gpgga_tokens[3]))
                lon_pole = gpgga_tokens[4]

                if self.__startup_completed is False:
                    # Finché non ho dati validi rimane giallo
                    if self.__valid_gpslike is False:
                        self.__valid_gpslike = lat != "0.0000" and lon != "0.0000"
                        continue

                    # Appena ho dati validi avvio il timer
                    if self.__valid_gpslike is True:
                        print("Avvio timer")
                        for i in range(STARTUP_TIME, 0, -1):
                            self.signals.progress.emit(i)
                            # Controllo se il fog manda dati validi durante il conteggio
                            # ad esempio nel caso il programma sia stato avviato dopo
                            # lo start del fog.
                            self.__valid_gpslike = lat != "0.0000" and lon != "0.0000"
                            if self.__valid_gpslike:
                                break
                            sleep(1)
                        self.__startup_completed = True
                        self.signals.startup_completed.emit()

                # if self.__valid_gpslike is False and lat != "0.0000" and lon != "0.0000":
                #    self.__startup_completed = True
                # Emetto segnale di startup fog completato
                #    self.signals.startup_completed.emit()
                # else:
                #    continue

                self.signals.on_data.emit(
                    (
                        f"{lat} {lat_pole}",
                        f"{lon} {lon_pole}",
                    )
                )

            except Exception as e:
                print("Errore udp fog:", e)
                continue

        self.signals.finished.emit("UDP fogger listener worker finished")
        print("data udp off")

    def stop(self):
        self.sock.close()
        self.sock = None
        self.running = False

from Enum.CommandStatus import SensorStatus
import subprocess
import datetime
import signal
import os

class PilotCamRecorder:
    def __init__(self) -> None:
        self.__status: SensorStatus = SensorStatus.OFF

    @property
    def status(self) -> SensorStatus:
        return self.__status

    @status.setter
    def status(self, value: SensorStatus):
        self.__status = value

    def turn_on(self):
       # fname = datetime.datetime.now().isoformat().split('.')[0].replace('-','').replace(':','')
       # self.__runner = subprocess.Popen(["ffmpeg.exe","-i","rtsp://169.254.138.26:554/live2.sdp", f"{fname}.mp4"], shell=False)
        #""" Avvia la registrazione del flusso RTSP con FFmpeg alla massima qualità. """
        fname = datetime.datetime.now().isoformat().split('.')[0].replace('-', '').replace(':', '')
        output_file = f"{fname}.mp4"

        ffmpeg_cmd = [
            "ffmpeg.exe",
            "-rtsp_transport", "tcp",   # Forza RTSP su TCP (migliore stabilità)
            "-i", "rtsp://169.254.138.26:554/live2.sdp",  # URL del flusso RTSP
            "-c", "copy",   # Copia il flusso senza ricodifica (massima qualità)
            "-buffer_size", "240M",  # Aggiunge un buffer per evitare perdita di pacchetti
            "-y",  # Sovrascrive il file se già esiste
            output_file  # Nome del file di output
        ]

        self.__runner = subprocess.Popen(ffmpeg_cmd, shell=False, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        self.__status = SensorStatus.ON
    
    def turn_off(self):
        self.__runner.send_signal(signal.CTRL_C_EVENT)
        ##self.__runner.kill()
    ##""" Ferma la registrazione in modo sicuro. """
        #if self.__runner:
        #    self.__runner.terminate()  # Chiude il processo FFmpeg in modo sicuro
        #    self.__runner.wait()  # Aspetta la chiusura completa del processo
        #    self.__runner = None
        #    self.__status = SensorStatus.OFF
    

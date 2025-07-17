from PySide6.QtCore import QObject, Signal
from paho.mqtt.client import MQTTMessage


# Main singals for updating the GUI
class SignalCommunicate(QObject):
    request_graph_update = Signal(str, dict)
    request_histogram_update = Signal(float, float, float)
    request_motor_graph_update = Signal(str)
    update_data_variables = Signal(MQTTMessage)


# Singals emitted by the fog listener worker
class WorkerFogSignals(QObject):
    error = Signal(tuple)
    finished = Signal(str)
    on_data = Signal(tuple)
    startup_completed = Signal()
    progress = Signal(int)


# Singlas emitted by the subscriber timer worker
class WorkerTimerSignals(QObject):
    time_up = Signal(str)


# Singals emitted by the subscriber listener worker
class WorkerSubscriberSignals(QObject):
    error = Signal(int)
    finished = Signal(str)
    on_data = Signal(tuple)


# Singal emitted by the bommom cam timer worker
class WorkerBottomCamSignals(QObject):
    finished = Signal()
    progress = Signal(int)

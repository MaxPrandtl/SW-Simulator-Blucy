from model.AHRSDataModel import AHRSDataModel
from model.AltitudeModel import AltitudeModel
from model.CTDDataModel import CTDDataModel
from model.MessageModel import MessageModel
from model.PositionDataModel import PositionDataModel
from model.ReferenceDataModel import ReferenceDataModel
from model.Thruster import Thruster
from model.ZModel import ZModel
from paho.mqtt.client import MQTTMessage
from utils.DataProcess import Data


class DataModel:
    def get_thruster_enable_command_topic(self):
        topics = [f"thrusters/EnableCommand{thruster.enable_status}" for thruster in self.thruster.values()]
        return topics

    def get_thruster_by_enable_command(self, value):
        value = int(value.replace("thrusters/EnableCommand", ""))
        for t in self.thruster.values():
            if t.enable_status == value:
                return t
        raise Exception("Cannot find thruster with enable status " + str(value))

    def __init__(self) -> None:
        self.vf = Thruster("vf", di=2, enable_status=3)
        self.vr = Thruster("vr", di=3, enable_status=4)
        self.hl = Thruster("hl", di=1, enable_status=2)
        self.hr = Thruster("hr", di=0, enable_status=1)
        self.bow = Thruster("bow", di=4, enable_status=5)
        self.stern = Thruster("stern", di=5, enable_status=6)

        self.thruster = {
            "ana_dig_io/DI02": self.vf,
            "ana_dig_io/DI03": self.vr,
            "ana_dig_io/DI01": self.hl,
            "ana_dig_io/DI00": self.hr,
            "ana_dig_io/DI04": self.bow,
            "ana_dig_io/DI05": self.stern,
        }

        self.ctd = CTDDataModel()
        self.ahrs = AHRSDataModel()
        self.position = PositionDataModel()
        self.altitude_model = AltitudeModel(low_th=2, high_th=5)
        self.z_model = ZModel()
        self.reference = ReferenceDataModel()
        self.fu = MessageModel()
        self.fv = MessageModel()
        self.tr = MessageModel()
        self.fw = MessageModel()
        self.w = MessageModel()
        self.u = MessageModel()
        self.v = MessageModel()
        self.r = MessageModel()
        # Conversione per la r (da radianti in gradi)
        self.r.set_processing_function(lambda m: MessageModel(Data.rad_to_deg(m.value), m.timestamp, m.valid))

        self.internal_temperature = MessageModel()
        self.internal_temperature.set_processing_function(
            lambda m: MessageModel(Data.dec_deg_to_deg(m.value), m.timestamp, m.valid)
        )

        # Contiene solo le variabili che non necessitano di elaborazione
        self.__data = {
            "miniSVS/pressure": self.ctd.pressure,
            "miniSVS/soundSpeed": self.ctd.soundSpeed,
            "miniCT/temperature": self.ctd.temperature,
            "miniCT/conductivity": self.ctd.conductivity,
            "NGC/force/tr/actual": self.tr,
            "NGC/force/fu/actual": self.fu,
            "NGC/force/fw/actual": self.fw,
            "NGC/force/fv/actual": self.fv,
            "NGC/velocity/body/r/actual": self.r,
            "NGC/velocity/body/u/actual": self.u,
            "NGC/velocity/body/v/actual": self.v,
            "NGC/velocity/body/w/actual": self.w,
            "NGC/pose/z/manual": self.reference.z,
            "NGC/pose/a/reference": self.reference.alt,
            "NGC/pose/psi/reference": self.reference.psi,
            "ib_ins/roll": self.ahrs.roll,
            "ib_ins/pitch": self.ahrs.pitch,
            "ib_ins/heading": self.ahrs.heading,
            "pa200/range": self.altitude_model.altitude,
            "NGC/pose/z/actual": self.z_model.z,
            "ana_dig_io/AI01": self.internal_temperature,
            "us_imu/latitude": self.position.lat,
            "us_imu/longitude": self.position.lon,
            
        }

    # Funzione utilizzata nel momento in cui l'impostazine dello z-offset viene cambiata
    def set_z_offset(self, z_offset: float) -> None:
        self.z_model.reset()
        ZModel.set_z_offset(z_offset)

    # Update data model with new data from MQTT message
    def update(self, msg: MQTTMessage):
        topic = msg.topic

        if topic in self.__data.keys():
            try:
                self.__data[topic].set_data(self.__parse_message(msg.payload))
            except Exception as ex:
                self.__data[topic] = self.__parse_message(msg.payload)
                print("errore", msg.topic, "\n", msg.payload)

    def get_data(self, topic: str) -> MessageModel:
        return self.__data[topic]

    # Get payload value form MQTT message
    def __parse_message(self, payload) -> MessageModel:

        tokens = payload.decode("utf-8").strip().split(" ")

        value = float(tokens[0])
        timstamp = int(tokens[1])
        valid = tokens[2]

        if value == 0:
            value = abs(value)

        return MessageModel(value, timstamp, valid)

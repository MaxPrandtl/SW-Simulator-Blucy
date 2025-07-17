from Enum.CommandStatus import BottomCamStatus, InfoLedStatus, SensorStatus
from controller.Controller import Controller


info_led_status_map = {
    InfoLedStatus.OFF: "off",
    InfoLedStatus.ON: "on",
    InfoLedStatus.WARN: "warn",
    InfoLedStatus.NO_COLOR: "no_color",
}


class ControlPanelHandler:
    def __init__(self, view, controller: Controller):
        self.view = view
        self.controller = controller

        # Status leds
        self.info_leds = {
            "water_leak": self.view.water_leak_led,
            "fog_status": self.view.fog_status_led,
            "ahrs_status": self.view.ahrs_status_led,
        }

        self.controls = {
            "light_up": self.view.light_up,
            "light_md": self.view.light_md,
            "light_dw": self.view.light_dw,
            "miniSVS": self.view.miniSVS_control,
            "miniCT": self.view.miniCT_control,
            "multi_beam": self.view.multi_beam_control,
            "pilot_cam": self.view.pilot_cam,
            "dvl": self.view.dvl_control,
            "usbl": self.view.usbl_control,
            "alt": self.view.alt_control,
        }

        self.bottom_cam = self.view.bottom_cam
        self.bottom_cam.clicked.connect(self.controller.sensor_handler.handle_bottom_cam)

        self.pilot_cam_record = self.view.pilot_cam_record
        self.pilot_cam_record.clicked.connect(self.controller.sensor_handler.handle_pilot_cam_record)

        for (component_name, component) in self.controls.items():
            component.clicked.connect(lambda *args, n=component_name: self.__handle_sensor(n))

    def __handle_sensor(self, component_name: str):
        self.controller.sensor_handler.publish_status(component_name)

    def setStatus(self, name, value):
        if name == "bottom_cam":
            if value == BottomCamStatus.OFF:
                self.bottom_cam.setProperty("status", "off")
            elif value == BottomCamStatus.ON:
                self.bottom_cam.setProperty("status", "on")
            elif value == BottomCamStatus.SHUTTING_DOWN:
                self.bottom_cam.setProperty("status", "shutdown")

            self.bottom_cam.style().unpolish(self.bottom_cam)
            self.bottom_cam.style().polish(self.bottom_cam)

            return

        v = True if value == SensorStatus.ON else False

        self.controls[name].setProperty("status", v)
        self.controls[name].style().unpolish(self.controls[name])
        self.controls[name].style().polish(self.controls[name])

    def set_info_led_status(self, name, value):
        self.info_leds[name].setProperty("status", info_led_status_map[value])
        self.info_leds[name].style().unpolish(self.info_leds[name])
        self.info_leds[name].style().polish(self.info_leds[name])

from decimal import Decimal
import math


class Data:
    @staticmethod
    def deg_min_to_deg(llmm_mm: str):
        llmm_mm = float(llmm_mm)
        t = Decimal(llmm_mm / 100)
        deg = int(t)
        mm_mm = (t - deg) / 60 * 100
        return (deg + mm_mm).quantize(Decimal("0.0001"))

    @staticmethod
    def convert_angle(angle):
        angle = float(angle)
        return angle % 360

    @staticmethod
    def deg_to_rad(angle):
        return angle * math.pi / 180

    @staticmethod
    def rad_to_deg(angle):
        return angle * 180 / math.pi

    @staticmethod
    def dec_deg_to_deg(angle):
        return angle * 10

    @staticmethod
    def get_battery_fit(raw_value):
        from .converter import v2p
        print('raw', raw_value)
        print('val', v2p(float(raw_value*10)))
        return float(v2p(float(raw_value*10)))/100
        
        #x = float(raw_value * 10 / 26.23)
        #x_3 = x * x * x
        #x_2 = x * x
#
        #p1 = -0.08061
        #p2 = 0.0255
        #p3 = 0.4346
        #p4 = 0.5604
#
        #v = p1 * x_3 + p2 * x_2 + p3 * x + p4
        #return v

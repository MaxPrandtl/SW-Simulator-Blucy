from typing import TYPE_CHECKING
from PySide6.QtCore import QRect, Qt, Slot
from PySide6.QtGui import QBrush, QColor, QPainter, QPainterPath
from ViewHandlers.plots.RealtimePlotHandler import RealtimePlotHandler


if TYPE_CHECKING:
    from view.MainWindow import MainWindow
    from controller.Controller import Controller


class MotorStatusGraphHandler(RealtimePlotHandler):
    def __init__(self, view: "MainWindow") -> None:
        super().__init__(view)

        self.controller = None

        self.view.thrust_surge.paintEvent = self.paint_surge
        self.view.thrust_sway.paintEvent = self.paint_sway
        self.view.thrust_heave.paintEvent = self.paint_heave
        self.view.thrust_yaw.paintEvent = self.paint_yaw

        self.__graphs = {
            "surge": self.view.thrust_surge,
            "sway": self.view.thrust_sway,
            "heave": self.view.thrust_heave,
            "yaw": self.view.thrust_yaw,
        }

    def connect_signal(self):
        self.view.signals.request_motor_graph_update.connect(self.request_graph_update)

    def set_controller(self, controller: "Controller"):
        self.controller = controller
        self.setup_plot()

    def setup_plot(self):
        if self.controller is not None:
            self.connect_signal()
            for graph in self.__graphs.values():
                graph.update()

    @Slot(str)
    def request_graph_update(self, key):
        self.__graphs[key].update()

    def paint_yaw(self, e):
        self.__draw_bar(self.view.thrust_yaw, self.controller.motor_status_data.get_data("yaw"))

    def paint_surge(self, e):
        self.__draw_bar(self.view.thrust_surge, self.controller.motor_status_data.get_data("surge"))

    def paint_sway(self, e):
        self.__draw_bar(self.view.thrust_sway, self.controller.motor_status_data.get_data("sway"))

    def paint_heave(self, e):
        self.__draw_bar(self.view.thrust_heave, self.controller.motor_status_data.get_data("heave"))

    def __draw_bar(self, widget, value):
        CANVAS_WIDTH = widget.width()
        CANVAS_HEIGHT = widget.height()

        PADDING_X = 20
        MAX_BAR_WIDTH = CANVAS_WIDTH - PADDING_X * 2

        color = QBrush(QColor(50, 180, 100)) if value > 0 else QBrush(Qt.red)

        bar_w = value * (MAX_BAR_WIDTH / 2) / 100
        bar_h = CANVAS_HEIGHT - CANVAS_HEIGHT * 0.7

        painter = QPainter(widget)
        painter.translate(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2 + bar_h - 5)
        painter.drawRect(-CANVAS_WIDTH / 2 + PADDING_X, -bar_h / 2, MAX_BAR_WIDTH, bar_h)

        bar = QPainterPath()
        bar.addRect(0, -bar_h / 2, bar_w, bar_h)
        painter.fillPath(bar, color)
        painter.drawPath(bar)

        for i in range(0, 9):
            x = -CANVAS_WIDTH / 2 + PADDING_X + i * (MAX_BAR_WIDTH / 8)
            if i % 2 == 0:
                text = str(int(i * 25 - 100))
                painter.drawText(
                    QRect(x - MAX_BAR_WIDTH / 8, -bar_h - 10, MAX_BAR_WIDTH / 4, bar_h),
                    Qt.AlignCenter,
                    text,
                )

            painter.drawLine(x, -bar_h / 2, x, -bar_h / 2 + 5)

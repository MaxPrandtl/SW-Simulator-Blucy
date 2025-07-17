from typing import TYPE_CHECKING
from pyqtgraph.dockarea import Dock


if TYPE_CHECKING:
    from view.MainWindow import MainWindow


class TelemetryHandler:
    def __init__(self, view: "MainWindow") -> None:
        self.view = view

        d1 = Dock("Dock 1")
        d2 = Dock("Dock 2")
        d3 = Dock("Dock 3")
        d4 = Dock("Dock 4")
        d5 = Dock("Dock 5")

        d1.addWidget(self.view.telemetry_widget)
        d2.addWidget(self.view.telemetry_widget_2)
        d3.addWidget(self.view.telemetry_widget_3)

        d4.addWidget(self.view.telemetry_widget_4)
        d5.addWidget(self.view.telemetry_widget_5)

        self.view.telemetry_dock_area.addDock(d1, "top")
        self.view.telemetry_dock_area.addDock(d2, "bottom", d1)
        self.view.telemetry_dock_area.addDock(d3, "left", d2)
        self.view.telemetry_dock_area_2.addDock(d4, "top")
        self.view.telemetry_dock_area_2.addDock(d5, "left", d4)

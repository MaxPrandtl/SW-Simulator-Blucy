from PySide6.QtCore import Slot
from ViewHandlers.plots.RealtimePlotHandler import RealtimePlotHandler
import pyqtgraph as pg


class TelemetryGraphsHandler(RealtimePlotHandler):
    def __init__(self, view) -> None:
        super().__init__(view)

        self.setup_plot()

    def connect_signal(self):
        self.view.signals.request_graph_update.connect(self.request_graph_update)

    # TODO: rinominare le dataline e creare i grafici
    def setup_plot(self):
        self.connect_signal()

        pen = pg.mkPen(color=(255, 0, 0), width=6)
        pen3 = pg.mkPen(color=(0, 255, 0), width=6)

        for plot_widget in self.view.findChildren(pg.PlotWidget):
            plot_widget.setBackground(pg.QtGui.QColor(255, 255, 255, 1))
            plot_widget.showGrid(x=True, y=True, alpha=0.4)

        self.view.telemetry_widget.getPlotItem().addLegend()
        self.view.telemetry_widget.getPlotItem().setTitle("Z")
        self.view.telemetry_widget.getPlotItem().setLabel("left", units="m")

        self.z_line = self.view.telemetry_widget.plot([], [], pen=pen)

        self.view.telemetry_widget_2.getPlotItem().addLegend().setLabelTextSize("16px")
        self.view.telemetry_widget_2.getPlotItem().setTitle("Pitch and Roll")
        self.view.telemetry_widget_2.getPlotItem().setLabel("left", units="deg")

        self.roll_line = self.view.telemetry_widget_2.plot([], [], pen=pg.mkPen("r", width=6), name="Roll")
        self.pitch_line = self.view.telemetry_widget_2.plot([], [], pen=pg.mkPen("g", width=6), name="Pitch")

        self.view.telemetry_widget_3.getPlotItem().setTitle("Altitude")
        self.altitude_line = self.view.telemetry_widget_3.plot([], [], pen=pen3)

    @Slot(str, dict)
    def request_graph_update(self, key, value):
        # TODO: mappare le linee e usare un loop?
        if key == "z":
            self.z_line.setData(value["x"], value["y"])
        elif key == "roll":
            self.roll_line.setData(value["x"], value["y"])
        elif key == "pitch":
            self.pitch_line.setData(value["x"], value["y"])
        elif key == "altitude":
            self.altitude_line.setData(value["x"], value["y"])

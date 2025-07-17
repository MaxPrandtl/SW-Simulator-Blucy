from typing import TYPE_CHECKING
import pyqtgraph as pg


if TYPE_CHECKING:
    from view.MainWindow import MainWindow


class SVPlotHandler:
    def __init__(self, view: "MainWindow") -> None:
        self.view = view
        self.setup_plot()

    def setup_plot(self):
        self.view.sv_profile_plot.getPlotItem().getViewBox().invertY(True)
        self.view.sv_profile_plot.getPlotItem().showGrid(x=True, y=True, alpha=0.6)
        self.view.sv_profile_plot.getPlotItem().setLabel("left", "Depth", units="m")
        self.view.sv_profile_plot.getPlotItem().setLabel("bottom", "Speed Velocity", units="m/s")

        self.pen = pg.mkPen(color=(255, 0, 0), width=6)

    def plot(self, x, y):
        self.view.sv_profile_plot.plot(x=x, y=y, clear=True, pen=self.pen)

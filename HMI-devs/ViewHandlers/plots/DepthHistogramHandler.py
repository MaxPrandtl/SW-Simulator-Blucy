from PySide6.QtCore import Slot
import pyqtgraph as pg

from ViewHandlers.plots.RealtimePlotHandler import RealtimePlotHandler


class DepthHistogramHandler(RealtimePlotHandler):
    def __init__(self, view) -> None:
        super().__init__(view)

        self.setup_plot()

    def connect_signal(self):
        self.view.signals.request_histogram_update.connect(self.request_graph_update)

    def setup_plot(self):
        self.connect_signal()

        self.view.bar_chart = pg.BarGraphItem(x=[1], height=[0], width=3, brush="g")
        self.view.line_chart = pg.InfiniteLine(pos=0, angle=0, pen=pg.mkPen(color=(255, 0, 0), width=10))
        self.view.line_chart.setValue(20)

        self.view.reference_line = pg.InfiniteLine(pos=0, angle=0, pen=pg.mkPen(color=(255, 255, 0), width=10))
        self.view.reference_line.setValue(10)

        self.view.histogram.getPlotItem().showAxis("bottom", show=False)
        self.view.histogram.getPlotItem().getViewBox().invertY(True)
        self.view.histogram.addItem(self.view.bar_chart)
        self.view.histogram.addItem(self.view.reference_line)
        self.view.histogram.addItem(self.view.line_chart)

        self.view.histogram.showGrid(x=False, y=True, alpha=1)
        self.view.histogram.setMouseEnabled(x=False, y=False)

    @Slot(float, float, float)
    def request_graph_update(self, z, sealevel, z_reference):
        self.view.line_chart.setValue(sealevel)
        self.view.reference_line.setValue(z_reference)
        self.view.bar_chart.setOpts(x=[1], height=[z], brush="b")

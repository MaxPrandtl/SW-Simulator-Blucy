from PySide6 import QtWidgets
from PySide6.QtCore import Qt
from PySide6.QtGui import QPixmap
from Signals import SignalCommunicate
from ViewHandlers.ViewTelemetryHandler import TelemetryHandler

from compiled_ui.MainWindow import Ui_MainWindow

from controller.Controller import Controller

from view.ErrorDialog import ErroDialog
from view.ExportSVDialog import ExportSVDialog
from view.TopicSelectorDialog import TopicSelectorDialog
from view.ExportDialog import ExportDialog

from utils.theme_compiler import ThemeCompiler

from ViewHandlers.ViewControlsHandler import ControlPanelHandler
from ViewHandlers.ViewDataHandler import ViewDataHandler
from ViewHandlers.ViewMenuHandler import MenuHandler
from ViewHandlers.ViewStatusbarHandler import StatusBarHandler
from ViewHandlers.ViewStatusPageHandler import StatusPageHandler
from ViewHandlers.ViewToolbarHandler import ToolBarHandler

from ViewHandlers.plots.SVPlotHandler import SVPlotHandler
from ViewHandlers.plots.DepthHistogramHandler import DepthHistogramHandler
from ViewHandlers.plots.TelemetryGraphsHandler import TelemetryGraphsHandler
from ViewHandlers.plots.MotorStatusGraphHandler import MotorStatusGraphHandler


class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.setupUi(self)

        self.controller: Controller = None

        # Preload delle immagini
        self.BLUCY_BACK = QPixmap(":/images/images/Blucy_back.png")
        self.BLUCY_SIDE = QPixmap(":/images/images/Blucy_side.png")
        self.BLUCY_UP = QPixmap(":/images/images/Blucy_up.png")

        # Setto come view iniziale al status page
        self.main_content_widget.setCurrentIndex(0)

        # Handle status bar
        self.status_bar_handler = StatusBarHandler(self)
        # Handle status page
        self.status_page_handler = StatusPageHandler(self)

        self.telemetry_handler = TelemetryHandler(self)

        # Rimuovo bordi dalle line edits
        for line_edit in self.findChildren(QtWidgets.QLineEdit):
            line_edit.setAttribute(Qt.WA_MacShowFocusRect, 0)

        # Rimovo bordi dalle spin box
        for spin_box in self.findChildren(QtWidgets.QSpinBox):
            spin_box.setAttribute(Qt.WA_MacShowFocusRect, 0)

        # ------------- Grafici -------------
        self.signals = SignalCommunicate()

        # Real time
        self.telemetry_graph_handler = TelemetryGraphsHandler(self)
        self.depth_histogram_handler = DepthHistogramHandler(self)
        self.motor_status_graph_handler = MotorStatusGraphHandler(self)

        # Static
        self.sv_plot_handler = SVPlotHandler(self)

        # Handle drag and drop su line edits e labels
        self.selected_log_folder_path_label.dragEnterEvent = self.__drag_event
        self.selected_svlog_folder_path_label.dragEnterEvent = self.__drag_event
        self.settings_exporter_path.dragEnterEvent = self.__drag_event
        self.settings_logger_path.dragEnterEvent = self.__drag_event
        self.settings_svexporter_path.dragEnterEvent = self.__drag_event
        self.settings_svlogger_path.dragEnterEvent = self.__drag_event
        self.selected_profile_path_label.dragEnterEvent = self.__drag_event

        # Close event
        self.closeEvent = self.__close_event

    def __drag_event(self, event):
        if event.mimeData().hasUrls():
            event.accept()
        else:
            event.ignore()

    def set_controller(self, controller: Controller):
        self.controller = controller
        # Handle menu events
        self.menu_handler = MenuHandler(self, self.controller)
        # Handle control panel
        self.control_panel_handler = ControlPanelHandler(self, self.controller)
        # Handle tool bar events
        self.tool_bar_handler = ToolBarHandler(self, self.controller)
        # Handle controls in data panel
        self.data_page_controls_handler = ViewDataHandler(self, self.controller)

        self.motor_status_graph_handler.set_controller(self.controller)

        # Set up dialogs
        self.error_dialog = ErroDialog(self)
        # TODO: Lazy loading
        self.topic_selector_dialog = TopicSelectorDialog(self, self.controller)
        self.export_dialog = ExportDialog(self, self.controller)
        self.export_sv_dialog = ExportSVDialog(self, self.controller)

        self.switch_theme()

    def switch_theme(self):
        if self.controller is not None:
            theme = self.controller.get_config("color-theme")
            theme_qss = ThemeCompiler.compile(theme)
            self.setStyleSheet(theme_qss)
            self.menu_handler.refresh_colors()

    def update_graph(self, key, value):
        self.signalComm.request_graph_update.emit(key, value)

    def update_histogram(self, z, sealevel):
        self.signalComm.request_histogram_update.emit(z, sealevel)

    def update_histogram_data(self, z, sealevel):
        self.line_chart.setValue(sealevel)
        self.bar_chart.setOpts(x=[1], height=[z], brush="b")

    def update_plot_data(self, key, value):
        if key == "z":
            self.data_line1.setData(value["x"], value["y"])
        elif key == "psi":
            self.data_line2.setData(value["x"], value["y"])
        elif key == "roll":
            self.roll_line.setData(value["x"], value["y"])
        elif key == "pitch":
            self.pitch_line.setData(value["x"], value["y"])
        elif key == "altitude":
            self.data_line3.setData(value["x"], value["y"])

    def __close_event(self, event):
        self.controller.data_handler.udp_fogger_worker.stop()
        self.controller.subscriber_handler.stop_process()
        event.accept()

    def show_error_dialog(self, message="", title="Error"):
        self.error_dialog.show(message, title)

    def open_topic_selector_dialog(self):
        self.topic_selector_dialog.show()

    def open_export_dialog(self):
        self.export_dialog.show()

    def open_sv_export_dialog(self):
        self.export_sv_dialog.show()

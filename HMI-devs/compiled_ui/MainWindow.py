# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'mainWindow.ui'
##
## Created by: Qt User Interface Compiler version 6.3.1
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QAbstractSpinBox, QApplication, QCheckBox, QDoubleSpinBox,
    QFrame, QGridLayout, QGroupBox, QHBoxLayout,
    QLabel, QLayout, QLineEdit, QMainWindow,
    QProgressBar, QPushButton, QScrollArea, QSizePolicy,
    QSpacerItem, QSpinBox, QStackedWidget, QVBoxLayout,
    QWidget)

from pyqtgraph import PlotWidget
from pyqtgraph.dockarea import DockArea
import resources_rc

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(1439, 949)
        MainWindow.setStyleSheet(u"* {\n"
"	padding: 0px;\n"
"	margin: 0px;\n"
"	color: white;\n"
"	background-color: #0B3340;\n"
"	border-radius: 10px;\n"
"}\n"
"\n"
"QFrame {\n"
"	border: none;\n"
"}\n"
"\n"
"QPushButton {\n"
"	border: 1px solid transparent;\n"
"	font-size: 20px;\n"
"	padding: 10px 10px;\n"
"	text-align: left;\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"	color: rgba(255, 255, 255, 100);\n"
"}\n"
"\n"
"QLabel[bg=\"none\"] {\n"
"	background-color: none;\n"
"}\n"
"\n"
"/* Menu */\n"
"\n"
"QPushButton[role=\"menu-btn\"] {\n"
"	background-color: #193849;\n"
"}\n"
"\n"
"QPushButton[role=\"menu-btn\"]:hover{\n"
"	background-color: #192E39;\n"
"}\n"
"\n"
"#leftMenuContainer {\n"
"	background-color:  #26526A;\n"
"	border: none;\n"
"}\n"
"\n"
"#leftMenuContainer > QWidget {\n"
"	background-color: none;\n"
"	border: none;\n"
"}\n"
"\n"
"\n"
"/* StackedWidget */\n"
"\n"
"#main_content_widget QLabel {\n"
"	padding: 0;\n"
"	font-size: 20px;\n"
"}\n"
"\n"
"#main_content_widget QWidget[bg=\"dark\"]{\n"
"	background-color: rgb(3, 25, 30);\n"
"}"
                        "\n"
"\n"
"#main_content_widget QFrame[bg=\"dark\"]{\n"
"	background-color: rgb(3, 25, 30);\n"
"}\n"
"\n"
"#main_content_widget QFrame[bg=\"none\"]{\n"
"	background-color: none;\n"
"}\n"
"\n"
"#main_content_widget QWidget {\n"
"	background-color: rgb(9, 40, 47);\n"
"}\n"
"\n"
"#main_content_widget QPushButton[bg=\"primary\"]{\n"
"	background-color: rgb(9, 51, 64);\n"
"}\n"
"\n"
"#main_content_widget QPushButton[bg=\"primary\"]:hover{\n"
"	\n"
"	background-color: rgb(9, 40, 47);\n"
"}\n"
"\n"
"#main_content_widget QPushButton[bg=\"info\"]{\n"
"	background-color: rgb(31, 83, 94);\n"
"}\n"
"\n"
"#main_content_widget QPushButton[bg=\"info\"]:hover{\n"
"	background-color: rgb(46, 138, 159);\n"
"}\n"
"\n"
"#main_content_widget QSpinBox, QDoubleSpinBox {\n"
"	font-size: 20px;\n"
"	border-radius: none;\n"
"	border-top-left-radius: 5px;\n"
"	border-bottom-left-radius: 5px;\n"
"	padding-left: 5px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;\n"
"}\n"
"\n"
"#main_content_widget QDoubleSpinBox:focus, QSpinBox:focus{\n"
""
                        "	border: 2px solid rgb(45, 78, 98);\n"
"}\n"
"\n"
"#main_content_widget QSpinBox::up-button, QDoubleSpinBox::up-button{ \n"
"	width: 30px;\n"
" }\n"
"#main_content_widget QSpinBox::down-button, QDoubleSpinBox::down-button { \n"
"	width: 30px;\n"
"}\n"
"\n"
"#main_content_widget QLabel[role=\"data\"] {\n"
"	background-color:  rgb(9, 51, 64);\n"
"	padding-left: 10px;\n"
"}\n"
"\n"
"\n"
"#start_sub_btn {\n"
"	padding-right: 40px;\n"
"	padding-left: 40px;\n"
"}\n"
"\n"
"#stop_sub_btn {\n"
"	padding-right: 40px;\n"
"	padding-left: 40px;\n"
"}\n"
"\n"
"#main_content_widget QLineEdit {\n"
"	background-color: rgb(8, 25, 29);\n"
"	font-size: 20px;\n"
"	padding: 10px;\n"
"	qproperty-frame: false ;\n"
"}\n"
"\n"
"#main_content_widget QLineEdit:focus{\n"
"	border: 2px solid rgb(45, 75, 99);\n"
"	padding: 8px;\n"
"}\n"
"\n"
"#main_content_widget QWidget[role=\"canvas\"] {\n"
"	background-color: rgb(7, 51, 64);\n"
"}\n"
"\n"
"/* Controls */\n"
"#controls_container  {\n"
"	background-color: rgb(9, 40, 47);\n"
"}\n"
"\n"
"#co"
                        "ntrols_container QWidget[bg=\"dark\"]{\n"
"	background-color: rgb(3, 25, 30);\n"
"}\n"
"\n"
"#controls_container QPushButton[control=\"pilot-cam\"] {\n"
"	border: 3px solid rgb(33, 70, 119);\n"
"	text-align: center;\n"
"	border-radius: 25px;\n"
"}\n"
"\n"
"#controls_container QPushButton[control=\"pilot-cam\"][status=\"off\"]{\n"
"	background-color: rgb(28, 46, 71);\n"
"}\n"
"\n"
"#controls_container QPushButton[control=\"pilot-cam\"][status=\"off\"]:hover{\n"
"	background-color: rgb(33, 55, 87);\n"
"}\n"
"\n"
"#controls_container QPushButton[control=\"pilot-cam\"][status=\"on\"]{\n"
"	background-color: rgb(44, 102, 211);\n"
"}\n"
"\n"
"#controls_container QPushButton[control=\"pilot-cam\"][status=\"on\"]:hover{\n"
"	background-color: rgb(45, 112, 235);\n"
"}\n"
"\n"
"#controls_container QPushButton[control=\"pilot-cam\"][status=\"shutdown\"]{\n"
"	background-color:  rgb(115, 109, 29);\n"
"}\n"
"\n"
"#controls_container QPushButton[control=\"general\"] {\n"
"	border: 3px solid rgb(33, 70, 119);\n"
"	text-align"
                        ": center;\n"
"	border-radius: 25px;\n"
"}\n"
"\n"
"#controls_container QPushButton[control=\"general\"][status=false]{\n"
"	background-color: rgb(28, 46, 71);\n"
"}\n"
"\n"
"#controls_container QPushButton[control=\"general\"][status=false]:hover{\n"
"	background-color: rgb(33, 55, 87);\n"
"}\n"
"\n"
"#controls_container QPushButton[control=\"general\"][status=true]{\n"
"	background-color: rgb(44, 102, 211);\n"
"}\n"
"\n"
"#controls_containerQPushButton[control=\"general\"][status=true]:hover{\n"
"	background-color: rgb(45, 112, 235);\n"
"}\n"
"\n"
"\n"
"QPushButton[control=\"infoled\"][status=\"off\"]{\n"
"	background-color: #811818;\n"
"	border:3px solid rgb(229, 30, 54);\n"
"}\n"
"\n"
"QPushButton[control=\"infoled\"][status=\"on\"]{\n"
"	background-color: rgb(46, 101, 28);\n"
"	border:3px solid rgb(112, 238, 72);\n"
"}\n"
"\n"
"QPushButton[control=\"infoled\"][status=\"warn\"]{\n"
"	border: 3px solid rgb(255, 178, 27);\n"
"	background-color: rgb(149, 93, 26);\n"
"}\n"
"\n"
"\n"
"\n"
"/* Status bar*/\n"
"#s"
                        "tatus_bar {\n"
"	background-color: rgb(8, 40, 47);\n"
"}\n"
"\n"
"QPushButton[status=\"off\"]{\n"
"	background-color: #811818;\n"
"	border:3px solid rgb(229, 30, 54);\n"
"}\n"
"\n"
"QPushButton[status=\"on\"]{\n"
"	background-color: rgb(46, 101, 28);\n"
"	border:3px solid rgb(112, 238, 72);\n"
"}\n"
"\n"
"QPushButton[status=\"starting\"]{\n"
"	border: 3px solid rgb(255, 178, 27);\n"
"	background-color: rgb(149, 93, 26);\n"
"}\n"
"\n"
"/* Variable state bar*/\n"
"#state_variable_bar {\n"
"	background-color: rgb(8, 40, 47);\n"
"}\n"
"\n"
"#battery_progress_bar[charge=\"low\"]:chunk{\n"
"    background-color: #F44336;\n"
"}\n"
"\n"
"#battery_progress_bar[charge=\"medium\"]:chunk{\n"
"    background-color: #FFB31A;\n"
"}\n"
"\n"
"#battery_progress_bar[charge=\"high\"]:chunk{\n"
"    background-color: #317C1A;\n"
"}\n"
"QPushButton[control=\"pilot_cam_record\"][status=\"off\"]{\n"
"	background-color: #811818;\n"
"	border:3px solid rgb(229, 30, 54);\n"
"}\n"
"\n"
"QPushButton[control=\"pilot_cam_record\"][status=\"on\"]{\n"
"	background-color: rgb(46, 101, 28);\n"
"	border:3px solid rgb(112, 238, 72);\n"
"}\n"
"")
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.centralwidget.setStyleSheet(u"")
        self.horizontalLayout_2 = QHBoxLayout(self.centralwidget)
        self.horizontalLayout_2.setSpacing(0)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.horizontalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.leftMenuContainer = QFrame(self.centralwidget)
        self.leftMenuContainer.setObjectName(u"leftMenuContainer")
        self.leftMenuContainer.setStyleSheet(u"")
        self.leftMenuContainer.setFrameShape(QFrame.StyledPanel)
        self.leftMenuContainer.setFrameShadow(QFrame.Raised)
        self.verticalLayout = QVBoxLayout(self.leftMenuContainer)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(6, -1, 6, -1)
        self.frame = QFrame(self.leftMenuContainer)
        self.frame.setObjectName(u"frame")
        sizePolicy = QSizePolicy(QSizePolicy.Preferred, QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.frame.sizePolicy().hasHeightForWidth())
        self.frame.setSizePolicy(sizePolicy)
        self.frame.setFrameShape(QFrame.StyledPanel)
        self.frame.setFrameShadow(QFrame.Raised)
        self.horizontalLayout = QHBoxLayout(self.frame)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.menu_status_selector = QPushButton(self.frame)
        self.menu_status_selector.setObjectName(u"menu_status_selector")
        self.menu_status_selector.setCursor(QCursor(Qt.PointingHandCursor))
        self.menu_status_selector.setStyleSheet(u"background-color: #03273D;")
        icon = QIcon()
        icon.addFile(u":/icons/icons/check-circle.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.menu_status_selector.setIcon(icon)
        self.menu_status_selector.setIconSize(QSize(24, 24))
        self.menu_status_selector.setFlat(True)

        self.horizontalLayout.addWidget(self.menu_status_selector)


        self.verticalLayout.addWidget(self.frame)

        self.frame_2 = QFrame(self.leftMenuContainer)
        self.frame_2.setObjectName(u"frame_2")
        self.frame_2.setFrameShape(QFrame.StyledPanel)
        self.frame_2.setFrameShadow(QFrame.Raised)
        self.verticalLayout_2 = QVBoxLayout(self.frame_2)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.menu_logger_selector = QPushButton(self.frame_2)
        self.menu_logger_selector.setObjectName(u"menu_logger_selector")
        self.menu_logger_selector.setCursor(QCursor(Qt.PointingHandCursor))
        icon1 = QIcon()
        icon1.addFile(u":/icons/icons/pen-tool.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.menu_logger_selector.setIcon(icon1)
        self.menu_logger_selector.setIconSize(QSize(24, 24))
        self.menu_logger_selector.setFlat(True)

        self.verticalLayout_2.addWidget(self.menu_logger_selector)

        self.menu_sub_selector = QPushButton(self.frame_2)
        self.menu_sub_selector.setObjectName(u"menu_sub_selector")
        self.menu_sub_selector.setCursor(QCursor(Qt.PointingHandCursor))
        icon2 = QIcon()
        icon2.addFile(u":/icons/icons/activity.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.menu_sub_selector.setIcon(icon2)
        self.menu_sub_selector.setIconSize(QSize(24, 24))
        self.menu_sub_selector.setFlat(True)

        self.verticalLayout_2.addWidget(self.menu_sub_selector)

        self.menu_data_selector = QPushButton(self.frame_2)
        self.menu_data_selector.setObjectName(u"menu_data_selector")
        self.menu_data_selector.setCursor(QCursor(Qt.PointingHandCursor))
        icon3 = QIcon()
        icon3.addFile(u":/icons/icons/bar-chart-2.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.menu_data_selector.setIcon(icon3)
        self.menu_data_selector.setIconSize(QSize(24, 24))
        self.menu_data_selector.setFlat(True)

        self.verticalLayout_2.addWidget(self.menu_data_selector)

        self.menu_light_selector = QPushButton(self.frame_2)
        self.menu_light_selector.setObjectName(u"menu_light_selector")
        self.menu_light_selector.setCursor(QCursor(Qt.PointingHandCursor))
        icon4 = QIcon()
        icon4.addFile(u":/icons/icons/tv.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.menu_light_selector.setIcon(icon4)
        self.menu_light_selector.setIconSize(QSize(24, 24))
        self.menu_light_selector.setFlat(True)

        self.verticalLayout_2.addWidget(self.menu_light_selector)


        self.verticalLayout.addWidget(self.frame_2)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.verticalLayout.addItem(self.verticalSpacer)

        self.toolbar = QWidget(self.leftMenuContainer)
        self.toolbar.setObjectName(u"toolbar")
        sizePolicy1 = QSizePolicy(QSizePolicy.Expanding, QSizePolicy.Preferred)
        sizePolicy1.setHorizontalStretch(1)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.toolbar.sizePolicy().hasHeightForWidth())
        self.toolbar.setSizePolicy(sizePolicy1)
        self.toolbar.setStyleSheet(u"QPushButton {\n"
"	font-size:  16px;\n"
"	padding: 5px 10px;\n"
"	background-color: rgb(34, 128, 105);\n"
"	color: white;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background-color: rgb(30, 136, 107);\n"
"}")
        self.verticalLayout_11 = QVBoxLayout(self.toolbar)
        self.verticalLayout_11.setObjectName(u"verticalLayout_11")
        self.tool_bar_sinaps = QPushButton(self.toolbar)
        self.tool_bar_sinaps.setObjectName(u"tool_bar_sinaps")
        font = QFont()
        self.tool_bar_sinaps.setFont(font)
        self.tool_bar_sinaps.setCursor(QCursor(Qt.PointingHandCursor))
        icon5 = QIcon()
        icon5.addFile(u":/icons/icons/tool.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.tool_bar_sinaps.setIcon(icon5)
        self.tool_bar_sinaps.setIconSize(QSize(24, 24))

        self.verticalLayout_11.addWidget(self.tool_bar_sinaps)

        self.tool_bar_fog_data = QPushButton(self.toolbar)
        self.tool_bar_fog_data.setObjectName(u"tool_bar_fog_data")
        self.tool_bar_fog_data.setFont(font)
        self.tool_bar_fog_data.setCursor(QCursor(Qt.PointingHandCursor))
        icon6 = QIcon()
        icon6.addFile(u":/icons/icons/monitor.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.tool_bar_fog_data.setIcon(icon6)
        self.tool_bar_fog_data.setIconSize(QSize(24, 24))

        self.verticalLayout_11.addWidget(self.tool_bar_fog_data)

        self.tool_bar_fog = QPushButton(self.toolbar)
        self.tool_bar_fog.setObjectName(u"tool_bar_fog")
        self.tool_bar_fog.setFont(font)
        self.tool_bar_fog.setCursor(QCursor(Qt.PointingHandCursor))
        icon7 = QIcon()
        icon7.addFile(u":/icons/icons/navigation.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.tool_bar_fog.setIcon(icon7)
        self.tool_bar_fog.setIconSize(QSize(24, 24))

        self.verticalLayout_11.addWidget(self.tool_bar_fog)

        self.tool_bar_webcam = QPushButton(self.toolbar)
        self.tool_bar_webcam.setObjectName(u"tool_bar_webcam")
        self.tool_bar_webcam.setFont(font)
        self.tool_bar_webcam.setCursor(QCursor(Qt.PointingHandCursor))
        icon8 = QIcon()
        icon8.addFile(u":/icons/icons/camera.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.tool_bar_webcam.setIcon(icon8)
        self.tool_bar_webcam.setIconSize(QSize(24, 24))

        self.verticalLayout_11.addWidget(self.tool_bar_webcam)

        
        self.tool_bar_nikon = QPushButton(self.toolbar)
        self.tool_bar_nikon.setObjectName(u"tool_bar_nikon")
        self.tool_bar_nikon.setFont(font)
        self.tool_bar_nikon.setCursor(QCursor(Qt.PointingHandCursor))
        icon8 = QIcon()
        icon8.addFile(u":/icons/icons/camera.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.tool_bar_nikon.setIcon(icon8)
        self.tool_bar_nikon.setIconSize(QSize(24, 24))

        self.verticalLayout_11.addWidget(self.tool_bar_nikon)

        
        self.tool_bar_multibeam = QPushButton(self.toolbar)
        self.tool_bar_multibeam.setObjectName(u"tool_bar_multibeam")
        self.tool_bar_multibeam.setFont(font)
        self.tool_bar_multibeam.setCursor(QCursor(Qt.PointingHandCursor))
        icon8 = QIcon()
        icon8.addFile(u":/icons/icons/multibeam.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.tool_bar_multibeam.setIcon(icon8)
        self.tool_bar_multibeam.setIconSize(QSize(24, 24))

        self.verticalLayout_11.addWidget(self.tool_bar_multibeam)

        
        self.tool_bar_qinsy = QPushButton(self.toolbar)
        self.tool_bar_qinsy.setObjectName(u"tool_bar_qinsy")
        self.tool_bar_qinsy.setFont(font)
        self.tool_bar_qinsy.setCursor(QCursor(Qt.PointingHandCursor))
        icon8 = QIcon()
        icon8.addFile(u":/icons/icons/camera.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.tool_bar_qinsy.setIcon(icon8)
        self.tool_bar_qinsy.setIconSize(QSize(24, 24))

        self.verticalLayout_11.addWidget(self.tool_bar_qinsy)


        self.verticalLayout.addWidget(self.toolbar)

        self.frame_3 = QFrame(self.leftMenuContainer)
        self.frame_3.setObjectName(u"frame_3")
        self.frame_3.setFrameShape(QFrame.StyledPanel)
        self.frame_3.setFrameShadow(QFrame.Raised)
        self.horizontalLayout_3 = QHBoxLayout(self.frame_3)
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.menu_settings_selector = QPushButton(self.frame_3)
        self.menu_settings_selector.setObjectName(u"menu_settings_selector")
        self.menu_settings_selector.setCursor(QCursor(Qt.PointingHandCursor))
        icon9 = QIcon()
        icon9.addFile(u":/icons/icons/settings.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.menu_settings_selector.setIcon(icon9)
        self.menu_settings_selector.setIconSize(QSize(24, 24))
        self.menu_settings_selector.setFlat(True)

        self.horizontalLayout_3.addWidget(self.menu_settings_selector)


        self.verticalLayout.addWidget(self.frame_3)


        self.horizontalLayout_2.addWidget(self.leftMenuContainer)

        self.rightMainContainer = QFrame(self.centralwidget)
        self.rightMainContainer.setObjectName(u"rightMainContainer")
        sizePolicy2 = QSizePolicy(QSizePolicy.Expanding, QSizePolicy.Preferred)
        sizePolicy2.setHorizontalStretch(0)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.rightMainContainer.sizePolicy().hasHeightForWidth())
        self.rightMainContainer.setSizePolicy(sizePolicy2)
        self.rightMainContainer.setAcceptDrops(True)
        self.rightMainContainer.setStyleSheet(u"")
        self.rightMainContainer.setFrameShape(QFrame.StyledPanel)
        self.rightMainContainer.setFrameShadow(QFrame.Raised)
        self.gridLayout_11 = QGridLayout(self.rightMainContainer)
        self.gridLayout_11.setSpacing(6)
        self.gridLayout_11.setObjectName(u"gridLayout_11")
        self.gridLayout_11.setContentsMargins(6, 6, 6, 6)
        self.histogram = PlotWidget(self.rightMainContainer)
        self.histogram.setObjectName(u"histogram")
        self.histogram.setMinimumSize(QSize(72, 0))
        self.histogram.setMaximumSize(QSize(90, 16777215))
        self.histogram.setStyleSheet(u"border-radius: 10px; background-color: black;")

        self.gridLayout_11.addWidget(self.histogram, 1, 1, 1, 1)

        self.main_content_widget = QStackedWidget(self.rightMainContainer)
        self.main_content_widget.setObjectName(u"main_content_widget")
        self.main_content_widget.setFocusPolicy(Qt.StrongFocus)
        self.main_content_widget.setStyleSheet(u"")
        self.main_content_widget.setLineWidth(1)
        self.status_widget = QWidget()
        self.status_widget.setObjectName(u"status_widget")
        self.verticalLayout_4 = QVBoxLayout(self.status_widget)
        self.verticalLayout_4.setObjectName(u"verticalLayout_4")
        self.verticalLayout_4.setContentsMargins(0, 0, 0, 0)
        self.frame_8 = QFrame(self.status_widget)
        self.frame_8.setObjectName(u"frame_8")
        self.frame_8.setStyleSheet(u"")
        self.frame_8.setFrameShape(QFrame.StyledPanel)
        self.frame_8.setFrameShadow(QFrame.Raised)
        self.gridLayout_2 = QGridLayout(self.frame_8)
        self.gridLayout_2.setSpacing(6)
        self.gridLayout_2.setObjectName(u"gridLayout_2")
        self.gridLayout_2.setContentsMargins(6, 6, 6, 6)
        self.verticalSpacer_3 = QSpacerItem(20, 40, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.gridLayout_2.addItem(self.verticalSpacer_3, 2, 0, 1, 2)

        self.frame_4 = QFrame(self.frame_8)
        self.frame_4.setObjectName(u"frame_4")
        sizePolicy2.setHeightForWidth(self.frame_4.sizePolicy().hasHeightForWidth())
        self.frame_4.setSizePolicy(sizePolicy2)
        self.frame_4.setFrameShape(QFrame.StyledPanel)
        self.frame_4.setFrameShadow(QFrame.Raised)
        self.verticalLayout_5 = QVBoxLayout(self.frame_4)
        self.verticalLayout_5.setObjectName(u"verticalLayout_5")
        self.status_page_ssh = QLabel(self.frame_4)
        self.status_page_ssh.setObjectName(u"status_page_ssh")

        self.verticalLayout_5.addWidget(self.status_page_ssh, 0, Qt.AlignTop)


        self.gridLayout_2.addWidget(self.frame_4, 0, 1, 1, 1)

        self.frame_7 = QFrame(self.frame_8)
        self.frame_7.setObjectName(u"frame_7")
        self.frame_7.setFrameShape(QFrame.StyledPanel)
        self.frame_7.setFrameShadow(QFrame.Raised)
        self.verticalLayout_8 = QVBoxLayout(self.frame_7)
        self.verticalLayout_8.setObjectName(u"verticalLayout_8")
        self.status_page_mqtt = QLabel(self.frame_7)
        self.status_page_mqtt.setObjectName(u"status_page_mqtt")

        self.verticalLayout_8.addWidget(self.status_page_mqtt, 0, Qt.AlignTop)

        self.frame_16 = QFrame(self.frame_7)
        self.frame_16.setObjectName(u"frame_16")
        self.frame_16.setFrameShape(QFrame.StyledPanel)
        self.frame_16.setFrameShadow(QFrame.Raised)
        self.verticalLayout_12 = QVBoxLayout(self.frame_16)
        self.verticalLayout_12.setObjectName(u"verticalLayout_12")
        self.status_page_fog_udp = QLabel(self.frame_16)
        self.status_page_fog_udp.setObjectName(u"status_page_fog_udp")

        self.verticalLayout_12.addWidget(self.status_page_fog_udp)


        self.verticalLayout_8.addWidget(self.frame_16, 0, Qt.AlignTop)


        self.gridLayout_2.addWidget(self.frame_7, 1, 0, 1, 1)

        self.frame_15 = QFrame(self.frame_8)
        self.frame_15.setObjectName(u"frame_15")
        self.frame_15.setFrameShape(QFrame.StyledPanel)
        self.frame_15.setFrameShadow(QFrame.Raised)
        self.verticalLayout_9 = QVBoxLayout(self.frame_15)
        self.verticalLayout_9.setObjectName(u"verticalLayout_9")
        self.status_page_logger = QLabel(self.frame_15)
        self.status_page_logger.setObjectName(u"status_page_logger")

        self.verticalLayout_9.addWidget(self.status_page_logger, 0, Qt.AlignTop)

        self.status_page_svlogger = QLabel(self.frame_15)
        self.status_page_svlogger.setObjectName(u"status_page_svlogger")

        self.verticalLayout_9.addWidget(self.status_page_svlogger)


        self.gridLayout_2.addWidget(self.frame_15, 1, 1, 1, 1)

        self.frame_5 = QFrame(self.frame_8)
        self.frame_5.setObjectName(u"frame_5")
        self.frame_5.setStyleSheet(u"QLabel[sub_data_status=\"disconnected\"]{\n"
"	color: rgb(77, 77, 77);\n"
"}\n"
"\n"
"QLabel[sub_data_status=\"ok\"]{\n"
"	color: rgb(69, 255, 114);\n"
"}\n"
"\n"
"QLabel[sub_data_status=\"timestamp\"]{\n"
"	color: rgb(255, 221, 19);\n"
"}\n"
"\n"
"QLabel[sub_data_status=\"invalid\"]{\n"
"	color: rgb(255, 136, 19);\n"
"}\n"
"\n"
"QLabel[sub_data_status=\"off\"]{\n"
"	color: rgb(255, 15, 6);\n"
"}")
        self.frame_5.setFrameShape(QFrame.StyledPanel)
        self.frame_5.setFrameShadow(QFrame.Raised)
        self.verticalLayout_6 = QVBoxLayout(self.frame_5)
        self.verticalLayout_6.setObjectName(u"verticalLayout_6")
        self.status_page_sub = QLabel(self.frame_5)
        self.status_page_sub.setObjectName(u"status_page_sub")

        self.verticalLayout_6.addWidget(self.status_page_sub, 0, Qt.AlignTop)

        self.frame_6 = QFrame(self.frame_5)
        self.frame_6.setObjectName(u"frame_6")
        self.frame_6.setFrameShape(QFrame.StyledPanel)
        self.frame_6.setFrameShadow(QFrame.Raised)
        self.verticalLayout_7 = QVBoxLayout(self.frame_6)
        self.verticalLayout_7.setObjectName(u"verticalLayout_7")
        self.status_page_minisvs = QLabel(self.frame_6)
        self.status_page_minisvs.setObjectName(u"status_page_minisvs")

        self.verticalLayout_7.addWidget(self.status_page_minisvs)

        self.status_page_us_imu = QLabel(self.frame_6)
        self.status_page_us_imu.setObjectName(u"status_page_us_imu")

        self.verticalLayout_7.addWidget(self.status_page_us_imu)


        self.verticalLayout_6.addWidget(self.frame_6, 0, Qt.AlignTop)


        self.gridLayout_2.addWidget(self.frame_5, 0, 0, 1, 1)

        self.gridLayout_2.setColumnStretch(0, 1)
        self.gridLayout_2.setColumnStretch(1, 1)

        self.verticalLayout_4.addWidget(self.frame_8)

        self.main_content_widget.addWidget(self.status_widget)
        self.logger_widget = QWidget()
        self.logger_widget.setObjectName(u"logger_widget")
        self.logger_widget.setStyleSheet(u"")
        self.gridLayout = QGridLayout(self.logger_widget)
        self.gridLayout.setObjectName(u"gridLayout")
        self.gridLayout.setSizeConstraint(QLayout.SetDefaultConstraint)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.frame_9 = QFrame(self.logger_widget)
        self.frame_9.setObjectName(u"frame_9")
        self.frame_9.setStyleSheet(u"")
        self.frame_9.setFrameShape(QFrame.StyledPanel)
        self.frame_9.setFrameShadow(QFrame.Raised)
        self.gridLayout_15 = QGridLayout(self.frame_9)
        self.gridLayout_15.setSpacing(6)
        self.gridLayout_15.setObjectName(u"gridLayout_15")
        self.gridLayout_15.setContentsMargins(6, 6, 6, 6)
        self.frame_10 = QFrame(self.frame_9)
        self.frame_10.setObjectName(u"frame_10")
        self.frame_10.setStyleSheet(u"")
        self.frame_10.setFrameShape(QFrame.StyledPanel)
        self.frame_10.setFrameShadow(QFrame.Raised)
        self.gridLayout_6 = QGridLayout(self.frame_10)
        self.gridLayout_6.setObjectName(u"gridLayout_6")
        self.gridLayout_6.setHorizontalSpacing(-1)
        self.topic_selected_counter_label = QLabel(self.frame_10)
        self.topic_selected_counter_label.setObjectName(u"topic_selected_counter_label")

        self.gridLayout_6.addWidget(self.topic_selected_counter_label, 4, 1, 1, 1, Qt.AlignLeft)

        self.label_31 = QLabel(self.frame_10)
        self.label_31.setObjectName(u"label_31")
        self.label_31.setFont(font)

        self.gridLayout_6.addWidget(self.label_31, 0, 0, 1, 1)

        self.selected_log_folder_path_label = QLabel(self.frame_10)
        self.selected_log_folder_path_label.setObjectName(u"selected_log_folder_path_label")
        self.selected_log_folder_path_label.setMinimumSize(QSize(348, 0))
        self.selected_log_folder_path_label.setFont(font)
        self.selected_log_folder_path_label.setAcceptDrops(True)
        self.selected_log_folder_path_label.setStyleSheet(u"")
        self.selected_log_folder_path_label.setScaledContents(True)
        self.selected_log_folder_path_label.setWordWrap(False)

        self.gridLayout_6.addWidget(self.selected_log_folder_path_label, 2, 1, 1, 4)

        self.stop_logger_btn = QPushButton(self.frame_10)
        self.stop_logger_btn.setObjectName(u"stop_logger_btn")
        self.stop_logger_btn.setEnabled(False)
        self.stop_logger_btn.setCursor(QCursor(Qt.PointingHandCursor))

        self.gridLayout_6.addWidget(self.stop_logger_btn, 4, 4, 1, 1, Qt.AlignRight)

        self.select_topic_btn = QPushButton(self.frame_10)
        self.select_topic_btn.setObjectName(u"select_topic_btn")
        self.select_topic_btn.setCursor(QCursor(Qt.PointingHandCursor))

        self.gridLayout_6.addWidget(self.select_topic_btn, 4, 0, 1, 1, Qt.AlignLeft)

        self.select_log_folder_btn = QPushButton(self.frame_10)
        self.select_log_folder_btn.setObjectName(u"select_log_folder_btn")
        self.select_log_folder_btn.setCursor(QCursor(Qt.PointingHandCursor))

        self.gridLayout_6.addWidget(self.select_log_folder_btn, 2, 0, 1, 1, Qt.AlignLeft)

        self.max_line_selector = QSpinBox(self.frame_10)
        self.max_line_selector.setObjectName(u"max_line_selector")
        self.max_line_selector.setStyleSheet(u"")
        self.max_line_selector.setMinimum(0)
        self.max_line_selector.setMaximum(10000000)

        self.gridLayout_6.addWidget(self.max_line_selector, 1, 1, 1, 1, Qt.AlignLeft)

        self.export_btn = QPushButton(self.frame_10)
        self.export_btn.setObjectName(u"export_btn")
        self.export_btn.setEnabled(True)
        self.export_btn.setCursor(QCursor(Qt.PointingHandCursor))
        self.export_btn.setStyleSheet(u"")

        self.gridLayout_6.addWidget(self.export_btn, 1, 4, 1, 1, Qt.AlignRight)

        self.start_logger_btn = QPushButton(self.frame_10)
        self.start_logger_btn.setObjectName(u"start_logger_btn")
        self.start_logger_btn.setEnabled(False)
        self.start_logger_btn.setCursor(QCursor(Qt.PointingHandCursor))

        self.gridLayout_6.addWidget(self.start_logger_btn, 4, 3, 1, 1, Qt.AlignRight)

        self.label = QLabel(self.frame_10)
        self.label.setObjectName(u"label")

        self.gridLayout_6.addWidget(self.label, 1, 0, 1, 1, Qt.AlignLeft)

        self.log_fog_data_checkbox = QCheckBox(self.frame_10)
        self.log_fog_data_checkbox.setObjectName(u"log_fog_data_checkbox")
        font1 = QFont()
        font1.setPointSize(20)
        self.log_fog_data_checkbox.setFont(font1)
        self.log_fog_data_checkbox.setStyleSheet(u"background: none;")
        self.log_fog_data_checkbox.setTristate(False)

        self.gridLayout_6.addWidget(self.log_fog_data_checkbox, 1, 2, 1, 1)

        self.gridLayout_6.setColumnStretch(1, 1)
        self.gridLayout_6.setColumnStretch(2, 3)

        self.gridLayout_15.addWidget(self.frame_10, 0, 0, 1, 2, Qt.AlignTop)

        self.frame_11 = QFrame(self.frame_9)
        self.frame_11.setObjectName(u"frame_11")
        self.frame_11.setMaximumSize(QSize(420, 16777215))
        self.frame_11.setFrameShape(QFrame.StyledPanel)
        self.frame_11.setFrameShadow(QFrame.Raised)
        self.gridLayout_14 = QGridLayout(self.frame_11)
        self.gridLayout_14.setObjectName(u"gridLayout_14")
        self.start_svlogger_btn = QPushButton(self.frame_11)
        self.start_svlogger_btn.setObjectName(u"start_svlogger_btn")
        self.start_svlogger_btn.setEnabled(False)
        self.start_svlogger_btn.setCursor(QCursor(Qt.PointingHandCursor))

        self.gridLayout_14.addWidget(self.start_svlogger_btn, 2, 1, 1, 1, Qt.AlignRight)

        self.line_14 = QFrame(self.frame_11)
        self.line_14.setObjectName(u"line_14")
        self.line_14.setFrameShape(QFrame.HLine)
        self.line_14.setFrameShadow(QFrame.Sunken)

        self.gridLayout_14.addWidget(self.line_14, 3, 0, 1, 3)

        self.stop_svlogger_btn = QPushButton(self.frame_11)
        self.stop_svlogger_btn.setObjectName(u"stop_svlogger_btn")
        self.stop_svlogger_btn.setEnabled(False)
        self.stop_svlogger_btn.setCursor(QCursor(Qt.PointingHandCursor))

        self.gridLayout_14.addWidget(self.stop_svlogger_btn, 2, 2, 1, 1, Qt.AlignRight)

        self.select_profile_btn = QPushButton(self.frame_11)
        self.select_profile_btn.setObjectName(u"select_profile_btn")
        self.select_profile_btn.setCursor(QCursor(Qt.PointingHandCursor))

        self.gridLayout_14.addWidget(self.select_profile_btn, 5, 0, 1, 1, Qt.AlignLeft)

        self.selected_profile_path_label = QLabel(self.frame_11)
        self.selected_profile_path_label.setObjectName(u"selected_profile_path_label")
        self.selected_profile_path_label.setMaximumSize(QSize(238, 16777215))
        self.selected_profile_path_label.setFont(font)
        self.selected_profile_path_label.setAcceptDrops(True)
        self.selected_profile_path_label.setStyleSheet(u"")

        self.gridLayout_14.addWidget(self.selected_profile_path_label, 5, 1, 1, 2)

        self.verticalSpacer_2 = QSpacerItem(20, 40, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.gridLayout_14.addItem(self.verticalSpacer_2, 7, 0, 1, 1)

        self.plot_profile_btn = QPushButton(self.frame_11)
        self.plot_profile_btn.setObjectName(u"plot_profile_btn")
        self.plot_profile_btn.setEnabled(False)
        self.plot_profile_btn.setCursor(QCursor(Qt.PointingHandCursor))

        self.gridLayout_14.addWidget(self.plot_profile_btn, 6, 0, 1, 1, Qt.AlignLeft)

        self.selected_svlog_folder_path_label = QLabel(self.frame_11)
        self.selected_svlog_folder_path_label.setObjectName(u"selected_svlog_folder_path_label")
        self.selected_svlog_folder_path_label.setFont(font)
        self.selected_svlog_folder_path_label.setAcceptDrops(True)
        self.selected_svlog_folder_path_label.setStyleSheet(u"")

        self.gridLayout_14.addWidget(self.selected_svlog_folder_path_label, 1, 1, 1, 2)

        self.label_32 = QLabel(self.frame_11)
        self.label_32.setObjectName(u"label_32")
        self.label_32.setFont(font)

        self.gridLayout_14.addWidget(self.label_32, 0, 0, 1, 1, Qt.AlignTop)

        self.select_svlog_folder_btn = QPushButton(self.frame_11)
        self.select_svlog_folder_btn.setObjectName(u"select_svlog_folder_btn")
        self.select_svlog_folder_btn.setEnabled(True)
        self.select_svlog_folder_btn.setCursor(QCursor(Qt.PointingHandCursor))
        self.select_svlog_folder_btn.setStyleSheet(u"")

        self.gridLayout_14.addWidget(self.select_svlog_folder_btn, 1, 0, 1, 1, Qt.AlignLeft)

        self.export_sv_profile_btn = QPushButton(self.frame_11)
        self.export_sv_profile_btn.setObjectName(u"export_sv_profile_btn")
        self.export_sv_profile_btn.setEnabled(True)
        self.export_sv_profile_btn.setCursor(QCursor(Qt.PointingHandCursor))
        self.export_sv_profile_btn.setStyleSheet(u"")

        self.gridLayout_14.addWidget(self.export_sv_profile_btn, 2, 0, 1, 1, Qt.AlignLeft)

        self.label_67 = QLabel(self.frame_11)
        self.label_67.setObjectName(u"label_67")

        self.gridLayout_14.addWidget(self.label_67, 4, 0, 1, 1)

        self.gridLayout_14.setColumnStretch(1, 1)

        self.gridLayout_15.addWidget(self.frame_11, 1, 0, 1, 1)

        self.sv_profile_plot = PlotWidget(self.frame_9)
        self.sv_profile_plot.setObjectName(u"sv_profile_plot")
        self.sv_profile_plot.setStyleSheet(u"background-color: rgb(29, 66, 81);")

        self.gridLayout_15.addWidget(self.sv_profile_plot, 1, 1, 1, 1)

        self.gridLayout_15.setRowStretch(1, 1)
        self.gridLayout_15.setColumnStretch(0, 1)
        self.gridLayout_15.setColumnStretch(1, 1)

        self.gridLayout.addWidget(self.frame_9, 0, 0, 1, 1)

        self.main_content_widget.addWidget(self.logger_widget)
        self.subscriber_widget = QWidget()
        self.subscriber_widget.setObjectName(u"subscriber_widget")
        self.subscriber_widget.setStyleSheet(u"")
        self.gridLayout_3 = QGridLayout(self.subscriber_widget)
        self.gridLayout_3.setObjectName(u"gridLayout_3")
        self.gridLayout_3.setContentsMargins(6, 6, 6, 6)
        self.ssh_error_alert_frame = QFrame(self.subscriber_widget)
        self.ssh_error_alert_frame.setObjectName(u"ssh_error_alert_frame")
        self.ssh_error_alert_frame.setFrameShape(QFrame.StyledPanel)
        self.ssh_error_alert_frame.setFrameShadow(QFrame.Raised)
        self.verticalLayout_13 = QVBoxLayout(self.ssh_error_alert_frame)
        self.verticalLayout_13.setObjectName(u"verticalLayout_13")
        self.label_6 = QLabel(self.ssh_error_alert_frame)
        self.label_6.setObjectName(u"label_6")

        self.verticalLayout_13.addWidget(self.label_6, 0, Qt.AlignHCenter)

        self.reconnect_ssh_btn = QPushButton(self.ssh_error_alert_frame)
        self.reconnect_ssh_btn.setObjectName(u"reconnect_ssh_btn")
        self.reconnect_ssh_btn.setCursor(QCursor(Qt.PointingHandCursor))

        self.verticalLayout_13.addWidget(self.reconnect_ssh_btn, 0, Qt.AlignHCenter)


        self.gridLayout_3.addWidget(self.ssh_error_alert_frame, 2, 0, 1, 1)

        self.verticalSpacer_7 = QSpacerItem(20, 40, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.gridLayout_3.addItem(self.verticalSpacer_7, 3, 0, 1, 1)

        self.sub_button_frame = QFrame(self.subscriber_widget)
        self.sub_button_frame.setObjectName(u"sub_button_frame")
        self.sub_button_frame.setStyleSheet(u"")
        self.sub_button_frame.setFrameShape(QFrame.StyledPanel)
        self.sub_button_frame.setFrameShadow(QFrame.Raised)
        self.horizontalLayout_10 = QHBoxLayout(self.sub_button_frame)
        self.horizontalLayout_10.setObjectName(u"horizontalLayout_10")
        self.start_sub_btn = QPushButton(self.sub_button_frame)
        self.start_sub_btn.setObjectName(u"start_sub_btn")
        self.start_sub_btn.setCursor(QCursor(Qt.PointingHandCursor))

        self.horizontalLayout_10.addWidget(self.start_sub_btn)

        self.horizontalSpacer_3 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_10.addItem(self.horizontalSpacer_3)

        self.stop_sub_btn = QPushButton(self.sub_button_frame)
        self.stop_sub_btn.setObjectName(u"stop_sub_btn")
        self.stop_sub_btn.setCursor(QCursor(Qt.PointingHandCursor))

        self.horizontalLayout_10.addWidget(self.stop_sub_btn)


        self.gridLayout_3.addWidget(self.sub_button_frame, 0, 0, 1, 1, Qt.AlignLeft)

        self.main_content_widget.addWidget(self.subscriber_widget)
        self.data_widget = QWidget()
        self.data_widget.setObjectName(u"data_widget")
        self.data_widget.setStyleSheet(u"")
        self.verticalLayout_14 = QVBoxLayout(self.data_widget)
        self.verticalLayout_14.setObjectName(u"verticalLayout_14")
        self.verticalLayout_14.setContentsMargins(0, 0, 0, 0)
        self.widget = QWidget(self.data_widget)
        self.widget.setObjectName(u"widget")
        self.widget.setStyleSheet(u"")
        self.gridLayout_5 = QGridLayout(self.widget)
        self.gridLayout_5.setSpacing(6)
        self.gridLayout_5.setObjectName(u"gridLayout_5")
        self.gridLayout_5.setContentsMargins(6, 6, 6, 6)
        self.groupBox_19 = QGroupBox(self.widget)
        self.groupBox_19.setObjectName(u"groupBox_19")
        font2 = QFont()
        font2.setPointSize(18)
        self.groupBox_19.setFont(font2)
        self.groupBox_19.setStyleSheet(u"QLabel[status=\"safe\"] {\n"
"	background-color: rgb(67, 151, 73);\n"
"}\n"
"\n"
"QLabel[status=\"warning\"] {\n"
"	background-color: rgb(218, 139, 49);\n"
"}\n"
"\n"
"QLabel[status=\"danger\"] {\n"
"	background-color: rgb(199, 54, 48);\n"
"}")
        self.groupBox_19.setAlignment(Qt.AlignCenter)
        self.gridLayout_4 = QGridLayout(self.groupBox_19)
        self.gridLayout_4.setObjectName(u"gridLayout_4")
        self.gridLayout_4.setHorizontalSpacing(12)
        self.gridLayout_4.setContentsMargins(-1, 30, -1, -1)
        self.label_14 = QLabel(self.groupBox_19)
        self.label_14.setObjectName(u"label_14")

        self.gridLayout_4.addWidget(self.label_14, 4, 0, 1, 1, Qt.AlignRight)

        self.label_27 = QLabel(self.groupBox_19)
        self.label_27.setObjectName(u"label_27")

        self.gridLayout_4.addWidget(self.label_27, 2, 4, 1, 1, Qt.AlignRight)

        self.data_x = QLabel(self.groupBox_19)
        self.data_x.setObjectName(u"data_x")

        self.gridLayout_4.addWidget(self.data_x, 0, 5, 1, 1)

        self.label_9 = QLabel(self.groupBox_19)
        self.label_9.setObjectName(u"label_9")

        self.gridLayout_4.addWidget(self.label_9, 0, 0, 1, 1, Qt.AlignRight)

        self.label_12 = QLabel(self.groupBox_19)
        self.label_12.setObjectName(u"label_12")

        self.gridLayout_4.addWidget(self.label_12, 2, 0, 1, 1, Qt.AlignRight)

        self.label_26 = QLabel(self.groupBox_19)
        self.label_26.setObjectName(u"label_26")

        self.gridLayout_4.addWidget(self.label_26, 1, 4, 1, 1, Qt.AlignRight)

        self.data_tr = QLabel(self.groupBox_19)
        self.data_tr.setObjectName(u"data_tr")

        self.gridLayout_4.addWidget(self.data_tr, 3, 1, 1, 1)

        self.data_fu = QLabel(self.groupBox_19)
        self.data_fu.setObjectName(u"data_fu")
        self.data_fu.setMargin(0)

        self.gridLayout_4.addWidget(self.data_fu, 0, 1, 1, 1)

        self.data_fw = QLabel(self.groupBox_19)
        self.data_fw.setObjectName(u"data_fw")

        self.gridLayout_4.addWidget(self.data_fw, 2, 1, 1, 1)

        self.label_13 = QLabel(self.groupBox_19)
        self.label_13.setObjectName(u"label_13")

        self.gridLayout_4.addWidget(self.label_13, 3, 0, 1, 1, Qt.AlignRight)

        self.label_28 = QLabel(self.groupBox_19)
        self.label_28.setObjectName(u"label_28")

        self.gridLayout_4.addWidget(self.label_28, 3, 4, 1, 1, Qt.AlignRight)

        self.data_fv = QLabel(self.groupBox_19)
        self.data_fv.setObjectName(u"data_fv")

        self.gridLayout_4.addWidget(self.data_fv, 1, 1, 1, 1)

        self.data_y = QLabel(self.groupBox_19)
        self.data_y.setObjectName(u"data_y")

        self.gridLayout_4.addWidget(self.data_y, 1, 5, 1, 1)

        self.label_19 = QLabel(self.groupBox_19)
        self.label_19.setObjectName(u"label_19")

        self.gridLayout_4.addWidget(self.label_19, 0, 2, 1, 1, Qt.AlignRight)

        self.data_altitude = QLabel(self.groupBox_19)
        self.data_altitude.setObjectName(u"data_altitude")

        self.gridLayout_4.addWidget(self.data_altitude, 4, 1, 1, 1)

        self.label_11 = QLabel(self.groupBox_19)
        self.label_11.setObjectName(u"label_11")

        self.gridLayout_4.addWidget(self.label_11, 1, 0, 1, 1, Qt.AlignRight)

        self.label_25 = QLabel(self.groupBox_19)
        self.label_25.setObjectName(u"label_25")

        self.gridLayout_4.addWidget(self.label_25, 0, 4, 1, 1, Qt.AlignRight)

        self.data_v = QLabel(self.groupBox_19)
        self.data_v.setObjectName(u"data_v")

        self.gridLayout_4.addWidget(self.data_v, 1, 3, 1, 1)

        self.data_u = QLabel(self.groupBox_19)
        self.data_u.setObjectName(u"data_u")

        self.gridLayout_4.addWidget(self.data_u, 0, 3, 1, 1)

        self.data_z = QLabel(self.groupBox_19)
        self.data_z.setObjectName(u"data_z")

        self.gridLayout_4.addWidget(self.data_z, 2, 5, 1, 1)

        self.data_psi = QLabel(self.groupBox_19)
        self.data_psi.setObjectName(u"data_psi")

        self.gridLayout_4.addWidget(self.data_psi, 3, 5, 1, 1)

        self.label_21 = QLabel(self.groupBox_19)
        self.label_21.setObjectName(u"label_21")

        self.gridLayout_4.addWidget(self.label_21, 1, 2, 1, 1, Qt.AlignRight)

        self.data_r = QLabel(self.groupBox_19)
        self.data_r.setObjectName(u"data_r")

        self.gridLayout_4.addWidget(self.data_r, 3, 3, 1, 1)

        self.label_22 = QLabel(self.groupBox_19)
        self.label_22.setObjectName(u"label_22")

        self.gridLayout_4.addWidget(self.label_22, 3, 2, 1, 1, Qt.AlignRight)

        self.label_54 = QLabel(self.groupBox_19)
        self.label_54.setObjectName(u"label_54")

        self.gridLayout_4.addWidget(self.label_54, 2, 2, 1, 1, Qt.AlignRight)

        self.data_w = QLabel(self.groupBox_19)
        self.data_w.setObjectName(u"data_w")

        self.gridLayout_4.addWidget(self.data_w, 2, 3, 1, 1)

        self.gridLayout_4.setColumnStretch(1, 1)
        self.gridLayout_4.setColumnStretch(3, 1)
        self.gridLayout_4.setColumnStretch(5, 1)

        self.gridLayout_5.addWidget(self.groupBox_19, 0, 0, 1, 1)

        self.groupBox_21 = QGroupBox(self.widget)
        self.groupBox_21.setObjectName(u"groupBox_21")
        self.groupBox_21.setFont(font2)
        self.groupBox_21.setAlignment(Qt.AlignCenter)
        self.gridLayout_7 = QGridLayout(self.groupBox_21)
        self.gridLayout_7.setObjectName(u"gridLayout_7")
        self.gridLayout_7.setContentsMargins(-1, 30, -1, -1)
        self.data_temperature = QLabel(self.groupBox_21)
        self.data_temperature.setObjectName(u"data_temperature")
        self.data_temperature.setMargin(10)

        self.gridLayout_7.addWidget(self.data_temperature, 1, 1, 1, 1)

        self.label_15 = QLabel(self.groupBox_21)
        self.label_15.setObjectName(u"label_15")

        self.gridLayout_7.addWidget(self.label_15, 3, 0, 1, 1, Qt.AlignRight)

        self.data_conductivity = QLabel(self.groupBox_21)
        self.data_conductivity.setObjectName(u"data_conductivity")
        self.data_conductivity.setMargin(10)

        self.gridLayout_7.addWidget(self.data_conductivity, 0, 1, 1, 1)

        self.data_sound_speed = QLabel(self.groupBox_21)
        self.data_sound_speed.setObjectName(u"data_sound_speed")
        self.data_sound_speed.setMargin(10)

        self.gridLayout_7.addWidget(self.data_sound_speed, 3, 1, 1, 1)

        self.label_17 = QLabel(self.groupBox_21)
        self.label_17.setObjectName(u"label_17")

        self.gridLayout_7.addWidget(self.label_17, 1, 0, 1, 1, Qt.AlignRight)

        self.label_10 = QLabel(self.groupBox_21)
        self.label_10.setObjectName(u"label_10")

        self.gridLayout_7.addWidget(self.label_10, 4, 0, 1, 1, Qt.AlignRight)

        self.label_16 = QLabel(self.groupBox_21)
        self.label_16.setObjectName(u"label_16")

        self.gridLayout_7.addWidget(self.label_16, 0, 0, 1, 1, Qt.AlignRight)

        self.data_pressure = QLabel(self.groupBox_21)
        self.data_pressure.setObjectName(u"data_pressure")
        self.data_pressure.setMargin(10)

        self.gridLayout_7.addWidget(self.data_pressure, 4, 1, 1, 1)

        self.horizontalSpacer_4 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.gridLayout_7.addItem(self.horizontalSpacer_4, 2, 0, 1, 2)

        self.gridLayout_7.setColumnStretch(1, 1)

        self.gridLayout_5.addWidget(self.groupBox_21, 0, 1, 1, 1)

        self.frame_18 = QFrame(self.widget)
        self.frame_18.setObjectName(u"frame_18")
        self.frame_18.setFrameShape(QFrame.StyledPanel)
        self.frame_18.setFrameShadow(QFrame.Raised)
        self.horizontalLayout_5 = QHBoxLayout(self.frame_18)
        self.horizontalLayout_5.setObjectName(u"horizontalLayout_5")
        self.horizontalLayout_5.setContentsMargins(12, -1, 12, -1)
        self.horizontalSpacer_6 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_5.addItem(self.horizontalSpacer_6)

        self.frame_14 = QFrame(self.frame_18)
        self.frame_14.setObjectName(u"frame_14")
        self.frame_14.setFrameShape(QFrame.StyledPanel)
        self.frame_14.setFrameShadow(QFrame.Raised)
        self.verticalLayout_18 = QVBoxLayout(self.frame_14)
        self.verticalLayout_18.setObjectName(u"verticalLayout_18")
        self.verticalLayout_18.setContentsMargins(0, -1, -1, -1)
        self.frame_26 = QFrame(self.frame_14)
        self.frame_26.setObjectName(u"frame_26")
        self.frame_26.setFrameShape(QFrame.StyledPanel)
        self.frame_26.setFrameShadow(QFrame.Raised)
        self.horizontalLayout_6 = QHBoxLayout(self.frame_26)
        self.horizontalLayout_6.setSpacing(12)
        self.horizontalLayout_6.setObjectName(u"horizontalLayout_6")
        self.horizontalLayout_6.setContentsMargins(0, -1, 0, 0)
        self.compass_canvas = QWidget(self.frame_26)
        self.compass_canvas.setObjectName(u"compass_canvas")
        self.compass_canvas.setMinimumSize(QSize(250, 250))
        self.compass_canvas.setMaximumSize(QSize(250, 250))
        self.compass_canvas.setStyleSheet(u"border-radius: 125%;")

        self.horizontalLayout_6.addWidget(self.compass_canvas)

        self.frame_25 = QFrame(self.frame_26)
        self.frame_25.setObjectName(u"frame_25")
        self.frame_25.setMinimumSize(QSize(100, 0))
        self.frame_25.setFrameShape(QFrame.StyledPanel)
        self.frame_25.setFrameShadow(QFrame.Raised)
        self.verticalLayout_21 = QVBoxLayout(self.frame_25)
        self.verticalLayout_21.setObjectName(u"verticalLayout_21")
        self.label_18 = QLabel(self.frame_25)
        self.label_18.setObjectName(u"label_18")

        self.verticalLayout_21.addWidget(self.label_18)

        self.heading_label = QLabel(self.frame_25)
        self.heading_label.setObjectName(u"heading_label")

        self.verticalLayout_21.addWidget(self.heading_label)


        self.horizontalLayout_6.addWidget(self.frame_25, 0, Qt.AlignLeft|Qt.AlignVCenter)


        self.verticalLayout_18.addWidget(self.frame_26, 0, Qt.AlignLeft)

        self.frame_20 = QFrame(self.frame_14)
        self.frame_20.setObjectName(u"frame_20")
        self.frame_20.setFrameShape(QFrame.StyledPanel)
        self.frame_20.setFrameShadow(QFrame.Raised)
        self.horizontalLayout_4 = QHBoxLayout(self.frame_20)
        self.horizontalLayout_4.setObjectName(u"horizontalLayout_4")
        self.horizontalLayout_4.setContentsMargins(-1, 0, -1, -1)
        self.label_8 = QLabel(self.frame_20)
        self.label_8.setObjectName(u"label_8")

        self.horizontalLayout_4.addWidget(self.label_8, 0, Qt.AlignRight)

        self.boat_position_spinbox = QDoubleSpinBox(self.frame_20)
        self.boat_position_spinbox.setObjectName(u"boat_position_spinbox")
        self.boat_position_spinbox.setFont(font)
        self.boat_position_spinbox.setStyleSheet(u"")
        self.boat_position_spinbox.setLocale(QLocale(QLocale.English, QLocale.UnitedStates))
        self.boat_position_spinbox.setWrapping(True)
        self.boat_position_spinbox.setButtonSymbols(QAbstractSpinBox.UpDownArrows)
        self.boat_position_spinbox.setAccelerated(True)
        self.boat_position_spinbox.setDecimals(2)
        self.boat_position_spinbox.setMaximum(360.000000000000000)

        self.horizontalLayout_4.addWidget(self.boat_position_spinbox, 0, Qt.AlignLeft)


        self.verticalLayout_18.addWidget(self.frame_20, 0, Qt.AlignHCenter|Qt.AlignVCenter)


        self.horizontalLayout_5.addWidget(self.frame_14, 0, Qt.AlignRight)

        self.horizontalSpacer_2 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_5.addItem(self.horizontalSpacer_2)

        self.frame_22 = QFrame(self.frame_18)
        self.frame_22.setObjectName(u"frame_22")
        self.frame_22.setFrameShape(QFrame.StyledPanel)
        self.frame_22.setFrameShadow(QFrame.Raised)
        self.gridLayout_20 = QGridLayout(self.frame_22)
        self.gridLayout_20.setObjectName(u"gridLayout_20")
        self.gridLayout_20.setHorizontalSpacing(12)
        self.gridLayout_20.setContentsMargins(0, -1, 0, -1)
        self.frame_23 = QFrame(self.frame_22)
        self.frame_23.setObjectName(u"frame_23")
        self.frame_23.setMinimumSize(QSize(100, 0))
        self.frame_23.setFrameShape(QFrame.StyledPanel)
        self.frame_23.setFrameShadow(QFrame.Raised)
        self.verticalLayout_19 = QVBoxLayout(self.frame_23)
        self.verticalLayout_19.setObjectName(u"verticalLayout_19")
        self.verticalLayout_19.setContentsMargins(12, -1, 12, -1)
        self.label_51 = QLabel(self.frame_23)
        self.label_51.setObjectName(u"label_51")

        self.verticalLayout_19.addWidget(self.label_51)

        self.pitch_label = QLabel(self.frame_23)
        self.pitch_label.setObjectName(u"pitch_label")

        self.verticalLayout_19.addWidget(self.pitch_label)


        self.gridLayout_20.addWidget(self.frame_23, 0, 1, 1, 1, Qt.AlignLeft|Qt.AlignVCenter)

        self.pitch_canvas = QWidget(self.frame_22)
        self.pitch_canvas.setObjectName(u"pitch_canvas")
        self.pitch_canvas.setMinimumSize(QSize(200, 200))
        self.pitch_canvas.setMaximumSize(QSize(200, 200))
        self.pitch_canvas.setStyleSheet(u"border-radius: 100%;\n"
"")

        self.gridLayout_20.addWidget(self.pitch_canvas, 0, 0, 1, 1)

        self.frame_24 = QFrame(self.frame_22)
        self.frame_24.setObjectName(u"frame_24")
        self.frame_24.setMinimumSize(QSize(100, 0))
        self.frame_24.setFrameShape(QFrame.StyledPanel)
        self.frame_24.setFrameShadow(QFrame.Raised)
        self.verticalLayout_20 = QVBoxLayout(self.frame_24)
        self.verticalLayout_20.setObjectName(u"verticalLayout_20")
        self.label_53 = QLabel(self.frame_24)
        self.label_53.setObjectName(u"label_53")

        self.verticalLayout_20.addWidget(self.label_53)

        self.roll_label = QLabel(self.frame_24)
        self.roll_label.setObjectName(u"roll_label")

        self.verticalLayout_20.addWidget(self.roll_label)


        self.gridLayout_20.addWidget(self.frame_24, 1, 1, 1, 1, Qt.AlignLeft|Qt.AlignVCenter)

        self.roll_canvas = QWidget(self.frame_22)
        self.roll_canvas.setObjectName(u"roll_canvas")
        self.roll_canvas.setMinimumSize(QSize(200, 200))
        self.roll_canvas.setMaximumSize(QSize(200, 200))
        self.roll_canvas.setStyleSheet(u"border-radius: 100%;\n"
"")

        self.gridLayout_20.addWidget(self.roll_canvas, 1, 0, 1, 1)


        self.horizontalLayout_5.addWidget(self.frame_22, 0, Qt.AlignLeft)

        self.horizontalSpacer_5 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_5.addItem(self.horizontalSpacer_5)


        self.gridLayout_5.addWidget(self.frame_18, 1, 0, 1, 2)

        self.gridLayout_5.setRowStretch(0, 1)
        self.gridLayout_5.setRowStretch(1, 2)
        self.gridLayout_5.setColumnStretch(0, 2)
        self.gridLayout_5.setColumnStretch(1, 1)

        self.verticalLayout_14.addWidget(self.widget)

        self.main_content_widget.addWidget(self.data_widget)
        self.telemetry_widget_container = QWidget()
        self.telemetry_widget_container.setObjectName(u"telemetry_widget_container")
        self.telemetry_widget_container.setStyleSheet(u"QFrame > QFrame {\n"
"	background-color: rgb(23, 23, 23);\n"
"}")
        self.verticalLayout_15 = QVBoxLayout(self.telemetry_widget_container)
        self.verticalLayout_15.setSpacing(6)
        self.verticalLayout_15.setObjectName(u"verticalLayout_15")
        self.verticalLayout_15.setContentsMargins(12, 12, 12, 12)
        self.telemetry_scroll_area = QScrollArea(self.telemetry_widget_container)
        self.telemetry_scroll_area.setObjectName(u"telemetry_scroll_area")
        self.telemetry_scroll_area.setStyleSheet(u"")
        self.telemetry_scroll_area.setWidgetResizable(True)
        self.telemetry_scroll_area_container = QWidget()
        self.telemetry_scroll_area_container.setObjectName(u"telemetry_scroll_area_container")
        self.telemetry_scroll_area_container.setGeometry(QRect(0, 0, 841, 1424))
        self.verticalLayout_22 = QVBoxLayout(self.telemetry_scroll_area_container)
        self.verticalLayout_22.setSpacing(24)
        self.verticalLayout_22.setObjectName(u"verticalLayout_22")
        self.verticalLayout_22.setContentsMargins(0, 0, 12, 0)
        self.telemetry_dock_area = DockArea(self.telemetry_scroll_area_container)
        self.telemetry_dock_area.setObjectName(u"telemetry_dock_area")
        self.telemetry_dock_area.setMinimumSize(QSize(0, 700))
        self.telemetry_widget = PlotWidget(self.telemetry_dock_area)
        self.telemetry_widget.setObjectName(u"telemetry_widget")
        self.telemetry_widget.setGeometry(QRect(10, 20, 231, 231))
        self.telemetry_widget.setStyleSheet(u"background-color: rgb(29, 66, 81);")
        self.telemetry_widget_3 = PlotWidget(self.telemetry_dock_area)
        self.telemetry_widget_3.setObjectName(u"telemetry_widget_3")
        self.telemetry_widget_3.setGeometry(QRect(280, 20, 231, 231))
        self.telemetry_widget_3.setStyleSheet(u"background-color: rgb(29, 66, 81);")
        self.telemetry_widget_2 = PlotWidget(self.telemetry_dock_area)
        self.telemetry_widget_2.setObjectName(u"telemetry_widget_2")
        self.telemetry_widget_2.setGeometry(QRect(530, 20, 211, 231))
        self.telemetry_widget_2.setStyleSheet(u"background-color: rgb(29, 66, 81);")

        self.verticalLayout_22.addWidget(self.telemetry_dock_area)

        self.telemetry_dock_area_2 = DockArea(self.telemetry_scroll_area_container)
        self.telemetry_dock_area_2.setObjectName(u"telemetry_dock_area_2")
        self.telemetry_dock_area_2.setMinimumSize(QSize(0, 700))
        self.telemetry_widget_5 = PlotWidget(self.telemetry_dock_area_2)
        self.telemetry_widget_5.setObjectName(u"telemetry_widget_5")
        self.telemetry_widget_5.setGeometry(QRect(510, 70, 211, 231))
        self.telemetry_widget_5.setStyleSheet(u"background-color: rgb(29, 66, 81);")
        self.telemetry_widget_4 = PlotWidget(self.telemetry_dock_area_2)
        self.telemetry_widget_4.setObjectName(u"telemetry_widget_4")
        self.telemetry_widget_4.setGeometry(QRect(100, 80, 211, 231))
        self.telemetry_widget_4.setStyleSheet(u"background-color: rgb(29, 66, 81);")

        self.verticalLayout_22.addWidget(self.telemetry_dock_area_2)

        self.verticalLayout_22.setStretch(0, 1)
        self.verticalLayout_22.setStretch(1, 1)
        self.telemetry_scroll_area.setWidget(self.telemetry_scroll_area_container)

        self.verticalLayout_15.addWidget(self.telemetry_scroll_area)

        self.main_content_widget.addWidget(self.telemetry_widget_container)
        self.settings_widget = QWidget()
        self.settings_widget.setObjectName(u"settings_widget")
        self.settings_widget.setStyleSheet(u"")
        self.gridLayout_8 = QGridLayout(self.settings_widget)
        self.gridLayout_8.setObjectName(u"gridLayout_8")
        self.gridLayout_8.setContentsMargins(0, 12, 0, -1)
        self.save_settings_btn = QPushButton(self.settings_widget)
        self.save_settings_btn.setObjectName(u"save_settings_btn")
        self.save_settings_btn.setCursor(QCursor(Qt.PointingHandCursor))
        self.save_settings_btn.setStyleSheet(u"QPushButton{\n"
"	padding: 10px 40px;\n"
"	margin-left: 12px\n"
"}")

        self.gridLayout_8.addWidget(self.save_settings_btn, 15, 0, 1, 1, Qt.AlignLeft)

        self.frame_27 = QFrame(self.settings_widget)
        self.frame_27.setObjectName(u"frame_27")
        self.frame_27.setStyleSheet(u"QFrame {margin-right: 12px; margin-left: 12px}")
        self.frame_27.setLocale(QLocale(QLocale.English, QLocale.UnitedStates))
        self.frame_27.setFrameShape(QFrame.StyledPanel)
        self.frame_27.setFrameShadow(QFrame.Raised)
        self.horizontalLayout_7 = QHBoxLayout(self.frame_27)
        self.horizontalLayout_7.setObjectName(u"horizontalLayout_7")
        self.horizontalLayout_7.setContentsMargins(12, -1, -1, -1)
        self.label_45 = QLabel(self.frame_27)
        self.label_45.setObjectName(u"label_45")

        self.horizontalLayout_7.addWidget(self.label_45)

        self.settings_z_offset = QDoubleSpinBox(self.frame_27)
        self.settings_z_offset.setObjectName(u"settings_z_offset")
        self.settings_z_offset.setLocale(QLocale(QLocale.English, QLocale.UnitedStates))
        self.settings_z_offset.setDecimals(3)
        self.settings_z_offset.setMinimum(-10.000000000000000)
        self.settings_z_offset.setMaximum(10.000000000000000)
        self.settings_z_offset.setSingleStep(0.010000000000000)

        self.horizontalLayout_7.addWidget(self.settings_z_offset)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_7.addItem(self.horizontalSpacer)

        self.label_58 = QLabel(self.frame_27)
        self.label_58.setObjectName(u"label_58")

        self.horizontalLayout_7.addWidget(self.label_58)

        self.settings_danger_threshold = QDoubleSpinBox(self.frame_27)
        self.settings_danger_threshold.setObjectName(u"settings_danger_threshold")

        self.horizontalLayout_7.addWidget(self.settings_danger_threshold)

        self.label_59 = QLabel(self.frame_27)
        self.label_59.setObjectName(u"label_59")

        self.horizontalLayout_7.addWidget(self.label_59)

        self.settings_warning_threshold = QDoubleSpinBox(self.frame_27)
        self.settings_warning_threshold.setObjectName(u"settings_warning_threshold")
        self.settings_warning_threshold.setLocale(QLocale(QLocale.English, QLocale.UnitedStates))
        self.settings_warning_threshold.setMaximum(1000.000000000000000)

        self.horizontalLayout_7.addWidget(self.settings_warning_threshold)


        self.gridLayout_8.addWidget(self.frame_27, 12, 0, 1, 2, Qt.AlignVCenter)

        self.theme_switch = QPushButton(self.settings_widget)
        self.theme_switch.setObjectName(u"theme_switch")
        self.theme_switch.setCursor(QCursor(Qt.PointingHandCursor))
        self.theme_switch.setStyleSheet(u"margin-right: 12px;")

        self.gridLayout_8.addWidget(self.theme_switch, 15, 1, 1, 1, Qt.AlignRight)

        self.settings_scroll_area = QScrollArea(self.settings_widget)
        self.settings_scroll_area.setObjectName(u"settings_scroll_area")
        self.settings_scroll_area.setStyleSheet(u"")
        self.settings_scroll_area.setWidgetResizable(True)
        self.settings_scroll_area_widget = QWidget()
        self.settings_scroll_area_widget.setObjectName(u"settings_scroll_area_widget")
        self.settings_scroll_area_widget.setGeometry(QRect(0, 0, 326, 687))
        self.settings_scroll_area_widget.setStyleSheet(u"")
        self.gridLayout_22 = QGridLayout(self.settings_scroll_area_widget)
        self.gridLayout_22.setObjectName(u"gridLayout_22")
        self.label_65 = QLabel(self.settings_scroll_area_widget)
        self.label_65.setObjectName(u"label_65")

        self.gridLayout_22.addWidget(self.label_65, 7, 0, 1, 1, Qt.AlignVCenter)

        self.label_40 = QLabel(self.settings_scroll_area_widget)
        self.label_40.setObjectName(u"label_40")

        self.gridLayout_22.addWidget(self.label_40, 12, 0, 1, 1, Qt.AlignVCenter)

        self.label_39 = QLabel(self.settings_scroll_area_widget)
        self.label_39.setObjectName(u"label_39")

        self.gridLayout_22.addWidget(self.label_39, 6, 0, 1, 1, Qt.AlignVCenter)

        self.settings_logger_path = QLineEdit(self.settings_scroll_area_widget)
        self.settings_logger_path.setObjectName(u"settings_logger_path")
        self.settings_logger_path.setStyleSheet(u"margin-right: 24px;")
        self.settings_logger_path.setFrame(False)

        self.gridLayout_22.addWidget(self.settings_logger_path, 5, 1, 1, 1)

        self.label_66 = QLabel(self.settings_scroll_area_widget)
        self.label_66.setObjectName(u"label_66")

        self.gridLayout_22.addWidget(self.label_66, 9, 0, 1, 1, Qt.AlignVCenter)

        self.settings_py_version = QLineEdit(self.settings_scroll_area_widget)
        self.settings_py_version.setObjectName(u"settings_py_version")
        self.settings_py_version.setStyleSheet(u"margin-right: 24px;")
        self.settings_py_version.setFrame(False)

        self.gridLayout_22.addWidget(self.settings_py_version, 3, 1, 1, 1)

        self.line_3 = QFrame(self.settings_scroll_area_widget)
        self.line_3.setObjectName(u"line_3")
        self.line_3.setFrameShape(QFrame.HLine)
        self.line_3.setFrameShadow(QFrame.Sunken)

        self.gridLayout_22.addWidget(self.line_3, 13, 0, 1, 2)

        self.settings_svexporter_path = QLineEdit(self.settings_scroll_area_widget)
        self.settings_svexporter_path.setObjectName(u"settings_svexporter_path")
        self.settings_svexporter_path.setStyleSheet(u"margin-right: 24px;")

        self.gridLayout_22.addWidget(self.settings_svexporter_path, 9, 1, 1, 1)

        self.settings_export_format = QLineEdit(self.settings_scroll_area_widget)
        self.settings_export_format.setObjectName(u"settings_export_format")
        self.settings_export_format.setStyleSheet(u"margin-right: 24px;")
        self.settings_export_format.setFrame(False)

        self.gridLayout_22.addWidget(self.settings_export_format, 12, 1, 1, 1)

        self.settings_exporter_path = QLineEdit(self.settings_scroll_area_widget)
        self.settings_exporter_path.setObjectName(u"settings_exporter_path")
        self.settings_exporter_path.setStyleSheet(u"margin-right: 24px;")
        self.settings_exporter_path.setFrame(False)

        self.gridLayout_22.addWidget(self.settings_exporter_path, 6, 1, 1, 1)

        self.settings_ground_ip = QLineEdit(self.settings_scroll_area_widget)
        self.settings_ground_ip.setObjectName(u"settings_ground_ip")
        self.settings_ground_ip.setStyleSheet(u"margin-right: 24px;")
        self.settings_ground_ip.setFrame(False)

        self.gridLayout_22.addWidget(self.settings_ground_ip, 1, 1, 1, 1)

        self.settings_max_lines = QLineEdit(self.settings_scroll_area_widget)
        self.settings_max_lines.setObjectName(u"settings_max_lines")
        self.settings_max_lines.setStyleSheet(u"margin-right: 24px;")
        self.settings_max_lines.setFrame(False)

        self.gridLayout_22.addWidget(self.settings_max_lines, 11, 1, 1, 1)

        self.label_43 = QLabel(self.settings_scroll_area_widget)
        self.label_43.setObjectName(u"label_43")

        self.gridLayout_22.addWidget(self.label_43, 3, 0, 1, 1, Qt.AlignVCenter)

        self.line_2 = QFrame(self.settings_scroll_area_widget)
        self.line_2.setObjectName(u"line_2")
        self.line_2.setFrameShape(QFrame.HLine)
        self.line_2.setFrameShadow(QFrame.Sunken)

        self.gridLayout_22.addWidget(self.line_2, 4, 0, 1, 2)

        self.settings_sub_run_path = QLineEdit(self.settings_scroll_area_widget)
        self.settings_sub_run_path.setObjectName(u"settings_sub_run_path")
        self.settings_sub_run_path.setStyleSheet(u"margin-right: 24px;")
        self.settings_sub_run_path.setFrame(False)

        self.gridLayout_22.addWidget(self.settings_sub_run_path, 14, 1, 1, 1)

        self.settings_sub_stop_path = QLineEdit(self.settings_scroll_area_widget)
        self.settings_sub_stop_path.setObjectName(u"settings_sub_stop_path")
        self.settings_sub_stop_path.setStyleSheet(u"margin-right: 24px;")
        self.settings_sub_stop_path.setFrame(False)

        self.gridLayout_22.addWidget(self.settings_sub_stop_path, 15, 1, 1, 1)

        self.line = QFrame(self.settings_scroll_area_widget)
        self.line.setObjectName(u"line")
        self.line.setFrameShape(QFrame.HLine)
        self.line.setFrameShadow(QFrame.Sunken)

        self.gridLayout_22.addWidget(self.line, 2, 0, 1, 2)

        self.settings_broker_ip = QLineEdit(self.settings_scroll_area_widget)
        self.settings_broker_ip.setObjectName(u"settings_broker_ip")
        self.settings_broker_ip.setStyleSheet(u"margin-right: 24px;")
        self.settings_broker_ip.setFrame(False)

        self.gridLayout_22.addWidget(self.settings_broker_ip, 0, 1, 1, 1)

        self.label_46 = QLabel(self.settings_scroll_area_widget)
        self.label_46.setObjectName(u"label_46")

        self.gridLayout_22.addWidget(self.label_46, 5, 0, 1, 1, Qt.AlignVCenter)

        self.label_36 = QLabel(self.settings_scroll_area_widget)
        self.label_36.setObjectName(u"label_36")

        self.gridLayout_22.addWidget(self.label_36, 0, 0, 1, 1, Qt.AlignVCenter)

        self.settings_svlogger_path = QLineEdit(self.settings_scroll_area_widget)
        self.settings_svlogger_path.setObjectName(u"settings_svlogger_path")
        self.settings_svlogger_path.setStyleSheet(u"margin-right: 24px;")

        self.gridLayout_22.addWidget(self.settings_svlogger_path, 7, 1, 1, 1)

        self.label_41 = QLabel(self.settings_scroll_area_widget)
        self.label_41.setObjectName(u"label_41")

        self.gridLayout_22.addWidget(self.label_41, 14, 0, 1, 1, Qt.AlignVCenter)

        self.label_44 = QLabel(self.settings_scroll_area_widget)
        self.label_44.setObjectName(u"label_44")

        self.gridLayout_22.addWidget(self.label_44, 11, 0, 1, 1, Qt.AlignVCenter)

        self.label_37 = QLabel(self.settings_scroll_area_widget)
        self.label_37.setObjectName(u"label_37")

        self.gridLayout_22.addWidget(self.label_37, 1, 0, 1, 1, Qt.AlignVCenter)

        self.label_42 = QLabel(self.settings_scroll_area_widget)
        self.label_42.setObjectName(u"label_42")

        self.gridLayout_22.addWidget(self.label_42, 15, 0, 1, 1, Qt.AlignVCenter)

        self.line_12 = QFrame(self.settings_scroll_area_widget)
        self.line_12.setObjectName(u"line_12")
        self.line_12.setFrameShape(QFrame.HLine)
        self.line_12.setFrameShadow(QFrame.Sunken)

        self.gridLayout_22.addWidget(self.line_12, 10, 0, 1, 2)

        self.settings_scroll_area.setWidget(self.settings_scroll_area_widget)

        self.gridLayout_8.addWidget(self.settings_scroll_area, 0, 0, 12, 2)

        self.main_content_widget.addWidget(self.settings_widget)

        self.gridLayout_11.addWidget(self.main_content_widget, 1, 0, 1, 1)

        self.controls_container = QFrame(self.rightMainContainer)
        self.controls_container.setObjectName(u"controls_container")
        self.controls_container.setMinimumSize(QSize(0, 0))
        self.controls_container.setStyleSheet(u"")
        self.controls_container.setFrameShape(QFrame.StyledPanel)
        self.controls_container.setFrameShadow(QFrame.Raised)
        self.verticalLayout_17 = QVBoxLayout(self.controls_container)
        self.verticalLayout_17.setSpacing(6)
        self.verticalLayout_17.setObjectName(u"verticalLayout_17")
        self.verticalLayout_17.setContentsMargins(6, 6, 6, 6)
        self.status_bar = QWidget(self.controls_container)
        self.status_bar.setObjectName(u"status_bar")
        self.status_bar.setStyleSheet(u"")
        self.gridLayout_12 = QGridLayout(self.status_bar)
        self.gridLayout_12.setObjectName(u"gridLayout_12")
        self.gridLayout_12.setVerticalSpacing(0)
        self.gridLayout_12.setContentsMargins(-1, 6, -1, 6)
        self.status_bar_logger = QPushButton(self.status_bar)
        self.status_bar_logger.setObjectName(u"status_bar_logger")
        self.status_bar_logger.setMinimumSize(QSize(30, 30))
        self.status_bar_logger.setMaximumSize(QSize(30, 30))
        self.status_bar_logger.setFont(font)
        self.status_bar_logger.setStyleSheet(u"text-align: center;\n"
"border-radius: 15px;")
        self.status_bar_logger.setIcon(icon1)

        self.gridLayout_12.addWidget(self.status_bar_logger, 1, 0, 1, 1, Qt.AlignHCenter)

        self.label_61 = QLabel(self.status_bar)
        self.label_61.setObjectName(u"label_61")

        self.gridLayout_12.addWidget(self.label_61, 0, 1, 1, 1, Qt.AlignHCenter)

        self.line_13 = QFrame(self.status_bar)
        self.line_13.setObjectName(u"line_13")
        self.line_13.setMinimumSize(QSize(0, 4))
        self.line_13.setStyleSheet(u"background-color: none;")
        self.line_13.setFrameShape(QFrame.HLine)
        self.line_13.setFrameShadow(QFrame.Sunken)

        self.gridLayout_12.addWidget(self.line_13, 2, 0, 1, 3)

        self.label_63 = QLabel(self.status_bar)
        self.label_63.setObjectName(u"label_63")
        self.label_63.setAlignment(Qt.AlignCenter)

        self.gridLayout_12.addWidget(self.label_63, 3, 0, 1, 1, Qt.AlignHCenter)

        self.status_bar_svlogger = QPushButton(self.status_bar)
        self.status_bar_svlogger.setObjectName(u"status_bar_svlogger")
        self.status_bar_svlogger.setMinimumSize(QSize(30, 30))
        self.status_bar_svlogger.setMaximumSize(QSize(30, 30))
        self.status_bar_svlogger.setStyleSheet(u"text-align: center;\n"
"border-radius: 15px;")
        icon10 = QIcon()
        icon10.addFile(u":/icons/icons/chevrons-down.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.status_bar_svlogger.setIcon(icon10)

        self.gridLayout_12.addWidget(self.status_bar_svlogger, 1, 1, 1, 1, Qt.AlignHCenter)

        self.label_60 = QLabel(self.status_bar)
        self.label_60.setObjectName(u"label_60")
        self.label_60.setAlignment(Qt.AlignCenter)

        self.gridLayout_12.addWidget(self.label_60, 0, 0, 1, 1, Qt.AlignHCenter)

        self.status_bar_subscriber = QPushButton(self.status_bar)
        self.status_bar_subscriber.setObjectName(u"status_bar_subscriber")
        sizePolicy3 = QSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
        sizePolicy3.setHorizontalStretch(0)
        sizePolicy3.setVerticalStretch(0)
        sizePolicy3.setHeightForWidth(self.status_bar_subscriber.sizePolicy().hasHeightForWidth())
        self.status_bar_subscriber.setSizePolicy(sizePolicy3)
        self.status_bar_subscriber.setMinimumSize(QSize(30, 30))
        self.status_bar_subscriber.setMaximumSize(QSize(30, 30))
        self.status_bar_subscriber.setFont(font)
        self.status_bar_subscriber.setStyleSheet(u"text-align: center;\n"
"border-radius: 15px;")
        self.status_bar_subscriber.setIcon(icon2)
        self.status_bar_subscriber.setIconSize(QSize(24, 24))

        self.gridLayout_12.addWidget(self.status_bar_subscriber, 4, 0, 1, 1, Qt.AlignHCenter)

        self.status_bar_mqtt = QPushButton(self.status_bar)
        self.status_bar_mqtt.setObjectName(u"status_bar_mqtt")
        self.status_bar_mqtt.setMinimumSize(QSize(30, 30))
        self.status_bar_mqtt.setMaximumSize(QSize(30, 30))
        self.status_bar_mqtt.setFont(font)
        self.status_bar_mqtt.setStyleSheet(u"text-align: center;\n"
"border-radius: 15px;")
        icon11 = QIcon()
        icon11.addFile(u":/icons/icons/eye.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.status_bar_mqtt.setIcon(icon11)

        self.gridLayout_12.addWidget(self.status_bar_mqtt, 1, 2, 1, 1)

        self.label_64 = QLabel(self.status_bar)
        self.label_64.setObjectName(u"label_64")

        self.gridLayout_12.addWidget(self.label_64, 0, 2, 1, 1)

        self.status_bar_ssh = QPushButton(self.status_bar)
        self.status_bar_ssh.setObjectName(u"status_bar_ssh")
        self.status_bar_ssh.setMinimumSize(QSize(30, 30))
        self.status_bar_ssh.setMaximumSize(QSize(30, 30))
        self.status_bar_ssh.setFont(font)
        self.status_bar_ssh.setStyleSheet(u"text-align: center;\n"
"border-radius: 15px;")
        icon12 = QIcon()
        icon12.addFile(u":/icons/icons/terminal.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.status_bar_ssh.setIcon(icon12)

        self.gridLayout_12.addWidget(self.status_bar_ssh, 4, 1, 1, 1, Qt.AlignHCenter)

        self.label_62 = QLabel(self.status_bar)
        self.label_62.setObjectName(u"label_62")
        self.label_62.setAlignment(Qt.AlignCenter)

        self.gridLayout_12.addWidget(self.label_62, 3, 1, 1, 1)

        self.gridLayout_12.setColumnStretch(1, 1)

        self.verticalLayout_17.addWidget(self.status_bar)

        self.groupBox = QGroupBox(self.controls_container)
        self.groupBox.setObjectName(u"groupBox")
        font3 = QFont()
        font3.setPointSize(14)
        self.groupBox.setFont(font3)
        self.groupBox.setAlignment(Qt.AlignCenter)
        self.groupBox.setFlat(False)
        self.verticalLayout_3 = QVBoxLayout(self.groupBox)
        self.verticalLayout_3.setSpacing(15)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.verticalLayout_3.setContentsMargins(0, 30, 0, 6)
        self.light_btn_container = QWidget(self.groupBox)
        self.light_btn_container.setObjectName(u"light_btn_container")
        self.light_btn_container.setStyleSheet(u"")
        self.gridLayout_10 = QGridLayout(self.light_btn_container)
        self.gridLayout_10.setSpacing(0)
        self.gridLayout_10.setObjectName(u"gridLayout_10")
        self.gridLayout_10.setContentsMargins(6, 0, 6, 0)
        self.light_up = QPushButton(self.light_btn_container)
        self.light_up.setObjectName(u"light_up")
        self.light_up.setMinimumSize(QSize(50, 50))
        self.light_up.setMaximumSize(QSize(50, 50))
        self.light_up.setCursor(QCursor(Qt.PointingHandCursor))
        self.light_up.setStyleSheet(u"")
        icon13 = QIcon()
        icon13.addFile(u":/icons/icons/sun.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.light_up.setIcon(icon13)
        self.light_up.setIconSize(QSize(24, 24))
        self.light_up.setCheckable(False)
        self.light_up.setChecked(False)
        self.light_up.setFlat(True)
        self.light_up.setProperty("status", False)

        self.gridLayout_10.addWidget(self.light_up, 2, 0, 1, 1, Qt.AlignHCenter)

        self.light_md = QPushButton(self.light_btn_container)
        self.light_md.setObjectName(u"light_md")
        self.light_md.setMinimumSize(QSize(50, 50))
        self.light_md.setMaximumSize(QSize(50, 50))
        self.light_md.setCursor(QCursor(Qt.PointingHandCursor))
        self.light_md.setIcon(icon13)
        self.light_md.setIconSize(QSize(24, 24))
        self.light_md.setCheckable(False)
        self.light_md.setFlat(True)
        self.light_md.setProperty("status", False)

        self.gridLayout_10.addWidget(self.light_md, 2, 1, 1, 1, Qt.AlignHCenter)

        self.label_2 = QLabel(self.light_btn_container)
        self.label_2.setObjectName(u"label_2")
        self.label_2.setFont(font3)

        self.gridLayout_10.addWidget(self.label_2, 0, 0, 1, 1, Qt.AlignHCenter)

        self.light_dw = QPushButton(self.light_btn_container)
        self.light_dw.setObjectName(u"light_dw")
        self.light_dw.setMinimumSize(QSize(50, 50))
        self.light_dw.setMaximumSize(QSize(50, 50))
        self.light_dw.setCursor(QCursor(Qt.PointingHandCursor))
        self.light_dw.setIcon(icon13)
        self.light_dw.setIconSize(QSize(24, 24))
        self.light_dw.setCheckable(False)
        self.light_dw.setFlat(True)
        self.light_dw.setProperty("status", False)

        self.gridLayout_10.addWidget(self.light_dw, 2, 2, 1, 1, Qt.AlignHCenter)

        self.label_3 = QLabel(self.light_btn_container)
        self.label_3.setObjectName(u"label_3")
        self.label_3.setFont(font3)

        self.gridLayout_10.addWidget(self.label_3, 0, 1, 1, 1, Qt.AlignHCenter)

        self.label_4 = QLabel(self.light_btn_container)
        self.label_4.setObjectName(u"label_4")
        self.label_4.setFont(font3)

        self.gridLayout_10.addWidget(self.label_4, 0, 2, 1, 1, Qt.AlignHCenter)


        self.verticalLayout_3.addWidget(self.light_btn_container)

        self.light_btn_container_2 = QWidget(self.groupBox)
        self.light_btn_container_2.setObjectName(u"light_btn_container_2")
        self.light_btn_container_2.setStyleSheet(u"")
        self.gridLayout_18 = QGridLayout(self.light_btn_container_2)
        self.gridLayout_18.setSpacing(0)
        self.gridLayout_18.setObjectName(u"gridLayout_18")
        self.gridLayout_18.setContentsMargins(6, 0, 6, 0)
        self.label_23 = QLabel(self.light_btn_container_2)
        self.label_23.setObjectName(u"label_23")
        self.label_23.setFont(font3)

        self.gridLayout_18.addWidget(self.label_23, 0, 0, 1, 1, Qt.AlignHCenter)

        self.multi_beam_control = QPushButton(self.light_btn_container_2)
        self.multi_beam_control.setObjectName(u"multi_beam_control")
        self.multi_beam_control.setMinimumSize(QSize(50, 50))
        self.multi_beam_control.setMaximumSize(QSize(50, 50))
        self.multi_beam_control.setCursor(QCursor(Qt.PointingHandCursor))
        icon14 = QIcon()
        icon14.addFile(u":/icons/icons/multibeam.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.multi_beam_control.setIcon(icon14)
        self.multi_beam_control.setIconSize(QSize(24, 24))
        self.multi_beam_control.setCheckable(False)
        self.multi_beam_control.setFlat(True)
        self.multi_beam_control.setProperty("status", False)

        self.gridLayout_18.addWidget(self.multi_beam_control, 1, 2, 1, 1, Qt.AlignHCenter)

        self.miniSVS_control = QPushButton(self.light_btn_container_2)
        self.miniSVS_control.setObjectName(u"miniSVS_control")
        self.miniSVS_control.setMinimumSize(QSize(50, 50))
        self.miniSVS_control.setMaximumSize(QSize(50, 50))
        self.miniSVS_control.setCursor(QCursor(Qt.PointingHandCursor))
        self.miniSVS_control.setStyleSheet(u"")
        icon15 = QIcon()
        icon15.addFile(u":/icons/icons/pressure.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.miniSVS_control.setIcon(icon15)
        self.miniSVS_control.setIconSize(QSize(24, 24))
        self.miniSVS_control.setCheckable(False)
        self.miniSVS_control.setChecked(False)
        self.miniSVS_control.setFlat(True)
        self.miniSVS_control.setProperty("status", False)

        self.gridLayout_18.addWidget(self.miniSVS_control, 1, 0, 1, 1, Qt.AlignHCenter)

        self.label_20 = QLabel(self.light_btn_container_2)
        self.label_20.setObjectName(u"label_20")
        self.label_20.setFont(font3)

        self.gridLayout_18.addWidget(self.label_20, 0, 1, 1, 1, Qt.AlignHCenter)

        self.label_24 = QLabel(self.light_btn_container_2)
        self.label_24.setObjectName(u"label_24")
        self.label_24.setFont(font3)

        self.gridLayout_18.addWidget(self.label_24, 0, 2, 1, 1, Qt.AlignHCenter)

        self.miniCT_control = QPushButton(self.light_btn_container_2)
        self.miniCT_control.setObjectName(u"miniCT_control")
        self.miniCT_control.setMinimumSize(QSize(50, 50))
        self.miniCT_control.setMaximumSize(QSize(50, 50))
        self.miniCT_control.setCursor(QCursor(Qt.PointingHandCursor))
        icon16 = QIcon()
        icon16.addFile(u":/icons/icons/thermometer.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.miniCT_control.setIcon(icon16)
        self.miniCT_control.setIconSize(QSize(24, 24))
        self.miniCT_control.setCheckable(False)
        self.miniCT_control.setFlat(True)
        self.miniCT_control.setProperty("status", False)

        self.gridLayout_18.addWidget(self.miniCT_control, 1, 1, 1, 1, Qt.AlignHCenter)


        self.verticalLayout_3.addWidget(self.light_btn_container_2)

        self.widget_2 = QWidget(self.groupBox)
        self.widget_2.setObjectName(u"widget_2")
        self.widget_2.setStyleSheet(u"")
        self.gridLayout_13 = QGridLayout(self.widget_2)
        self.gridLayout_13.setSpacing(0)
        self.gridLayout_13.setObjectName(u"gridLayout_13")
        self.gridLayout_13.setContentsMargins(6, 0, 6, 0)
        self.bottom_cam_timer_label = QLabel(self.widget_2)
        self.bottom_cam_timer_label.setObjectName(u"bottom_cam_timer_label")
        self.bottom_cam_timer_label.setStyleSheet(u"")
        self.bottom_cam_timer_label.setAlignment(Qt.AlignCenter)

        self.gridLayout_13.addWidget(self.bottom_cam_timer_label, 0, 0, 1, 1, Qt.AlignHCenter)

        self.label_74 = QLabel(self.widget_2)
        self.label_74.setObjectName(u"label_74")

        self.gridLayout_13.addWidget(self.label_74, 0, 1, 1, 1, Qt.AlignHCenter)

        self.bottom_cam = QPushButton(self.widget_2)
        self.bottom_cam.setObjectName(u"bottom_cam")
        self.bottom_cam.setMaximumSize(QSize(50, 50))
        self.bottom_cam.setCursor(QCursor(Qt.PointingHandCursor))
        self.bottom_cam.setStyleSheet(u"")
        self.bottom_cam.setIcon(icon8)
        self.bottom_cam.setIconSize(QSize(24, 24))

        self.gridLayout_13.addWidget(self.bottom_cam, 1, 0, 1, 1, Qt.AlignHCenter)

        self.pilot_cam = QPushButton(self.widget_2)
        self.pilot_cam.setObjectName(u"pilot_cam")
        self.pilot_cam.setMaximumSize(QSize(50, 50))
        self.pilot_cam.setIcon(icon8)
        self.pilot_cam.setIconSize(QSize(24, 24))
        self.pilot_cam.setProperty("status", False)

        self.gridLayout_13.addWidget(self.pilot_cam, 1, 1, 1, 1, Qt.AlignHCenter)


        self.label_record = QLabel(self.widget_2)
        self.label_record.setObjectName(u"label_record")

        self.gridLayout_13.addWidget(self.label_record, 0, 2, 1, 1, Qt.AlignHCenter)
        
        self.pilot_cam_record = QPushButton(self.widget_2)
        self.pilot_cam_record.setObjectName(u"pilot_cam_record")
        self.pilot_cam_record.setMaximumSize(QSize(50, 50))
        self.pilot_cam_record.setCursor(QCursor(Qt.PointingHandCursor))
        self.pilot_cam_record.setIcon(icon8)
        self.pilot_cam_record.setIconSize(QSize(24, 24))
        self.pilot_cam_record.setProperty("status", False)

        self.gridLayout_13.addWidget(self.pilot_cam_record, 1, 2, 1, 1, Qt.AlignHCenter)


        self.verticalLayout_3.addWidget(self.widget_2)


        self.verticalLayout_17.addWidget(self.groupBox, 0, Qt.AlignTop)

        self.groupBox_2 = QGroupBox(self.controls_container)
        self.groupBox_2.setObjectName(u"groupBox_2")
        self.groupBox_2.setFont(font3)
        self.groupBox_2.setAlignment(Qt.AlignCenter)
        self.groupBox_2.setFlat(False)
        self.verticalLayout_16 = QVBoxLayout(self.groupBox_2)
        self.verticalLayout_16.setObjectName(u"verticalLayout_16")
        self.verticalLayout_16.setContentsMargins(0, 30, 0, 6)
        self.light_btn_container_4 = QWidget(self.groupBox_2)
        self.light_btn_container_4.setObjectName(u"light_btn_container_4")
        self.light_btn_container_4.setStyleSheet(u"")
        self.gridLayout_21 = QGridLayout(self.light_btn_container_4)
        self.gridLayout_21.setSpacing(0)
        self.gridLayout_21.setObjectName(u"gridLayout_21")
        self.gridLayout_21.setContentsMargins(6, 0, 6, 0)
        self.label_50 = QLabel(self.light_btn_container_4)
        self.label_50.setObjectName(u"label_50")
        self.label_50.setFont(font3)

        self.gridLayout_21.addWidget(self.label_50, 0, 0, 1, 1, Qt.AlignHCenter)

        self.label_52 = QLabel(self.light_btn_container_4)
        self.label_52.setObjectName(u"label_52")
        self.label_52.setFont(font3)

        self.gridLayout_21.addWidget(self.label_52, 0, 1, 1, 1, Qt.AlignHCenter)

        self.label_29 = QLabel(self.light_btn_container_4)
        self.label_29.setObjectName(u"label_29")
        self.label_29.setFont(font3)

        self.gridLayout_21.addWidget(self.label_29, 0, 2, 1, 1, Qt.AlignHCenter)

        self.dvl_control = QPushButton(self.light_btn_container_4)
        self.dvl_control.setObjectName(u"dvl_control")
        self.dvl_control.setMinimumSize(QSize(50, 50))
        self.dvl_control.setMaximumSize(QSize(50, 50))
        self.dvl_control.setCursor(QCursor(Qt.PointingHandCursor))
        self.dvl_control.setStyleSheet(u"")
        self.dvl_control.setIcon(icon13)
        self.dvl_control.setIconSize(QSize(24, 24))
        self.dvl_control.setCheckable(False)
        self.dvl_control.setChecked(False)
        self.dvl_control.setFlat(True)
        self.dvl_control.setProperty("status", False)

        self.gridLayout_21.addWidget(self.dvl_control, 1, 0, 1, 1)

        self.alt_control = QPushButton(self.light_btn_container_4)
        self.alt_control.setObjectName(u"alt_control")
        self.alt_control.setMinimumSize(QSize(50, 50))
        self.alt_control.setMaximumSize(QSize(50, 50))
        self.alt_control.setCursor(QCursor(Qt.PointingHandCursor))
        self.alt_control.setStyleSheet(u"")
        self.alt_control.setIcon(icon13)
        self.alt_control.setIconSize(QSize(24, 24))
        self.alt_control.setCheckable(False)
        self.alt_control.setChecked(False)
        self.alt_control.setFlat(True)
        self.alt_control.setProperty("status", False)

        self.gridLayout_21.addWidget(self.alt_control, 1, 1, 1, 1)

        self.usbl_control = QPushButton(self.light_btn_container_4)
        self.usbl_control.setObjectName(u"usbl_control")
        self.usbl_control.setMinimumSize(QSize(50, 50))
        self.usbl_control.setMaximumSize(QSize(50, 50))
        self.usbl_control.setCursor(QCursor(Qt.PointingHandCursor))
        self.usbl_control.setIcon(icon13)
        self.usbl_control.setIconSize(QSize(24, 24))
        self.usbl_control.setCheckable(False)
        self.usbl_control.setFlat(True)
        self.usbl_control.setProperty("status", False)

        self.gridLayout_21.addWidget(self.usbl_control, 1, 2, 1, 1)


        self.verticalLayout_16.addWidget(self.light_btn_container_4)

        self.light_btn_container_3 = QWidget(self.groupBox_2)
        self.light_btn_container_3.setObjectName(u"light_btn_container_3")
        self.light_btn_container_3.setStyleSheet(u"")
        self.gridLayout_19 = QGridLayout(self.light_btn_container_3)
        self.gridLayout_19.setSpacing(0)
        self.gridLayout_19.setObjectName(u"gridLayout_19")
        self.gridLayout_19.setContentsMargins(6, 0, 6, 0)
        self.label_5 = QLabel(self.light_btn_container_3)
        self.label_5.setObjectName(u"label_5")
        self.label_5.setFont(font3)

        self.gridLayout_19.addWidget(self.label_5, 0, 1, 1, 1, Qt.AlignHCenter)

        self.label_7 = QLabel(self.light_btn_container_3)
        self.label_7.setObjectName(u"label_7")
        self.label_7.setFont(font3)

        self.gridLayout_19.addWidget(self.label_7, 0, 0, 1, 1, Qt.AlignHCenter)

        self.ahrs_status_led = QPushButton(self.light_btn_container_3)
        self.ahrs_status_led.setObjectName(u"ahrs_status_led")
        self.ahrs_status_led.setMinimumSize(QSize(40, 40))
        self.ahrs_status_led.setMaximumSize(QSize(40, 40))
        self.ahrs_status_led.setCursor(QCursor(Qt.ArrowCursor))
        self.ahrs_status_led.setStyleSheet(u"text-align: center;\n"
"border-radius: 20px;")
        self.ahrs_status_led.setIcon(icon13)
        self.ahrs_status_led.setIconSize(QSize(24, 24))
        self.ahrs_status_led.setCheckable(False)
        self.ahrs_status_led.setFlat(True)

        self.gridLayout_19.addWidget(self.ahrs_status_led, 1, 1, 1, 1, Qt.AlignHCenter)

        self.fog_status_led = QPushButton(self.light_btn_container_3)
        self.fog_status_led.setObjectName(u"fog_status_led")
        self.fog_status_led.setMinimumSize(QSize(40, 40))
        self.fog_status_led.setMaximumSize(QSize(40, 40))
        self.fog_status_led.setCursor(QCursor(Qt.ArrowCursor))
        self.fog_status_led.setStyleSheet(u"text-align: center;\n"
"border-radius: 20px;")
        self.fog_status_led.setIcon(icon13)
        self.fog_status_led.setIconSize(QSize(24, 24))
        self.fog_status_led.setCheckable(False)
        self.fog_status_led.setChecked(False)
        self.fog_status_led.setFlat(True)

        self.gridLayout_19.addWidget(self.fog_status_led, 1, 0, 1, 1, Qt.AlignHCenter)


        self.verticalLayout_16.addWidget(self.light_btn_container_3)


        self.verticalLayout_17.addWidget(self.groupBox_2, 0, Qt.AlignTop)

        self.widget_3 = QWidget(self.controls_container)
        self.widget_3.setObjectName(u"widget_3")
        self.gridLayout_23 = QGridLayout(self.widget_3)
        self.gridLayout_23.setObjectName(u"gridLayout_23")
        self.gridLayout_23.setHorizontalSpacing(6)
        self.gridLayout_23.setVerticalSpacing(3)
        self.gridLayout_23.setContentsMargins(6, 6, 6, 6)
        self.label_69 = QLabel(self.widget_3)
        self.label_69.setObjectName(u"label_69")
        self.label_69.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.gridLayout_23.addWidget(self.label_69, 0, 0, 1, 1)

        self.thrust_yaw = QWidget(self.widget_3)
        self.thrust_yaw.setObjectName(u"thrust_yaw")

        self.gridLayout_23.addWidget(self.thrust_yaw, 3, 1, 1, 1)

        self.thrust_heave = QWidget(self.widget_3)
        self.thrust_heave.setObjectName(u"thrust_heave")

        self.gridLayout_23.addWidget(self.thrust_heave, 2, 1, 1, 1)

        self.thrust_sway = QWidget(self.widget_3)
        self.thrust_sway.setObjectName(u"thrust_sway")

        self.gridLayout_23.addWidget(self.thrust_sway, 1, 1, 1, 1)

        self.label_70 = QLabel(self.widget_3)
        self.label_70.setObjectName(u"label_70")
        self.label_70.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.gridLayout_23.addWidget(self.label_70, 2, 0, 1, 1)

        self.label_68 = QLabel(self.widget_3)
        self.label_68.setObjectName(u"label_68")
        self.label_68.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.gridLayout_23.addWidget(self.label_68, 1, 0, 1, 1)

        self.thrust_surge = QWidget(self.widget_3)
        self.thrust_surge.setObjectName(u"thrust_surge")

        self.gridLayout_23.addWidget(self.thrust_surge, 0, 1, 1, 1)

        self.label_73 = QLabel(self.widget_3)
        self.label_73.setObjectName(u"label_73")
        self.label_73.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.gridLayout_23.addWidget(self.label_73, 3, 0, 1, 1)

        self.gridLayout_23.setColumnStretch(1, 1)

        self.verticalLayout_17.addWidget(self.widget_3)

        self.verticalLayout_17.setStretch(3, 1)

        self.gridLayout_11.addWidget(self.controls_container, 1, 2, 1, 1)

        self.state_variable_bar = QFrame(self.rightMainContainer)
        self.state_variable_bar.setObjectName(u"state_variable_bar")
        self.state_variable_bar.setFrameShape(QFrame.StyledPanel)
        self.state_variable_bar.setFrameShadow(QFrame.Raised)
        self.gridLayout_16 = QGridLayout(self.state_variable_bar)
        self.gridLayout_16.setObjectName(u"gridLayout_16")
        self.gridLayout_16.setVerticalSpacing(2)
        self.gridLayout_16.setContentsMargins(-1, 6, -1, 6)
        self.label_35 = QLabel(self.state_variable_bar)
        self.label_35.setObjectName(u"label_35")
        self.label_35.setAlignment(Qt.AlignCenter)

        self.gridLayout_16.addWidget(self.label_35, 0, 6, 1, 1)

        self.line_9 = QFrame(self.state_variable_bar)
        self.line_9.setObjectName(u"line_9")
        self.line_9.setFrameShape(QFrame.VLine)
        self.line_9.setFrameShadow(QFrame.Sunken)

        self.gridLayout_16.addWidget(self.line_9, 0, 13, 2, 1)

        self.label_38 = QLabel(self.state_variable_bar)
        self.label_38.setObjectName(u"label_38")
        self.label_38.setAlignment(Qt.AlignCenter)

        self.gridLayout_16.addWidget(self.label_38, 0, 8, 1, 1, Qt.AlignTop)

        self.state_var_speed = QLabel(self.state_variable_bar)
        self.state_var_speed.setObjectName(u"state_var_speed")
        font4 = QFont()
        font4.setPointSize(16)
        self.state_var_speed.setFont(font4)
        self.state_var_speed.setAlignment(Qt.AlignCenter)

        self.gridLayout_16.addWidget(self.state_var_speed, 1, 4, 1, 1)

        self.state_var_depth = QLabel(self.state_variable_bar)
        self.state_var_depth.setObjectName(u"state_var_depth")
        self.state_var_depth.setFont(font4)
        self.state_var_depth.setAlignment(Qt.AlignCenter)

        self.gridLayout_16.addWidget(self.state_var_depth, 1, 2, 1, 1)

        self.label_33 = QLabel(self.state_variable_bar)
        self.label_33.setObjectName(u"label_33")
        self.label_33.setAlignment(Qt.AlignCenter)

        self.gridLayout_16.addWidget(self.label_33, 0, 2, 1, 1)

        self.line_6 = QFrame(self.state_variable_bar)
        self.line_6.setObjectName(u"line_6")
        self.line_6.setFrameShape(QFrame.VLine)
        self.line_6.setFrameShadow(QFrame.Sunken)

        self.gridLayout_16.addWidget(self.line_6, 0, 5, 2, 1)

        self.line_5 = QFrame(self.state_variable_bar)
        self.line_5.setObjectName(u"line_5")
        self.line_5.setFrameShape(QFrame.VLine)
        self.line_5.setFrameShadow(QFrame.Sunken)

        self.gridLayout_16.addWidget(self.line_5, 0, 3, 2, 1)

        self.line_4 = QFrame(self.state_variable_bar)
        self.line_4.setObjectName(u"line_4")
        self.line_4.setFrameShape(QFrame.VLine)
        self.line_4.setFrameShadow(QFrame.Sunken)

        self.gridLayout_16.addWidget(self.line_4, 0, 1, 2, 1)

        self.line_15 = QFrame(self.state_variable_bar)
        self.line_15.setObjectName(u"line_15")
        self.line_15.setFrameShape(QFrame.VLine)
        self.line_15.setFrameShadow(QFrame.Sunken)

        self.gridLayout_16.addWidget(self.line_15, 0, 9, 2, 1)

        self.label_30 = QLabel(self.state_variable_bar)
        self.label_30.setObjectName(u"label_30")
        self.label_30.setAlignment(Qt.AlignCenter)

        self.gridLayout_16.addWidget(self.label_30, 0, 0, 1, 1, Qt.AlignTop)

        self.label_47 = QLabel(self.state_variable_bar)
        self.label_47.setObjectName(u"label_47")
        self.label_47.setAlignment(Qt.AlignCenter)

        self.gridLayout_16.addWidget(self.label_47, 0, 16, 1, 1, Qt.AlignTop)

        self.label_49 = QLabel(self.state_variable_bar)
        self.label_49.setObjectName(u"label_49")
        self.label_49.setAlignment(Qt.AlignCenter)

        self.gridLayout_16.addWidget(self.label_49, 0, 14, 1, 1, Qt.AlignTop)

        self.line_10 = QFrame(self.state_variable_bar)
        self.line_10.setObjectName(u"line_10")
        self.line_10.setFrameShape(QFrame.VLine)
        self.line_10.setFrameShadow(QFrame.Sunken)

        self.gridLayout_16.addWidget(self.line_10, 0, 15, 2, 1)

        self.label_48 = QLabel(self.state_variable_bar)
        self.label_48.setObjectName(u"label_48")
        self.label_48.setAlignment(Qt.AlignCenter)

        self.gridLayout_16.addWidget(self.label_48, 0, 12, 1, 1)

        self.frame_12 = QFrame(self.state_variable_bar)
        self.frame_12.setObjectName(u"frame_12")
        self.frame_12.setFrameShape(QFrame.StyledPanel)
        self.frame_12.setFrameShadow(QFrame.Raised)
        self.verticalLayout_10 = QVBoxLayout(self.frame_12)
        self.verticalLayout_10.setObjectName(u"verticalLayout_10")
        self.verticalLayout_10.setContentsMargins(-1, 6, -1, 6)
        self.state_var_lat = QLabel(self.frame_12)
        self.state_var_lat.setObjectName(u"state_var_lat")
        self.state_var_lat.setFont(font4)

        self.verticalLayout_10.addWidget(self.state_var_lat)

        self.state_var_lon = QLabel(self.frame_12)
        self.state_var_lon.setObjectName(u"state_var_lon")
        self.state_var_lon.setFont(font4)

        self.verticalLayout_10.addWidget(self.state_var_lon)


        self.gridLayout_16.addWidget(self.frame_12, 1, 0, 1, 1, Qt.AlignTop)

        self.state_var_heading = QLabel(self.state_variable_bar)
        self.state_var_heading.setObjectName(u"state_var_heading")
        self.state_var_heading.setFont(font4)
        self.state_var_heading.setAlignment(Qt.AlignCenter)

        self.gridLayout_16.addWidget(self.state_var_heading, 1, 6, 1, 1)

        self.line_8 = QFrame(self.state_variable_bar)
        self.line_8.setObjectName(u"line_8")
        self.line_8.setFrameShape(QFrame.VLine)
        self.line_8.setFrameShadow(QFrame.Sunken)

        self.gridLayout_16.addWidget(self.line_8, 0, 11, 2, 1)

        self.water_leak_led = QPushButton(self.state_variable_bar)
        self.water_leak_led.setObjectName(u"water_leak_led")
        self.water_leak_led.setMinimumSize(QSize(40, 40))
        self.water_leak_led.setMaximumSize(QSize(40, 40))
        self.water_leak_led.setStyleSheet(u"text-align: center;\n"
"border-radius: 20px;")
        icon17 = QIcon()
        icon17.addFile(u":/icons/icons/water-leak.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.water_leak_led.setIcon(icon17)
        self.water_leak_led.setIconSize(QSize(20, 20))

        self.gridLayout_16.addWidget(self.water_leak_led, 1, 14, 1, 1, Qt.AlignHCenter)

        self.frame_13 = QFrame(self.state_variable_bar)
        self.frame_13.setObjectName(u"frame_13")
        self.frame_13.setStyleSheet(u"b")
        self.frame_13.setFrameShape(QFrame.StyledPanel)
        self.frame_13.setFrameShadow(QFrame.Raised)
        self.gridLayout_17 = QGridLayout(self.frame_13)
        self.gridLayout_17.setObjectName(u"gridLayout_17")
        self.gridLayout_17.setContentsMargins(-1, 6, -1, 6)
        self.label_57 = QLabel(self.frame_13)
        self.label_57.setObjectName(u"label_57")
        self.label_57.setFont(font4)
        self.label_57.setAlignment(Qt.AlignCenter)

        self.gridLayout_17.addWidget(self.label_57, 0, 1, 1, 1)

        self.state_var_roll = QLabel(self.frame_13)
        self.state_var_roll.setObjectName(u"state_var_roll")
        self.state_var_roll.setFont(font4)
        self.state_var_roll.setAlignment(Qt.AlignCenter)

        self.gridLayout_17.addWidget(self.state_var_roll, 1, 1, 1, 1)

        self.state_var_pitch = QLabel(self.frame_13)
        self.state_var_pitch.setObjectName(u"state_var_pitch")
        self.state_var_pitch.setFont(font4)
        self.state_var_pitch.setAlignment(Qt.AlignCenter)

        self.gridLayout_17.addWidget(self.state_var_pitch, 1, 0, 1, 1)

        self.label_56 = QLabel(self.frame_13)
        self.label_56.setObjectName(u"label_56")
        self.label_56.setFont(font4)
        self.label_56.setAlignment(Qt.AlignCenter)

        self.gridLayout_17.addWidget(self.label_56, 0, 0, 1, 1)


        self.gridLayout_16.addWidget(self.frame_13, 1, 8, 1, 1, Qt.AlignTop)

        self.battery_progress_bar = QProgressBar(self.state_variable_bar)
        self.battery_progress_bar.setObjectName(u"battery_progress_bar")
        self.battery_progress_bar.setMinimumSize(QSize(95, 50))
        self.battery_progress_bar.setMaximumSize(QSize(95, 16777215))
        self.battery_progress_bar.setStyleSheet(u"")
        self.battery_progress_bar.setValue(24)
        self.battery_progress_bar.setAlignment(Qt.AlignHCenter|Qt.AlignTop)
        self.battery_progress_bar.setOrientation(Qt.Horizontal)
        self.battery_progress_bar.setInvertedAppearance(False)
        self.battery_progress_bar.setTextDirection(QProgressBar.TopToBottom)

        self.gridLayout_16.addWidget(self.battery_progress_bar, 1, 16, 1, 1, Qt.AlignHCenter|Qt.AlignVCenter)

        self.state_var_temperature = QLabel(self.state_variable_bar)
        self.state_var_temperature.setObjectName(u"state_var_temperature")
        self.state_var_temperature.setFont(font4)
        self.state_var_temperature.setAlignment(Qt.AlignCenter)

        self.gridLayout_16.addWidget(self.state_var_temperature, 1, 12, 1, 1)

        self.label_34 = QLabel(self.state_variable_bar)
        self.label_34.setObjectName(u"label_34")
        self.label_34.setAlignment(Qt.AlignCenter)

        self.gridLayout_16.addWidget(self.label_34, 0, 4, 1, 1)

        self.line_7 = QFrame(self.state_variable_bar)
        self.line_7.setObjectName(u"line_7")
        self.line_7.setFrameShape(QFrame.VLine)
        self.line_7.setFrameShadow(QFrame.Sunken)

        self.gridLayout_16.addWidget(self.line_7, 0, 7, 2, 1)

        self.label_55 = QLabel(self.state_variable_bar)
        self.label_55.setObjectName(u"label_55")

        self.gridLayout_16.addWidget(self.label_55, 0, 10, 1, 1)

        self.frame_17 = QFrame(self.state_variable_bar)
        self.frame_17.setObjectName(u"frame_17")
        self.frame_17.setFrameShape(QFrame.StyledPanel)
        self.frame_17.setFrameShadow(QFrame.Raised)
        self.gridLayout_9 = QGridLayout(self.frame_17)
        self.gridLayout_9.setObjectName(u"gridLayout_9")
        self.gridLayout_9.setContentsMargins(-1, 6, -1, 6)
        self.label_71 = QLabel(self.frame_17)
        self.label_71.setObjectName(u"label_71")
        self.label_71.setFont(font4)

        self.gridLayout_9.addWidget(self.label_71, 0, 0, 1, 1)

        self.label_72 = QLabel(self.frame_17)
        self.label_72.setObjectName(u"label_72")
        self.label_72.setFont(font4)

        self.gridLayout_9.addWidget(self.label_72, 0, 1, 1, 1)

        self.label_75 = QLabel(self.frame_17)
        self.label_75.setObjectName(u"label_75")
        self.label_75.setFont(font4)

        self.gridLayout_9.addWidget(self.label_75, 0, 2, 1, 1)

        self.state_var_ref_z = QLabel(self.frame_17)
        self.state_var_ref_z.setObjectName(u"state_var_ref_z")
        self.state_var_ref_z.setFont(font4)

        self.gridLayout_9.addWidget(self.state_var_ref_z, 1, 0, 1, 1)

        self.state_var_ref_alt = QLabel(self.frame_17)
        self.state_var_ref_alt.setObjectName(u"state_var_ref_alt")
        self.state_var_ref_alt.setFont(font4)

        self.gridLayout_9.addWidget(self.state_var_ref_alt, 1, 1, 1, 1)

        self.state_var_ref_psi = QLabel(self.frame_17)
        self.state_var_ref_psi.setObjectName(u"state_var_ref_psi")
        self.state_var_ref_psi.setFont(font4)

        self.gridLayout_9.addWidget(self.state_var_ref_psi, 1, 2, 1, 1)


        self.gridLayout_16.addWidget(self.frame_17, 1, 10, 1, 1)

        self.gridLayout_16.setColumnStretch(0, 1)
        self.gridLayout_16.setColumnStretch(8, 1)
        self.gridLayout_16.setColumnStretch(10, 1)

        self.gridLayout_11.addWidget(self.state_variable_bar, 0, 0, 1, 3)

        self.gridLayout_11.setColumnStretch(0, 3)
        self.gridLayout_11.setColumnStretch(2, 1)

        self.horizontalLayout_2.addWidget(self.rightMainContainer)

        self.horizontalLayout_2.setStretch(1, 1)
        MainWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MainWindow)

        self.main_content_widget.setCurrentIndex(4)
        self.light_up.setDefault(False)
        self.light_md.setDefault(False)
        self.light_dw.setDefault(False)
        self.multi_beam_control.setDefault(False)
        self.miniSVS_control.setDefault(False)
        self.miniCT_control.setDefault(False)
        self.dvl_control.setDefault(False)
        self.alt_control.setDefault(False)
        self.usbl_control.setDefault(False)
        self.ahrs_status_led.setDefault(False)
        self.fog_status_led.setDefault(False)


        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"MainWindow", None))
        self.menu_status_selector.setText(QCoreApplication.translate("MainWindow", u"Status", None))
        self.menu_status_selector.setProperty("role", QCoreApplication.translate("MainWindow", u"menu-btn", None))
        self.menu_logger_selector.setText(QCoreApplication.translate("MainWindow", u"Logger", None))
        self.menu_logger_selector.setProperty("role", QCoreApplication.translate("MainWindow", u"menu-btn", None))
        self.menu_sub_selector.setText(QCoreApplication.translate("MainWindow", u"Subscriber", None))
        self.menu_sub_selector.setProperty("role", QCoreApplication.translate("MainWindow", u"menu-btn", None))
        self.menu_data_selector.setText(QCoreApplication.translate("MainWindow", u"Data", None))
        self.menu_data_selector.setProperty("role", QCoreApplication.translate("MainWindow", u"menu-btn", None))
        self.menu_light_selector.setText(QCoreApplication.translate("MainWindow", u"Telemetry", None))
        self.menu_light_selector.setProperty("role", QCoreApplication.translate("MainWindow", u"menu-btn", None))
        self.tool_bar_sinaps.setText(QCoreApplication.translate("MainWindow", u"SiNAPS", None))
        self.tool_bar_fog_data.setText(QCoreApplication.translate("MainWindow", u"FOG Data", None))
        self.tool_bar_fog.setText(QCoreApplication.translate("MainWindow", u"FOG", None))
        self.tool_bar_webcam.setText(QCoreApplication.translate("MainWindow", u"Webcam", None))
        self.tool_bar_multibeam.setText(QCoreApplication.translate("MainWindow", u"Multibeam", None))
        self.tool_bar_qinsy.setText(QCoreApplication.translate("MainWindow", u"Qinsy", None))
        self.tool_bar_nikon.setText(QCoreApplication.translate("MainWindow", u"Nikon Viewer", None))
        self.menu_settings_selector.setText(QCoreApplication.translate("MainWindow", u"Settings", None))
        self.menu_settings_selector.setProperty("role", QCoreApplication.translate("MainWindow", u"menu-btn", None))
        self.main_content_widget.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.frame_4.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.status_page_ssh.setText(QCoreApplication.translate("MainWindow", u"<html>SSH Client status: <span style=\"color:red\">OFF</span></html>", None))
        self.status_page_ssh.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.frame_7.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.status_page_mqtt.setText(QCoreApplication.translate("MainWindow", u"<html>Navigation data client: <span style=\"color:red\">OFF</span></html>", None))
        self.status_page_mqtt.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.frame_16.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.status_page_fog_udp.setText(QCoreApplication.translate("MainWindow", u"<html>FOG UDP Listener (GPGGA): <span style=\"color:red\">OFF</span></html>", None))
        self.status_page_fog_udp.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.frame_15.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.status_page_logger.setText(QCoreApplication.translate("MainWindow", u"<html>Main logger status: <span style=\"color:red\">OFF</span></html>", None))
        self.status_page_logger.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.status_page_svlogger.setText(QCoreApplication.translate("MainWindow", u"SV logger status: <html>Logger status: <span style=\"color:red\">OFF</span></html>", None))
        self.status_page_svlogger.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.frame_5.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.status_page_sub.setText(QCoreApplication.translate("MainWindow", u"<html>Subscriber status: <span style=\"color:red\">OFF</span></html>", None))
        self.status_page_sub.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.frame_6.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.status_page_minisvs.setText(QCoreApplication.translate("MainWindow", u"MINISVS", None))
        self.status_page_minisvs.setProperty("sub_data_status", QCoreApplication.translate("MainWindow", u"disconnected", None))
        self.status_page_minisvs.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.status_page_us_imu.setText(QCoreApplication.translate("MainWindow", u"US_IMU", None))
        self.status_page_us_imu.setProperty("sub_data_status", QCoreApplication.translate("MainWindow", u"disconnected", None))
        self.status_page_us_imu.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.frame_10.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.topic_selected_counter_label.setText(QCoreApplication.translate("MainWindow", u"11 topics selected", None))
        self.topic_selected_counter_label.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_31.setText(QCoreApplication.translate("MainWindow", u"Main Logger", None))
        self.label_31.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.selected_log_folder_path_label.setText(QCoreApplication.translate("MainWindow", u"No folder selected ", None))
        self.selected_log_folder_path_label.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.stop_logger_btn.setText(QCoreApplication.translate("MainWindow", u"Stop", None))
        self.stop_logger_btn.setProperty("bg", QCoreApplication.translate("MainWindow", u"primary", None))
        self.select_topic_btn.setText(QCoreApplication.translate("MainWindow", u"Select topics", None))
        self.select_topic_btn.setProperty("bg", QCoreApplication.translate("MainWindow", u"primary", None))
        self.select_log_folder_btn.setText(QCoreApplication.translate("MainWindow", u"Select logs folder", None))
        self.select_log_folder_btn.setProperty("bg", QCoreApplication.translate("MainWindow", u"primary", None))
        self.export_btn.setText(QCoreApplication.translate("MainWindow", u"Export", None))
        self.export_btn.setProperty("bg", QCoreApplication.translate("MainWindow", u"info", None))
        self.start_logger_btn.setText(QCoreApplication.translate("MainWindow", u"Start", None))
        self.start_logger_btn.setProperty("bg", QCoreApplication.translate("MainWindow", u"primary", None))
        self.label.setText(QCoreApplication.translate("MainWindow", u"Max lines per file", None))
        self.label.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.log_fog_data_checkbox.setText(QCoreApplication.translate("MainWindow", u"Log fog data", None))
        self.frame_11.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.start_svlogger_btn.setText(QCoreApplication.translate("MainWindow", u"Start", None))
        self.start_svlogger_btn.setProperty("bg", QCoreApplication.translate("MainWindow", u"primary", None))
        self.stop_svlogger_btn.setText(QCoreApplication.translate("MainWindow", u"Stop", None))
        self.stop_svlogger_btn.setProperty("bg", QCoreApplication.translate("MainWindow", u"primary", None))
        self.select_profile_btn.setText(QCoreApplication.translate("MainWindow", u"Select profile", None))
        self.select_profile_btn.setProperty("bg", QCoreApplication.translate("MainWindow", u"primary", None))
        self.selected_profile_path_label.setText(QCoreApplication.translate("MainWindow", u"No file selected", None))
        self.selected_profile_path_label.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.plot_profile_btn.setText(QCoreApplication.translate("MainWindow", u"Plot data", None))
        self.plot_profile_btn.setProperty("bg", QCoreApplication.translate("MainWindow", u"info", None))
        self.selected_svlog_folder_path_label.setText(QCoreApplication.translate("MainWindow", u"No file selected", None))
        self.selected_svlog_folder_path_label.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_32.setText(QCoreApplication.translate("MainWindow", u"SV Profile Logger", None))
        self.label_32.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.select_svlog_folder_btn.setText(QCoreApplication.translate("MainWindow", u"Select folder", None))
        self.select_svlog_folder_btn.setProperty("bg", QCoreApplication.translate("MainWindow", u"primary", None))
        self.export_sv_profile_btn.setText(QCoreApplication.translate("MainWindow", u"Export", None))
        self.export_sv_profile_btn.setProperty("bg", QCoreApplication.translate("MainWindow", u"info", None))
        self.label_67.setText(QCoreApplication.translate("MainWindow", u"SV Profile Plotter", None))
        self.label_67.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.ssh_error_alert_frame.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.label_6.setText(QCoreApplication.translate("MainWindow", u"SSH Connection error", None))
        self.label_6.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.reconnect_ssh_btn.setText(QCoreApplication.translate("MainWindow", u"Reconnect", None))
        self.reconnect_ssh_btn.setProperty("bg", QCoreApplication.translate("MainWindow", u"primary", None))
        self.sub_button_frame.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.start_sub_btn.setText(QCoreApplication.translate("MainWindow", u"Start", None))
        self.start_sub_btn.setProperty("bg", QCoreApplication.translate("MainWindow", u"primary", None))
        self.stop_sub_btn.setText(QCoreApplication.translate("MainWindow", u"Stop", None))
        self.stop_sub_btn.setProperty("bg", QCoreApplication.translate("MainWindow", u"primary", None))
        self.groupBox_19.setTitle(QCoreApplication.translate("MainWindow", u"State Variables", None))
        self.groupBox_19.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.label_14.setText(QCoreApplication.translate("MainWindow", u"Altitude", None))
        self.label_14.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_27.setText(QCoreApplication.translate("MainWindow", u"z", None))
        self.label_27.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.data_x.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_x.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.label_9.setText(QCoreApplication.translate("MainWindow", u"Fu", None))
        self.label_9.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_12.setText(QCoreApplication.translate("MainWindow", u"Fw", None))
        self.label_12.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_26.setText(QCoreApplication.translate("MainWindow", u"y", None))
        self.label_26.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.data_tr.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_tr.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.data_fu.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_fu.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.data_fw.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_fw.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.label_13.setText(QCoreApplication.translate("MainWindow", u"Tr", None))
        self.label_13.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_28.setText(QCoreApplication.translate("MainWindow", u"psi", None))
        self.label_28.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.data_fv.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_fv.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.data_y.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_y.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.label_19.setText(QCoreApplication.translate("MainWindow", u"u", None))
        self.label_19.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.data_altitude.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_altitude.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.data_altitude.setProperty("status", QCoreApplication.translate("MainWindow", u"danger", None))
        self.label_11.setText(QCoreApplication.translate("MainWindow", u"Fv", None))
        self.label_11.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_25.setText(QCoreApplication.translate("MainWindow", u"x", None))
        self.label_25.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.data_v.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_v.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.data_u.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_u.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.data_z.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_z.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.data_psi.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_psi.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.label_21.setText(QCoreApplication.translate("MainWindow", u"v", None))
        self.label_21.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.data_r.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_r.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.label_22.setText(QCoreApplication.translate("MainWindow", u"r", None))
        self.label_22.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_54.setText(QCoreApplication.translate("MainWindow", u"w", None))
        self.label_54.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.data_w.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_w.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.groupBox_21.setTitle(QCoreApplication.translate("MainWindow", u"Science Variables", None))
        self.groupBox_21.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.data_temperature.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_temperature.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.label_15.setText(QCoreApplication.translate("MainWindow", u"Sound Speed", None))
        self.label_15.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.data_conductivity.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_conductivity.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.data_sound_speed.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_sound_speed.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.label_17.setText(QCoreApplication.translate("MainWindow", u"Temperature", None))
        self.label_17.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_10.setText(QCoreApplication.translate("MainWindow", u"Pressure", None))
        self.label_10.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_16.setText(QCoreApplication.translate("MainWindow", u"Conductivity", None))
        self.label_16.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.data_pressure.setText(QCoreApplication.translate("MainWindow", u"0.0", None))
        self.data_pressure.setProperty("role", QCoreApplication.translate("MainWindow", u"data", None))
        self.frame_18.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.frame_14.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.frame_26.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.compass_canvas.setProperty("role", QCoreApplication.translate("MainWindow", u"canvas", None))
        self.label_18.setText(QCoreApplication.translate("MainWindow", u"Heading", None))
        self.label_18.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.heading_label.setText(QCoreApplication.translate("MainWindow", u"0.00\u00b0", None))
        self.frame_20.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_8.setText(QCoreApplication.translate("MainWindow", u"Boat position", None))
        self.label_8.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.frame_22.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.label_51.setText(QCoreApplication.translate("MainWindow", u"Pitch", None))
        self.pitch_label.setText(QCoreApplication.translate("MainWindow", u"0.00\u00b0", None))
        self.pitch_canvas.setProperty("role", QCoreApplication.translate("MainWindow", u"canvas", None))
        self.label_53.setText(QCoreApplication.translate("MainWindow", u"Roll", None))
        self.roll_label.setText(QCoreApplication.translate("MainWindow", u"0.00\u00b0", None))
        self.roll_canvas.setProperty("role", QCoreApplication.translate("MainWindow", u"canvas", None))
        self.telemetry_dock_area.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.telemetry_dock_area_2.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.save_settings_btn.setText(QCoreApplication.translate("MainWindow", u"Save", None))
        self.save_settings_btn.setProperty("bg", QCoreApplication.translate("MainWindow", u"info", None))
        self.frame_27.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.label_45.setText(QCoreApplication.translate("MainWindow", u"z-offset", None))
        self.label_45.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_58.setText(QCoreApplication.translate("MainWindow", u"Danger threshold", None))
        self.label_58.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_59.setText(QCoreApplication.translate("MainWindow", u"Warning threshold", None))
        self.label_59.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.theme_switch.setText(QCoreApplication.translate("MainWindow", u"Toggle theme", None))
        self.theme_switch.setProperty("bg", QCoreApplication.translate("MainWindow", u"info", None))
        self.label_65.setText(QCoreApplication.translate("MainWindow", u"SV logger path", None))
        self.label_40.setText(QCoreApplication.translate("MainWindow", u"Default export format", None))
        self.label_39.setText(QCoreApplication.translate("MainWindow", u"Main exporter path", None))
        self.label_66.setText(QCoreApplication.translate("MainWindow", u"SV exporter pah", None))
        self.label_43.setText(QCoreApplication.translate("MainWindow", u"Python version", None))
        self.label_46.setText(QCoreApplication.translate("MainWindow", u"Main logger path", None))
        self.label_36.setText(QCoreApplication.translate("MainWindow", u"MQTT Broker ip", None))
        self.label_41.setText(QCoreApplication.translate("MainWindow", u"Subscriber run script path", None))
        self.label_44.setText(QCoreApplication.translate("MainWindow", u"Max lines per file", None))
        self.label_37.setText(QCoreApplication.translate("MainWindow", u"Ground machine ip", None))
        self.label_42.setText(QCoreApplication.translate("MainWindow", u"Subscriber stop script path", None))
        self.status_bar.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.status_bar_logger.setText("")
        self.status_bar_logger.setProperty("status", QCoreApplication.translate("MainWindow", u"off", None))
        self.label_61.setText(QCoreApplication.translate("MainWindow", u"SV LOGGER", None))
        self.label_61.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_63.setText(QCoreApplication.translate("MainWindow", u"SUBSCRIBER", None))
        self.label_63.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.status_bar_svlogger.setText("")
        self.status_bar_svlogger.setProperty("status", QCoreApplication.translate("MainWindow", u"off", None))
        self.label_60.setText(QCoreApplication.translate("MainWindow", u"LOGGER", None))
        self.label_60.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.status_bar_subscriber.setText("")
        self.status_bar_subscriber.setProperty("status", QCoreApplication.translate("MainWindow", u"off", None))
        self.status_bar_mqtt.setText("")
        self.status_bar_mqtt.setProperty("status", QCoreApplication.translate("MainWindow", u"off", None))
        self.label_64.setText(QCoreApplication.translate("MainWindow", u"MQTT", None))
        self.label_64.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.status_bar_ssh.setText("")
        self.status_bar_ssh.setProperty("status", QCoreApplication.translate("MainWindow", u"off", None))
        self.label_62.setText(QCoreApplication.translate("MainWindow", u"SSH", None))
        self.label_62.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.groupBox.setTitle(QCoreApplication.translate("MainWindow", u"Science", None))
        self.groupBox.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.light_btn_container.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.light_up.setText("")
        self.light_up.setProperty("control", QCoreApplication.translate("MainWindow", u"general", None))
        self.light_md.setText("")
        self.light_md.setProperty("control", QCoreApplication.translate("MainWindow", u"general", None))
        self.label_2.setText(QCoreApplication.translate("MainWindow", u"UP", None))
        self.label_2.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.light_dw.setText("")
        self.light_dw.setProperty("control", QCoreApplication.translate("MainWindow", u"general", None))
        self.label_3.setText(QCoreApplication.translate("MainWindow", u"MD", None))
        self.label_3.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_4.setText(QCoreApplication.translate("MainWindow", u"DW", None))
        self.label_4.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.light_btn_container_2.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.label_23.setText(QCoreApplication.translate("MainWindow", u"miniSVS", None))
        self.label_23.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.multi_beam_control.setText("")
        self.multi_beam_control.setProperty("control", QCoreApplication.translate("MainWindow", u"general", None))
        self.miniSVS_control.setText("")
        self.miniSVS_control.setProperty("control", QCoreApplication.translate("MainWindow", u"general", None))
        self.label_20.setText(QCoreApplication.translate("MainWindow", u"miniCT", None))
        self.label_20.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_24.setText(QCoreApplication.translate("MainWindow", u"MBES", None))
        self.label_24.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.miniCT_control.setText("")
        self.miniCT_control.setProperty("control", QCoreApplication.translate("MainWindow", u"general", None))
        self.widget_2.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.bottom_cam_timer_label.setText(QCoreApplication.translate("MainWindow", u"Bottom cam", None))
        self.bottom_cam_timer_label.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_74.setText(QCoreApplication.translate("MainWindow", u"Pilot cam", None))
        self.label_74.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.bottom_cam.setText("")
        self.bottom_cam.setProperty("control", QCoreApplication.translate("MainWindow", u"pilot-cam", None))
        self.bottom_cam.setProperty("status", QCoreApplication.translate("MainWindow", u"off", None))
        self.pilot_cam.setText("")
        self.pilot_cam.setProperty("control", QCoreApplication.translate("MainWindow", u"general", None))
        self.pilot_cam_record.setText("")
        self.pilot_cam_record.setProperty("status", QCoreApplication.translate("MainWindow", u"off", None))
        self.groupBox_2.setTitle(QCoreApplication.translate("MainWindow", u"NGC", None))
        self.groupBox_2.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.light_btn_container_4.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.label_50.setText(QCoreApplication.translate("MainWindow", u"DVL", None))
        self.label_50.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_52.setText(QCoreApplication.translate("MainWindow", u"ALT", None))
        self.label_52.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_29.setText(QCoreApplication.translate("MainWindow", u"USBL", None))
        self.label_29.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.dvl_control.setText("")
        self.dvl_control.setProperty("control", QCoreApplication.translate("MainWindow", u"general", None))
        self.alt_control.setText("")
        self.alt_control.setProperty("control", QCoreApplication.translate("MainWindow", u"general", None))
        self.usbl_control.setText("")
        self.usbl_control.setProperty("control", QCoreApplication.translate("MainWindow", u"general", None))
        self.light_btn_container_3.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.label_5.setText(QCoreApplication.translate("MainWindow", u"AHRS", None))
        self.label_5.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_7.setText(QCoreApplication.translate("MainWindow", u"FOG", None))
        self.label_7.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.ahrs_status_led.setText("")
        self.ahrs_status_led.setProperty("status", QCoreApplication.translate("MainWindow", u"warn", None))
        self.ahrs_status_led.setProperty("control", QCoreApplication.translate("MainWindow", u"infoled", None))
        self.fog_status_led.setText("")
        self.fog_status_led.setProperty("status", QCoreApplication.translate("MainWindow", u"warn", None))
        self.fog_status_led.setProperty("control", QCoreApplication.translate("MainWindow", u"infoled", None))
        self.widget_3.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.label_69.setText(QCoreApplication.translate("MainWindow", u"SURGE", None))
        self.label_69.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_70.setText(QCoreApplication.translate("MainWindow", u"HEAVE", None))
        self.label_70.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_68.setText(QCoreApplication.translate("MainWindow", u"SWAY", None))
        self.label_68.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_73.setText(QCoreApplication.translate("MainWindow", u"YAW", None))
        self.label_73.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.state_variable_bar.setProperty("bg", QCoreApplication.translate("MainWindow", u"dark", None))
        self.label_35.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">Heading</span><br/>degrees</p></body></html>", None))
        self.label_35.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_38.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">Attitude</span><br/>degrees</p></body></html>", None))
        self.label_38.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.state_var_speed.setText(QCoreApplication.translate("MainWindow", u"0.000", None))
        self.state_var_speed.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.state_var_depth.setText(QCoreApplication.translate("MainWindow", u"0.000", None))
        self.state_var_depth.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_33.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">Depth</span><br/>m</p></body></html>", None))
        self.label_33.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_30.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">Position</span><br/></p></body></html>", None))
        self.label_30.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_47.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">Battery</span></p></body></html>", None))
        self.label_47.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_49.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">Water leak</span></p></body></html>", None))
        self.label_49.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_48.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">Temperature</span><br/>\u00b0C</p></body></html>", None))
        self.label_48.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.state_var_lat.setText(QCoreApplication.translate("MainWindow", u"Latitude", None))
        self.state_var_lon.setText(QCoreApplication.translate("MainWindow", u"Longitude", None))
        self.state_var_heading.setText(QCoreApplication.translate("MainWindow", u"0.000", None))
        self.state_var_heading.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.water_leak_led.setText("")
        self.water_leak_led.setProperty("status", QCoreApplication.translate("MainWindow", u"warn", None))
        self.water_leak_led.setProperty("control", QCoreApplication.translate("MainWindow", u"infoled", None))
        self.label_57.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p><span style=\" font-weight:700;\">Roll</span></p></body></html>", None))
        self.state_var_roll.setText(QCoreApplication.translate("MainWindow", u"0.00", None))
        self.state_var_pitch.setText(QCoreApplication.translate("MainWindow", u"0.00", None))
        self.label_56.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p><span style=\" font-weight:700;\">Pitch</span></p></body></html>", None))
        self.battery_progress_bar.setProperty("charge", QCoreApplication.translate("MainWindow", u"low", None))
        self.state_var_temperature.setText(QCoreApplication.translate("MainWindow", u"0.000", None))
        self.state_var_temperature.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_34.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">Speed</span><br/>kn</p></body></html>", None))
        self.label_34.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_55.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; font-weight:700;\">NGC Reference</span><br/></p></body></html>", None))
        self.label_55.setProperty("bg", QCoreApplication.translate("MainWindow", u"none", None))
        self.label_71.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\"><span style=\" font-weight:700;\">Z</span></p></body></html>", None))
        self.label_72.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\"><span style=\" font-weight:700;\">Alt</span></p></body></html>", None))
        self.label_75.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\"><span style=\" font-weight:700;\">Psi</span></p></body></html>", None))
        self.state_var_ref_z.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\">0.00</p></body></html>", None))
        self.state_var_ref_alt.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\">0.00</p></body></html>", None))
        self.state_var_ref_psi.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\">0.00</p></body></html>", None))
    # retranslateUi


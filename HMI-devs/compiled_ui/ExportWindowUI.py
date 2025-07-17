# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'export_window.ui'
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
from PySide6.QtWidgets import (QApplication, QCheckBox, QFrame, QGridLayout,
    QGroupBox, QHBoxLayout, QLabel, QLineEdit,
    QPushButton, QRadioButton, QSizePolicy, QWidget)

class Ui_export_window_main(object):
    def setupUi(self, export_window_main):
        if not export_window_main.objectName():
            export_window_main.setObjectName(u"export_window_main")
        export_window_main.resize(671, 458)
        export_window_main.setMaximumSize(QSize(16777215, 458))
        export_window_main.setStyleSheet(u"* {\n"
"	background-color: rgb(9, 51, 64); \n"
"	border-radius: 10px;\n"
"	font-size: 16px;\n"
"}\n"
"\n"
"QLabel {\n"
"	font-size: 18px;\n"
"}\n"
"\n"
"QLineEdit {\n"
"	background-color: rgb(8, 25, 29);\n"
"	font-size: 20px;\n"
"	padding: 10px;\n"
"	qproperty-frame: false ;\n"
"}\n"
"\n"
"QLineEdit:focus{\n"
"	outline: none !important;\n"
"	border: 2px solid rgb(45, 75, 99)\n"
"}\n"
"\n"
"QPushButton {\n"
"	border: 1px solid transparent;\n"
"	font-size: 20px;\n"
"	padding: 10px 10px;\n"
"	text-align: center;\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"	color: rgba(255, 255, 255, 100);\n"
"}")
        self.gridLayout = QGridLayout(export_window_main)
        self.gridLayout.setObjectName(u"gridLayout")
        self.label_3 = QLabel(export_window_main)
        self.label_3.setObjectName(u"label_3")
        self.label_3.setAlignment(Qt.AlignCenter)
        self.label_3.setIndent(-1)

        self.gridLayout.addWidget(self.label_3, 3, 0, 1, 1)

        self.filters_input = QLineEdit(export_window_main)
        self.filters_input.setObjectName(u"filters_input")

        self.gridLayout.addWidget(self.filters_input, 3, 2, 1, 1)

        self.central_frame = QFrame(export_window_main)
        self.central_frame.setObjectName(u"central_frame")
        self.central_frame.setMaximumSize(QSize(16777215, 201))
        self.central_frame.setStyleSheet(u"")
        self.central_frame.setFrameShape(QFrame.StyledPanel)
        self.central_frame.setFrameShadow(QFrame.Raised)
        self.gridLayout_2 = QGridLayout(self.central_frame)
        self.gridLayout_2.setObjectName(u"gridLayout_2")
        self.source_btn = QPushButton(self.central_frame)
        self.source_btn.setObjectName(u"source_btn")
        self.source_btn.setCursor(QCursor(Qt.PointingHandCursor))
        self.source_btn.setStyleSheet(u"")

        self.gridLayout_2.addWidget(self.source_btn, 0, 0, 1, 1)

        self.source_label = QLabel(self.central_frame)
        self.source_label.setObjectName(u"source_label")
        font = QFont()
        self.source_label.setFont(font)
        self.source_label.setAcceptDrops(True)
        self.source_label.setStyleSheet(u"")

        self.gridLayout_2.addWidget(self.source_label, 0, 1, 1, 1)

        self.destination_btn = QPushButton(self.central_frame)
        self.destination_btn.setObjectName(u"destination_btn")
        self.destination_btn.setCursor(QCursor(Qt.PointingHandCursor))
        self.destination_btn.setStyleSheet(u"")

        self.gridLayout_2.addWidget(self.destination_btn, 1, 0, 1, 1)

        self.destination_label = QLabel(self.central_frame)
        self.destination_label.setObjectName(u"destination_label")
        self.destination_label.setFont(font)
        self.destination_label.setAcceptDrops(True)
        self.destination_label.setStyleSheet(u"")

        self.gridLayout_2.addWidget(self.destination_label, 1, 1, 1, 1)

        self.gridLayout_2.setColumnStretch(1, 1)

        self.gridLayout.addWidget(self.central_frame, 1, 0, 1, 3)

        self.start_export_btn = QPushButton(export_window_main)
        self.start_export_btn.setObjectName(u"start_export_btn")
        self.start_export_btn.setCursor(QCursor(Qt.PointingHandCursor))
        self.start_export_btn.setStyleSheet(u"")

        self.gridLayout.addWidget(self.start_export_btn, 4, 0, 1, 1)

        self.format_groupbox = QGroupBox(export_window_main)
        self.format_groupbox.setObjectName(u"format_groupbox")
        self.format_groupbox.setMaximumSize(QSize(16777215, 80))
        self.format_groupbox.setStyleSheet(u"")
        self.horizontalLayout = QHBoxLayout(self.format_groupbox)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.horizontalLayout.setContentsMargins(-1, 25, -1, -1)
        self.json_btn = QRadioButton(self.format_groupbox)
        self.json_btn.setObjectName(u"json_btn")

        self.horizontalLayout.addWidget(self.json_btn)

        self.csv_btn = QRadioButton(self.format_groupbox)
        self.csv_btn.setObjectName(u"csv_btn")

        self.horizontalLayout.addWidget(self.csv_btn)

        self.split_file_btn = QCheckBox(self.format_groupbox)
        self.split_file_btn.setObjectName(u"split_file_btn")

        self.horizontalLayout.addWidget(self.split_file_btn)


        self.gridLayout.addWidget(self.format_groupbox, 0, 0, 1, 3)


        self.retranslateUi(export_window_main)

        QMetaObject.connectSlotsByName(export_window_main)
    # setupUi

    def retranslateUi(self, export_window_main):
        export_window_main.setWindowTitle(QCoreApplication.translate("export_window_main", u"Form", None))
        self.label_3.setText(QCoreApplication.translate("export_window_main", u"Filters", None))
        self.source_btn.setText(QCoreApplication.translate("export_window_main", u"Source", None))
        self.source_btn.setProperty("color", QCoreApplication.translate("export_window_main", u"primary", None))
        self.source_label.setText(QCoreApplication.translate("export_window_main", u"Source logs folder", None))
        self.destination_btn.setText(QCoreApplication.translate("export_window_main", u"Destination", None))
        self.destination_btn.setProperty("color", QCoreApplication.translate("export_window_main", u"primary", None))
        self.destination_label.setText(QCoreApplication.translate("export_window_main", u"Destination folder", None))
        self.start_export_btn.setText(QCoreApplication.translate("export_window_main", u"Export", None))
        self.start_export_btn.setProperty("color", QCoreApplication.translate("export_window_main", u"highlight", None))
        self.format_groupbox.setTitle(QCoreApplication.translate("export_window_main", u"Format", None))
        self.json_btn.setText(QCoreApplication.translate("export_window_main", u"json", None))
        self.csv_btn.setText(QCoreApplication.translate("export_window_main", u"csv", None))
        self.split_file_btn.setText(QCoreApplication.translate("export_window_main", u"Split files", None))
    # retranslateUi


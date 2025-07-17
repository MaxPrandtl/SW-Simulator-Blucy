# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'export_sv_window.ui'
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
from PySide6.QtWidgets import (QApplication, QGridLayout, QLabel, QLineEdit,
    QPushButton, QSizePolicy, QVBoxLayout, QWidget)

class Ui_Form(object):
    def setupUi(self, Form):
        if not Form.objectName():
            Form.setObjectName(u"Form")
        Form.resize(739, 501)
        Form.setStyleSheet(u"* {\n"
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
        self.verticalLayout = QVBoxLayout(Form)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.central_widget = QWidget(Form)
        self.central_widget.setObjectName(u"central_widget")
        self.central_widget.setStyleSheet(u"")
        self.gridLayout_3 = QGridLayout(self.central_widget)
        self.gridLayout_3.setObjectName(u"gridLayout_3")
        self.export_profile_file_name = QLineEdit(self.central_widget)
        self.export_profile_file_name.setObjectName(u"export_profile_file_name")

        self.gridLayout_3.addWidget(self.export_profile_file_name, 2, 1, 1, 1)

        self.export_source_profile_btn = QPushButton(self.central_widget)
        self.export_source_profile_btn.setObjectName(u"export_source_profile_btn")
        self.export_source_profile_btn.setCursor(QCursor(Qt.PointingHandCursor))
        self.export_source_profile_btn.setStyleSheet(u"")

        self.gridLayout_3.addWidget(self.export_source_profile_btn, 0, 0, 1, 1, Qt.AlignLeft)

        self.export_site_info = QLineEdit(self.central_widget)
        self.export_site_info.setObjectName(u"export_site_info")

        self.gridLayout_3.addWidget(self.export_site_info, 3, 1, 1, 1)

        self.export_tare = QLineEdit(self.central_widget)
        self.export_tare.setObjectName(u"export_tare")

        self.gridLayout_3.addWidget(self.export_tare, 4, 1, 1, 1)

        self.label_3 = QLabel(self.central_widget)
        self.label_3.setObjectName(u"label_3")

        self.gridLayout_3.addWidget(self.label_3, 2, 0, 1, 1)

        self.label_2 = QLabel(self.central_widget)
        self.label_2.setObjectName(u"label_2")

        self.gridLayout_3.addWidget(self.label_2, 4, 0, 1, 1)

        self.export_destination_folder_btn = QPushButton(self.central_widget)
        self.export_destination_folder_btn.setObjectName(u"export_destination_folder_btn")
        self.export_destination_folder_btn.setCursor(QCursor(Qt.PointingHandCursor))
        self.export_destination_folder_btn.setStyleSheet(u"")

        self.gridLayout_3.addWidget(self.export_destination_folder_btn, 1, 0, 1, 1, Qt.AlignLeft)

        self.label = QLabel(self.central_widget)
        self.label.setObjectName(u"label")

        self.gridLayout_3.addWidget(self.label, 3, 0, 1, 1, Qt.AlignLeft)

        self.export_destination_profile_folder_label = QLabel(self.central_widget)
        self.export_destination_profile_folder_label.setObjectName(u"export_destination_profile_folder_label")
        font = QFont()
        self.export_destination_profile_folder_label.setFont(font)
        self.export_destination_profile_folder_label.setAcceptDrops(True)
        self.export_destination_profile_folder_label.setStyleSheet(u"")

        self.gridLayout_3.addWidget(self.export_destination_profile_folder_label, 1, 1, 1, 1)

        self.export_source_profile_file_label = QLabel(self.central_widget)
        self.export_source_profile_file_label.setObjectName(u"export_source_profile_file_label")
        self.export_source_profile_file_label.setFont(font)
        self.export_source_profile_file_label.setAcceptDrops(True)
        self.export_source_profile_file_label.setStyleSheet(u"")

        self.gridLayout_3.addWidget(self.export_source_profile_file_label, 0, 1, 1, 1)

        self.export_profile_btn = QPushButton(self.central_widget)
        self.export_profile_btn.setObjectName(u"export_profile_btn")
        self.export_profile_btn.setCursor(QCursor(Qt.PointingHandCursor))

        self.gridLayout_3.addWidget(self.export_profile_btn, 5, 0, 1, 1)


        self.verticalLayout.addWidget(self.central_widget)


        self.retranslateUi(Form)

        QMetaObject.connectSlotsByName(Form)
    # setupUi

    def retranslateUi(self, Form):
        Form.setWindowTitle(QCoreApplication.translate("Form", u"Form", None))
        self.export_source_profile_btn.setText(QCoreApplication.translate("Form", u"Source", None))
        self.export_source_profile_btn.setProperty("color", QCoreApplication.translate("Form", u"secondary", None))
        self.label_3.setText(QCoreApplication.translate("Form", u"Exported file name", None))
        self.label_3.setProperty("bg", QCoreApplication.translate("Form", u"none", None))
        self.label_2.setText(QCoreApplication.translate("Form", u"Tare", None))
        self.label_2.setProperty("bg", QCoreApplication.translate("Form", u"none", None))
        self.export_destination_folder_btn.setText(QCoreApplication.translate("Form", u"Destination", None))
        self.export_destination_folder_btn.setProperty("color", QCoreApplication.translate("Form", u"secondary", None))
        self.label.setText(QCoreApplication.translate("Form", u"Site info", None))
        self.label.setProperty("bg", QCoreApplication.translate("Form", u"none", None))
        self.export_destination_profile_folder_label.setText(QCoreApplication.translate("Form", u"Destination path", None))
        self.export_source_profile_file_label.setText(QCoreApplication.translate("Form", u"Source CTD file", None))
        self.export_profile_btn.setText(QCoreApplication.translate("Form", u"Export", None))
        self.export_profile_btn.setProperty("color", QCoreApplication.translate("Form", u"highlight", None))
    # retranslateUi


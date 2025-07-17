# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'error_dialog.ui'
##
## Created by: Qt User Interface Compiler version 5.15.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import *
from PySide6.QtGui import *
from PySide6.QtWidgets import *

import resources_rc

class Ui_Error(object):
    def setupUi(self, Error):
        if not Error.objectName():
            Error.setObjectName(u"Error")
        Error.resize(478, 196)
        sizePolicy = QSizePolicy(QSizePolicy.Fixed, QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(Error.sizePolicy().hasHeightForWidth())
        Error.setSizePolicy(sizePolicy)
        Error.setMinimumSize(QSize(478, 196))
        Error.setMaximumSize(QSize(478, 196))
        Error.setStyleSheet(u"QDialog {\n"
"	background-color: rgb(49, 49, 49);\n"
"}\n"
"\n"
"QFrame {\n"
"	border: none;\n"
"}")
        self.horizontalLayout = QHBoxLayout(Error)
        self.horizontalLayout.setSpacing(10)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.frame_2 = QFrame(Error)
        self.frame_2.setObjectName(u"frame_2")
        self.frame_2.setFrameShape(QFrame.StyledPanel)
        self.frame_2.setFrameShadow(QFrame.Raised)
        self.verticalLayout_2 = QVBoxLayout(self.frame_2)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.pushButton = QPushButton(self.frame_2)
        self.pushButton.setObjectName(u"pushButton")
        sizePolicy1 = QSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
        sizePolicy1.setHorizontalStretch(75)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.pushButton.sizePolicy().hasHeightForWidth())
        self.pushButton.setSizePolicy(sizePolicy1)
        self.pushButton.setMinimumSize(QSize(101, 101))
        self.pushButton.setMaximumSize(QSize(101, 101))
        self.pushButton.setStyleSheet(u"background-color: rgb(255, 93, 86);\n"
"border-radius: 50%;")
        icon = QIcon()
        icon.addFile(u":/icons/icons/alert-triangle.svg", QSize(), QIcon.Normal, QIcon.Off)
        self.pushButton.setIcon(icon)
        self.pushButton.setIconSize(QSize(40, 40))
        self.pushButton.setFlat(True)

        self.verticalLayout_2.addWidget(self.pushButton)


        self.horizontalLayout.addWidget(self.frame_2, 0, Qt.AlignTop)

        self.frame = QFrame(Error)
        self.frame.setObjectName(u"frame")
        self.frame.setFrameShape(QFrame.StyledPanel)
        self.frame.setFrameShadow(QFrame.Raised)
        self.verticalLayout = QVBoxLayout(self.frame)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.title = QLabel(self.frame)
        self.title.setObjectName(u"title")
        font = QFont()
        font.setPointSize(24)
        self.title.setFont(font)

        self.verticalLayout.addWidget(self.title)

        self.message = QPlainTextEdit(self.frame)
        self.message.setObjectName(u"message")
        font1 = QFont()
        font1.setPointSize(18)
        self.message.setFont(font1)
        self.message.setUndoRedoEnabled(False)
        self.message.setReadOnly(True)

        self.verticalLayout.addWidget(self.message)


        self.horizontalLayout.addWidget(self.frame)


        self.retranslateUi(Error)

        QMetaObject.connectSlotsByName(Error)
    # setupUi

    def retranslateUi(self, Error):
        Error.setWindowTitle(QCoreApplication.translate("Error", u"Dialog", None))
        self.pushButton.setText("")
        self.title.setText(QCoreApplication.translate("Error", u"Error", None))
        self.message.setPlainText("")
    # retranslateUi


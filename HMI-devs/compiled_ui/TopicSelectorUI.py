# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'topic_selector.ui'
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
from PySide6.QtWidgets import (QApplication, QDialog, QGridLayout, QLabel,
    QLineEdit, QListWidget, QListWidgetItem, QPushButton,
    QSizePolicy, QSpacerItem, QWidget)

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        if not Dialog.objectName():
            Dialog.setObjectName(u"Dialog")
        Dialog.resize(529, 499)
        Dialog.setStyleSheet(u"* {\n"
"	background-color: rgb(9, 51, 64); \n"
"	border-radius: 10px;\n"
"	font-size: 16px;\n"
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
"	text-align: left;\n"
"}\n"
"\n"
"\n"
"QPushButton:disabled {\n"
"	color: rgba(255, 255, 255, 100);\n"
"}")
        self.gridLayout = QGridLayout(Dialog)
        self.gridLayout.setObjectName(u"gridLayout")
        self.add_btn = QPushButton(Dialog)
        self.add_btn.setObjectName(u"add_btn")
        self.add_btn.setMinimumSize(QSize(40, 40))
        self.add_btn.setMaximumSize(QSize(40, 40))
        self.add_btn.setCursor(QCursor(Qt.PointingHandCursor))
        self.add_btn.setStyleSheet(u"QPushButton {\n"
"	background-color: rgb(9, 40, 47);\n"
"	text-align: center;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	\n"
"	background-color: rgb(6, 25, 29);\n"
"}\n"
"")
        self.add_btn.setFlat(True)

        self.gridLayout.addWidget(self.add_btn, 1, 3, 1, 1)

        self.new_topic_input = QLineEdit(Dialog)
        self.new_topic_input.setObjectName(u"new_topic_input")
        self.new_topic_input.setStyleSheet(u"background-color: rgb(7, 25, 29);")

        self.gridLayout.addWidget(self.new_topic_input, 1, 1, 1, 1)

        self.topic_list_widget = QListWidget(Dialog)
        self.topic_list_widget.setObjectName(u"topic_list_widget")
        self.topic_list_widget.setStyleSheet(u"background-color: rgb(7, 25, 29);")

        self.gridLayout.addWidget(self.topic_list_widget, 0, 0, 1, 6)

        self.save_btn = QPushButton(Dialog)
        self.save_btn.setObjectName(u"save_btn")
        self.save_btn.setCursor(QCursor(Qt.PointingHandCursor))
        self.save_btn.setStyleSheet(u"QPushButton{\n"
"	background-color: rgb(31, 83, 94);\n"
"}\n"
"\n"
"QPushButton:enabled:hover{\n"
"	background-color: rgb(46, 138, 159);\n"
"}")
        self.save_btn.setFlat(True)

        self.gridLayout.addWidget(self.save_btn, 3, 0, 1, 1, Qt.AlignLeft)

        self.label = QLabel(Dialog)
        self.label.setObjectName(u"label")

        self.gridLayout.addWidget(self.label, 1, 0, 1, 1)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.gridLayout.addItem(self.horizontalSpacer, 1, 2, 1, 1)

        self.remove_btn = QPushButton(Dialog)
        self.remove_btn.setObjectName(u"remove_btn")
        self.remove_btn.setEnabled(True)
        self.remove_btn.setMinimumSize(QSize(40, 40))
        self.remove_btn.setMaximumSize(QSize(40, 40))
        self.remove_btn.setCursor(QCursor(Qt.PointingHandCursor))
        self.remove_btn.setStyleSheet(u"QPushButton {\n"
"	background-color: rgb(9, 40, 47);\n"
"	text-align: center;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	\n"
"	background-color: rgb(6, 25, 29);\n"
"}\n"
"")
        self.remove_btn.setFlat(True)

        self.gridLayout.addWidget(self.remove_btn, 1, 4, 1, 1)


        self.retranslateUi(Dialog)

        QMetaObject.connectSlotsByName(Dialog)
    # setupUi

    def retranslateUi(self, Dialog):
        Dialog.setWindowTitle(QCoreApplication.translate("Dialog", u"Dialog", None))
        self.add_btn.setText(QCoreApplication.translate("Dialog", u"+", None))
        self.save_btn.setText(QCoreApplication.translate("Dialog", u"Save", None))
        self.label.setText(QCoreApplication.translate("Dialog", u"Topic name:", None))
        self.remove_btn.setText(QCoreApplication.translate("Dialog", u"-", None))
    # retranslateUi


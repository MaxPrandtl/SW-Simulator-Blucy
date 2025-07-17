from PySide6.QtWidgets import QDialog
from compiled_ui.TopicSelectorUI import Ui_Dialog
from controller.Controller import Controller


class TopicSelectorDialog(Ui_Dialog, QDialog):
    def __init__(self, parent=None, controller: Controller = None):
        super().__init__(parent)
        self.setupUi(self)

        self.controller = controller

        self.add_btn.clicked.connect(self.__add_topic)
        self.remove_btn.clicked.connect(self.__remove_topic)

        self.remove_btn.setEnabled(False)
        self.topic_list_widget.currentItemChanged.connect(self.__enable_remove_btn)

        self.save_btn.clicked.connect(self.__save_topics)

        self.topics = None

        self.closeEvent = lambda e: self.topic_list_widget.clear()

    def __save_topics(self):
        new_config = self.controller.get_all_config().copy()
        new_config["topics"] = list(self.topics)
        self.controller.save_and_update(new_config)
        # self.close()

    def __enable_remove_btn(self, current, previous):
        self.remove_btn.setEnabled(current is not None)

    def __remove_topic(self):
        item = self.topic_list_widget.currentItem()
        if item is None:
            return
        topic = item.text()
        self.topic_list_widget.takeItem(self.topic_list_widget.row(item))
        self.topics.remove(topic)
        # print(topic)

    def __add_topic(self):
        topic = self.new_topic_input.text()
        if topic == "" or topic in self.topics:
            return
        self.topic_list_widget.addItem(topic)
        self.new_topic_input.clear()
        self.topics.add(topic)
        # print(topic)

    def show(self):
        super().show()
        self.topics = set(self.controller.get_config("topics"))
        for topic in self.topics:
            self.topic_list_widget.addItem(topic)

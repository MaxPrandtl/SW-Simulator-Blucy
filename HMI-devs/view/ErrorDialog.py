from PySide6.QtWidgets import QDialog
from compiled_ui.ErrorDialogUI import Ui_Error


class ErroDialog(Ui_Error, QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setupUi(self)

    def show(self, message="", title="Error"):
        super().show()
        self.message.setPlainText(message)
        self.title.setText(title)
        self.exec()

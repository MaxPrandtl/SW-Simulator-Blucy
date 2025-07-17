from abc import ABC, abstractmethod
from typing import TYPE_CHECKING


if TYPE_CHECKING:
    from view.MainWindow import MainWindow


# Classe astratta (modella il funzionamento di un handler per un plot realtime)
class RealtimePlotHandler(ABC):
    def __init__(self, view: "MainWindow") -> None:
        self.view = view

    @abstractmethod
    def setup_plot(self):
        pass

    @abstractmethod
    def request_graph_update(self, **kwargs):
        pass

    @abstractmethod
    def connect_signal(self):
        pass

from typing import Union


class MessageModel:
    """
    Modella la struttura di un messaggio proveniente da MQTT. Contiene 3 attributi:
    :param value: Valore
    :param timestamp: Timestamp del messaggio
    :param valid: Indica se il messaggio è valido
    """

    def __init__(self, value: float = 0.0, timestamp: int = 0, valid: Union[str, bool] = True) -> None:
        self.__value = value
        self.__timestamp = timestamp
        self.__valid: bool = valid if isinstance(valid, bool) else True if valid == "1" else False

        # Funzione richiamata da quei valori che necessitano di elaborazione prima dell'assegnamento
        self.__processing_function = None

    def set_processing_function(self, function):
        """Imposta una funzione di processing per il messaggio, eseguita prima dell'assegnamento."""
        self.__processing_function = function

    def set_data(self, message_model):
        """
        Metodo per l'assegnamento di nuovi valori.

        :param value: MessageModel contenenti i nuovi valori da assegnare.

        Si consiglia di usare sempre il metodo set_data() per assegnare i nuovi valori, in modo che
        la funzione di processing venga eseguita (se definita).
        """
        if self.__processing_function is not None:
            message_model = self.__processing_function(message_model)

        self.value = message_model.value
        self.timestamp = message_model.timestamp
        self.valid = message_model.valid

    @property
    def value(self) -> float:
        return self.__value

    @value.setter
    def value(self, value: float) -> None:
        self.__value = value

    @property
    def timestamp(self) -> float:
        return self.__timestamp

    @timestamp.setter
    def timestamp(self, timestamp: int) -> None:
        self.__timestamp = int(timestamp)

    @property
    def valid(self) -> bool:
        return self.__valid

    @valid.setter
    def valid(self, valid: Union[str, bool]) -> None:
        if isinstance(valid, str):
            self.__valid = True if valid == "1" else False
        else:
            self.__valid = valid

    # Richiamare il metodo str() su un oggetto MessageModel
    # permette di ottenere il valore del messaggio in formattato con 4 cifre decimali
    def __str__(self) -> str:
        return "%.4f" % self.value

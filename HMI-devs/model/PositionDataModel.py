class PositionDataModel:
    def __init__(self) -> None:
        self.__lat = None
        self.__lon = None
        self.__lat_pole = None
        self.__lon_pole = None

    @property
    def lat(self) -> str:
        return f"{self.__lat} {self.__lat_pole}"

    @lat.setter
    def lat(self, lat) -> None:
        tokens = lat.split(" ")
        self.__lat = tokens[0]
        self.__lat_pole = tokens[1]

    @property
    def lon(self) -> str:
        return f"{self.__lon} {self.__lon_pole}"

    @lon.setter
    def lon(self, lon) -> None:
        tokens = lon.split(" ")
        self.__lon = tokens[0]
        self.__lon_pole = tokens[1]

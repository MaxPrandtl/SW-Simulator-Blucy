# Singleton class to store the current timestamp
class Timestamp(object):
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super(Timestamp, cls).__new__(cls)

            cls.value = 0

        return cls._instance

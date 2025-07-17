from enum import Enum, unique


@unique
class Formats(Enum):
  JSON = 'JSON'
  CSV = 'CSV'
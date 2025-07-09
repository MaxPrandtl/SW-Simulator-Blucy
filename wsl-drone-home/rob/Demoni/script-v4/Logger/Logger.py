from pathlib import Path

class Logger:
  def __init__(self, file_path=None, fields=None, separator=';') -> None:
    if file_path == None:
      raise ValueError('Path to file not given')
    
    self.__file_path = Path(file_path)
    self.__separator = separator

    if self.__file_path.is_dir():
      raise ValueError('Path must be a file')
    
    if not self.__file_path.is_file():
      self.__file_path.touch()
      if type(fields) is list:
        self.log(self.__separator.join(fields))

  @property
  def separator(self):
    return self.__separator


  def log(self, data):
    with self.__file_path.open(mode='a') as f:
      f.write('%s\n' % data)
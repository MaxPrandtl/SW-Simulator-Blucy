class ExportFilter:
  def __init__(self) -> None:
    self.empty = True
    self.__filter = dict()   # Verranno esportati solo i subtopic specificati 
  
  def add(self, topic):
    if topic.strip() == '': return

    tokens = topic.split('/')
    n_tokens = len(tokens)

    if self.__filter.get(tokens[0]) == None:
      self.__filter[tokens[0]] = []
        
    if n_tokens == 1:
        self.__filter.get(tokens[0]).append('#')
    elif n_tokens > 1: 
      if tokens[1].strip() == '':
        # Se dopo / c'è una stringa vuota
        self.__filter.get(tokens[0]).append('#')
      else:
        # Se dopo / c'è una stringa valida aggiungo al set dei subtopoic
        self.__filter.get(tokens[0]).append('/'.join(tokens[1:]))

    self.empty = False

  def get_items(self):
    return self.__filter


  def __str__(self) -> str:
    return str(self.__filter)
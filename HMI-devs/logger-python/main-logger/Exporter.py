import datetime
import sys
from pathlib import Path
from queue import Queue
from threading import Thread
from ExportFilter import ExportFilter
import json
from Formats import Formats
# import tracemalloc

CSV_SEPARATOR = ';'

class Exporter:
  def __init__(self, source_folder=None, topics_filter=None, destination_folder=None, file_format='json'):
    if source_folder == None:
      raise ValueError('Specificare il nome della cartella da cui esportare i logs')

    if destination_folder == None:
      raise ValueError('Specificare il nome della cartella in cui esportare i logs')

    self.source_folder = Path(source_folder)
    self.source_folder_path = source_folder
    self.destination_folder = Path(destination_folder)

    if not self.source_folder.is_dir():
      raise ValueError('Il percorso della cartella source specificata non è valido')

    if not self.destination_folder.is_dir():
      raise ValueError('Il percorso della cartella di destinazione non è valido')


    if isinstance(topics_filter, ExportFilter):
      self.topics_filter = topics_filter  
    else:
      self.topics_filter = ExportFilter()

    self.data = {}
    self.processing_queue = Queue(maxsize=-1)
    self.__all_files_read = False
    self.format = Formats(file_format.upper())
    
    self.paths = []
    self.path = ''


  def export(self, file_split=False):
    if file_split == False:
      t = Thread(target=self.__build_dict_worker)
      t.start()
      self.__read_files()
      t.join()

      if self.format == Formats.JSON:
        with open(self.destination_folder.joinpath('exported.json'), 'w') as res:
          res.write(json.dumps(self.data))
      elif self.format == Formats.CSV:
        now = datetime.datetime.now()
        print(self.source_folder)
        fname = self.source_folder_path.split('/')[-1]
        #csv_folder_name = f'CSV_{now.strftime("%d-%m-%Y_%H-%M-%S")}'
        csv_folder_name = f'CSV_{fname}'

        csv_folder = self.destination_folder.joinpath(csv_folder_name)
        if not csv_folder.is_dir():
          Path.mkdir(self.destination_folder.joinpath(csv_folder_name))

        self.export_csv(csv_folder)
      else:
        print('formato non valido')
        exit(-3)
    else:
      self.__read_files_split()

  def join_path(self, path_arr):
    p = Path()
    for path in path_arr:
      p = p.joinpath(path)
    return p

  # Esporta i dati in formato csv
  # il parametro file_count è diverso da None solo in caso di split
  def export_csv(self, destination_csv_folder, file_count=None):
    print('-' *10 + '\n')
    csv_data = {}

    # Costruzione di un dizionario csv
    for key in self.data.keys():
      topic = '/'.join(key.split('/')[:-1])
      last_subtopic = key.split('/')[-1]
      if csv_data.get(topic) == None:
        csv_data[topic] = {}
      
      csv_data[topic][last_subtopic] = self.data[f'{topic}/{last_subtopic}']

    # Nel caso la modalità sia singolo file elimino l'oggetto data dalla memoria
    # altrimenti lo reinizializzo poiché verrà utilizzato per i prossimi chunks
    if file_count == None:
      del self.data
    else:
      self.data = {}

    for topic in list(csv_data.keys()):
      Path.mkdir(destination_csv_folder.joinpath(topic), parents=True, exist_ok=True)
      name = topic.split('/')[-1]

      if file_count != None:
        name += str(file_count)

      for subtopic in csv_data[topic]:
        print(topic, name, subtopic)
        
        headers = []
        mat = []

        with open(destination_csv_folder.joinpath(topic, f'{subtopic}.csv'), 'a') as csv_writer:

          # Controllo il numero di token che contiente il dato 
          first = csv_data[topic][subtopic][0].split(CSV_SEPARATOR)
          n_tokens = len(first)
          
          for _ in range(n_tokens):
            headers.append(subtopic)
          
          print(topic, subtopic, headers)            
          
          mat.append(csv_data[topic][subtopic])

          mat = list(map(list, zip(*mat))) # Trasposizione della matrice

          h_str = CSV_SEPARATOR.join(headers)

          csv_writer.write(h_str + '\n')

          for row in mat:
            row_str = CSV_SEPARATOR.join(row) + '\n'
            csv_writer.write(row_str)


  # Crea una lista di array contenenti i file
  # es: [['file1.log', 'file2.log'], ['file3.log', 'file4.log']]
  # def __split_into_chunks(self, a, n):
  #   k, m = divmod(len(a), n)
  #   return (a[i * k + min(i, m):(i + 1) * k + min(i + 1, m)] for i in range(n))


  def __split_into_chunks(self, l, n):
    d, r = divmod(len(l), n)
    for i in range(n):
        si = (d+1)*(i if i < r else r) + d*(0 if i < r else i - r)
        yield l[si:si+(d+1 if i < r else d)]

  # Metodo che legge un numero di file predefinito 
  def __read_files_split(self):
    file_count = 0
    max_file = 30
    files = sorted(self.source_folder.glob('*.log'))
    number_of_files = len(files)
    if number_of_files < max_file:
      n_chunks = 1
    else:
      n_chunks = int(number_of_files / max_file) # max_file max files in a single chunk

    chunks = self.__split_into_chunks(files, n_chunks)

    # Per ogni chunk processo i file al suo interno
    for c in chunks:
      print(f'chunk {file_count} start')
     
      # Aggiungo il contenuto dei file alla coda
      self.__all_files_read = False
      for f in c:
        self.processing_queue.put(f.read_text())
      self.__all_files_read = True

      # Richiamo il metodo per la creazione del dizionario, in questo caso in modo sincrono
      self.__build_dict_worker()  
      
      csv_folder = self.destination_folder.joinpath('csv')
      if not csv_folder.is_dir():
        Path.mkdir(self.destination_folder.joinpath('csv'))
        
      self.export_csv(csv_folder, file_count)

      print(f'chunk {file_count} done')

      file_count += 1


  # Legge tutti i file e li inserisce nella coda, mentre il thread li processa e li salva su file
  def __read_files(self):
    count = 0
    for p in sorted(self.source_folder.glob('*.log')):
      print(f'File {count}')
      count += 1
      self.processing_queue.put(p.read_text())
    self.__all_files_read = True

  # Funziona in modo sincrono nel caso di split e asincrono nel caso di singolo file
  # Esegue una prima elaborazione dei dati memorizzandoli in un dizionario
  def __build_dict_worker(self):
    while not self.__all_files_read or not self.processing_queue.empty():
      if not self.processing_queue.empty():
        lines = self.processing_queue.get().split('\n')
        # Per ogni riga del file
        for line in lines:
          if line.strip() == '': 
            continue

          full_topic = line.split(' ')[0]
          levels = full_topic.split('/')
          payload = line.split(' ')[1:]

          main_topic = levels[0]
          subtopics = '/'.join(levels[1:])

          # Applico eventualmente il filtro per i topic
          if not self.topics_filter.empty:
            res = self.topics_filter.get_items().get(main_topic)
            if res == None or (not '#' in res and not subtopics in res):
              continue

          if self.format == Formats.JSON:
            # Generazione dinamica del json
            prevLevel = self.data
            prevProp = levels.pop(0)

            for prop in levels:
              if prevLevel.get(prevProp) == None:
                prevLevel[prevProp] = {}

              prevLevel = prevLevel[prevProp]
              prevProp = prop

            if prevLevel.get(prevProp) == None:
              prevLevel[prevProp] = []
            
            prevLevel[prevProp].append(payload)

          elif self.format == Formats.CSV:
            payload_csv = CSV_SEPARATOR.join(payload)

            if self.data.get(full_topic) == None:
              self.data[full_topic] = []
            
            self.data[full_topic].append(payload_csv)
      

if len(sys.argv) != 6:
  print('Numero di argomenti non valido')
  exit(2)

topics = sys.argv[3].split(',')

lf = ExportFilter()

for topic in topics:
  topic = topic.strip()
  lf.add(topic)

ex = Exporter(topics_filter=lf, source_folder=sys.argv[1], destination_folder=sys.argv[2], file_format=sys.argv[4])

# tracemalloc.start()

split_option = sys.argv[5] == '1'
ex.export(file_split=split_option)
exit(0)
# print(tracemalloc.get_traced_memory())

# tracemalloc.stop()

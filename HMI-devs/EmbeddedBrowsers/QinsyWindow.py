import os

cmd="C:\\Program Files (x86)\\QPS\\Qinsy\\9.5.2\\Console.exe"

os.system("Powershell -Command \"& { Start-Process '"+cmd+"' -Verb RunAs } \" ")


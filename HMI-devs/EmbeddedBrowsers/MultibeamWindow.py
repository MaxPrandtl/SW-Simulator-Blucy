import os

cmd="C:\\R2Sonic 2020\\R2Sonic.exe"

os.system("Powershell -Command \"& { Start-Process '"+cmd+"' -Verb RunAs } \" ")
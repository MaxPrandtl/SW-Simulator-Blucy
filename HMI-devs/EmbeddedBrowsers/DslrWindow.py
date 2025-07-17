import os

cmd="C:\\Users\\Max Menghini\\Desktop\\qDslrDashboard_V3.6.7_Windows_X64\\qDslrDashboard.exe"

os.system("Powershell -Command \"& { Start-Process '"+cmd+"' -Verb RunAs } \" ")
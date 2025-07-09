import subprocess
def miniSVS_subs():
 #Definizione funzioni di subscription topic MiniSVS (vedi lista in documents)

 sub_pressure = subprocess.Popen(
    ['mosquitto_sub','-t', 'miniSVS/pressure', '-C', '1'],
    stdout=subprocess.PIPE, universal_newlines=True)
 sub_soundSpeed = subprocess.Popen(
    ['mosquitto_sub','-t', 'miniSVS/soundSpeed', '-C', '1'],
    stdout=subprocess.PIPE, universal_newlines=True)

 #Iscrizione ai subtopic GPGGA (valore 'nometopic' e valore controllo tempo 'time_nometopic')

 msg_pressure = sub_pressure.communicate() 
 pressure = msg_pressure[0].split()[0]
 time_pressure = msg_pressure[0].split()[1]
 

 msg_soundSpeed = sub_soundSpeed.communicate() 
 soundSpeed = msg_soundSpeed[0].split()[0]
 time_soundSpeed = msg_soundSpeed[0].split()[1]
 
 return pressure, soundSpeed

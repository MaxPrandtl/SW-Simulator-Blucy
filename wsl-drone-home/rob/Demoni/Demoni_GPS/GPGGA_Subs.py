import subprocess
def GPGGA_subs():
 #Definizione funzioni di subscription topic GPGGA (vedi lista in documents)

 sub_TimestampNano = subprocess.Popen(
    ['mosquitto_sub','-t', 'us_imu/utcTimestampNanoseconds', '-C', '1'],
    stdout=subprocess.PIPE, universal_newlines=True)
 sub_latitudeDD = subprocess.Popen(
    ['mosquitto_sub','-t', 'us_imu/latitude', '-C', '1'],
    stdout=subprocess.PIPE, universal_newlines=True)
 sub_longitudeDD = subprocess.Popen(
    ['mosquitto_sub','-t', 'us_imu/longitude', '-C', '1'],
    stdout=subprocess.PIPE, universal_newlines=True)
 sub_GNSSquality = subprocess.Popen(
    ['mosquitto_sub','-t', 'us_imu/utcTimestampFlags', '-C', '1'],
    stdout=subprocess.PIPE, universal_newlines=True)
 sub_heightAE = subprocess.Popen(
    ['mosquitto_sub','-t', 'us_imu/heightAboveEllipsoid', '-C', '1'],
    stdout=subprocess.PIPE, universal_newlines=True)
 sub_heightAMSL = subprocess.Popen(
    ['mosquitto_sub','-t', 'us_imu/heightAboveMSL', '-C', '1'],
    stdout=subprocess.PIPE, universal_newlines=True)
 sub_HDOP = subprocess.Popen(
    ['mosquitto_sub','-t', 'us_imu/horizontalAccuracy', '-C', '1'],
    stdout=subprocess.PIPE, universal_newlines=True)

 #Iscrizione ai subtopic GPGGA (valore 'nometopic' e valore controllo tempo 'time_nometopic')

 msg_TimestampNano = sub_TimestampNano.communicate() 
 TimestampNano = msg_TimestampNano[0].split()[0]
 time_TimestampNano = msg_TimestampNano[0].split()[1]
 

 msg_latitudeDD = sub_latitudeDD.communicate() 
 latitudeDD = msg_latitudeDD[0].split()[0]
 time_latitudeDD = msg_TimestampNano[0].split()[1]
 
 msg_longitudeDD = sub_longitudeDD.communicate() 
 longitudeDD = msg_longitudeDD[0].split()[0]
 time_longitudeDD = msg_longitudeDD[0].split()[1]

 msg_GNSSquality = sub_GNSSquality.communicate() 
 GNSSquality = msg_GNSSquality[0].split()[0]
 time_GNSSquality = msg_GNSSquality[0].split()[1]

 msg_heightAE = sub_heightAE.communicate() 
 heightAE = msg_heightAE[0].split()[0]
 time_heightAE = msg_heightAE[0].split()[1]

 msg_heightAMSL = sub_heightAMSL.communicate() 
 heightAMSL = msg_heightAMSL[0].split()[0]
 time_heightAMSL = msg_heightAMSL[0].split()[1]
 
 msg_HDOP = sub_HDOP.communicate() 
 HDOP=msg_HDOP[0].split()[0]
 time_HDOP=msg_HDOP[0].split()[1]

 #print(TimestampNano,time_TimestampNano,latitudeDD,time_latitudeDD,longitudeDD,time_longitudeDD,GNSSquality,time_GNSSquality,heightAE,time_heightAE,heightAMSL,time_heightAMSL,HDOP,time_HDOP)
 #if #tutti i valori time_qualcosa sono uguali return stringhe, vai sul return else return print ('scrivi nu cazzo di messaggio
 return TimestampNano,latitudeDD,longitudeDD,GNSSquality,heightAE,heightAMSL,HDOP

#TimestampNano,latitudeDD,longitudeDD,GNSSquality,heightAE,heightAMSL,HDOP = GPGGA_subs()

from statistics import mean
from sys import argv
from datetime import datetime

# Args:
#  0: exporter.py
#  1: log_file path
#  2: destination path
#  3: site info
#  4: tare
#  5: latitude

if __name__ == "__main__":

    if len(argv) != 6:
        raise Exception("Numero di argomenti non corretto")

    log_file_path = argv[1]
    destination_path = argv[2]
    site_info = argv[3]
    tare = float(argv[4])
    latitude = argv[5]

    data = {}

    print(destination_path)

    # Now: 12/07/2021 13:10:54
    # Battery Level: 1.2V
    # MiniSVP: S/N 42994
    # Site info: WERBELLINSEE
    # Calibrated: 16/01/2014
    # Latitude: 52.915959
    # Mode: P9.999993e-2
    # Tare: 10.0664
    # Pressure units: dBar

    with open(destination_path, "a") as f:
        f.write("Now: " + datetime.now().strftime("%d/%m/%Y %H:%M:%S") + "\n")
        f.write("Battery Level: " + "1.2V" + "\n")
        f.write("MiniSVP: S/N 42994" + "\n")
        f.write("Site info: " + site_info + "\n")
        f.write("Calibrated: " + "16/01/2014" + "\n")
        f.write("Latitude: " + latitude + "\n")
        f.write("Mode: " + "P9.999993e-2" + "\n")
        f.write("Tare: " + str(tare) + "\n")
        f.write("Pressure units: " + "dBar" + "\n")

    with open(log_file_path, "r") as f:
        lines = f.readlines()

    flag1 = False
    flag2 = False
    flag3 = False

    tmp = {"pressure": 0, "temperature": 0, "soundSpeed": 0}

    for line in lines:
        line = line.strip().split(" ")
        if line[0] == "miniSVS/pressure":
            flag1 = True
            tmp["pressure"] = float(line[1]) - tare

        if line[0] == "miniCT/temperature":
            flag2 = True
            tmp["temperature"] = float(line[1])

        if line[0] == "miniSVS/soundSpeed":
            flag3 = True
            tmp["soundSpeed"] = float(line[1])

        if flag1 and flag2 and flag3:
            if tmp["pressure"] not in data:
                data[tmp["pressure"]] = {"temperature": [], "soundSpeed": []}

            data[tmp["pressure"]]["temperature"].append(tmp["temperature"])
            data[tmp["pressure"]]["soundSpeed"].append(tmp["soundSpeed"])

            # with open(destination_path, 'a') as f:
            #  f.write('%06.3f %06.3f %08.3f\n' % (tmp['pressure'], tmp['temperature'], tmp['soundSpeed']))

            flag1 = False
            flag2 = False
            flag3 = False

            tmp["pressure"] = 0
            tmp["temperature"] = 0
            tmp["soundSpeed"] = 0

    # print(data)
    for k, v in data.items():

        data[k]["temperature"] = mean(v["temperature"])
        data[k]["soundSpeed"] = mean(v["soundSpeed"])

    # Save on file in 3 columns
    with open(destination_path, "a") as f:
        for k, v in data.items():
            f.write("%06.3f %06.3f %08.3f\n" % (k, v["temperature"], v["soundSpeed"]))

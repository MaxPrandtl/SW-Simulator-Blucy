# SW-Simulator-Blucy

This repository contains all the necessary files and instructions to set up a virtual simulation environment for the Blucy unmanned underwater drone using WSL2, Ubuntu 22.04, Mosquitto MQTT broker, and custom firmware and sensor drivers.

---

## 🚀 Simulation Environment Setup (WSL + Ubuntu + Mosquitto)

### 1️⃣ Install WSL (Windows Subsystem for Linux)

WSL2 is the default for Windows 10 and 11.  
Open PowerShell as Administrator and run:

```powershell
wsl --install
```
---

### 2️⃣ Install Ubuntu 22.04

Install Ubuntu using this command:
```powershell
wsl --install -d Ubuntu-22.04
```
Alternatively, install it via the Microsoft Store.

---

### 3️⃣ Configure WSL user

During the first launch, Ubuntu will ask for a default user. Use:

Username: rob

Password: any secure password of your choice

This user will have sudo privileges and will be used to run all commands.

---

### 4️⃣ Update Ubuntu packages
Run these commands inside the Ubuntu (WSL) terminal:
```powershell
sudo apt-get update
sudo add-apt-repository universe
sudo apt-get update
```

---

### 5️⃣ Install development tools and dependencies
```powershell
sudo apt-get install -y build-essential cmake gdb git pkg-config valgrind
sudo apt-get install -y python3 python3-pip python3-venv
sudo apt-get install -y libboost-all-dev libssl-dev libffi-dev libtool autoconf automake
```

---

### 6️⃣ Install and configure Mosquitto (MQTT broker)
Install the broker and clients:
```powershell
sudo apt-get install -y mosquitto mosquitto-clients
```
Configure Mosquitto:
```powershell
sudo nano /etc/mosquitto/mosquitto.conf
```
Replace the content with:
```powershell
pid_file /var/run/mosquitto/mosquitto.pid
persistence true
persistence_location /var/lib/mosquitto/
log_dest file /var/log/mosquitto/mosquitto.log

# Allow anonymous access (for testing only)
allow_anonymous true
listener 1883
```
Save with CTRL+O, press Enter, then exit with CTRL+X.

Enable Mosquitto to start automatically:
```powershell
sudo systemctl enable mosquitto
sudo systemctl start mosquitto
```

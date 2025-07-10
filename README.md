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

###2️⃣ Install Ubuntu 22.04
Install Ubuntu using this command:
```powershell
wsl --install -d Ubuntu-22.04
```
Alternatively, install it via the Microsoft Store.

---

###3️⃣ Configure WSL user
During the first launch, Ubuntu will ask for a default user. Use:

Username: rob

Password: any secure password of your choice

This user will have sudo privileges and will be used to run all commands.

---

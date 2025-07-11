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
---

7️⃣ Manual Download of Firmware (`rob`) and Drivers (`c++-mscl`)

Download the repository as a `.zip` file (instead of using Git) following these steps:

#### 📥 A. Download and extract the project

1. Go to the GitHub repository:  
   [https://github.com/MaxPrandtl/SW-Simulator-Blucy](https://github.com/MaxPrandtl/SW-Simulator-Blucy)

2. Click the green **“Code”** button and select **“Download ZIP”**

3. Extract the ZIP archive to your **Windows Desktop**

You will have a folder like this on your desktop:
```
SW-Simulator-Blucy-main/
├── drivers/
│   └── c++-mscl/
├── wsl-drone-home/
│   └── rob/
│       ├── bak/
│       ├── bin/
│       ├── debs/
│       ├── Demoni/
│       ├── ixblue_stdbin_decoder/
│       ├── Roboworld/
│       ├── sync/
│       ├── .bash_logout
│       ├── .bashrc
│       ├── .profile
│       ├── sushidrop_start.sh
│       └── sushidrop_start_MOD.sh
├── README.md
```

- `wsl-drone-home/rob/` → contains the custom firmware environment
- `drivers/c++-mscl/` → contains C++ sensor drivers

---

### 8️⃣ Copy firmware and drivers into WSL

After extracting the ZIP folder to your Windows Desktop, run the following commands in your **Ubuntu (WSL)** terminal to properly copy the firmware and the drivers into the correct locations.

---

#### 📁 A. Copy the entire firmware content (including hidden files) into `/home/rob`

```bash
sudo cp -r "/mnt/c/Users/<YourUsername>/Desktop/SW-Simulator-Blucy-main/wsl-drone-home/rob/." ~/
```

> Replace `<YourUsername>` with your actual Windows username.  
> If your username includes spaces (e.g. `Max Menghini`), wrap the path in quotes as shown above.

✅ This command:
- Copies all **files and folders** from `rob/`, including **hidden files** (e.g. `.bashrc`, `.profile`)
- Does **not** create an extra `rob/` folder
- Merges everything directly into your WSL home (`/home/rob`)

To verify the result:

```bash
ls -la ~
```

You should see all firmware-related files, such as:

```
sushidrop_start.sh
Demoni/
Roboworld/
.bashrc
.profile
...
```

If needed, fix file ownership with:

```bash
sudo chown -R rob:rob ~/
```

---

#### ⚙️ B. Copy the `c++-mscl` driver folder into `/usr/share/`

```bash
sudo cp -r "/mnt/c/Users/<YourUsername>/Desktop/SW-Simulator-Blucy-main/drivers/c++-mscl" /usr/share/
```

✅ This creates the folder `/usr/share/c++-mscl/` with all its subfolders and hidden files.

To check:

```bash
ls -la /usr/share/c++-mscl
```


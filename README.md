# Environmental Monitoring System

## Introduction  
The **Environmental Monitoring System** is a lightweight C-based application designed to fetch and analyze weather data in real-time using the [WeatherStack API](https://weatherstack.com/). This project provides insights into the current environmental conditions of a specified location and alerts users to extreme weather conditions like high temperatures or low humidity. It automates periodic data collection and is compatible with Linux environments, including Virtual Machines (VMs) and Windows Subsystem for Linux (WSL).

---

## Features  
1. **Weather Data Fetching**:  
   - Fetches real-time weather data using the WeatherStack API.  
   - Supports configurable locations for dynamic monitoring.

2. **Environmental Alerts**:  
   - Issues alerts if:  
     - Temperature exceeds 30°C.  
     - Humidity falls below 20%.  
   - Alerts are displayed using desktop notifications on Linux systems (`notify-send`).

3. **Data Storage**:  
   - Raw JSON responses are saved to `raw_data.txt` for debugging.  
   - Parsed weather data is saved in a human-readable format in `environmental_data.txt`.

4. **Automation with Crontab**:  
   - Automate the script to run periodically using Linux's crontab utility for continuous monitoring.

5. **Linux & Virtual Environment Compatibility**:  
   - Fully compatible with Linux distributions.  
   - Runs seamlessly on VMs or WSL on Windows systems.

---

## Installation and Setup  

### Prerequisites  
1. Install required libraries:
   - [libcurl](https://curl.se/) for HTTP requests:
     ```bash
     sudo apt-get install libcurl4-openssl-dev
     ```
   - [JSON-C](https://github.com/json-c/json-c) for JSON parsing:
     ```bash
     sudo apt-get install libjson-c-dev
     ```

2. Have a C compiler installed, such as GCC:
   ```bash
   sudo apt-get install gcc
   ```

3. (Optional) Install `notify-send` for desktop notifications:
   ```bash
   sudo apt-get install libnotify-bin
   ```

---

### Cloning the Repository  
Clone the repository to your local machine:
```bash
git clone https://github.com/your-username/environmental-monitoring-project.git
cd environmental-monitoring-project
```

---

### Compilation and Execution  
Compile the program using the following command:
```bash
gcc -o weather_monitor main.c -lcurl -ljson-c
```

Run the program:
```bash
./weather_monitor
```

---

## Automating with Crontab  
To periodically fetch and analyze weather data, you can use **crontab** to schedule the program. For example, to run the program every hour:

1. Open the crontab editor:
   ```bash
   crontab -e
   ```

2. Add the following line to schedule the program:
   ```bash
   0 * * * * /path/to/environmental-monitoring-project/weather_monitor
   ```

3. Save and exit. The program will now run every hour.

---

## Running on a Virtual Machine or WSL  
This project works seamlessly in virtualized Linux environments or WSL.  

1. **Using WSL**:  
   - Install WSL on Windows. Follow the [WSL installation guide](https://learn.microsoft.com/en-us/windows/wsl/install).  
   - Set up the project as described above.

2. **Using a Virtual Machine**:  
   - Install a VM software like VirtualBox or VMware.  
   - Create a Linux-based VM and set up the project inside it.

---

## Alerts and Notifications  
### Desktop Notifications  
The program uses `notify-send` to issue desktop alerts for:
- High temperature (`>30°C`).  
- Low humidity (`<20%`).

### Logs and Records  
All fetched data is stored in the following files for reference:
- `raw_data.txt`: Contains raw JSON responses from the API.  
- `environmental_data.txt`: Contains structured, human-readable weather data.

---

## Future Enhancements  
- Add support for multiple locations in a single execution.  
- Enable email or SMS notifications for critical alerts.  
- Enhance the program to work on Windows systems natively.

---

## Contributing  
We welcome contributions! Feel free to fork this repository, create a new branch for your changes, and submit a pull request.

---

## License  
This project is open-source and available under the [MIT License](LICENSE).

---

### Author  
Developed by [HAADI KHAN](https://github.com/your-username)  
For any queries or suggestions, feel free to reach out.  

--- 

This README covers all essential aspects of your project and includes clear instructions for usage, setup, and automation. Let me know if you need further customizations!

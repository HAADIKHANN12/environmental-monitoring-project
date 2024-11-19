Environmental Monitoring System
Overview
This project is an integrated environmental monitoring system designed in C. It interacts with a free weather API to fetch real-time environmental data, such as temperature and humidity, for a specified location. The system processes, stores, and analyzes the data while generating alerts for critical environmental conditions.

Key Features
Real-Time Data Retrieval: Fetch live weather data from the WeatherStack API.
Data Storage: Save both raw JSON responses and processed environmental data to files.
Real-Time Alerts: Notify users of high temperature or low humidity conditions.
Automation: Use a shell script to automate periodic data retrieval every 10 minutes.
Code Optimization: Implements pointers and dynamic memory allocation for efficient data handling.
Modularity: Organized with header files for better readability and maintainability.
Setup Instructions
1. Prerequisites
Install the following software:
GCC: For compiling C programs.
cURL: For making HTTP requests.
JSON-C: A library for parsing JSON.
libnotify (Linux): For sending real-time alerts.
Git: For version control and cloning the repository.
Cron (Linux) or Task Scheduler (Windows): For automation.
Sign up for a free WeatherStack API key at weatherstack.com.
2. Clone Repository
bash
Copy code
git clone <repository-link>
cd <repository-name>
3. Configure Your API Key
Replace <API_KEY> in the main.c file with your WeatherStack API key.

4. Compile the Program
bash
Copy code
gcc -g -o environmental_monitor main.c -ljson-c -lcurl
5. Run the Program
bash
Copy code
./environmental_monitor
6. Automate Data Retrieval
To automate data retrieval every 10 minutes, follow these steps:

For Linux:
Edit your crontab:
bash
Copy code
crontab -e
Add the following line to execute the script every 10 minutes:
bash
Copy code
*/10 * * * * bash -c "/full/path/to/data_retrieval.sh"
For Windows:
Use Task Scheduler:
Set the task action to:
bash
Copy code
bash -c "/full/path/to/data_retrieval.sh"
7. View Alerts
On Linux, ensure libnotify is installed for notifications:

bash
Copy code
sudo apt install libnotify-bin
Project Files
main.c: The main program for fetching, processing, and storing weather data.
weather.h: Header file for modularizing the code.
data_retrieval.sh: Shell script for automating data retrieval.
raw_data.txt: Stores raw JSON responses.
environmental_data.txt: Stores processed environmental data.
License
This project is open-source and available under the MIT License

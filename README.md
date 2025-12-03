E-NOSE Monitoring System
A comprehensive environmental gas monitoring system with real-time visualization, Arduino control, Edge Impulse ML integration, and InfluxDB data storage.

https://img.shields.io/badge/Platform-Python%2520%257C%2520Rust%2520%257C%2520Arduino-blue
https://img.shields.io/badge/Version-2.0.0-green
https://img.shields.io/badge/License-MIT-yellow

📋 Overview
The E-NOSE (Electronic Nose) system is a sophisticated gas monitoring solution that combines multiple sensor technologies for accurate environmental analysis. This system features real-time data acquisition, machine learning capabilities, and comprehensive data storage.

✨ Features
Real-time Monitoring: Live visualization of 7 gas sensors

Direct Arduino Control: TCP-based communication with Arduino controllers

Edge Impulse Integration: Machine learning model training and inference

InfluxDB Storage: Time-series data storage and analytics

Rust Backend: High-performance data processing server

Modern UI: PyQt6-based desktop application with real-time plots

Export Capabilities: CSV export for data analysis

🛠️ System Architecture
text
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Arduino Nano  │    │   Rust Backend  │    │  PyQt6 Frontend │
│  with Sensors   │────│  (HTTP Server)  │────│   (GUI App)     │
│                 │    │                 │    │                 │
│ • GM-XXX Series │    │ • Data Storage  │    │ • Real-time Plots│
│ • MiCS-5524     │    │ • API Endpoints │    │ • Control Panel │
│ • Actuators     │    │ • CSV Export    │    │ • Edge Impulse  │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                      │                       │
         │                      │                       │
         ▼                      ▼                       ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│  Edge Impulse   │    │    InfluxDB     │    │   Data Export   │
│   ML Platform   │    │   Time Series   │    │     (CSV)       │
│                 │    │                 │    │                 │
│ • Model Training│    │ • Long-term     │    │ • Excel/SPSS    │
│ • Anomaly Detect│    │   Storage       │    │ • Analysis      │
│ • Classification│    │ • Dashboards    │    │ • Reports       │
└─────────────────┘    └─────────────────┘    └─────────────────┘
📊 Sensor Configuration
GM-XXX Series Sensors
NO₂ Sensor: Nitrogen dioxide detection

Ethanol Sensor: Alcohol concentration

VOC Sensor: Volatile organic compounds

CO Sensor: Carbon monoxide

MiCS-5524 Sensors
CO Sensor: Carbon monoxide (alternative)

Ethanol Sensor: Alcohol concentration

VOC Sensor: Volatile organic compounds

🚀 Quick Start
Prerequisites
bash
# Python 3.8 or higher
python --version

# Rust (for backend)
rustc --version

# Arduino IDE (for microcontroller programming)
Installation
Clone the repository

bash
git clone https://github.com/yourusername/e-nose-system.git
cd e-nose-system
Install Python dependencies

bash
pip install -r requirements.txt
Install Rust dependencies

bash
cd rust-backend
cargo build --release
Configure Arduino

Upload arduino/enoise_controller.ino to your Arduino

Update IP address in the Arduino code

Connect sensors according to pin configuration

Configuration
Backend Configuration (config.toml)

toml
[backend]
ip = "192.168.1.146"
port = 8080

[arduino]
ip = "192.168.1.122"
port = 8082

[edge_impulse]
api_key = "your_api_key_here"
project_id = "your_project_id"
hmac_key = "your_hmac_key"

[influxdb]
url = "http://localhost:8086"
token = "your_influxdb_token"
org = "your_organization"
bucket = "e_nose_data"
Environment Variables (optional)

bash
export EDGE_IMPULSE_API_KEY="your_key"
export INFLUXDB_TOKEN="your_token"
🖥️ Usage
Starting the System
Start Rust Backend

bash
cd rust-backend
cargo run --release
Start Frontend Application

bash
python frontend/main.py
Arduino Setup

Power on Arduino

Ensure network connectivity

The system will auto-connect

Main Interface Controls
Button	Function	Description
📡 START MONITOR	Start data acquisition	Begins reading sensor data
⏹ STOP MONITOR	Stop monitoring	Pauses data collection
📊 GET STATUS	System status check	Displays connection and sensor status
💾 EXPORT CSV	Data export	Saves data to CSV file
🔗 Test Connection	Connectivity test	Tests all system connections
🧪 Test EI	Edge Impulse test	Tests ML integration
START/STOP	Arduino control	Direct actuator control
Direct Arduino Control
The system supports direct TCP commands to Arduino:

python
# Start sampling cycle
START_SAMPLING
# Stop sampling
STOP_SAMPLING
# Get status
GET_STATUS
Sampling cycle:

Fan: 2 minutes (air circulation)

Pump: 4 minutes (gas sampling)

Repeat: Continuous cycle

🧠 Edge Impulse Integration
Setup Edge Impulse
Create Project

bash
edge-impulse-daemon
Configure Sensors

json
{
  "sensors": [
    {"name": "GMXXX_NO2", "units": "ppm"},
    {"name": "GMXXX_Ethanol", "units": "ppm"},
    {"name": "GMXXX_VOC", "units": "ppm"},
    {"name": "GMXXX_CO", "units": "ppm"},
    {"name": "MiCS5524_CO", "units": "ppm"},
    {"name": "MiCS5524_Ethanol", "units": "ppm"},
    {"name": "MiCS5524_VOC", "units": "ppm"}
  ]
}
API Configuration

Get API Key from Edge Impulse Dashboard

Set HMAC Key in device settings

Configure project ID

Data Format for Edge Impulse
python
{
  "protected": {
    "ver": "v1",
    "alg": "HS256",
    "iat": timestamp
  },
  "signature": "hmac_signature",
  "payload": {
    "device_name": "e-nose-arduino-01",
    "device_type": "ARDUINO_NANO",
    "interval_ms": 1000,
    "sensors": [...],
    "values": [no2, eth_gm, voc_gm, co_gm, co_mics, eth_mics, voc_mics]
  }
}
📈 InfluxDB Integration
Installation
bash
# Install InfluxDB
wget https://dl.influxdata.com/influxdb/releases/influxdb2-2.7.1-amd64.deb
sudo dpkg -i influxdb2-2.7.1-amd64.deb

# Start InfluxDB
sudo systemctl start influxdb
Configuration
Initial Setup

bash
influx setup
# Follow prompts to create:
# - Organization
# - Bucket
# - Admin user
# - Token
Python Client Setup

python
from influxdb_client import InfluxDBClient

client = InfluxDBClient(
    url="http://localhost:8086",
    token="your_token",
    org="your_org"
)
Data Writing Example

python
write_api = client.write_api(write_options=SYNCHRONOUS)

point = Point("sensor_data") \
    .tag("device", "e-nose-01") \
    .field("no2", no2_value) \
    .field("co", co_value) \
    .field("voc", voc_value) \
    .time(datetime.utcnow())

write_api.write(bucket="e_nose_data", org="your_org", record=point)
Query Examples
python
# Query last hour of data
query = '''
from(bucket: "e_nose_data")
  |> range(start: -1h)
  |> filter(fn: (r) => r._measurement == "sensor_data")
  |> filter(fn: (r) => r._field == "no2" or r._field == "co")
'''

# Get averages
query = '''
from(bucket: "e_nose_data")
  |> range(start: -24h)
  |> filter(fn: (r) => r._measurement == "sensor_data")
  |> aggregateWindow(every: 1h, fn: mean, createEmpty: false)
  |> yield(name: "hourly_average")
'''
Dashboard Setup
Create Dashboard in InfluxDB UI

Add visualization widgets

Configure alerts for threshold breaches

Set up automated reports

🔧 API Documentation
Rust Backend Endpoints
Endpoint	Method	Description
/api/status	GET	System status and Arduino connection
/api/data	GET	Latest sensor data (JSON array)
/api/csv	GET	Export all data as CSV
/api/start	POST	Start monitoring
/api/stop	POST	Stop monitoring
/api/config	POST	Update configuration
Response Format
Status Endpoint:

json
{
  "arduino_connected": true,
  "current_level": 1,
  "current_state": 2,
  "data_points": 1250,
  "session_start": "2024-01-15T10:30:00Z"
}
Data Endpoint:

json
[
  {
    "timestamp": "2024-01-15T10:30:00Z",
    "no2": 0.123,
    "eth_gm": 4.567,
    "voc_gm": 2.345,
    "co_gm": 1.234,
    "co_m": 0.987,
    "eth_m": 3.456,
    "voc_m": 2.789
  }
]

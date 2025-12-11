E-NOSE Monitoring System
Sistem pemantauan gas lingkungan lengkap dengan visualisasi real-time, kontrol Arduino, integrasi ML Edge Impulse, dan penyimpanan data InfluxDB.

https://img.shields.io/badge/Platform-Python%2520%257C%2520Rust%2520%257C%2520Arduino-blue
https://img.shields.io/badge/Version-2.0.0-green
https://img.shields.io/badge/License-MIT-yellow

📋 Gambaran Umum
Sistem E-NOSE (Electronic Nose) adalah solusi pemantauan gas canggih yang menggabungkan berbagai teknologi sensor untuk analisis lingkungan yang akurat. Sistem ini menampilkan akuisisi data real-time, kemampuan machine learning, dan penyimpanan data komprehensif.

✨ Fitur Utama
📊 Monitoring Real-time: Visualisasi langsung 7 sensor gas

🎯 Kontrol Arduino Langsung: Komunikasi TCP dengan Arduino

🧠 Integrasi Edge Impulse: Pelatihan dan inferensi model machine learning

💾 Penyimpanan InfluxDB: Penyimpanan data time-series dan analitik

⚡ Backend Rust: Server pemrosesan data berkinerja tinggi

🖥️ UI Modern: Aplikasi desktop berbasis PyQt6 dengan plot real-time

📈 Ekspor Data: Ekspor CSV untuk analisis data

🏗️ Arsitektur Sistem
text
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Arduino Nano  │    │   Rust Backend  │    │  PyQt6 Frontend │
│  dengan Sensor  │────│  (Server HTTP)  │────│   (App GUI)     │
│                 │    │                 │    │                 │
│ • Seri GM-XXX   │    │ • Penyimpanan   │    │ • Plot Real-time│
│ • MiCS-5524     │    │ • Endpoint API  │    │ • Panel Kontrol │
│ • Aktuator      │    │ • Ekspor CSV    │    │ • Edge Impulse  │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                      │                       │
         │                      │                       │
         ▼                      ▼                       ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│  Edge Impulse   │    │    InfluxDB     │    │   Ekspor Data   │
│  Platform ML    │    │   Time Series   │    │     (CSV)       │
│                 │    │                 │    │                 │
│ • Pelatihan Model│   │ • Penyimpanan   │    │ • Excel/SPSS    │
│ • Deteksi Anomali│   │   Jangka Panjang│    │ • Analisis      │
│ • Klasifikasi    │   │ • Dashboard     │    │ • Laporan       │
└─────────────────┘    └─────────────────┘    └─────────────────┘
🔧 Instalasi Cepat
Prasyarat
bash
# Python 3.8 atau lebih tinggi
python --version

# Rust (untuk backend)
rustc --version

# Arduino IDE (untuk pemrograman mikrokontroler)
Langkah Instalasi
Clone repository

bash
git clone https://github.com/username/e-nose-system.git
cd e-nose-system
Instal dependensi Python

bash
pip install -r requirements.txt
Instal Rust dependencies

bash
cd rust-backend
cargo build --release
Konfigurasi Arduino

Upload arduino/enoise_controller.ino ke Arduino

Update alamat IP di kode Arduino

Hubungkan sensor sesuai konfigurasi pin

⚙️ Konfigurasi
1. Backend Rust
Edit file config.toml di folder rust-backend:

toml
[server]
ip = "192.168.1.146"
port = 8080

[arduino]
ip = "192.168.1.122"
port = 8082
baud_rate = 115200

[influxdb]
url = "http://localhost:8086"
token = "your_token_here"
org = "enose_org"
bucket = "sensor_data"
enable = true

[database]
path = "./data/sensor_data.db"
2. Frontend Python
Konfigurasi langsung di file main.py:

python
# Konfigurasi alamat IP
RUST_IP = "192.168.1.146"    # IP backend Rust
RUST_PORT = 8080             # Port HTTP backend

# Konfigurasi Arduino Direct Control
ARDUINO_IP = "192.168.1.122" # IP Arduino
ARDUINO_PORT = 8082          # Port Arduino

# Konfigurasi Edge Impulse
EI_API_KEY = "ei_xxxxx"      # API Key Edge Impulse
EI_PROJECT_ID = "819619"     # ID Project Edge Impulse
EI_HMAC_KEY = "xxxxx"        # HMAC Key untuk signature
3. Setup InfluxDB
Instalasi InfluxDB
Untuk Ubuntu/Debian:

bash
# Download dan instal InfluxDB
wget https://dl.influxdata.com/influxdb/releases/influxdb2-2.7.1-amd64.deb
sudo dpkg -i influxdb2-2.7.1-amd64.deb

# Mulai layanan InfluxDB
sudo systemctl start influxdb
sudo systemctl enable influxdb

# Setup awal
influx setup
# Ikuti prompt untuk membuat:
# - Organization
# - Bucket  
# - Admin user
# - Token
Untuk Docker:

bash
# Jalankan InfluxDB dengan Docker
docker run -d -p 8086:8086 \
  --name influxdb \
  -v influxdb2:/var/lib/influxdb2 \
  influxdb:2.7
Buat Token dan Bucket
Akses UI InfluxDB: http://localhost:8086

Load Data → API Tokens → Generate API Token

Pilih permissions:

Read/write access untuk bucket

Read access untuk telegraf configs

Simpan token dan masukkan ke konfigurasi Rust backend

🚀 Penggunaan
Memulai Sistem Lengkap
1. Jalankan InfluxDB
bash
# Jika menggunakan Docker
docker run -d -p 8086:8086 \
  --name influxdb \
  -v influxdb2:/var/lib/influxdb2 \
  -e DOCKER_INFLUXDB_INIT_MODE=setup \
  -e DOCKER_INFLUXDB_INIT_USERNAME=admin \
  -e DOCKER_INFLUXDB_INIT_PASSWORD=password123 \
  -e DOCKER_INFLUXDB_INIT_ORG=enose_org \
  -e DOCKER_INFLUXDB_INIT_BUCKET=sensor_data \
  influxdb:2.7

# Cek status
docker logs influxdb
2. Jalankan Backend Rust
bash
cd rust-backend
cargo run --release

# Atau dengan konfigurasi khusus
RUST_LOG=info cargo run --release -- --config config/production.toml
3. Jalankan Frontend Python
bash
python main.py

# Mode debug
python -m main --debug
4. Power ON Arduino
Sambungkan Arduino ke power

Pastikan koneksi jaringan stabil

Sistem akan otomatis terhubung

Antarmuka Utama
https://docs/images/ui_screenshot.png

Panel Kontrol
Tombol	Fungsi	Deskripsi
📡 START MONITOR	Mulai akuisisi data	Memulai pembacaan data sensor
⏹ STOP MONITOR	Hentikan monitoring	Menghentikan pengumpulan data
📊 GET STATUS	Status sistem	Menampilkan status koneksi dan sensor
💾 EXPORT CSV	Ekspor data	Menyimpan data ke file CSV
🔗 Test Connection	Test konektivitas	Menguji semua koneksi sistem
🧪 Test EI	Test Edge Impulse	Menguji integrasi ML
START/STOP	Kontrol Arduino	Kontrol aktuator langsung
Kontrol Arduino Langsung
Sistem mendukung perintah TCP langsung ke Arduino:

python
# Mulai siklus sampling
START_SAMPLING
# Hentikan sampling
STOP_SAMPLING
# Dapatkan status
GET_STATUS
Siklus Sampling:

Kipas: 2 menit (sirkulasi udara)

Pompa: 4 menit (sampling gas)

Ulangi: Siklus berkelanjutan

🧠 Integrasi Edge Impulse
Setup Project Edge Impulse
Buat project baru di Edge Impulse

Tambahkan device dengan nama: e-nose-arduino-01

Generate API Key dan HMAC Key

Konfigurasi di Kode
python
# Di file main.py
EI_API_KEY = "ei_bf401cc81aec83ef5d47055abbb90a596ad1d26f1c5f4306"
EI_PROJECT_ID = "819619"
EI_HMAC_KEY = "5ac940e46d7acb60201701a7f6d3dff3"
Format Data untuk Edge Impulse
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
📈 Integrasi InfluxDB
Inisialisasi Client InfluxDB
Rust Backend:

rust
// Di rust-backend/src/influxdb.rs
use influxdb2::Client;

pub async fn init_client() -> Client {
    let url = "http://localhost:8086".to_string();
    let token = std::env::var("INFLUXDB_TOKEN").unwrap();
    let org = "enose_org".to_string();
    
    Client::new(url, org, token)
}
Python Frontend (opsional):

python
# Untuk query data dari InfluxDB langsung
from influxdb_client import InfluxDBClient

client = InfluxDBClient(
    url="http://localhost:8086",
    token="your_token",
    org="enose_org"
)
Menulis Data ke InfluxDB
rust
// Contoh write data di Rust
use influxdb2::models::DataPoint;
use influxdb2::Client;

async fn write_sensor_data(client: &Client, sensor_data: &SensorData) -> Result<(), Box<dyn std::error::Error>> {
    let point = DataPoint::builder("sensor_measurements")
        .tag("device", "e_nose_01")
        .tag("location", "lab_1")
        .field("no2", sensor_data.no2)
        .field("ethanol_gm", sensor_data.eth_gm)
        .field("voc_gm", sensor_data.voc_gm)
        .field("co_gm", sensor_data.co_gm)
        .field("co_mics", sensor_data.co_m)
        .field("ethanol_mics", sensor_data.eth_m)
        .field("voc_mics", sensor_data.voc_m)
        .timestamp(chrono::Utc::now())
        .build()?;
    
    client.write("sensor_data", std::iter::once(point)).await?;
    Ok(())
}
Query Data dari InfluxDB
Flux Query Examples:

flux
// 1. Data 1 jam terakhir
from(bucket: "sensor_data")
  |> range(start: -1h)
  |> filter(fn: (r) => r._measurement == "sensor_measurements")
  |> filter(fn: (r) => r._field == "no2" or r._field == "co_gm")

// 2. Rata-rata per jam
from(bucket: "sensor_data")
  |> range(start: -24h)
  |> filter(fn: (r) => r._measurement == "sensor_measurements")
  |> aggregateWindow(every: 1h, fn: mean, createEmpty: false)

// 3. Data dengan threshold
from(bucket: "sensor_data")
  |> range(start: -1h)
  |> filter(fn: (r) => r._measurement == "sensor_measurements")
  |> filter(fn: (r) => r._field == "no2")
  |> filter(fn: (r) => r._value > 0.5)  // Threshold 0.5 ppm
Dashboard InfluxDB
Akses UI: http://localhost:8086

Buat Dashboard baru

Tambahkan cell dengan query

Konfigurasi visualisasi (line chart, gauge, histogram)

Set alert untuk threshold tertentu

🔌 Setup Hardware
Komponen yang Dibutuhkan
Mikrokontroler: Arduino Nano 33 IoT

Sensor Gas:

GM-102B (NO₂) - Deteksi nitrogen dioksida

GM-302B (Ethanol) - Konsentrasi alkohol

GM-502B (VOC) - Senyawa organik volatil

GM-702B (CO) - Karbon monoksida

MiCS-5524 (CO, Ethanol, VOC) - Sensor alternatif

Aktuator:

Kipas DC 12V

Pompa Peristaltik

Modul Relay

Power Supply: DC 12V/5A

Enclosure: Box IP-rated untuk perlindungan

Diagram Kabel
text
Arduino Nano 33 IoT
├── A0 → GM-102B (NO₂) Output Analog
├── A1 → GM-302B (Ethanol) Output Analog  
├── A2 → GM-502B (VOC) Output Analog
├── A3 → GM-702B (CO) Output Analog
├── A4 → MiCS-5524 CO Output Analog
├── A5 → MiCS-5524 Ethanol Output Analog
├── A6 → MiCS-5524 VOC Output Analog
├── D2 → Relay 1 (Kontrol Kipas)
├── D3 → Relay 2 (Kontrol Pompa)
├── D4 → LED Status
├── 5V → VCC Sensor
└── GND → Ground Sensor
Kalibrasi Sensor
Kalibrasi Baseline:

bash
# Jalankan tanpa gas sampel
python calibrate.py --mode baseline --duration 300
Kalibrasi dengan Gas Referensi:

bash
# Gunakan gas referensi yang diketahui
python calibrate.py --mode reference --gas co --concentration 50
📁 Struktur Project
text
e-nose-system/
├── frontend/
│   ├── main.py              # Aplikasi PyQt6 utama
│   ├── edge_impulse.py      # Integrasi Edge Impulse
│   ├── requirements.txt     # Dependensi Python
│   └── config/
│       └── settings.json    # Konfigurasi frontend
├── rust-backend/
│   ├── src/
│   │   ├── main.rs          # Server HTTP
│   │   ├── influxdb.rs      # Integrasi InfluxDB
│   │   ├── arduino.rs       # Komunikasi Arduino
│   │   └── models.rs        # Model data
│   ├── Cargo.toml           # Dependensi Rust
│   └── config.toml          # Konfigurasi backend
├── arduino/
│   └── enose_controller.ino # Firmware Arduino
├── docs/
│   ├── setup.md             # Panduan setup
│   └── api.md               # Dokumentasi API
└── README.md                # File ini
🐳 Deployment dengan Docker
docker-compose.yml
yaml
version: '3.8'
services:
  influxdb:
    image: influxdb:2.7
    container_name: enose-influxdb
    ports:
      - "8086:8086"
    volumes:
      - influxdb-data:/var/lib/influxdb2
      - influxdb-config:/etc/influxdb2
    environment:
      - DOCKER_INFLUXDB_INIT_MODE=setup
      - DOCKER_INFLUXDB_INIT_USERNAME=admin
      - DOCKER_INFLUXDB_INIT_PASSWORD=securepassword
      - DOCKER_INFLUXDB_INIT_ORG=enose-org
      - DOCKER_INFLUXDB_INIT_BUCKET=sensor_data
      - DOCKER_INFLUXDB_INIT_ADMIN_TOKEN=your-super-secret-token

  rust-backend:
    build: ./rust-backend
    container_name: enose-backend
    ports:
      - "8080:8080"
    depends_on:
      - influxdb
    environment:
      - INFLUXDB_URL=http://influxdb:8086
      - INFLUXDB_TOKEN=your-super-secret-token
      - INFLUXDB_ORG=enose-org
      - INFLUXDB_BUCKET=sensor_data
    volumes:
      - ./config:/app/config

volumes:
  influxdb-data:
  influxdb-config:
Docker Build & Run
bash
# Build semua service
docker-compose build

# Jalankan services
docker-compose up -d

# Lihat logs
docker-compose logs -f

# Hentikan services
docker-compose down
🧪 Testing
Test Koneksi Edge Impulse
python
# Jalankan test koneksi
python -c "
from edge_impulse import EdgeImpulseIntegration
ei = EdgeImpulseIntegration()
print('Testing Edge Impulse connection...')
result = ei.test_connection()
print(f'Result: {result}')
"
Test InfluxDB Connection
python
# Test koneksi InfluxDB
from influxdb_client import InfluxDBClient

client = InfluxDBClient(url="http://localhost:8086", token="your_token")
health = client.health()
print(f"InfluxDB Health: {health.status}")
📊 Contoh Analisis Data
Analisis dengan Pandas
python
import pandas as pd
from influxdb_client import InfluxDBClient
import matplotlib.pyplot as plt

# Query data dari InfluxDB
client = InfluxDBClient(url="http://localhost:8086", token="your_token")
query = '''
from(bucket: "sensor_data")
  |> range(start: -24h)
  |> filter(fn: (r) => r._measurement == "sensor_measurements")
  |> pivot(rowKey: ["_time"], columnKey: ["_field"], valueColumn: "_value")
'''

df = client.query_api().query_data_frame(query)

# Statistik dasar
print("Statistik Sensor:")
print(df[['no2', 'ethanol_gm', 'voc_gm', 'co_gm']].describe())

# Visualisasi
fig, axes = plt.subplots(2, 2, figsize=(12, 8))
df['no2'].plot(ax=axes[0,0], title='NO₂ Concentration', color='red')
df['co_gm'].plot(ax=axes[0,1], title='CO Concentration', color='blue')
df['ethanol_gm'].plot(ax=axes[1,0], title='Ethanol Concentration', color='green')
df['voc_gm'].plot(ax=axes[1,1], title='VOC Concentration', color='orange')

plt.tight_layout()
plt.savefig('sensor_analysis.png')
plt.show()
🚨 Troubleshooting
Masalah Umum dan Solusi
Masalah	Kemungkinan Penyebab	Solusi
Arduino tidak terkoneksi	IP salah, Arduino tidak aktif	Periksa IP, restart Arduino
Edge Impulse error 401	API Key tidak valid	Generate API Key baru
InfluxDB connection failed	Token salah, bucket tidak ada	Periksa token dan buat bucket
Pembacaan sensor tidak stabil	Kalibrasi perlu, power supply	Kalibrasi ulang, periksa power
Latensi tinggi di plot	Data points terlalu banyak	Kurangi data points, tingkatkan interval
Log Files
bash
# Log aplikasi
tail -f ~/.logs/e-nose/frontend.log

# Log InfluxDB
docker logs influxdb

# Log sistem
journalctl -u influxdb
journalctl -u e-nose-backend
📄 API Documentation
Endpoints Rust Backend
Endpoint	Method	Deskripsi
/api/status	GET	Status sistem dan koneksi Arduino
/api/data	GET	Data sensor terbaru (JSON array)
/api/csv	GET	Ekspor semua data sebagai CSV
/api/start	POST	Mulai monitoring
/api/stop	POST	Hentikan monitoring
/api/config	POST	Update konfigurasi
Contoh Response
Status Endpoint:

json
{
  "arduino_connected": true,
  "current_level": 1,
  "current_state": 2,
  "data_points": 1250,
  "session_start": "2024-01-15T10:30:00Z",
  "influxdb_connected": true
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
    "voc_m": 2.789,
    "influxdb_saved": true
  }
]
🔒 Keamanan
Best Practices
Jangan hardcode credentials:

python
# Salah
API_KEY = "ei_xxxxx"

# Benar
import os
API_KEY = os.getenv('EDGE_IMPULSE_API_KEY')
Gunakan environment variables:

bash
# .env file
EDGE_IMPULSE_API_KEY=ei_xxxxx
INFLUXDB_TOKEN=your_token
ARDUINO_IP=192.168.1.122
Regular key rotation:

Rotasi token InfluxDB setiap 90 hari

Update API Key Edge Impulse jika diperlukan

📈 Metrik Kinerja
Throughput Data: 10 sampel/detik

Efisiensi Penyimpanan: ~1MB/jam (terkompresi)

Waktu Response: < 100ms untuk API calls

Refresh Rate UI: Interval 1 detik

Penggunaan Memory: < 500MB untuk sistem lengkap

🤝 Berkontribusi
Fork repository

Buat branch fitur (git checkout -b feature/amazing-feature)

Commit perubahan (git commit -m 'Add amazing feature')

Push ke branch (git push origin feature/amazing-feature)

Buat Pull Request

Development Setup
bash
# Install dev dependencies
pip install -r requirements-dev.txt

# Setup pre-commit hooks
pre-commit install

# Run tests
pytest tests/
📝 License
Proyek ini dilisensikan di bawah MIT License - lihat file LICENSE untuk detail.

🙏 Acknowledgments
Pabrikan sensor: Winsen, SGX Sensortech

Kontributor open source

Partner penelitian dan kolaborator

📞 Support
Untuk bantuan:

Cek troubleshooting guide

Cari issue yang ada di issues

Buat issue baru dengan informasi detail

⚠️ Catatan Penting: Sistem ini dirancang untuk tujuan penelitian dan edukasi. Selalu ikuti pedoman keselamatan ketika bekerja dengan gas dan peralatan listrik.

Terakhir Diperbarui: Januari 2024
Versi: 2.0.0
Maintainer: Tim E-NOSE Research

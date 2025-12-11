// ========================================
// E-NOSE ZIZU — FINAL 2025 
// SAMPLING CYCLE: KIPAS 2 MENIT -> POMPA 4 MENIT -> REPEAT
// ========================================

#include <WiFiS3.h>
#include <Wire.h>
#include "Multichannel_Gas_GMXXX.h"

// ==================== WIFI ====================
const char* ssid     = "WARKOP TERAS 5G";
const char* pass     = "nutrisari";
const char* RUST_IP  = "192.168.1.13";
const int   RUST_PORT = 8081;
WiFiClient rustClient;

// ✅ BARU: Python Direct Control
WiFiServer pythonServer(8082);  // Port untuk Python direct
WiFiClient pythonClient;

// ==================== SENSOR & MOTOR (SAMA) ====================
GAS_GMXXX<TwoWire> gas;
#define MICS_PIN    A1
#define RLOAD       820.0
#define VCC         5.0
float R0_mics = 100000.0;

const int PWM_KIPAS   = 10;
const int DIR_KIPAS_1 = 12;
const int DIR_KIPAS_2 = 13;
const int PWM_POMPA   = 11;
const int DIR_POMPA_1 = 8;
const int DIR_POMPA_2 = 9;

// ==================== FSM & TIMING (SAMA) ====================
enum State { IDLE, FAN_ON, PUMP_ON };
State currentState = IDLE;
unsigned long stateTime = 0;
bool samplingActive = false;

const unsigned long FAN_DURATION = 120000;    // 2 menit
const unsigned long PUMP_DURATION = 240000;   // 4 menit

unsigned long lastSend = 0;
unsigned long lastPrint = 0;

// ==================== MOTOR CONTROL (SAMA) ====================
void kipas(int speed, bool buang = false) {
  digitalWrite(DIR_KIPAS_1, buang ? LOW : HIGH);
  digitalWrite(DIR_KIPAS_2, buang ? HIGH : LOW);
  analogWrite(PWM_KIPAS, speed);
}

void pompa(int speed) {
  digitalWrite(DIR_POMPA_1, HIGH);
  digitalWrite(DIR_POMPA_2, LOW);
  analogWrite(PWM_POMPA, speed);
}

void stopAll() { 
  analogWrite(PWM_KIPAS, 0); 
  analogWrite(PWM_POMPA, 0); 
}

// ==================== MiCS-5524 (SAMA) ====================
float calculateRs() {
  int raw = analogRead(MICS_PIN);
  if (raw < 10) return -1;
  float Vout = raw * (VCC / 1023.0);
  if (Vout >= VCC || Vout <= 0) return -1;
  return RLOAD * ((VCC - Vout) / Vout);
}

float ppmFromRatio(float ratio, String gasType) {
  if (ratio <= 0 || R0_mics == 0) return -1;
  float ppm = 0.0;
  if (gasType == "CO")      ppm = pow(10, (log10(ratio) - 0.35) / -0.85);
  else if (gasType == "C2H5OH") ppm = pow(10, (log10(ratio) - 0.15) / -0.65);
  else if (gasType == "VOC")    ppm = pow(10, (log10(ratio) + 0.1) / -0.75);
  return (ppm >= 0 && ppm <= 5000) ? ppm : -1;
}

// ==================== SAMPLING CYCLE (SAMA) ====================
void startSampling() {
  if (!samplingActive) {
    samplingActive = true;
    currentState = FAN_ON;
    stateTime = millis();
    kipas(255);
    pompa(0);
    Serial.println("🔄 SAMPLING STARTED - LOOP: 2m KIPAS -> 4m POMPA -> REPEAT");
    Serial.println("🌀 KIPAS NYALA (2 MENIT)");
  }
}

void stopSampling() {
  samplingActive = false;
  currentState = IDLE;
  stopAll();
  Serial.println("⏹️ SAMPLING STOPPED");
}

void runSamplingCycle() {
  if (!samplingActive) return;
  
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - stateTime;
  
  switch (currentState) {
    case FAN_ON:
      if (elapsed >= FAN_DURATION) {
        currentState = PUMP_ON;
        stateTime = currentTime;
        kipas(0);
        pompa(255);
        Serial.println("💧 POMPA NYALA (4 MENIT)");
      }
      break;
      
    case PUMP_ON:
      if (elapsed >= PUMP_DURATION) {
        currentState = FAN_ON;
        stateTime = currentTime;
        kipas(255);
        pompa(0);
        Serial.println("🌀 KIPAS NYALA (2 MENIT)");
      }
      break;
  }
}

// ==================== ✅ BARU: PYTHON DIRECT CONTROL ====================
void checkPythonCommands() {
  // Terima koneksi baru dari Python
  WiFiClient newClient = pythonServer.available();
  if (newClient) {
    if (pythonClient && pythonClient.connected()) {
      pythonClient.stop();  // Stop previous connection
    }
    pythonClient = newClient;
    Serial.println("🎯 Python Connected for Direct Control");
  }
  
  // Baca commands dari Python
  if (pythonClient && pythonClient.connected() && pythonClient.available()) {
    String command = pythonClient.readStringUntil('\n');
    command.trim();
    Serial.print("🎯 DIRECT FROM PYTHON: ");
    Serial.println(command);
    executeCommand(command);
  }
  
  // Check jika client disconnected
  if (pythonClient && !pythonClient.connected()) {
    pythonClient.stop();
    Serial.println("🎯 Python Disconnected");
  }
}

void executeCommand(String command) {
  Serial.print("🎛️ Received: ");
  Serial.println(command);
  
  if (command == "START_SAMPLING") {
    startSampling();
  } 
  else if (command == "STOP_SAMPLING") {
    stopSampling();
  }
  else if (command == "KIPAS_ON") {
    kipas(255);
    samplingActive = false;
    Serial.println("✅ Kipas NYALA MANUAL");
  }
  else if (command == "KIPAS_OFF") {
    kipas(0);
    samplingActive = false;
    Serial.println("✅ Kipas MATI MANUAL");
  }
  else if (command == "POMPA_ON") {
    pompa(255);
    samplingActive = false;
    Serial.println("✅ Pompa NYALA");
  }
  else if (command == "POMPA_OFF") {
    pompa(0);
    samplingActive = false;
    Serial.println("✅ Pompa MATI");
  }
}

// ==================== ✅ BARU: WiFi STATUS CHECK ====================
void checkWiFiStatus() {
  Serial.println("\n=== WiFi Status ===");
  Serial.print("Status: ");
  switch(WiFi.status()) {
    case WL_IDLE_STATUS: Serial.println("IDLE"); break;
    case WL_NO_SSID_AVAIL: Serial.println("NO SSID"); break;
    case WL_SCAN_COMPLETED: Serial.println("SCAN COMPLETED"); break;
    case WL_CONNECTED: Serial.println("CONNECTED"); break;
    case WL_CONNECT_FAILED: Serial.println("CONNECT FAILED"); break;
    case WL_CONNECTION_LOST: Serial.println("CONNECTION LOST"); break;
    case WL_DISCONNECTED: Serial.println("DISCONNECTED"); break;
    default: Serial.println("UNKNOWN"); break;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("📡 IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("📶 Signal Strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    Serial.print("🔧 Subnet Mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("🚪 Gateway: ");
    Serial.println(WiFi.gatewayIP());
  } else {
    Serial.println("❌ WiFi NOT CONNECTED");
  }
  Serial.println("===================");
}

// ==================== SETUP ====================
void setup() {
  Serial.begin(9600);
  delay(1000);
  
  Serial.println("🚀 E-NOSE ZIZU Starting...");
  
  // Setup Motor Pins
  pinMode(DIR_KIPAS_1, OUTPUT); pinMode(DIR_KIPAS_2, OUTPUT); pinMode(PWM_KIPAS, OUTPUT);
  pinMode(DIR_POMPA_1, OUTPUT); pinMode(DIR_POMPA_2, OUTPUT); pinMode(PWM_POMPA, OUTPUT);
  stopAll();

  // Setup Sensors
  Wire.begin();
  gas.begin(Wire, 0x08);

  // Kalibrasi R0
  delay(2000);
  float Rs_air = calculateRs();
  if (Rs_air > 0) {
    R0_mics = Rs_air;
    Serial.print("🔧 R0 MiCS-5524 terukur: "); 
    Serial.print(R0_mics/1000.0, 2); 
    Serial.println(" kΩ");
  }

  // ==================== WiFi CONNECTION ====================
  Serial.println();
  Serial.println("📡 Connecting to WiFi...");
  Serial.print("SSID: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(1000);
    Serial.print(".");
    attempts++;
    
    if (attempts % 10 == 0) {
      Serial.println();
      Serial.print("Attempt ");
      Serial.print(attempts);
      Serial.println("/30");
    }
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("✅ WiFi Connected Successfully!");
    
    // Print detailed WiFi info
    checkWiFiStatus();
    
  } else {
    Serial.println();
    Serial.println("❌ WiFi Connection FAILED!");
    Serial.println("Possible issues:");
    Serial.println("1. Wrong SSID/password");
    Serial.println("2. Weak signal");
    Serial.println("3. Router issues");
    Serial.println("Please check WiFi settings and restart Arduino manually");
    // Tidak restart otomatis untuk Uno R4 WiFi
  }
  
  // ✅ BARU: Start Python Direct Control Server
  pythonServer.begin();
  Serial.println("🎯 Python Direct Control ready on port 8082");
  Serial.println("===========================================");
  Serial.println("E-NOSE ZIZU SIAP — Tunggu START_SAMPLING");
  Serial.println("===========================================");
}

// ==================== LOOP ====================
void loop() {
  // Check commands dari Serial Monitor
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n'); cmd.trim();
    executeCommand(cmd);
  }

  // ✅ BARU: Check commands dari Python Direct
  checkPythonCommands();

  // ✅ BARU: Periodic WiFi status check
  static unsigned long lastWiFiCheck = 0;
  if (millis() - lastWiFiCheck >= 30000) {  // Setiap 30 detik
    lastWiFiCheck = millis();
    checkWiFiStatus();
  }

  // Kirim sensor data ke Rust
  if (millis() - lastSend >= 250) { 
    lastSend = millis(); 
    sendSensorData(); 
  }
  
  // Jalankan sampling cycle jika aktif
  runSamplingCycle();
}

// ==================== KIRIM DATA KE RUST ====================
void sendSensorData() {
  // GM-XXX
  float no2 = (gas.measure_NO2()  < 30000) ? gas.measure_NO2()  / 1000.0 : -1.0;
  float eth = (gas.measure_C2H5OH()< 30000) ? gas.measure_C2H5OH()/ 1000.0 : -1.0;
  float voc = (gas.measure_VOC()  < 30000) ? gas.measure_VOC()  / 1000.0 : -1.0;
  float co  = (gas.measure_CO()   < 30000) ? gas.measure_CO()   / 1000.0 : -1.0;

  // MiCS-5524
  float Rs = calculateRs();
  float co_mics = (Rs > 0) ? ppmFromRatio(Rs / R0_mics, "CO") : -1.0;
  float eth_mics = (Rs > 0) ? ppmFromRatio(Rs / R0_mics, "C2H5OH") : -1.0;
  float voc_mics = (Rs > 0) ? ppmFromRatio(Rs / R0_mics, "VOC") : -1.0;

  // Format sesuai Main.rs
  int current_state = 0;
  int current_level = samplingActive ? 1 : 0;
  
  if (samplingActive) {
    current_state = (currentState == FAN_ON) ? 1 : 2;
  }

  String data = "SENSOR:";
  data += String(no2,3) + "," + String(eth,3) + "," + String(voc,3) + "," + String(co,3) + ",";
  data += String(co_mics,3) + "," + String(eth_mics,3) + "," + String(voc_mics,3) + ",";
  data += String(current_state) + "," + String(current_level);

  // Kirim ke Rust
  if (rustClient.connect(RUST_IP, RUST_PORT)) {
    rustClient.print(data + "\n");
    rustClient.stop();
    // Serial.println("📡 Data sent to Rust");  // Uncomment untuk debug
  } else {
    Serial.println("❌ Failed to connect to Rust backend");
  }
}
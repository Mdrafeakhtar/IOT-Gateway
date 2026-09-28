# 🌐 IoT Gateway — ESP32 Multi-Node Monitoring & Web Dashboard

> **An ESP32-based IoT Gateway for collecting, processing, displaying, and remotely monitoring data from multiple distributed sensor nodes over Wi-Fi.**

![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![Language](https://img.shields.io/badge/Language-Embedded%20C%2FC%2B%2B-orange)
![Communication](https://img.shields.io/badge/Communication-UART%20%7C%20I2C-green)
![Network](https://img.shields.io/badge/Network-Wi--Fi-brightgreen)
![Dashboard](https://img.shields.io/badge/Dashboard-Web%20Server-purple)

## 📌 Project Overview

This project implements a **multi-node IoT Gateway using ESP32**. The gateway acts as a central embedded controller that receives sensor information from distributed nodes, tracks node communication status, presents information on a **16×2 I2C LCD**, and provides a browser-based monitoring dashboard over Wi-Fi.

The project demonstrates practical **embedded systems, IoT gateway, UART communication, networking, sensor acquisition, web-server, and real-time monitoring** concepts.

## 🏗️ System Architecture

```text
 Node 1 ──┐
 Node 2 ──┤
 Node 3 ──┼── UART ──> ESP32 IoT Gateway ──> Wi-Fi ──> Web Dashboard
 Node 4 ──┘                  │
                            └──────────────> 16×2 I2C LCD
```

### Node Configuration

| Node | ID | Data |
|---|---:|---|
| Node 1 | `0x10` | Temperature + Humidity |
| Node 2 | `0x20` | Temperature + Humidity |
| Node 3 | `0x30` | Motion |
| Node 4 | `0x40` | Temperature + Humidity |

## ✨ Key Features

- 🔌 Four-node IoT monitoring architecture
- 📡 UART-based node communication
- 🌡️ Temperature and humidity monitoring
- 🚶 Motion monitoring
- 🟢 Node online/offline detection
- 📟 16×2 I2C LCD local display
- 📶 ESP32 Wi-Fi connectivity
- 🌐 Embedded web dashboard
- 🔐 Login-protected dashboard
- 📊 JSON sensor-data endpoint
- 🔄 Automatic dashboard data refresh
- ⚡ Edge-side data processing
- 🧩 Expandable architecture for additional nodes

## 🔄 Data Flow

```text
Sensors
   ↓
Remote Sensor Nodes
   ↓
UART Packets
   ↓
ESP32 Gateway
   ├── Validate Data
   ├── Identify Node
   ├── Update Node Status
   ├── Update LCD
   └── Update Web Dashboard
             ↓
       JSON Sensor Data
             ↓
       Browser Dashboard
```

## 🧠 Gateway Operation

The ESP32 continuously performs:

1. Initialize GPIO, UART and I2C.
2. Connect to the configured Wi-Fi network.
3. Start the embedded web server.
4. Receive packets from sensor nodes.
5. Identify the transmitting node.
6. Update the latest sensor values.
7. Record the node's last communication time.
8. Update local LCD information.
9. Serve data to the web dashboard.
10. Detect nodes that exceed the communication timeout.
11. Repeat continuously.

## ⏱️ Node Health Monitoring

Each node has a **last-seen timestamp**.

```text
Packet received
      ↓
Update last-seen time
      ↓
Compare current time
with timeout threshold
      ↓
 ┌───────────────┐
 │ Within limit? │
 └───────┬───────┘
      Yes│   │No
         ↓   ↓
      ONLINE OFFLINE
```

This prevents stale readings from being treated as current data.

## 🛠️ Hardware

- ESP32 development board
- 16×2 I2C LCD
- Distributed sensor nodes
- Temperature/humidity sensors
- Motion sensor
- UART communication interface
- Wi-Fi network

## 🔧 ESP32 Pin Configuration

| Function | GPIO |
|---|---:|
| UART RX | `GPIO 16` |
| UART TX | `GPIO 17` |
| I2C SDA | Board/configured SDA |
| I2C SCL | Board/configured SCL |

> Verify the I2C pins and electrical levels for your specific ESP32 board before deployment.

## 💻 Software Stack

| Layer | Technology |
|---|---|
| Controller | ESP32 |
| Firmware | Arduino Framework |
| Language | Embedded C/C++ |
| Local Communication | UART |
| Display Communication | I2C |
| Network | Wi-Fi |
| Web Server | ESP32 WebServer |
| Frontend | HTML / CSS / JavaScript |
| Data Format | JSON |
| IDE | Arduino IDE |

## 📚 Libraries

Typical libraries used by the firmware include:

```cpp
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LiquidCrystal_I2C.h>
```

Install the **ESP32 board package by Espressif Systems** through Arduino IDE's Boards Manager.

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/Mdrafeakhtar/IOT-Gateway.git
cd IOT-Gateway
```

### 2. Open the Firmware

Open:

```text
Final_IOT_Gateway.ino
```

in Arduino IDE.

### 3. Configure Wi-Fi

Set your local network credentials:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

### 4. Configure Dashboard Password

Use a strong password locally:

```cpp
const char* loginPass = "CHANGE_THIS_ADMIN_PASSWORD";
```

**Never commit real credentials to a public repository.**

### 5. Select ESP32 Board

In Arduino IDE:

```text
Tools → Board → ESP32 Arduino → Your ESP32 Board
```

Select the correct serial port.

### 6. Upload

Click **Upload** and wait for the firmware to finish uploading.

### 7. Monitor Serial Output

Open:

```text
Tools → Serial Monitor
```

Use the baud rate configured by the firmware.

### 8. Open Dashboard

After the ESP32 connects to Wi-Fi, open its IP address from a device on the same network:

```text
http://ESP32_IP_ADDRESS
```

## 🌐 Web Dashboard

The ESP32 hosts a lightweight web application directly from the microcontroller.

The dashboard is designed to show:

- Node 1 temperature/humidity
- Node 2 temperature/humidity
- Node 3 motion status
- Node 4 temperature/humidity
- Online/offline status
- Live sensor information
- Automatic data refresh

Conceptual layout:

```text
┌──────────────────────────────────────────┐
│          IoT GATEWAY DASHBOARD           │
├──────────────────────────────────────────┤
│                                          │
│ NODE 1       NODE 2       NODE 3         │
│ Online       Online       Online         │
│ 28.4 °C      29.1 °C      Motion: YES   │
│ 65 % RH      61 % RH                     │
│                                          │
│ NODE 4                                   │
│ Online                                   │
│ 27.8 °C / 67 % RH                        │
│                                          │
└──────────────────────────────────────────┘
```

## 📊 JSON Data Interface

The gateway provides sensor information through a JSON endpoint for browser-side updates and future integrations.

Example structure:

```json
{
  "node1": {
    "temperature": 28.4,
    "humidity": 65
  },
  "node2": {
    "temperature": 29.1,
    "humidity": 61
  },
  "node3": {
    "motion": true
  },
  "node4": {
    "temperature": 27.8,
    "humidity": 67
  }
}
```

The actual response depends on the firmware implementation and received node packets.

## 📟 LCD Interface

The local 16×2 I2C LCD provides gateway/node information without requiring a computer or phone.

Example:

```text
Node 1
T:28.4C H:65%
```

or:

```text
Node 3
Motion: YES
```

The exact display sequence is controlled by the firmware.

## 🔐 Security

The public GitHub version uses placeholders:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* loginPass = "CHANGE_THIS_ADMIN_PASSWORD";
```

For real deployment:

- Use strong unique credentials.
- Never commit Wi-Fi passwords.
- Never publish production credentials.
- Avoid exposing the ESP32 directly to the public Internet.
- Use HTTPS/TLS for production systems.
- Consider secure secret storage and stronger authentication.

## 🧪 Testing Checklist

### Hardware

- [ ] ESP32 powers correctly
- [ ] LCD initializes
- [ ] UART wiring verified
- [ ] Node IDs configured
- [ ] Sensor nodes transmit packets
- [ ] Wi-Fi connection established

### Firmware

- [ ] Node 1 data received
- [ ] Node 2 data received
- [ ] Node 3 motion data received
- [ ] Node 4 data received
- [ ] Online/offline detection verified
- [ ] LCD output verified
- [ ] Web server starts
- [ ] Dashboard login works
- [ ] JSON endpoint works

### Network

- [ ] ESP32 receives an IP address
- [ ] Dashboard opens on the local network
- [ ] Live data refresh works
- [ ] Gateway remains stable during continuous operation

## 🧯 Troubleshooting

### ESP32 cannot connect to Wi-Fi

Check:

```text
SSID
Password
2.4 GHz Wi-Fi availability
ESP32 power supply
```

### LCD is blank

Check:

```text
SDA
SCL
VCC
GND
I2C address
```

Common LCD addresses include:

```text
0x27
0x3F
```

### Node stays OFFLINE

Check:

```text
UART TX/RX
Common GND
Node ID
Baud rate
Packet format
Power supply
Timeout configuration
```

### Dashboard does not open

Check the ESP32 IP address in Serial Monitor and make sure the phone/computer and ESP32 are connected to the same network.

## 📈 Engineering Concepts Demonstrated

### Embedded Systems
- ESP32 firmware development
- GPIO
- UART
- I2C
- Real-time monitoring
- Embedded debugging

### IoT
- Multi-node architecture
- Gateway design
- Sensor data acquisition
- Wi-Fi networking
- Node health monitoring
- Edge processing

### Software
- C/C++
- HTML
- CSS
- JavaScript
- JSON
- Embedded HTTP server

### System Design
- Distributed sensing
- Data aggregation
- Local visualization
- Remote monitoring
- Fault/status detection

## 📁 Project Structure

```text
IOT-Gateway/
│
├── Final_IOT_Gateway.ino
├── README.md
├── .gitignore
│
└── docs/
    └── screenshots/
```

## 📸 Screenshots & Demo

Add actual project screenshots to:

```text
docs/screenshots/
```

Recommended screenshots:

```text
dashboard.png
hardware.jpg
lcd-display.jpg
node-setup.jpg
serial-monitor.png
```

Then add them to the README:

```markdown
![IoT Gateway Dashboard](docs/screenshots/dashboard.png)
```

## 🔮 Future Improvements

- [ ] MQTT integration
- [ ] HTTPS/TLS
- [ ] Cloud database
- [ ] Historical sensor graphs
- [ ] SD-card data logging
- [ ] OTA firmware updates
- [ ] Mobile application
- [ ] Email/SMS alerts
- [ ] Automatic node discovery
- [ ] CRC/checksum validation
- [ ] Configurable node registration
- [ ] Role-based authentication
- [ ] NTP time synchronization
- [ ] ESP32 NVS configuration storage
- [ ] Remote device management

## 💼 Resume Description

**IoT Gateway — ESP32 Multi-Node Monitoring System**

> Designed and developed an ESP32-based IoT gateway for multi-node sensor data acquisition using UART, with Wi-Fi connectivity, 16×2 I2C LCD visualization, node health monitoring, JSON data serving, authentication, and a browser-based real-time monitoring dashboard.

### Technologies

`ESP32` `Embedded C/C++` `UART` `I2C` `Wi-Fi` `IoT` `WebServer` `JSON` `HTML` `CSS` `JavaScript`

## 👨‍💻 Author

**Md Rafe Akhtar**

Embedded Systems & IoT Engineer

GitHub: https://github.com/Mdrafeakhtar

## ⭐ Support

If you find this project useful for learning **ESP32, embedded systems, or IoT gateway development**, consider giving the repository a ⭐.

## 📄 License

No open-source license has been selected yet. Add an appropriate license before allowing unrestricted reuse or redistribution.

# IoT Gateway Pro

An ESP32-based IoT Gateway that collects data from multiple sensor nodes, displays node information on a 16x2 I2C LCD, and provides a browser-based monitoring dashboard over Wi-Fi.

## Features

- ESP32 IoT Gateway
- Multi-node sensor communication over UART
- Four-node monitoring architecture
- Temperature and humidity monitoring for Nodes 1, 2 and 4
- Motion monitoring for Node 3
- Online/offline node status detection
- 16x2 I2C LCD local display
- Wi-Fi web server
- Secure login page for dashboard access
- Live JSON sensor endpoint
- Responsive web dashboard
- Automatic dashboard data refresh
- UART communication with external nodes

## System Overview

```text
        Node 1 ──┐
        Node 2 ──┤
        Node 3 ──┼──> ESP32 IoT Gateway ──> Wi-Fi Dashboard
        Node 4 ──┘              │
                                └──> 16x2 I2C LCD
```

## Node Data

| Node | Data |
|---|---|
| Node 1 | Temperature + Humidity |
| Node 2 | Temperature + Humidity |
| Node 3 | Motion Status |
| Node 4 | Temperature + Humidity |

A node is considered online when valid data has been received within the configured timeout period.

## Hardware

- ESP32
- 16x2 I2C LCD
- External sensor nodes
- UART communication interface
- Wi-Fi network

## Pin Configuration

| Function | ESP32 Pin |
|---|---:|
| UART RX | GPIO 16 |
| UART TX | GPIO 17 |
| I2C LCD | Default ESP32 I2C pins |

## Node IDs

```text
0x10 → Node 1
0x20 → Node 2
0x30 → Node 3
0x40 → Node 4
```

## Arduino Libraries

Install/use the following libraries:

- Wire
- LiquidCrystal_I2C
- WiFi
- WebServer

`Wire`, `WiFi`, and `WebServer` are normally available with the ESP32 Arduino core.

## Setup

1. Install Arduino IDE.
2. Install the ESP32 board package.
3. Install the required libraries.
4. Open `Final_IOT_Gateway.ino`.
5. Configure your Wi-Fi credentials in the sketch.
6. Set a strong dashboard password.
7. Select your ESP32 board and COM/serial port.
8. Upload the firmware.
9. Open the Serial Monitor if debugging is required.
10. Connect to the IP address displayed on the LCD.
11. Log in to access the dashboard.

## Security

**Do not commit real Wi-Fi passwords or production credentials to GitHub.**

This repository intentionally uses placeholders:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* loginPass = "CHANGE_THIS_ADMIN_PASSWORD";
```

Replace them locally before flashing the ESP32.

## Dashboard

The web dashboard provides:

- Node 1 temperature/humidity
- Node 2 temperature/humidity
- Node 3 motion status
- Node 4 temperature/humidity
- Online/offline status for each node
- Automatic live data refresh

## Skills Demonstrated

- Embedded C/C++
- ESP32
- IoT Gateway Design
- UART Communication
- I2C
- Wi-Fi Networking
- WebServer on Microcontrollers
- HTML/CSS/JavaScript
- Sensor Data Acquisition
- Embedded Debugging
- Real-Time Monitoring
- Multi-node IoT Architecture

## Future Improvements

- Replace hard-coded credentials with a secure configuration system
- Add MQTT or HTTPS cloud connectivity
- Store historical sensor data
- Add charts and data logging
- Add role-based authentication
- Add OTA firmware updates
- Add configurable node registration

## License

Add a license appropriate for your project before distributing the code publicly.

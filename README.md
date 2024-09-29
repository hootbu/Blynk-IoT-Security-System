# ESP32 IoT Security System with Blynk

## Overview
This project demonstrates an IoT-based security system using an ESP32 board. It integrates RFID for access control, an MPU6050 accelerometer/gyroscope sensor, a servo motor, and an RGB LED for visual feedback. The system is controlled remotely via the **Blynk IoT Cloud** platform using a mobile interface.

## Features
- **RFID Access Control**: Authenticate users with RFID cards.
- **MPU6050 Sensor**: Monitor accelerometer and gyroscope data when access control is disabled.
- **Servo Motor**: Physically control access by rotating the motor when authentication is successful.
- **RGB LED Feedback**: Indicates system status (e.g., blue for idle, green for success, red for failure).
- **Blynk Control**: Control the system remotely, switch between RFID and sensor modes, and monitor data in real-time via the Blynk app.

## Hardware Components
- **ESP32 Board**: Main microcontroller.
- **RFID Module**: Used for wireless communication and authentication.
- **MPU6050 IMU Sensor**: Tracks accelerometer and gyroscope data.
- **Servo Motor**: Controls physical access (opens/closes).
- **RGB LED**: Displays system status.
- **Arduino UNO**: Used for power and additional I/O.
- **Jumper Wires & Breadboard**: For circuit connections.

## Circuit Diagram
- **ESP32**:  
  - SDA (RFID) -> D21  
  - SCK (RFID) -> D18  
  - MOSI (RFID) -> D23  
  - MISO (RFID) -> D19  
  - RST (RFID) -> D32  
  - SDA (MPU6050) -> D21  
  - SCL (MPU6050) -> D22  
  - RGB LED -> D25 (Red), D26 (Green), D27 (Blue)  
  - Servo Motor -> D13 (Control)

## Installation & Setup
### 1. Clone the repository:
- git clone https://github.com/yourusername/ESP32-IoT-Security-System
- cd ESP32-IoT-Security-System

### 2. Install Required Libraries:
- Blynk Library: Arduino IDE > Library Manager > Blynk
- RFID Library: Arduino IDE > Library Manager > MFRC522
- MPU6050 Library: Arduino IDE > Library Manager > MPU6050

### 3. Setup Blynk:
- Create a new project in the Blynk mobile app.
- Add widgets for button, slider, and sensor data.
- Define the components in Datastreams within the Blynk Console.

### 4. Upload the Code:
- Open ESP32_IoT_Security.ino in Arduino IDE.
- Configure your WiFi credentials and Blynk Auth Token in the code.
- Upload the code to the ESP32 board.

### Usage
- Power up the ESP32 and other components.
- Open the Blynk app and start the project.
- Use the button in the app to toggle between RFID Mode (controls access) and Sensor Mode (reads MPU6050 data).
- Monitor all data and control the servo via the Blynk interface.

### Demo
![image](https://github.com/user-attachments/assets/bf779868-401f-42f5-bb88-d76b0e9ea694)
> Fritzing Circuit Diagram

![image](https://github.com/user-attachments/assets/ddc50756-521f-4eb9-9ad9-53883b5613aa)
> Photo of the Circuit

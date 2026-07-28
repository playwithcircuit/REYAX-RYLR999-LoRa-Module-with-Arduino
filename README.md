# REYAX RYLR999 LoRa Module with Arduino

A practical guide to interfacing the **REYAX RYLR999 LoRa module** with Arduino Uno for long-range wireless communication.

![How to Interface Reyax RYLR999 LoRa Module with Arduino](https://playwithcircuit.com/wp-content/uploads/2025/12/How-to-Interface-Reyax-RYLR999-LoRa-Module-with-Arduino.webp)

This project demonstrates how to establish reliable point-to-point communication between two Arduino boards using the RYLR999 LoRa transceiver. One Arduino acts as the **Initiator**, periodically transmitting a message, while the second Arduino functions as the **Responder**, receives the packet, and sends an acknowledgement back. The project introduces LoRa communication, module configuration through AT commands, and UART-based interfacing with Arduino.

> Developed by **Play with Circuit** to help makers and embedded developers learn practical LoRa communication with Arduino.

---

# Introduction

Wireless communication plays an important role in modern IoT systems. Technologies such as Wi-Fi and Bluetooth are ideal for short-range applications but consume relatively high power and have limited operating distance. Cellular networks offer wide coverage but increase both hardware complexity and operating costs.

LoRa (Long Range) technology addresses these challenges by enabling wireless communication over several kilometres while consuming very little power. This makes it suitable for battery-operated IoT devices that transmit small amounts of data such as sensor readings, alarm notifications, GPS coordinates, and control commands.

In this project, two Arduino Uno boards communicate wirelessly using **REYAX RYLR999 LoRa modules**. The objective is to understand the module interface, configure communication using AT commands, and build a reliable point-to-point wireless link.

---

# About the REYAX RYLR999 Module

The **REYAX RYLR999 Lite** is a long-range wireless communication module that integrates both **LoRa** and **Bluetooth Low Energy (BLE)** into a compact package. The module supports long-distance LoRa communication over the **868 MHz** and **915 MHz** ISM bands while simultaneously offering Bluetooth connectivity through dedicated UART interfaces.

Unlike conventional LoRa modules, the RYLR999 provides separate UART channels for BLE and LoRa communication, allowing developers to independently communicate with each subsystem. The module is configured using AT commands, making it easy to change parameters such as network ID, device address, operating frequency, and other communication settings without modifying firmware.

One of the notable capabilities of the RYLR999 is its ability to function as a **BLE-to-LoRa bridge**, enabling mobile devices to exchange information across long distances through LoRa communication.

Because of its high output power, low standby current, and long communication range, the module is suitable for industrial monitoring, agriculture, environmental sensing, smart cities, logistics, and remote telemetry applications.

---

# Key Features

- Long-range LoRa communication up to several kilometres
- Integrated Bluetooth Low Energy (BLE)
- LoRa frequency bands: 868 MHz / 915 MHz
- LoRa output power up to +30 dBm (1 W)
- BLE output power up to +20 dBm
- UART interface for simple microcontroller integration
- Configurable through AT commands
- Ultra-low power sleep mode
- Operating temperature: -40°C to +85°C

---

# RYLR999 Pinout

The module consists of seven interface pins that provide power, reset, BLE UART, and LoRa UART communication.

![Reyax RYLR999 LoRa Module Pinout](https://playwithcircuit.com/wp-content/uploads/2025/12/RYLR999-Pinout.webp)


| Pin | Description |
|------|-------------|
| **VDD** | 5V power input (4.75V–5.25V) |
| **RST** | Active LOW reset input |
| **TXD_BLE** | BLE UART transmit output |
| **RXD_LoRa** | LoRa UART receive input for AT commands |
| **TXD_LoRa** | LoRa UART transmit output |
| **RXD_BLE** | BLE UART receive input |
| **GND** | Ground reference |

The LoRa UART interface is used throughout this project for communication with the Arduino Uno.

---

# Interfacing the RYLR999 with Arduino Uno

The project uses two identical hardware setups.

- **Initiator Arduino** – Periodically transmits a message through the LoRa module.
- **Responder Arduino** – Waits for incoming packets and replies with an acknowledgement.

Communication between the Arduino and the RYLR999 takes place through the UART interface. Since the RYLR999 operates using **3.3V logic levels**, a **bidirectional voltage level shifter** is used between the Arduino Uno and the LoRa UART pins to safely convert the Arduino's 5V signals.

The Arduino sends AT commands to configure and transmit data through the module, while received LoRa packets are returned through the UART interface for processing.

This simple point-to-point setup provides an excellent foundation before implementing larger LoRa-based sensor networks or industrial monitoring systems.

---

# Hardware Requirements

- Arduino Uno R3 ×2
- REYAX RYLR999 Module ×2
- 3.3V ↔ 5V Bidirectional Logic Level Shifter ×2
- 16×2 I2C LCD (optional for transmitter)
- Jumper Wires
- Breadboards
- USB Cable
- 12V Adapter (for Arduino supply)

---

# Wiring Connections

## Initiator Circuit

![Wiring Connections for the Initiator Setup](https://playwithcircuit.com/wp-content/uploads/2025/12/Wiring-Connections-for-the-Initiator-Setup.webp)

The Initiator Arduino sends commands to the RYLR999 module and displays the communication status on a 16×2 I2C LCD.

The module's **VDD** pin is connected to the Arduino's 5V supply, while **GND** is connected to the common ground. Communication between the Arduino and the LoRa UART passes through a bidirectional voltage level shifter to convert the Arduino's 5V UART signals into safe 3.3V logic levels required by the RYLR999.

The LCD is connected through the Arduino's I2C interface using the SDA and SCL pins, allowing transmitted messages and communication status to be displayed.

---

## Responder Circuit

![Wiring Connections for the Responder Setup](https://playwithcircuit.com/wp-content/uploads/2025/12/Wiring-RYLR999-Module-with-Arduino-UNO-Receiving-Side.webp)

The Responder Arduino continuously monitors incoming LoRa packets through its RYLR999 module.

The wiring is identical to the Initiator setup except that no LCD is required. The Arduino receives the incoming packet through the LoRa UART interface, processes the received command, and immediately transmits a response back through the same module.

---

# How This Project Works

After powering both Arduino boards, each communicates with its respective RYLR999 module through the UART interface.

The Initiator periodically sends a text message such as **"Are you there?"** using an AT command. The LoRa module modulates the data using **Chirp Spread Spectrum (CSS)** modulation and transmits it over the selected LoRa frequency.

When the Responder receives the packet, the RYLR999 demodulates the signal and forwards the received data to the Arduino through UART. The Arduino verifies the received message and responds with **"Yes"**, which is transmitted back through the second LoRa module.

This bidirectional communication demonstrates reliable long-range wireless data exchange and provides the foundation for developing applications such as remote sensor monitoring, industrial automation, agricultural systems, and IoT devices.

---

## Applications

- Industrial IoT
- Smart Agriculture
- Environmental Monitoring
- Remote Sensor Networks
- Asset Tracking
- Smart Metering
- Building Automation
- Wireless Telemetry

---

## About Play with Circuit

**Play with Circuit** publishes practical tutorials on Arduino, ESP32, sensors, communication protocols, and embedded systems to help students, hobbyists, and engineers build real-world electronics projects.

# Hardware Setup Guide

This guide will walk you through the process of setting up the hardware for your BPM & SpO₂ monitoring project.

## Components You'll Need

*   **ESP32 or ESP32-S3 Microcontroller:** This is the "brain" of our project.
*   **MAX30102 Pulse Oximeter & Heart Rate Sensor:** This sensor will measure your heart rate and blood oxygen levels.
*   **SSD1306 OLED Display (128x64):** This screen will show the live data from the sensor.
*   **Jumper Wires:** To connect all the components.
*   **Breadboard:** To make it easy to connect everything without soldering.

## Wiring Diagram

Here's how you'll connect the components:

| ESP32 Pin | Component Pin    |
| :-------- | :--------------- |
| `GND`     | `GND` (All)      |
| `3V3`     | `VIN` (All)      |
| `22`      | `SCL` (All)      |
| `21`      | `SDA` (All)      |

**Note:** All the components in this project use the I2C communication protocol, which means they can all share the same `SDA` and `SCL` pins.

## Step-by-Step Instructions

1.  **Mount the ESP32:** Place the ESP32 on the breadboard.
2.  **Connect the MAX30102 Sensor:**
    *   Connect `VIN` to the `3V3` pin on the ESP32.
    *   Connect `GND` to the `GND` pin on the ESP32.
    *   Connect `SCL` to pin `22` on the ESP32.
    *   Connect `SDA` to pin `21` on the ESP32.
3.  **Connect the OLED Display:**
    *   Connect `VIN` to the `3V3` pin on the ESP32.
    *   Connect `GND` to the `GND` pin on the ESP32.
    *   Connect `SCL` to pin `22` on the ESP32.
    *   Connect `SDA` to pin `21` on the ESP32.

Once everything is connected, your hardware setup is complete!

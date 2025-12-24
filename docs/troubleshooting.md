# Troubleshooting Guide

This guide will help you with some common issues you might encounter.

## Hardware Issues

*   **No data on the OLED screen:**
    *   Check your wiring. Make sure all the connections are secure and in the right place.
    *   Make sure the ESP32 is powered on.
*   **"NO FINGER" message on the OLED screen:**
    *   The MAX30102 sensor is very sensitive to movement. Make sure your finger is placed firmly on the sensor and you're not moving.
*   **Unstable readings:**
    *   Try a different finger.
    *   Make sure you're in a relaxed state.

## Firebase Issues

*   **Data not appearing in the database:**
    *   Check your Wi-Fi credentials in the `bpm_spo2_esp32.ino` file.
    *   Check your Firebase API Key and Database URL in the `bpm_spo2_esp32.ino` file.
    *   Make sure your Firebase Realtime Database is in "test mode".

## Dashboard Issues

*   **Data not appearing on the dashboard:**
    *   Check your Firebase credentials in the `dashboard.html` and `trends.html` files.
    *   Make sure you're logged in to the dashboard.
*   **"No valid data available" message on the trends page:**
    *   Make sure the ESP32 has been running for a while and has had a chance to upload some data.

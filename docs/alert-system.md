# Educational Alert System Guide

This guide explains how the educational alert system works and how to configure it.

## What is the Alert System?

The alert system is a feature that demonstrates how you can use the data from your ESP32 to trigger notifications. It's designed to be an educational tool and is **not a medical alert system**.

## How it Works

1.  **Configurable Thresholds:** The system uses thresholds that you can set in Firebase to define a "normal" range for BPM and SpO₂.
2.  **Anomaly Detection:** The web dashboard continuously monitors the data in Firebase. If a reading falls outside the normal range, it's considered an "anomaly".
3.  **Email Alerts:** When an anomaly is detected, the dashboard will send an email to a pre-configured address.

## Step-by-Step Configuration

1.  **Set Thresholds in Firebase:**
    *   In your Firebase Realtime Database, create a new node called `config`.
    *   Inside `config`, create a child node called `thresholds`.
    *   Add the following key-value pairs to the `thresholds` node:
        *   `bpm_max`: `120`
        *   `bpm_min`: `50`
        *   `spo2_min`: `92`
    *   You can change these values to whatever you like for your demonstration.
2.  **Configure EmailJS:**
    *   Sign up for a free account at [EmailJS](https://www.emailjs.com/).
    *   **Create a new Email Service:** This will connect EmailJS to your email account (e.g., Gmail).
    *   **Create a new Email Template:** This will be the template for the email that gets sent. You can use the following variables in your template:
        *   `{{device_id}}`: The ID of the ESP32.
        *   `{{anomaly_type}}`: The type of anomaly (e.g., "Low BPM").
        *   `{{anomaly_value}}`: The value that triggered the alert.
        *   `{{timestamp}}`: The time the anomaly occurred.
        *   `{{disclaimer}}`: A non-medical disclaimer.
    *   **Update the Dashboard:**
        *   Open `web-dashboard/trends.html` in a text editor.
        *   Find the following lines and replace the placeholders with your EmailJS credentials:
            ```javascript
            emailjs.init("YOUR_USER_ID");

            // ...

            emailjs.send('YOUR_SERVICE_ID', 'YOUR_TEMPLATE_ID', templateParams)
            ```

Your alert system is now configured!

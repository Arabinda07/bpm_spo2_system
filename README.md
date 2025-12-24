# BPM & SpO₂ IoT Monitoring System (Educational Project)

## 📌 Project Overview
This project is an **educational IoT system** designed for school students (Classes 6–8) to understand how **heart rate (BPM)** and **oxygen saturation (SpO₂)** signals can be sensed, visualised, and analysed using electronics and software.

The system is built using an **ESP32 / ESP32-S3**, a **MAX30102 optical sensor**, and a **web-based dashboard** powered by Firebase.

⚠️ **Important:**
This project is **NOT a medical device**.
All readings are indicative and intended **only for learning and demonstration purposes**.

---

## 🎯 Educational Objectives
Students learn:
- Basics of **biomedical signal sensing** (PPG)
- Use of **microcontrollers (ESP32)**
- Real-time data transmission using **Wi-Fi**
- Cloud data storage using **Firebase**
- Data visualisation using **Chart.js**
- Ethical boundaries in health-related technology

---

## 🧠 System Architecture (High Level)

ESP32 + MAX30102
→ Wi-Fi
→ Firebase Realtime Database
→ Web Dashboard (Live + Trends + Analysis)

---

## 🔧 Hardware Components
- ESP32 / ESP32-S3 microcontroller
- MAX30102 Pulse Oximeter & Heart Rate Sensor
- OLED Display (128×64)
- Buzzer (for non-medical alerts)
- USB power source

---

## 💻 Software Components
- Arduino IDE (ESP32 firmware)
- Firebase Realtime Database
- HTML / CSS / JavaScript
- Chart.js (for graphs)
- EmailJS (for educational alert demonstration)

---

## 📊 Dashboard Features
- Live BPM & SpO₂ display
- Session timer
- 1-day and 7-day trend charts
- Qualitative (non-medical) feedback
- Educational advisory alerts (email demonstration)
- UI optimised for projector & judge readability

---

## 🚨 Educational Alert System
The system includes an educational alert feature to demonstrate how anomaly detection can be implemented in IoT systems. This feature is for **demonstration purposes only** and is not a medical alert system.

### How it Works
1.  **Configurable Thresholds:** The anomaly detection logic is based on configurable thresholds for BPM and SpO₂. These thresholds are stored in the Firebase Realtime Database.
2.  **Anomaly Detection:** The web dashboard fetches the 1-day and 7-day trend data and compares it against the configured thresholds.
3.  **Educational Alerts:** If a data point falls outside the "normal" range, an educational advisory alert is sent via email using EmailJS.

### Configuration
To configure the educational alert system, you need to:

1.  **Set Thresholds in Firebase:**
    *   In your Firebase Realtime Database, create a new node called `config`.
    *   Inside `config`, create a child node called `thresholds`.
    *   Add the following key-value pairs to the `thresholds` node:
        *   `bpm_max`: `120`
        *   `bpm_min`: `50`
        *   `spo2_min`: `92`
    *   You can adjust these values to suit your demonstration needs.

2.  **Configure EmailJS:**
    *   Sign up for a free account at [EmailJS](https://www.emailjs.com/).
    *   Create a new email service and a new email template.
    *   In `web-dashboard/trends.html`, find the following lines and replace the placeholders with your EmailJS credentials:
        ```javascript
        /* ================= EMAIL ALERTS ================= */
        // Initialize EmailJS with your User ID
        emailjs.init("YOUR_USER_ID");

        function sendAnomalyAlert(details) {
          // ...
          emailjs.send('YOUR_SERVICE_ID', 'YOUR_TEMPLATE_ID', templateParams)
          // ...
        }
        ```

---

## 🛡️ Ethics, Safety & Responsibility
- This system **does not diagnose** any medical condition
- No medical claims are made
- Alerts are **educational advisories only**
- All cloud credentials are kept **private** and replaced with placeholders in this repository
- Designed strictly for **learning, exhibitions, and demonstrations**

---

## 📁 Repository Structure
esp32-firmware/ → ESP32 Arduino code
web-dashboard/ → HTML, CSS, JavaScript files
docs/ → Documentation & screenshots
README.md → Project documentation


---

## 🚀 How to Use (Educational Setup)
1. Replace placeholder Wi-Fi and Firebase values with your own
2. Upload the firmware to ESP32 using Arduino IDE
3. Open the web dashboard files in a browser
4. Observe live and trend-based data visualisation

> ⚠️ Cloud setup instructions are intentionally high-level for safety.

---

## 👩‍🏫 Intended Audience
- School students (Classes 6–10)
- STEM / ATL labs
- CBSE / State-level science exhibitions
- Teachers demonstrating IoT + data concepts

---

## 📜 License
License will be added after final review.

---

## 🙏 Acknowledgement
This project was developed as part of an educational initiative to promote **hands-on STEM learning** and responsible innovation.

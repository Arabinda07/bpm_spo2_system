# BPM & SpO₂ IoT Monitoring System (Educational Project)

## 📌 Project Overview
This project is an **educational IoT system** designed for school students (Classes 6–8) to understand how **heart rate (BPM)** and **oxygen saturation (SpO₂)** signals can be sensed, visualised, and analysed using electronics and software.

The system is built using an **ESP32 / ESP32-S3**, a **MAX30102 optical sensor**, and a **web-based dashboard** powered by Firebase.

⚠️ **Important:**
This project is **NOT a medical device**.
All readings are indicative and intended **only for learning and demonstration purposes**.

---

## 🚀 Getting Started

To get started with this project, please refer to the following guides:

*   [Hardware Setup](./docs/setup-hardware.md)
*   [Firebase Setup](./docs/setup-firebase.md)
*   [Dashboard Setup](./docs/setup-dashboard.md)
*   [Educational Alert System](./docs/alert-system.md)
*   [Troubleshooting](./docs/troubleshooting.md)

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

## 🛡️ Ethics, Safety & Responsibility
- This system **does not diagnose** any medical condition
- No medical claims are made
- Alerts are **educational advisories only**
- All cloud credentials are kept **private** and replaced with placeholders in this repository
- Designed strictly for **learning, exhibitions, and demonstrations**

---

## 📁 Repository Structure
- **bpm_spo2_esp32/**: ESP32 Arduino code
- **web-dashboard/**: HTML, CSS, and JavaScript files for the web dashboard
- **docs/**: Detailed documentation and setup guides
- **README.md**: This file (project overview)

---

## 👩‍🏫 Intended Audience
- School students (Classes 6–10)
- STEM / ATL labs
- CBSE / State-level science exhibitions
- Teachers demonstrating IoT + data concepts


---

## 🙏 Acknowledgement
This project was developed as part of an educational initiative to promote **hands-on STEM learning** and responsible innovation.

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

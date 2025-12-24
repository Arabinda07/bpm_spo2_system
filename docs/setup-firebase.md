# Firebase Setup Guide

This guide will help you set up a Firebase Realtime Database for your project. Firebase is a cloud service that will store the data from your ESP32.

## What is Firebase?

Firebase is a platform from Google that helps you build web and mobile applications. We'll be using its **Realtime Database** feature to store the BPM and SpO₂ data from our ESP32.

## Step-by-Step Instructions

1.  **Create a Google Account:** If you don't have one already, you'll need a Google account to use Firebase.
2.  **Go to the Firebase Console:** Open your web browser and navigate to the [Firebase Console](https://console.firebase.google.com/).
3.  **Create a New Project:**
    *   Click on "Add project".
    *   Give your project a name (e.g., "Health Monitor").
    *   Accept the terms and conditions and click "Continue".
    *   You can disable Google Analytics for this project.
    *   Click "Create project".
4.  **Create a Realtime Database:**
    *   From the left-hand menu, go to "Build" > "Realtime Database".
    *   Click "Create Database".
    *   Choose a location for your database (the default is fine).
    *   Select "Start in **test mode**". This will allow your ESP32 to write data to the database without any special authentication.
    *   Click "Enable".
5.  **Get Your Database URL and API Key:**
    *   Your **Database URL** will be at the top of the Realtime Database page. It will look something like `https://your-project-name-default-rtdb.firebaseio.com`.
    *   To get your **API Key**, go to "Project settings" (the gear icon at the top of the left-hand menu).
    *   Under the "General" tab, you'll find your "Web API Key".
6.  **Update the Firmware:**
    *   Open the `bpm_spo2_esp32.ino` file in the Arduino IDE.
    *   Find the following lines and replace the placeholders with your credentials:
        ```c++
        #define API_KEY "YOUR_FIREBASE_API_KEY"
        #define DATABASE_URL "YOUR_FIREBASE_PROJECT"
        ```
    *   You'll also need to update the `FIREBASE_BASE_PATH` to match the path you want to use in your database. The default is `"/YOUR_READINGS_PATH"`.

Your Firebase setup is now complete!

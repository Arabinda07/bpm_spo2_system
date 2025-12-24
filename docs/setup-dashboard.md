# Dashboard Setup Guide

This guide will help you set up the web dashboard to visualize the data from your ESP32.

## What is the Dashboard?

The dashboard is a web page that connects to your Firebase Realtime Database and displays the BPM and SpO₂ data in real-time. It also provides a historical view of the data.

## Step-by-Step Instructions

1.  **Open the Dashboard Files:**
    *   Navigate to the `web-dashboard` directory in the project folder.
    *   You'll find several HTML files. The main ones are `index.html` (the login page), `dashboard.html` (the live dashboard), and `trends.html` (the historical trends).
2.  **Configure Firebase in the Dashboard:**
    *   You'll need to update the Firebase configuration in `dashboard.html` and `trends.html`.
    *   Open each file in a text editor and find the following lines:
        ```javascript
        firebase.initializeApp({
          apiKey: "YOUR_FIREBASE_API_KEY",
          authDomain: "AUTH_DOMAIN_ID",
          projectId: "PROJECT_ID",
          appId: "APP_ID"
        });
        ```
    *   Replace the placeholders with your Firebase project's credentials. You can find these in the Firebase Console under "Project settings" > "General".
3.  **Open the Dashboard in Your Browser:**
    *   Simply open the `index.html` file in your web browser.
    *   You'll be prompted to log in. Since we're in test mode, you can use any email and password.
    *   Once you're logged in, you'll be redirected to the live dashboard.

Your dashboard setup is now complete!

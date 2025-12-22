/**************************************************
 * BPM + SpO2 Trend Monitoring (CLOUD READY)
 * MCU: ESP32 / ESP32-S3 Mini
 * Sensor: MAX30102 (Reflectance)
 * Display: SSD1306 OLED 128x64
 **************************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30105.h"
#include <math.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <time.h>

// Helper headers for Firebase
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// ================= Wi-Fi ====================
#define WIFI_SSID "YOUR_WIFI_ID"
#define WIFI_PASS "WIFI_PASSWORD"

// ================= Firebase (Unauthenticated Device Mode) =================
#define API_KEY "YOUR_FIREBASE_API_KEY"
#define DATABASE_URL "YOUR_FIREBASE_PROJECT"

// Path MUST match dashboard
#define FIREBASE_BASE_PATH "/YOUR_READINGS_PATH"

FirebaseData fbdo;
FirebaseAuth auth;        // left empty on purpose
FirebaseConfig config;

// -------------------------------------------------------------------------

void setupFirebase() {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.signer.test_mode = true;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("✅ Firebase initialised ");
}


unsigned long lastFirebaseUpload = 0;
#define FIREBASE_UPLOAD_INTERVAL 20000  // 20 seconds

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ================= Sensor =================
MAX30105 sensor;

// ================= Finger Detection =================
#define FINGER_THRESHOLD 5000

// ================= DC / AC Thresholds =================
#define MIN_DC_IR   50000
#define MIN_DC_RED  60000
#define MIN_AC_IR   70
#define MIN_AC_RED  150
#define RED_AC_SCALE 0.5

// ================= Timing =================
#define MA_WINDOW 5
#define STABILISATION_TIME 3000
#define REFRACTORY_MS 500
#define IBI_MIN_MS 500
#define IBI_MAX_MS 2000
#define MIN_VALLEY_DELTA 40

// ================= SpO2 Configuration =================
#define SPO2_BEATS_REQUIRED 6
#define R_AVG_WINDOWS 3

// ================= SQI =================
enum SQI_Level { SQI_POOR, SQI_FAIR, SQI_GOOD };
SQI_Level sqi = SQI_POOR;

#define MIN_ACDC_IR   0.0005
#define MIN_ACDC_RED  0.0008
#define MAX_R_STD     0.20
#define MAX_BPM_STD   8

// ================= State =================
enum State { NO_FINGER, STABILISING, MEASURING };
State state = NO_FINGER;

// ================= Globals =================
unsigned long fingerPlacedTime = 0;
unsigned long lastBeatTime = 0;
bool timeSynced = false;
unsigned long long fallbackEpochMs = 0;
unsigned long long bootMillis = 0;

// ================= Signal Conditioning =================
float dcIR = 0, dcRED = 0;
const float DC_ALPHA = 0.95;

int maBuffer[MA_WINDOW];
int maIndex = 0;
long maSum = 0;

int prev2 = 0, prev1 = 0;

// ================= BPM =================
int bpmBuf[3] = {0, 0, 0};
int bpmPtr = 0;
int stableBPM = 0;
bool bpmValid = false;

// ================= SpO2 =================
float acIR_peak = -1e6, acIR_valley = 1e6;
float acRED_peak = -1e6, acRED_valley = 1e6;

float acIRsum = 0, acREDsum = 0;
float dcIRsum = 0, dcREDsum = 0;
int validBeatCount = 0;

float R_buffer[R_AVG_WINDOWS];
int R_index = 0;
bool R_buffer_full = false;

float R_hist[5];
int R_hist_idx = 0;
bool R_hist_full = false;

float bpm_hist[5];
int bpm_hist_idx = 0;
bool bpm_hist_full = false;

int lastSpO2 = -1;

void waitForTimeSync() {
  time_t now = 0;
  int retries = 0;

  Serial.println("⏳ Waiting for NTP time sync...");

  while (now < 1700000000 && retries < 40) {
    time(&now);
    delay(500);
    retries++;
  }

  if (now >= 1700000000) {
  timeSynced = true;
  bootMillis = millis();   // mark reference point
  Serial.println("✅ NTP time synced");
    }
 else {
    timeSynced = false;
    Serial.println("❌ NTP blocked, using fallback time");
  }
}

// Deduplication
int lastSentBPM = -1;
int lastSentSpO2 = -1;

// ================= Utility =================
int movingAverage(int x) {
  maSum -= maBuffer[maIndex];
  maBuffer[maIndex] = x;
  maSum += x;
  maIndex = (maIndex + 1) % MA_WINDOW;
  return maSum / MA_WINDOW;
}

int median3(int a, int b, int c) {
  if ((a >= b && a <= c) || (a >= c && a <= b)) return a;
  if ((b >= a && b <= c) || (b >= c && b <= a)) return b;
  return c;
}

float stddev(float *arr, int n) {
  float mean = 0;
  for (int i = 0; i < n; i++) mean += arr[i];
  mean /= n;
  float var = 0;
  for (int i = 0; i < n; i++)
    var += (arr[i] - mean) * (arr[i] - mean);
  return sqrt(var / n);
}

// ================= Time =================
unsigned long long getEpochMs() {
  time_t now;
  time(&now);

  // Case 1: NTP available → use pure epoch time
  if (now >= 1700000000) {
    return (unsigned long long)now * 1000ULL;
  }

  // Case 2: NTP unavailable → monotonic fallback
  if (bootMillis > 0) {
    return 1700000000000ULL + (millis() - bootMillis);
  }

  // Case 3: No valid time source
  return 0;
}


// ================= Firebase Upload =================
void uploadToFirebase() {
  if (WiFi.status() != WL_CONNECTED) return;
  if (!Firebase.ready()) return;

  bool valid =
    bpmValid &&
    stableBPM >= 40 && stableBPM <= 180 &&
    lastSpO2 >= 85 && lastSpO2 <= 100 &&
    (sqi == SQI_GOOD || sqi == SQI_FAIR);

  if (!valid) return;

  // Deduplicate
  if (abs(stableBPM - lastSentBPM) < 2 &&
      abs(lastSpO2 - lastSentSpO2) < 1) return;

  unsigned long now = millis();
  if (now - lastFirebaseUpload < FIREBASE_UPLOAD_INTERVAL) return;
  lastFirebaseUpload = now;
  
  unsigned long long ts = getEpochMs();

  // Reject invalid timestamps
    if (ts == 0) {
      Serial.println("⚠ No valid time source, skipping upload");
      return;
    }

  
  FirebaseJson json;
  json.set("bpm", stableBPM);
  json.set("spo2", lastSpO2);
  json.set("sqi", (int)sqi);
  json.set("timestamp", ts);

  String path = String(FIREBASE_BASE_PATH) + "/" + String(ts);

  if (Firebase.RTDB.setJSON(&fbdo, path.c_str(), &json)) {
    Serial.println("✅ Firebase upload OK");
    lastSentBPM = stableBPM;
    lastSentSpO2 = lastSpO2;
  } else {
    Serial.print("❌ Firebase upload FAILED: ");
    Serial.println(fbdo.errorReason());
  }

}

// ================= OLED =================
void drawOLED() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(
    state == NO_FINGER ? "STATUS: NO FINGER" :
    state == STABILISING ? "STATUS: STABILISING" :
    bpmValid ? "STATUS: READY" : "STATUS: MEASURING"
  );

  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 14); display.print("BPM");
  display.setCursor(72, 14); display.print("SpO2");

  display.setTextSize(3);
  display.setCursor(0, 26);
  bpmValid ? display.print(stableBPM) : display.print("--");

  display.setCursor(72, 26);
  (sqi >= SQI_FAIR && lastSpO2 > 0) ? display.print(lastSpO2) : display.print("--");

  // Tiny WiFi Indicator
  display.setTextSize(1);
  display.setCursor(100, 0);
  display.print(WiFi.status() == WL_CONNECTED ? "W+" : "W-");

  display.display();
}

// ================= Setup =================
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("Connecting WiFi...");
  display.display();

  sensor.begin(Wire, I2C_SPEED_STANDARD);
  sensor.setup();
  sensor.setPulseAmplitudeIR(0x1F);
  sensor.setPulseAmplitudeRed(0x1F);
  sensor.setPulseAmplitudeGreen(0);

  // WiFi Connection with Notification
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("WiFi Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");
  
  display.clearDisplay();
  display.setCursor(0, 20);
  display.println("WiFi Connected!");
  display.println(WiFi.localIP());
  display.display();
  delay(1500);

  configTime(
  0, 0,
  "pool.ntp.org",
  "time.google.com",
  "time.nist.gov"
);
waitForTimeSync();


  // Firebase Config
 setupFirebase();

  for (int i = 0; i < MA_WINDOW; i++) maBuffer[i] = 0;
}

// ================= Loop =================
void loop() {
  long rawIR = sensor.getIR();
  long rawRED = sensor.getRed();
  bool fingerPresent = rawIR > FINGER_THRESHOLD;

  if (!fingerPresent) {
    state = NO_FINGER;
    bpmValid = false;
    lastSpO2 = -1;
    sqi = SQI_POOR;
    drawOLED();
    delay(40);
    return;
  }

  if (state == NO_FINGER) {
    state = STABILISING;
    fingerPlacedTime = millis();
    dcIR = rawIR;
    dcRED = rawRED;
    maSum = 0;
    maIndex = 0;
    prev1 = prev2 = 0;
    lastBeatTime = 0;
    validBeatCount = 0;
    acIRsum = acREDsum = dcIRsum = dcREDsum = 0;
    R_index = 0;
    R_buffer_full = false;
    R_hist_idx = 0;
    R_hist_full = false;
    bpm_hist_idx = 0;
    bpm_hist_full = false;
  }

  if (state == STABILISING) {
    if (millis() - fingerPlacedTime > STABILISATION_TIME) {
      state = MEASURING;
    }
    drawOLED();
    delay(40);
    return;
  }

  dcIR = DC_ALPHA * dcIR + (1 - DC_ALPHA) * rawIR;
  dcRED = DC_ALPHA * dcRED + (1 - DC_ALPHA) * rawRED;

  int acIR = rawIR - dcIR;
  int acRED = rawRED - dcRED;
  int filtIR = movingAverage(acIR);

  acIR_peak = max(acIR_peak, (float)acIR);
  acIR_valley = min(acIR_valley, (float)acIR);
  acRED_peak = max(acRED_peak, (float)acRED);
  acRED_valley = min(acRED_valley, (float)acRED);

  bool valleyDetected = false;
  unsigned long now = millis();

  if (prev2 > prev1 &&
      prev1 < filtIR &&
      abs(prev1) > MIN_VALLEY_DELTA &&
      (now - lastBeatTime) > REFRACTORY_MS) {
    valleyDetected = true;
  }

  prev2 = prev1;
  prev1 = filtIR;

  if (valleyDetected) {
    if (lastBeatTime > 0) {
      unsigned long ibi = now - lastBeatTime;
      if (ibi >= IBI_MIN_MS && ibi <= IBI_MAX_MS) {
        int bpm = 60000 / ibi;
        bpmBuf[bpmPtr] = bpm;
        bpmPtr = (bpmPtr + 1) % 3;

        if (bpmBuf[0] && bpmBuf[1] && bpmBuf[2]) {
          stableBPM = median3(bpmBuf[0], bpmBuf[1], bpmBuf[2]);
          bpmValid = true;
          bpm_hist[bpm_hist_idx++] = stableBPM;
          if (bpm_hist_idx >= 5) {
            bpm_hist_idx = 0;
            bpm_hist_full = true;
          }
        }

        float AC_IR = acIR_peak - acIR_valley;
        float AC_RED = (acRED_peak - acRED_valley) * RED_AC_SCALE;

        bool beatOK =
          AC_IR >= MIN_AC_IR &&
          AC_RED >= MIN_AC_RED &&
          dcIR >= MIN_DC_IR &&
          dcRED >= MIN_DC_RED;

        if (beatOK) {
          acIRsum += AC_IR;
          acREDsum += AC_RED;
          dcIRsum += dcIR;
          dcREDsum += dcRED;
          validBeatCount++;

          if (validBeatCount >= SPO2_BEATS_REQUIRED) {
            float R_window = (acREDsum / dcREDsum) / (acIRsum / dcIRsum);
            R_buffer[R_index++] = R_window;
            if (R_index >= R_AVG_WINDOWS) { R_index = 0; R_buffer_full = true; }

            float R_avg = 0;
            int count = R_buffer_full ? R_AVG_WINDOWS : R_index;
            for (int i = 0; i < count; i++) R_avg += R_buffer[i];
            R_avg /= count;

            R_hist[R_hist_idx++] = R_avg;
            if (R_hist_idx >= 5) { R_hist_idx = 0; R_hist_full = true; }

            float acdcIR  = AC_IR / dcIR;
            float acdcRED = AC_RED / dcRED;
            float R_sd   = R_hist_full ? stddev(R_hist, 5) : 999;
            float bpm_sd = bpm_hist_full ? stddev(bpm_hist, 5) : 999;

            if (acdcIR > MIN_ACDC_IR && acdcRED > MIN_ACDC_RED && R_sd < MAX_R_STD && bpm_sd < MAX_BPM_STD) {
              sqi = SQI_GOOD;
            } else if (acdcIR > MIN_ACDC_IR / 2) {
              sqi = SQI_FAIR;
            } else {
              sqi = SQI_POOR;
            }

            float spo2f = 107.0 - 10.0 * R_avg;
            lastSpO2 = constrain((int)(spo2f + 0.5), 85, 100);

            Serial.print("BPM="); Serial.print(stableBPM);
            Serial.print(" | SpO2="); Serial.print(lastSpO2);
            Serial.print("% | SQI=");
            if (sqi == SQI_GOOD) Serial.println("GOOD");
            else if (sqi == SQI_FAIR) Serial.println("FAIR");
            else Serial.println("POOR");

            uploadToFirebase();

            acIRsum = acREDsum = dcIRsum = dcREDsum = 0;
            validBeatCount = 0;
          }
        }
      }
    }
    lastBeatTime = now;
    acIR_peak = acIR_valley = acRED_peak = acRED_valley = 0;
  }

  drawOLED();
  delay(40);
}
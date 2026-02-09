#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* firmwareUrl = "http://YOUR_FIRMWARE_FILE_SERVER_IP:8000/firmware.bin";

// 펌웨어 버전 정보 (수동 관리)
#define FIRMWARE_VERSION "v1.0.0"

void printDeviceInfo() {
  Serial.println("\n===== ESP32 Device Info =====");
  Serial.printf("Firmware Version : %s\n", FIRMWARE_VERSION);
  Serial.printf("Chip Model       : %s\n", ESP.getChipModel());
  Serial.printf("Chip Revision    : %d\n", ESP.getChipRevision());
  Serial.printf("Flash Size       : %d KB\n", ESP.getFlashChipSize() / 1024);
  Serial.printf("Free Heap        : %d bytes\n", ESP.getFreeHeap());
  Serial.printf("WiFi RSSI        : %d dBm\n", WiFi.RSSI());
  Serial.printf("Firmware URL     : %s\n", firmwareUrl);
  Serial.println("=============================\n");
}

void printHelp() {
  Serial.println("\n===== Serial Commands =====");
  Serial.println("'o' : Run OTA update & reboot");
  Serial.println("'i' : Print device info");
  Serial.println("'h' : Print help");
  Serial.println("'r' : Reboot ESP32");
  Serial.println("===========================\n");
}

void printProgress(size_t completed, size_t total) {
  Serial.printf("Progress: %d%%\r", (completed * 100) / total);
}

void performOTAUpdate() {
  WiFiClient client;
  HTTPClient http;

  Serial.println("[OTA] Connecting to update server...");
  if (http.begin(client, firmwareUrl)) {
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      int contentLength = http.getSize();

      if (contentLength > 0) {
        bool canBegin = Update.begin(contentLength);

        if (canBegin) {
          Serial.println("[OTA] Start updating...");
          WiFiClient* stream = http.getStreamPtr();
          size_t written = 0;
          size_t total = contentLength;
          uint8_t buff[1024];

          while (http.connected() && (written < total)) {
            size_t available = stream->available();
            if (available) {
              int bytesRead = stream->readBytes(buff, ((available > sizeof(buff)) ? sizeof(buff) : available));
              written += Update.write(buff, bytesRead);
              printProgress(written, total);
            }
            delay(1);
          }

          if (Update.end()) {
            Serial.println("\n[OTA] Update completed successfully!");
            if (Update.isFinished()) {
              Serial.println("[OTA] Rebooting...");
              ESP.restart();
            } else {
              Serial.println("[OTA] Update not finished? Something went wrong!");
            }
          } else {
            Serial.printf("[OTA] Update error: %s\n", Update.errorString());
          }

        } else {
          Serial.println("[OTA] Not enough space to begin OTA");
        }
      } else {
        Serial.println("[OTA] Content-Length not valid");
      }
    } else {
      Serial.printf("[OTA] HTTP GET failed, code: %d\n", httpCode);
    }
    http.end();
  } else {
    Serial.println("[OTA] Unable to connect to update server");
  }
}

void setup() {
  Serial.begin(115200);
  
  delay(500);
  while (Serial.available()) Serial.read();
  Serial.println("Cleared serial buffer after boot.");

  Serial.println("\n=== ESP32 OTA Update Boilerplate ===");

  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
  
  printDeviceInfo();
  printHelp();

}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    switch (cmd) {
      case 'o':
        Serial.println("[CMD] OTA Update & Reboot");
        performOTAUpdate();
        break;

      case 'i':
        Serial.println("[CMD] Print Device Info");
        printDeviceInfo();
        break;

      case 'r':
        Serial.println("[CMD] Rebooting...");
        ESP.restart();
        break;

      case 'h':
        Serial.println("[CMD] Print Help");
        printHelp();
        break;

      default:
        Serial.println("[CMD] Unknown command");
        break;
    }
  }
}
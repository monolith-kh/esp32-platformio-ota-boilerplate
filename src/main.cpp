#define HOSTNAME        "ESP32_OTA"
#define WIFI_SSID       "type ssid"
#define WIFI_PASSWORD   "type password" 

#define OTA_PORT        9811
#define OTA_PASSWORD    "password"

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

void WifiInit()
{
    log_i("Init Wifi");

    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(HOSTNAME);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    log_i("WiFi connected");
    log_i("IP address: ");
    Serial.println(IPAddress(WiFi.localIP()));
}

void OTAInit()
{
    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.setHostname(HOSTNAME);
    ArduinoOTA.setPassword(OTA_PASSWORD);

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else {  // U_FS
            type = "filesystem";
        }

        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
        }
    });
    ArduinoOTA.begin();
}

void setup(void)
{
    log_i("Hello RINGGGO by OTA");

    WifiInit();
    OTAInit();

}


void loop(void)
{
    ArduinoOTA.handle();
}

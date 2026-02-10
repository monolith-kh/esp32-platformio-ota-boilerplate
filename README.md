## 🧠 ESP32 OTA Demo (Using Update.h)
이 프로젝트는 ESP32에서 `Update.h`를 사용해
HTTP 서버로부터 펌웨어를 다운로드하고 업데이트하는 `OTA(Over-The-Air)` 데모 예제입니다.

시리얼 명령으로 직접 OTA를 실행하고, 기기 정보를 확인할 수 있습니다.

### 🚀 Features
- `Wi-Fi` 연결 후 명령어 입력으로 OTA 제어
- `Update.h`를 직접 사용하여 `Flash`에 펌웨어 기록
- OTA 완료 후 `자동 재부팅`
- 현재 펌웨어 버전 및 디바이스 정보 확인 가능

### 🧩 Serial Commands
| Command |	Description |
| -- | -- |
| `o`	| OTA 업데이트 실행 후 자동 리셋 |
| `i`	| 현재 ESP32 정보 출력 (펌웨어 버전 포함) |
| `r`	| ESP32 리셋 |


## ⚙️ Setup
1. Wi-Fi 정보 수정
    ``` cpp
    const char* ssid = "YOUR_WIFI_SSID";
    const char* password = "YOUR_WIFI_PASSWORD";
    ```
2. 펌웨어 URL 설정
    ``` cpp
    const char* firmwareUrl = "http://YOUR_FIRMWARE_FILE_SERVER_IP:8000/firmware.bin";
    ```
3. 버전 정보 관리
    ``` cpp
    #define FIRMWARE_VERSION "v1.0.0"
    ```

## 🧪 OTA Test Guide
1. PC에서 테스트용 HTTP 서버 실행 (8000 포트)
    ``` bash
    python3 -m http.server 8000
    ```
2. `FIRMWARE_VERSION` 변경하여 새롭게 빌드한 firmware.bin 파일을 같은 디렉토리에 둠
3. 시리얼 모니터에서 명령어 입력
    1. i → 현재 버전 확인
    2. o → OTA 업데이트 실행
4. OTA 완료 후 자동 리부트되며 새로운 펌웨어 실행
5. 다시 i 입력 → 버전이 변경되었는지 확인

### 📄 Example Serial Output
``` bash
Connecting to WiFi...
WiFi connected!
IP address: xxx.xxx.xxx.xxx

===== Serial Commands =====
'o' : Run OTA update & reboot
'i' : Print device info
'r' : Reboot ESP32
===========================

> i
Firmware Version : v1.0.0
Chip Model       : ESP32-S3
Flash Size       : 16384 KB
WiFi RSSI        : -52 dBm

> o
[OTA] Connecting to update server...
[OTA] Start updating...
Progress: 100%
[OTA] Update completed successfully!
[OTA] Rebooting...

```

## 🧰 Dependencies
- WiFi.h￼
- HTTPClient.h￼
- Update.h￼

## 🧑‍💻 Author
`KONHee` ESP32 OTA 실험용 프로젝트 🌞

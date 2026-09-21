# 🦯 Smart Blind Stick — IoT Assistive Device

> An Arduino-based assistive mobility device for visually impaired users,
> combining multi-directional obstacle detection, water hazard sensing,
> dual audio-haptic feedback, and an autonomous SOS emergency alert system.

---

## 📌 Overview

The standard white cane only detects obstacles on direct physical contact.
This project extends that with smart sensing and emergency features:

- ✅ **3-directional ultrasonic sensing** — front, left, right coverage
- ✅ **Water/puddle detection** at ground level
- ✅ **Dual feedback** — active buzzer + 2 vibration motors simultaneously
- ✅ **Autonomous SOS** — detects fall/trapped condition, plays Morse code SOS (no internet required)
- ✅ **GPS + Telegram alert** — sends caregiver a Google Maps link when WiFi is available

**Total hardware cost: ~$25 USD | All components off-the-shelf**

---

## 🛠️ Hardware Components

| Component | Qty | Pin |
|---|---|---|
| Arduino UNO R4 WiFi | 1 | — |
| HC-SR04 Ultrasonic Sensor | 3 | D2/D3, D4/D5, D6/D7 |
| NEO-6M GPS Module | 1 | D8 (TX), D9 (RX) |
| Active Buzzer | 1 | D11 |
| Vibration Motor Module | 2 | D12, D13 |
| Resistive Water Sensor | 1 | A0 |
| 9V Battery | 1 | DC barrel jack |

---

## 🔔 Alert Logic

| Distance | Buzzer | Vibration |
|---|---|---|
| > 60 cm | Silent | Off |
| ≤ 60 cm | 1800 Hz slow beep | Both ON |
| ≤ 30 cm | 2500 Hz fast beep | Both ON |
| Water detected | 3000 Hz rapid | Both ON |
| 🆘 All sensors < 15 cm | Morse SOS | Max continuous |

---

## 🚨 SOS Emergency Feature

When all 3 sensors simultaneously detect obstacles within 15 cm,
the system assumes the user has fallen or is trapped and:

1. Plays international **Morse code SOS** pattern `(· · · — — — · · ·)`
2. Activates **both vibration motors** continuously
3. If WiFi available → sends **GPS coordinates + Google Maps link** to caregiver via Telegram

> ⚡ The local buzzer + vibration response works with **no internet connection**.

---

## 📦 Libraries Required

Install via Arduino IDE → Library Manager:

---

## 🚀 Getting Started

### 1. Wire the components
Follow the pin table above. Connect all VCC pins to the 5V rail
and all GND pins to the GND rail on the breadboard.

### 2. Install libraries
In Arduino IDE → Tools → Manage Libraries → search and install `TinyGPSPlus`

### 3. Configure credentials (optional — for Telegram alerts)
In `smart_blind_stick.ino`, update these lines:

```cpp
const char* WIFI_SSID = "Thokar";
const char* WIFI_PASS = "your_password";
const String BOT_TOKEN = "your_telegram_bot_token";
const String CHAT_ID   = "your_chat_id";
```

### 4. Upload and test
- Select **Arduino UNO R4 WiFi** as board
- Select correct COM port
- Upload → Open Serial Monitor at **9600 baud**

---

## 📁 Repository Structure

---

## 📄 Research Paper

A full journal-format research paper documenting the design,
implementation, and results is included in the `/paper/` folder.

**Title:** Design and Implementation of an IoT-Enabled Smart Blind Stick
with Multi-Sensor Obstacle Detection and Emergency Alert System

**Authors:** Tamang Jay Kumar, Nur Alam

**Institution:** Kyungdong University (Global Campus), Republic of Korea

---

## 🎓 Academic Context

Built as a final project for the Department of Smart Computing,
Kyungdong University (Global Campus), Republic of Korea.

| | |
|---|---|
| **Author** | Tamang Jay Kumar |
| **Supervisor** | Prof. Nur Alam |
| **Institution** | Kyungdong University (Global Campus) |
| **Field** | Assistive Technology / IoT / Embedded Systems |

---

## 📜 License

MIT License — free to use, modify, and build upon.

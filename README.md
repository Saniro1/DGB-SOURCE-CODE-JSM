# DGB (Dual Gyro Button) 1000 Hz Firmware with Auto Touch Detection

Extracted and reconstructed source code from `Normal DGB Firmware (auto touch detection) (2).uf2`.

Designed for **Waveshare RP2040-Zero** (Raspberry Pi RP2040) paired with **Dual Bosch Sensortec BMI160** 6-axis IMUs and capacitive **Auto Touch Detection** for competitive gyro aiming with **JoyShockMapper**.

---

## ⚡ Features & Specifications

* **Dual BMI160 Sensor Fusion:** Dual 6-axis IMUs run concurrently over high-speed SPI0 at 1600 Hz ODR. Their signals are averaged in real time to drastically reduce uncorrelated sensor noise and thermal drift.
* **1000 Hz USB HID Reporting:** High-speed 1 ms interrupt polling (`bInterval = 1`) emulating a Sony DualShock 4 controller.
* **Auto Touch Detection (Touch-to-Aim):** Uses RC charge-time capacitive measurement between GP9 (Pulse) and GP12 (Foil). Resting your thumb/finger on the button enables the gyro; lifting your finger instantly disengages the gyro ("ratcheting").
* **Startup Zero-Rate Calibration:** Automatically averages 1000 resting samples on boot to eliminate stationary gyro drift and bias.
* **JoyShockMapper Native Support:** Seamless plug-and-play with JoyShockMapper using the DualShock 4 HID protocol.

---

## 🔌 Hardware Wiring & Pinout Matrix

### Waveshare RP2040-Zero

| Pin | Function | Connected To | Notes |
| :--- | :--- | :--- | :--- |
| **GP0** | SPI0 MISO (RX) | Sensor 1 & 2 SDO | Shared SPI MISO Data Line |
| **GP1** | SPI0 CS1 | Sensor 1 CS | Chip Select for Primary BMI160 |
| **GP2** | SPI0 SCK | Sensor 1 & 2 SCK | Shared SPI Clock Line |
| **GP3** | SPI0 MOSI (TX) | Sensor 1 & 2 SDI | Shared SPI MOSI Data Line |
| **GP5** | SPI0 CS2 | Sensor 2 CS | Chip Select for Secondary BMI160 |
| **GP9** | Touch Send | 1MΩ - 10MΩ Resistor | Drives pulse for capacitive RC timing |
| **GP12** | Touch Recv | Resistor + Touch Foil | Connect to conductive copper tape / button foil |
| **3V3** | Power (3.3V) | Sensor 1 & 2 VDD/VDDIO | 3.3V Power Rail |
| **GND** | Ground | Sensor 1 & 2 GND | Common Ground |

---

## 🛠️ How to Compile & Flash

### Prerequisites
1. Install [Arduino IDE](https://www.arduino.cc/en/software) (2.x recommended).
2. Add the **Raspberry Pi Pico / RP2040** board package by Earle Philhower:
   - Preferences -> Additional Boards Manager URLs:  
     `https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json`
   - Boards Manager -> Install **Raspberry Pi Pico/RP2040**.
3. Install **Adafruit TinyUSB** library via the Library Manager.

### Flashing via UF2
1. If you already have the compiled binary `Normal DGB Firmware (auto touch detection) (2).uf2`:
   - Hold the **BOOT** button on the RP2040-Zero and plug in USB-C (or double-tap RST).
   - Drag and drop `Normal DGB Firmware (auto touch detection) (2).uf2` onto the mounted `RPI-RP2` drive.
   - The board reboots immediately into 1000 Hz DualShock 4 mode.

---

## 🎮 JoyShockMapper Configuration

Add the following to your JoyShockMapper configuration file:

```ini
# Enable gyro aiming only when finger touches the DGB contact foil
GYRO_OFF = NONE
GYRO_ON = TOUCH
GYRO_SENS = 4.0
GYRO_DEADZONE = 0.10
GYRO_SMOOTH_TIME = 0.05
REAL_WORLD_CALIBRATION = 64.0
AUTO_CALIBRATE = YES
```

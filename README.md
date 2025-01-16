# IoT Conveyor Monitor for Counting and Fall Detection

This project aims to track objects on a conveyor belt while detecting potential collisions and falls using an IR sensor and a vibration sensor. The system sends alerts and counts the objects in real-time. It is designed to work with Arduino Mega and NodeMCU, and integrates seamlessly with a cloud-based service for remote monitoring.

## Requirements

### Hardware:

- **Arduino Mega**: This will act as the main controller for the system.
- **NodeMCU (ESP8266)**: Responsible for connecting the system to Wi-Fi and communicating with the cloud.
- **IR Sensor**: Used to count the objects passing through the conveyor.
- **Vibration Sensor**: Detects any vibrations that might indicate a fall or collision.

### Software:

- **Arduino IDE**: Used to write and upload code to the boards.
- **Cloud API service (e.g., ThingSpeak)**: For remote monitoring of the conveyor system.

### Libraries:

- `Wire.h`: For I2C communication between the boards (Arduino Mega and NodeMCU).
- `ESP8266WiFi.h`: To enable Wi-Fi connectivity on NodeMCU.
- `ESP8266HTTPClient.h`: To send data to the cloud server.

---

## Sensor Integration

1. **IR Sensor**:

   - The IR sensor detects objects passing through the conveyor belt. It is connected to the Arduino Mega on pin D2.
   - As objects pass through the IR sensor, the Arduino counts the objects by detecting interruptions in the IR beam.

2. **Vibration Sensor**:

   - The vibration sensor monitors any disturbances like falls or collisions on the conveyor. It is connected to pin D3 on the Arduino Mega.
   - If a vibration is detected, the system triggers a warning.

---

## Board Setup

### Arduino Mega:

1. **Connect the IR sensor** to pin D2.
2. **Connect the vibration sensor** to pin D3.
3. **Wire the Arduino Mega and NodeMCU using I2C**:
   - **SDA (Data)**: Connect to pin D20 of Arduino Mega to D1 of NodeMCU.
   - **SCL (Clock)**: Connect to pin D21 of Arduino Mega to D2 NodeMCU.
4. **Upload the code** from `Arduino_mega.ino` to the Arduino Mega using the Arduino IDE.

### NodeMCU:

1. **Connect the NodeMCU** to the computer via USB.
2. **Upload the code** from `Node_mcu.ino` to the NodeMCU using the Arduino IDE.

---

## Compilation and Uploading Code

1. **Install the ESP8266 board** in the Arduino IDE:

   - Go to **File > Preferences** and enter `http://arduino.esp8266.com/stable/package_esp8266com_index.json` in the Additional Boards Manager URLs field.
   - Go to **Tools > Board > Boards Manager** and install the ESP8266 package.

2. **Install libraries**:

   - Go to **Sketch > Include Library > Manage Libraries**.
   - Install the libraries: `Wire`, `ESP8266WiFi`, `ESP8266HTTPClient`.

3. **Upload the code**:

   - Open `Arduino_mega.ino` for the Arduino Mega.
   - Select **Tools > Board > Arduino Mega** and the appropriate port.
   - Upload the code.
   - Open `Node_mcu.ino` for the NodeMCU and upload it similarly.

---

## Cloud Setup

1. Sign up for a cloud service like [ThingSpeak](https://thingspeak.com/).
2. Create a new channel to store the data (e.g., count, warning).
3. Obtain the API key for your channel.
4. Replace the placeholders for `writeAPIKey` and `server` in `Node_mcu.ino` with your respective API key and server URL.

---

## Cloning the Repository

To get started with the project, you can clone this repository directly to your local machine using Git. Run the following command:

```bash
git clone https://github.com/khalidmk27/IoT-Conveyor-Monitor-for-Counting-and-Fall-detection.git

```

---

## Troubleshooting

- **Wi-Fi issues:** Ensure your NodeMCU is connected to a stable Wi-Fi network and that the credentials in the code are correct.

- **Sensor issues:** Double-check the wiring for the IR and vibration sensors and ensure they are properly connected to the correct pins on the Arduino Mega.

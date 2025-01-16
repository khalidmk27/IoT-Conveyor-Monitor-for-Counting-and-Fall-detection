#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Wi-Fi credentials
const char* ssid = "YOUR_WiFi_SSID"; //Add your Wifi Name
const char* password = "YOUR_WiFi_PASSWORD"; //Add your Wifi password

// cloud API details
const char* writeAPIKey = "API_KEY"; //Add your write API key of your cloud
const char* server = "API_SERVER";  //Add your API server eg: http://api.thingspeak.com

// Variables
int inputCount = 0;
int warning = 0;
unsigned long warningStartTime = 0;

void setup() {
  // Start Serial communication
  Serial.begin(9600);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");

  // Initialize I2C communication
  Wire.begin(4, 5);   // SDA=D2, SCL=D1 for NodeMCU ESP8266
}

void loop() {
  // Request data from Arduino Mega (I2C slave address 8)
  Wire.requestFrom(8, 32);  // Request up to 32 bytes from Arduino Mega
  String receivedData = "";
  
  // Read the received data from the I2C bus
  while (Wire.available()) {
    char c = Wire.read();  // Read each byte from Arduino Mega
    if (isPrintable(c)) {  // Ensure only printable characters are appended
      receivedData += c;
    }
  }

  // Process the received data
  if (receivedData.length() > 0) {
    Serial.println("Received: " + receivedData);
    
    // Check if received data matches "Count:%d"
    if (receivedData.startsWith("Count:")) {
    receivedData.trim(); // Clean up the data
    int count = receivedData.substring(6).toInt(); // Extract the count value
    if (count >= 0) { // Ensure count is non-negative
        inputCount = count;
        Serial.println("Updated Count: " + String(inputCount));
        uploadDataTocloud(inputCount, warning); // Upload count to cloud
    } else {
        Serial.println("Invalid count value received: " + receivedData);
    }
}
    
    // Check if the received data is "Warning!"
    if (receivedData == "Warning!") {
      warning = 1;
      warningStartTime = millis();  // Start the 20-second timer
      Serial.println("WARNING: Warning triggered!");
      uploadDataTocloud(inputCount, warning);  // Upload warning to cloud
    }
  }

  // Reset the warning after 20 seconds
  if (warning == 1 && (millis() - warningStartTime >= 20000)) {
    warning = 0;
    Serial.println("Warning reset.");
    uploadDataTocloud(inputCount, warning);  // Upload reset status to cloud
  }

  delay(1000);  // Delay before checking again
}

// Function to upload data to cloud
void uploadDataTocloud(int count, int warn) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "/update?api_key=" + writeAPIKey + 
                 "&field1=" + String(count) + 
                 "&field2=" + String(warn);
    WiFiClient client;  // Create a WiFiClient object
    http.begin(client, url);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("cloud Response: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error sending data: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }
    http.end();
  } else {
    Serial.println("Wi-Fi not connected. Reconnecting...");
    WiFi.begin(ssid, password);  // Reconnect to Wi-Fi
  }
}

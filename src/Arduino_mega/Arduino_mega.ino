#include <Wire.h>

// Pin assignments for sensors
const int IR_PIN = 2;           // IR sensor connected to D2
const int VIBRATION_PIN = A0;   // Vibration sensor connected to A0

// Variables for object counting
volatile unsigned int objectCount = 0;
volatile bool irState = false;
volatile unsigned long lastIrTime = 0;        // Timestamp for debounce
const unsigned long debounceDelay = 50;      // Debounce delay in milliseconds

// Variables for vibration detection
volatile bool vibrationWarning = false;
const int vibrationThreshold = 50;           // Threshold for triggering vibration warning
int previousVibrationValue = 0;              // Store previous vibration value

void setup() {
  // Initialize pins
  pinMode(IR_PIN, INPUT);

  // Attach interrupt for IR sensor
  attachInterrupt(digitalPinToInterrupt(IR_PIN), countObject, RISING);

  // Initialize hardware I2C for counts
  Wire.begin(8);                /* Join I2C bus with address 8 */
  Wire.onRequest(requestEvent); /* Register request event */

  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  // Monitor vibration sensor
  int currentVibrationValue = analogRead(VIBRATION_PIN); // Read analog input from vibration sensor

  // Check if there is a change in vibration value
  if (abs(currentVibrationValue - previousVibrationValue) > vibrationThreshold) {
    vibrationWarning = true;
    Serial.println("Vibration Warning triggered");
  }

  // Update previous value
  previousVibrationValue = currentVibrationValue;

  delay(100); // Allow some processing time
}

// Interrupt Service Routine for IR sensor
void countObject() {
  unsigned long currentTime = millis();

  // Implement debounce logic
  if (currentTime - lastIrTime > debounceDelay) {
    irState = !irState; // Toggle state
    if (irState) { // Count on a specific edge
      objectCount++;
      Serial.println(objectCount);
    }
    lastIrTime = currentTime; // Update debounce timestamp
  }
}

// Function to handle data transmission
void requestEvent() {
  char buffer[32]; // Buffer for transmitting data

  if (vibrationWarning) {
    snprintf(buffer, sizeof(buffer), "Warning!"); // Prepare warning message
    Serial.println("Vibration Warning sent");
    vibrationWarning = false; // Reset warning state after transmission
  } else {
    snprintf(buffer, sizeof(buffer), "Count:%d", objectCount); // Prepare count message
  }

  Wire.write(buffer, strlen(buffer)); /* Send only the valid data */
}

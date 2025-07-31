#include <BluetoothSerial.h>
#include "SSD1306.h" // OLED library

BluetoothSerial SerialBT;
SSD1306 display(0x3c, 5, 4);

int sensorPin = 36;
int sensorValue = 0;
String receivedText = "";

bool wasConnected = false;  // Track previous connection state

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT");
  Serial.println("Bluetooth started. Pair with 'ESP32_BT'");

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
}

void loop() {
  sensorValue = analogRead(sensorPin);

  // Check current connection state
  bool isConnected = SerialBT.hasClient();

  // If just connected (was disconnected before)
  if (isConnected && !wasConnected) {
    Serial.println("Client connected!");
    SerialBT.println("You are connected to the container");
  }
  wasConnected = isConnected; // Update state

  // Send sensor value only if connected
  if (isConnected) {
    SerialBT.println("Sensor: " + String(sensorValue));
  }

  // Read incoming Bluetooth data
  if (SerialBT.available()) {
    receivedText = SerialBT.readStringUntil('\n');
    Serial.println("Received: " + receivedText);
  }

  // Update OLED display
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setColor(WHITE);
  display.drawString(64, 0, "LDR: " + String(sensorValue));
  display.drawString(64, 25, receivedText);
  display.display();

  delay(1000);
}

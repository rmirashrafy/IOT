#include <BluetoothSerial.h>
#include "SSD1306.h" // OLED library (same as SSD1306Wire.h)

BluetoothSerial SerialBT;

// OLED display on I2C using pins SDA = GPIO5, SCL = GPIO4
SSD1306 display(0x3c, 5, 4);

int sensorPin = 36;   // LDR input pin
int sensorValue = 0;  // Variable to store analog value
String receivedText = ""; // Text received via Bluetooth

void setup() {
  // Serial Monitor
  Serial.begin(115200);
  
  // Start Bluetooth with name "ESP32_BT"
  SerialBT.begin("ESP32_BT");
  Serial.println("Bluetooth started. Pair with 'ESP32_BT'");

  // OLED setup
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16); // Smaller font to fit both lines
}

void loop() {
  // Read LDR sensor
  sensorValue = analogRead(sensorPin);

  // Send sensor value to Bluetooth
  SerialBT.println("Sensor: " + String(sensorValue));

  // Check if data is received via Bluetooth
  if (SerialBT.available()) {
    receivedText = SerialBT.readStringUntil('\n');
    Serial.println("Received: " + receivedText);
  }

  // Update OLED display
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setColor(WHITE);
  display.drawString(64, 0, "LDR: " + String(sensorValue));
  display.drawString(64, 25, receivedText); // Show received text
  display.display();

  delay(1000);
}

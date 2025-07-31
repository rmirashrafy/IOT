#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);        // Monitor Serial
  SerialBT.begin("ESP32_BT");  // Bluetooth device name
  Serial.println("Bluetooth started. Pair to 'ESP32_BT'");
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());  // Send PC input to Bluetooth
  }

  if (SerialBT.available()) {
    char incoming = SerialBT.read();   // Read from Bluetooth
    Serial.write(incoming);           // Show on Serial Monitor
  }
}

#include <HCSR04.h>
#include <ESP32Servo.h>
#include "SSD1306.h"
#include <BluetoothSerial.h>

// === Define Ultrasonic Sensor Pins ===
// Sensor 1
#define TRIG1_PIN   5
#define ECHO1_PIN   4

// Sensor 2
#define TRIG2_PIN   0
#define ECHO2_PIN   2

// Sensor 3 (controls the servo)
#define TRIG3_PIN   14
#define ECHO3_PIN   12

// === Servo and Button Pins ===
#define SERVO_PIN   25
#define BUTTON_PIN  13

// === Create Ultrasonic Sensor Objects ===
UltraSonicDistanceSensor sensor1(TRIG1_PIN, ECHO1_PIN);
UltraSonicDistanceSensor sensor2(TRIG2_PIN, ECHO2_PIN);
UltraSonicDistanceSensor sensor3(TRIG3_PIN, ECHO3_PIN);

// === Create OLED Display Object ===
SSD1306 display(0x3c, 5, 4);

// === Create Servo Object ===
Servo myServo;

// === Create Bluetooth Object ===
BluetoothSerial SerialBT;
bool wasConnected = false;  // Track previous connection state

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT");  // Bluetooth device name
  Serial.println("Bluetooth started. Pair with 'ESP32_BT'");

  // Servo setup
  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 1000, 2000);

  // Button setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // OLED setup
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);

  Serial.println("Setup complete");
}

void loop() {
  display.clear();

  // === Read distances ===
  float dist1 = sensor1.measureDistanceCm();
  float dist2 = sensor2.measureDistanceCm();
  float dist3 = sensor3.measureDistanceCm();

  // === Read button state ===
  bool buttonPressed = digitalRead(BUTTON_PIN) == LOW;

  // === Print to Serial ===
  Serial.print("Sensor 1: ");
  dist1 == -1.0 ? Serial.println("No echo") : Serial.println(String(dist1) + " cm");

  Serial.print("Sensor 2: ");
  dist2 == -1.0 ? Serial.println("No echo") : Serial.println(String(dist2) + " cm");

  Serial.print("Sensor 3 (with servo): ");
  dist3 == -1.0 ? Serial.println("No echo") : Serial.println(String(dist3) + " cm");

  Serial.print("Button state: ");
  Serial.println(buttonPressed ? "PRESSED" : "RELEASED");

  String Value;
  // === Servo Control ===
  if ((dist3 != -1.0 && dist3 < 7.0) || buttonPressed) {
    myServo.writeMicroseconds(1000); // Rotate
    Value = "Status: ON";
  } else {
    myServo.writeMicroseconds(1500); // Stop
    Value = "Status: Off";
  }

  // === Bluetooth Notification ===
  bool isConnected = SerialBT.hasClient();

  if (isConnected && !wasConnected) {
    Serial.println("Bluetooth client connected!");
    SerialBT.println("You are connected to the container");
  }
  wasConnected = isConnected;

  // === Send data via Bluetooth ===
  if (isConnected) {
    SerialBT.println("Sensor1: " + String(dist1));
    SerialBT.println("Sensor2: " + String(dist2));
    SerialBT.println("Sensor3: " + String(dist3));
    SerialBT.println(Value);
  }

  // === OLED Display ===
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, Value);
  display.setFont(ArialMT_Plain_15);
  display.display();

  delay(150);
}

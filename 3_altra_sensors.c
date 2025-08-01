#include <HCSR04.h>
#include <ESP32Servo.h>

// === Define Ultrasonic Sensor Pins ===
// Sensor 1
#define TRIG1_PIN   5// e.g., 15
#define ECHO1_PIN   4// e.g., 13

// Sensor 2
#define TRIG2_PIN   0//
#define ECHO2_PIN   2//

// Sensor 3 (this one controls the servo)
#define TRIG3_PIN   14//
#define ECHO3_PIN   12//

// === Define Servo Pin ===
#define SERVO_PIN   25// e.g., 25

// === Create Ultrasonic Sensor Objects ===
UltraSonicDistanceSensor sensor1(TRIG1_PIN, ECHO1_PIN);
UltraSonicDistanceSensor sensor2(TRIG2_PIN, ECHO2_PIN);
UltraSonicDistanceSensor sensor3(TRIG3_PIN, ECHO3_PIN);

// === Create Servo Object ===
Servo myServo;

void setup() {
  Serial.begin(115200);

  // Initialize servo
  myServo.setPeriodHertz(50);                // 50Hz PWM
  myServo.attach(SERVO_PIN, 1000, 2000);     // 1ms to 2ms PWM
}

void loop() {
  // === Read all distances ===
  float dist1 = sensor1.measureDistanceCm();
  float dist2 = sensor2.measureDistanceCm();
  float dist3 = sensor3.measureDistanceCm();  // servo sensor

  // === Print distances ===
  Serial.print("Sensor 1: ");
  dist1 == -1.0 ? Serial.println("No echo") : Serial.println(String(dist1) + " cm");

  Serial.print("Sensor 2: ");
  dist2 == -1.0 ? Serial.println("No echo") : Serial.println(String(dist2) + " cm");

  Serial.print("Sensor 3 (with servo): ");
  dist3 == -1.0 ? Serial.println("No echo") : Serial.println(String(dist3) + " cm");

  // === Servo Control Based on Sensor 3 ===
  if (dist3 == -1.0) {
    myServo.writeMicroseconds(1500); // Stop
  } else if (dist3 < 7.0) {
    myServo.writeMicroseconds(1000); // Rotate forward fast
  } else {
    myServo.writeMicroseconds(1500); // Stop
  }

  delay(100);
}

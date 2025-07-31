//sensor , servo
#include <HCSR04.h>
#include <ESP32Servo.h>

// Ultrasonic sensor pins
#define TRIG_PIN 15
#define ECHO_PIN 13

// Servo pin
#define SERVO_PIN 25

// Create sensor and servo objects
UltraSonicDistanceSensor distanceSensor(TRIG_PIN, ECHO_PIN);
Servo myServo;

void setup() {
  Serial.begin(115200);

  // Initialize servo
  myServo.setPeriodHertz(50);    // 50Hz PWM for servo
  myServo.attach(SERVO_PIN, 1000, 2000); // PWM range: 1000µs to 2000µs
}

void loop() {
  float distance = distanceSensor.measureDistanceCm();

  if (distance == -1.0) {
    Serial.println("No echo received");
    myServo.writeMicroseconds(1500); // stop
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance < 7) {
      myServo.writeMicroseconds(1000); // full forward
    } else {
      myServo.writeMicroseconds(1500); // stop
    }
  }

  delay(500);
}

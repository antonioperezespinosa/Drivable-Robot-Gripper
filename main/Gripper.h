#pragma once

#include <ESP32Servo.h>


class Gripper {
private:
    Servo servo;          // Servo object
    int servoPin;         // Servo signal pin

    int openPosition = 50;    // Open angle
    int closedPosition = 130; // Closed angle

    bool isOpen = true;   // Tracks gripper state

public:
    Gripper(int pin) : servoPin(pin) {}

    void begin() {
        servo.setPeriodHertz(50); // Standard servo frequency
        servo.attach(servoPin);   // Connect servo to pin
        servo.write(openPosition); // Start open
    }

    void toggle() {
        if (isOpen) {
            close();
        } else {
            open();
        }
    }

    void open() {
        servo.write(openPosition); // Move to open angle
        isOpen = true;
    }

    void close() {
        servo.write(closedPosition); // Move to closed angle
        isOpen = false;
    }

    void setOpenPosition(int angle) {
        openPosition = angle; // Change open angle
    }

    void setClosedPosition(int angle) {
        closedPosition = angle; // Change closed angle
    }

    void update() {
        // Future gripper logic goes here
    }
};
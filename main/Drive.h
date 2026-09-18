#pragma once

#include <Arduino.h>
#include <Motoron.h>

class Drive {
public:
    Drive(MotoronI2C& motoron)
        : motoron(motoron) {
    }

    void update(int x, int y) {
        int forward = -y;
        int turn = x;

        // Ignore small joystick drift
        if (abs(forward) < DEADZONE) {
            forward = 0;
        }

        if (abs(turn) < DEADZONE) {
            turn = 0;
        }

        // Arcade drive mixing
        int left = forward + turn;
        int right = forward - turn;

        // Keep both values inside the joystick range
        int largest = max(abs(left), abs(right));

        if (largest > 512) {
            left = left * 512 / largest;
            right = right * 512 / largest;
        }

        // Convert Bluepad32 range to Motoron range
        left = map(left, -512, 512, -800, 800);
        right = map(right, -512, 512, -800, 800);

        motoron.setSpeed(1, left);
        motoron.setSpeed(2, right);
    }

    void stop() {
        motoron.setSpeed(1, 0);
        motoron.setSpeed(2, 0);
    }

private:
    MotoronI2C& motoron;

    static const int DEADZONE = 40;
};
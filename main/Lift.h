#pragma once

#include <Motoron.h>

class Lift {
public:
    Lift(MotoronI2C& motoron)
        : motoron(motoron) {
    }

    void update(bool upPressed, bool downPressed) {

        if (upPressed && !downPressed) {
            motoron.setSpeed(3, LIFT_SPEED);
        }
        else if (downPressed && !upPressed) {
            motoron.setSpeed(3, -LIFT_SPEED);
        }
        else {
            stop();
        }
    }

    void stop() {
        motoron.setSpeed(3, 0);
    }

private:
    MotoronI2C& motoron;

    static const int LIFT_SPEED = 500;
};
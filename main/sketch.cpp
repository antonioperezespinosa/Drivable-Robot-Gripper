#include "sdkconfig.h"

#include <Arduino.h>
#include <Wire.h>
#include <Motoron.h>

#include "Controls.h"
#include "Gripper.h"
#include "Drive.h"
#include "Lift.h"


// ---------- Motor controller ----------

MotoronI2C motoron;


// ---------- Robot components ----------

Controls controls;
Gripper gripper(18);

Drive drive(motoron);
Lift lift(motoron);


void setup() {

    // Start I2C communication with Motoron
    // SDA = GPIO 21
    // SCL = GPIO 22
    Wire.begin(21, 22);


    // Initialize Motoron
    motoron.reinitialize();
    motoron.clearResetFlag();


    // Smooth drive acceleration
    motoron.setMaxAcceleration(1, 120);
    motoron.setMaxAcceleration(2, 120);


    // Smooth drive deceleration
    motoron.setMaxDeceleration(1, 200);
    motoron.setMaxDeceleration(2, 200);


    // Start controller and gripper
    controls.begin();
    gripper.begin();
}


void loop() {

    // Read controller
    controls.update();


    if (controls.connected()) {

        // ---------- Drive ----------

        drive.update(
            controls.leftX(),
            controls.leftY()
        );


        // ---------- Lift ----------

        lift.update(
            controls.held(Controls::Button::R1),
            controls.held(Controls::Button::L1)
        );


        // ---------- Gripper ----------

        if (controls.pressed(Controls::Button::Cross)) {
            gripper.toggle();
        }
    }

    else {

        // Stop DC motors if controller disconnects
        drive.stop();
        lift.stop();
    }


    // Short control-loop delay
    delay(10);
}
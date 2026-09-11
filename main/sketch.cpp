#include "sdkconfig.h"
#include <Arduino.h>
#include "Controls.h"
#include "Gripper.h"

// Robot components
Controls controls;
Gripper gripper(18);


void setup() {
    controls.begin();
    gripper.begin();
}


void loop() {

    controls.update();


    // ---------- Arbitration ----------

    if (controls.pressed(Controls::Button::Cross)) {
        gripper.toggle();
    }


    // ---------- Component updates ----------

    gripper.update();

    delay(10);
}
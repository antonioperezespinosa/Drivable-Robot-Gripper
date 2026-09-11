#pragma once

#include <Bluepad32.h>

class Controls {
public:

    // PlayStation button names
    enum class Button : uint16_t {
        Cross    = BUTTON_A,
        Circle   = BUTTON_B,
        Square   = BUTTON_X,
        Triangle = BUTTON_Y,
        L1       = BUTTON_SHOULDER_L,
        R1       = BUTTON_SHOULDER_R,
        L3       = BUTTON_THUMB_L,
        R3       = BUTTON_THUMB_R
    };


    void begin() {
        instance() = this;

        // Bluepad32 connection handling stays inside this class
        BP32.setup(
            &Controls::onConnected,
            &Controls::onDisconnected
        );

        BP32.enableVirtualDevice(false);
        BP32.enableBLEService(false);
    }


    void update() {
        pressedButtons = 0;

        // Get newest controller data
        if (!BP32.update() || !connected()) {
            return;
        }

        uint16_t currentButtons = controller->buttons();

        // Buttons that are down now, but were not down before
        pressedButtons = currentButtons & ~previousButtons;

        previousButtons = currentButtons;
    }


    // True once when the button is first pressed
    bool pressed(Button button) const {
        return pressedButtons & mask(button);
    }


    // True as long as the button is held
    bool held(Button button) const {
        return connected() &&
               (controller->buttons() & mask(button));
    }


    // Joysticks
    int leftX() const {
        return connected() ? controller->axisX() : 0;
    }

    int leftY() const {
        return connected() ? controller->axisY() : 0;
    }

    int rightX() const {
        return connected() ? controller->axisRX() : 0;
    }

    int rightY() const {
        return connected() ? controller->axisRY() : 0;
    }


    bool connected() const {
        return controller && controller->isConnected();
    }


private:

    ControllerPtr controller = nullptr;

    uint16_t previousButtons = 0;
    uint16_t pressedButtons = 0;


    static uint16_t mask(Button button) {
        return static_cast<uint16_t>(button);
    }


    // Gives Bluepad32 callbacks access to this Controls object
    static Controls*& instance() {
        static Controls* controls = nullptr;
        return controls;
    }


    static void onConnected(ControllerPtr ctl) {
        Controls* self = instance();

        // Only use one gamepad
        if (self &&
            self->controller == nullptr &&
            ctl->isGamepad()) {

            self->controller = ctl;
            self->previousButtons = 0;
        }
    }


    static void onDisconnected(ControllerPtr ctl) {
        Controls* self = instance();

        if (self && self->controller == ctl) {
            self->controller = nullptr;
            self->previousButtons = 0;
            self->pressedButtons = 0;
        }
    }
};
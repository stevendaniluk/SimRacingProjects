//#define SIM_SERIAL_DEBUG

#include <Joystick.h>
#include "ButtonJoystick.h"
#include "FunkySwitch.h"

// Button box input for a sim racing rig that sends inputs as joystick signals.
//
// This is setup for the following inputs:
//   - Ignition toggle switch
//   - Starter momentary button
//   - Array of 8 momentary buttons
//   - Funky switch (up, down, left, right, push, encoder)

#define PIN_IGNITION 7
#define PIN_STARTER 8

#define PIN_B1 10
#define PIN_B2 16
#define PIN_B3 14
#define PIN_B4 15
#define PIN_B5 A0
#define PIN_B6 A1
#define PIN_B7 A2
#define PIN_B8 A3

#define PIN_UP 4
#define PIN_DOWN 5
#define PIN_LEFT 3
#define PIN_RIGHT 2
#define PIN_PUSH 6
#define PIN_ENC_UP 0
#define PIN_ENC_DOWN 1

const uint8_t TOTAL_BUTTONS = 17;
const uint8_t NUM_BUTTON_INPUTS = 10;

Joystick_* joystick;
ButtonInput* buttons[NUM_BUTTON_INPUTS];
FunkySwitch* funky_switch;

void encUpISR();
void encDownISR();

void setup() {
#ifdef SIM_SERIAL_DEBUG
  Serial.begin(9600);
#endif

  joystick = new Joystick_(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
    TOTAL_BUTTONS, 0,      // Button Count, Hat Switch Count
    false, false, false,   // X and Y, but no Z Axis
    false, false, false,   // No Rx, Ry, or Rz
    false, false,          // No rudder or throttle
    false, false, false);  // No accelerator, brake, or steering

#ifndef SIM_SERIAL_DEBUG
  joystick->begin();
#endif

  buttons[0] = new ButtonJoystick(joystick, 0, PIN_IGNITION);
  buttons[1] = new ButtonJoystick(joystick, 1, PIN_STARTER);

  buttons[2] = new ButtonJoystick(joystick, 2, PIN_B1);
  buttons[3] = new ButtonJoystick(joystick, 3, PIN_B2);
  buttons[4] = new ButtonJoystick(joystick, 4, PIN_B3);
  buttons[5] = new ButtonJoystick(joystick, 5, PIN_B4);
  buttons[6] = new ButtonJoystick(joystick, 6, PIN_B5);
  buttons[7] = new ButtonJoystick(joystick, 7, PIN_B6);
  buttons[8] = new ButtonJoystick(joystick, 8, PIN_B7);
  buttons[9] = new ButtonJoystick(joystick, 9, PIN_B8, false);

  // Funky switch
  uint8_t funky_ids [7] = {10, 11, 12, 13, 14, 15, 16};
  uint8_t funky_pins [7] = {PIN_UP, PIN_DOWN, PIN_LEFT, PIN_RIGHT, PIN_PUSH, PIN_ENC_UP, PIN_ENC_DOWN};
  funky_switch = new FunkySwitch(joystick, funky_ids, funky_pins, encUpISR, encDownISR);
}

void loop() {
  funky_switch->updateToggle();
  funky_switch->updateEncoder();
  for (size_t i = 0; i < NUM_BUTTON_INPUTS; ++i) {
    buttons[i]->update();
  }

  delay(50);
}

void encUpISR() {
  funky_switch->encUpChange();
}

void encDownISR() {
  funky_switch->encDownChange();
}

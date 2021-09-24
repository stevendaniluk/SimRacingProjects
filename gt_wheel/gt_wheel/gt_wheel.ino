//#define SIM_SERIAL_DEBUG

#include <Joystick.h>
#include "ButtonJoystick.h"
#include "EncoderJoystick.h"
#include "FunkySwitch.h"

// GT wheel input for a sim racing rig that sends inputs as joystick signals.
//
// This is setup for the following inputs:
//   - 8 Momentary buttons
//   - 1 Encoder
//   - 1 Funky switch (up, down, left, right, push, encoder)
//   - 2 Shifter inputs

// Funky switch
#define PIN_ENC_UP 0
#define PIN_ENC_DOWN 1
#define PIN_UP 2
#define PIN_DOWN 3
#define PIN_LEFT 4
#define PIN_RIGHT 5
#define PIN_PUSH 6

// Momentary buttons
#define PIN_B1 9
#define PIN_B2 10
#define PIN_B3 14
#define PIN_B4 15
#define PIN_B5 16
#define PIN_B6 A1
#define PIN_B7 A2
#define PIN_B8 A3

// Two momentary buttons on one analog input
#define PIN_COMBO A0
#define COMBO_A_LOW 120
#define COMBO_A_HIGH 130
#define COMBO_B_LOW 35
#define COMBO_B_HIGH 45

// Encoder
#define PIN_ENC_1 7
#define PIN_ENC_2 8

const uint8_t NUM_BUTTON_INPUTS = 10;
const uint8_t TOTAL_BUTTONS = 19;

Joystick_* joystick;
ButtonInput* buttons[NUM_BUTTON_INPUTS];
FunkySwitch* funky_switch;
EncoderJoystick* encoder;

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

  buttons[0] = new ButtonJoystick(joystick, 0, PIN_B1);
  buttons[1] = new ButtonJoystick(joystick, 1, PIN_B2);
  buttons[2] = new ButtonJoystick(joystick, 2, PIN_B3);
  buttons[3] = new ButtonJoystick(joystick, 3, PIN_B4);
  buttons[4] = new ButtonJoystick(joystick, 4, PIN_B5);
  buttons[5] = new ButtonJoystick(joystick, 5, PIN_B6);
  buttons[6] = new ButtonJoystick(joystick, 6, PIN_B7);
  buttons[7] = new ButtonJoystick(joystick, 7, PIN_B8);
  buttons[8] = new ButtonJoystick(joystick, 8, PIN_COMBO, COMBO_A_LOW, COMBO_A_HIGH);
  buttons[9] = new ButtonJoystick(joystick, 9, PIN_COMBO, COMBO_B_LOW, COMBO_B_HIGH);

  // Funky switch
  uint8_t funky_ids [7] = {10, 11, 12, 13, 14, 15, 16};
  uint8_t funky_pins [7] = {PIN_UP, PIN_DOWN, PIN_LEFT, PIN_RIGHT, PIN_PUSH, PIN_ENC_UP, PIN_ENC_DOWN};
  funky_switch = new FunkySwitch(joystick, funky_ids, funky_pins, encUpISR, encDownISR);

  // Encoder
  encoder = new EncoderJoystick(joystick, 17, 18, PIN_ENC_1, PIN_ENC_2);
}

void loop() {
  funky_switch->updateToggle();
  funky_switch->updateEncoder();
  encoder->update();
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

#include "io.h"

namespace {
const int kLeftBeamBreakSensor = A7;
const int kRightBeamBreakSensor = A5;

const int kLeftMotor = 5;
const int kRightMotor = 6;

const int kLeftTurnLED = 2;
const int kStraightLED = 3;
const int kRightTurnLED = 4;
}  // namespace

IO::IO() {
  pinMode(kLeftBeamBreakSensor, INPUT);
  pinMode(kRightBeamBreakSensor, INPUT);

  pinMode(kLeftMotor, OUTPUT);
  pinMode(kRightMotor, OUTPUT);

  pinMode(kLeftTurnLED, OUTPUT);
  pinMode(kStraightLED, OUTPUT);
  pinMode(kRightTurnLED, OUTPUT);
}

Sensors IO::GetSensors() {

}

void IO::WriteOutputs(float left_motor, float right_motor,
                      float left_turn_brightness, float right_turn_brightness,
                      float straight_brightness) {
                      
}

#include "io.h"

namespace {
//const int kLeftBeamBreakSensor = A7;
//const int kRightBeamBreakSensor = A5;

const int kLeftScannerSensor = A7;
const int kRightScannerSensor = A5;

const int kLeftMotor = 5;
const int kRightMotor = 6;

const int kLeftTurnLED = 2;
const int kStraightLED = 3;
const int kRightTurnLED = 4;
}  // namespace

IO::IO() {
  pinMode(kLeftScannerSensor, INPUT);
  pinMode(kRightScannerSensor, INPUT);

  pinMode(kLeftMotor, OUTPUT);
  pinMode(kRightMotor, OUTPUT);

  pinMode(kLeftTurnLED, OUTPUT);
  pinMode(kStraightLED, OUTPUT);
  pinMode(kRightTurnLED, OUTPUT);
}

RawSensors IO::GetSensors() {
  RawSensors current_sensors;

  current_sensors.left_scanner = analogRead(kLeftScannerSensor) / 1023.0;
  current_sensors.right_scanner = analogRead(kRightScannerSensor) / 1023.0;

  current_sensors.left_wheel_beam_break = false;
  current_sensors.right_wheel_beam_break = false;

  return current_sensors;
}

void IO::WriteOutputs(float left_motor, float right_motor,
                      float left_turn_brightness, float right_turn_brightness,
                      float straight_brightness) {
  analogWrite(kLeftMotor, left_motor * 255);
  analogWrite(kRightMotor, left_motor * 255);

  analogWrite(kLeftTurnLED, left_turn_brightness * 255);
  analogWrite(kRightTurnLED, right_turn_brightness * 255);

  analogWrite(kStraightLED, straight_brightness * 255);
}

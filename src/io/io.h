#pragma once

struct RawSensors {
  float left_scanner;
  float right_scanner;
  bool left_wheel_beam_break;
  bool right_wheel_beam_break;
};

class IO {
 public:
  IO();

  RawSensors GetSensors();
  void WriteOutputs(float left_motor, float right_motor,
                    float left_turn_brightness, float right_turn_brightness,
                    float straight_brightness);
};

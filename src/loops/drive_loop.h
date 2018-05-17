#pragma once

#include "../io/io.h"
#include "../sensor_filter/sensor_filter.h"

class DriveLoop {
 public:
  DriveLoop();

  // Flight loop state machine states.
  enum State {
    ZEROING,
    FOLLOW_PATH,
    FINISH
  };

  State state() const { return state_; }

  void Run();

 private:
  void RunIteration();

  IO io_;
  SensorFilter sensor_filter_;

  State state_;

  bool running_;
};

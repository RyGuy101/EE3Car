#include "drive_loop.h"

namespace {
const int kLoopFrequency = 100;
}

DriveLoop::DriveLoop() : state_(ZEROING), running_(false) {}

void DriveLoop::Run() {
  running_ = true;

  sensor_filter_.Zero();

  while (running_) {
    double start_time = micros();
    RunIteration();

    delayMicroseconds(1e6 / kLoopFrequency - (micros() - start_time) / 1e6);
  }
}

void DriveLoop::RunIteration() {
  RawSensors raw_sensors = io_.GetSensors();
  sensor_filter_.ProcessRawSensors(raw_sensors);

  switch (state_) {
    case ZEROING:
      break;
    case FOLLOW_PATH:
      break;
    case FINISH:
      break;
  }
}

#include "drive_loop.h"

namespace {
const int kLoopFrequency = 100;
}

DriveLoop::DriveLoop() : state_(ZEROING), running_(false) {}

void DriveLoop::Run() {
  running_ = true;

  while (running_) {
    double start_time = micros();
    RunIteration();

    delayMicroseconds(1e6 / kLoopFrequency - (micros() - start_time) / 1e6);
  }
}

void DriveLoop::RunIteration() {
  switch (state_) {
    case ZEROING:
      break;
    case FOLLOW_PATH:
      break;
    case FINISH:
      break;
  }
}

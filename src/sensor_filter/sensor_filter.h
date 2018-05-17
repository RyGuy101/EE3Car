#pragma once

#include "../io/io.h"

class MovingAverageFilter {
 public:
  MovingAverageFilter();
  ~MovingAverageFilter();

  float Process(float measurement);

 private:
  float *history_;
  int size_;
  int index_;
};

struct TrackFollowerStatus {
  bool hit_stop;
  float estimated_angle;
};

class TrackFollowerFilter {
 public:
  TrackFollowerFilter();
  ~TrackFollowerFilter();

  TrackFollowerStatus ProcessRawSensors(RawSensors raw_sensors);
  void Zero();

 private:
  MovingAverageFilter left_sensor_;
  MovingAverageFilter right_sensor_;

  float left_brightness_threshold_;
  float right_brightness_threshold_;

  int zero_ticks_;
};

class SensorFilter {
 public:
  SensorFilter();
  void Zero();

  void ProcessRawSensors(RawSensors raw_sensors);

 private:
  TrackFollowerFilter track_follower_filter_;
};

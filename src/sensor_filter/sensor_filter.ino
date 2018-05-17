#include "sensor_filter.h"

namespace {
float kDarkTrackFollowerSensor = 100 / 1024.0;
float kTrackFollowerSensorDistanceFromWheels = 4;  // inches
float kTrackFollowerSensorDistanceFromCenter = 1;  // inches
}  // namespace

MovingAverageFilter::MovingAverageFilter()
    : history_(new float[10]), size_(0), index_(0) {
  history_ = new float[10];
  Serial.begin(9600);
}

MovingAverageFilter::~MovingAverageFilter() { delete[] history_; }

float MovingAverageFilter::Process(float measurement) {
  if (size_ < 10) {
    size_++;
  }

  index_++;
  if (index_ >= 10) {
    index_ = 0;
  }

  history_[index_] = measurement;

  float sum = 0;
  for (int i = 0; i < size_; i++) {
    sum += history_[i];
  }

  return sum / size_;
}

TrackFollowerFilter::TrackFollowerFilter()
    : zero_ticks_(0),
      left_brightness_threshold_(1),
      right_brightness_threshold_(1) {}

TrackFollowerFilter::~TrackFollowerFilter() {}

void TrackFollowerFilter::Zero() { zero_ticks_ = 100; }

TrackFollowerStatus TrackFollowerFilter::ProcessRawSensors(
    RawSensors raw_sensors) {
  TrackFollowerStatus track_follower_status;
  track_follower_status.hit_stop = false;
  track_follower_status.estimated_angle = 0;

  float left_filtered = left_sensor_.Process(raw_sensors.left_scanner);
  float right_filtered = right_sensor_.Process(raw_sensors.right_scanner);

  if (zero_ticks_ > 0) {
    left_brightness_threshold_ = left_filtered - kDarkTrackFollowerSensor;
    right_brightness_threshold_ = right_filtered - kDarkTrackFollowerSensor;
    zero_ticks_--;
  }

  float left_sensor_blocked_fraction =
      1 -
      (left_filtered - kDarkTrackFollowerSensor) / left_brightness_threshold_;
  float right_sensor_blocked_fraction =
      1 -
      (right_filtered - kDarkTrackFollowerSensor) / right_brightness_threshold_;

  if (left_sensor_blocked_fraction < 0) left_sensor_blocked_fraction = 0;
  if (left_sensor_blocked_fraction > 1) left_sensor_blocked_fraction = 1;
  if (right_sensor_blocked_fraction < 0) right_sensor_blocked_fraction = 0;
  if (right_sensor_blocked_fraction > 1) right_sensor_blocked_fraction = 1;

  static int should_print = 0;
  if(should_print++ % 100 == 0) {
    Serial.print("l: ");
    Serial.println(left_sensor_blocked_fraction);
    Serial.print("r: ");
    Serial.println(right_sensor_blocked_fraction);
  }

  if (left_sensor_blocked_fraction > 0.95 &&
      right_sensor_blocked_fraction > 0.95) {
    // At the end of the track since both sensors were blocked.
    track_follower_status.hit_stop = true;

    // Finish line is perpendicular to the track, so angle should be 0.
    track_follower_status.estimated_angle = 0;

    return track_follower_status;
  }

  // Choose the sensor that has a greater block ratio, and use that to find the
  // angle between the car and the line it is supposed to follow.
  if (left_sensor_blocked_fraction > right_sensor_blocked_fraction) {
    track_follower_status.estimated_angle = atan(
        -kTrackFollowerSensorDistanceFromCenter * left_sensor_blocked_fraction /
        kTrackFollowerSensorDistanceFromWheels);
  } else {
    track_follower_status.estimated_angle = atan(
        kTrackFollowerSensorDistanceFromCenter *
        right_sensor_blocked_fraction / kTrackFollowerSensorDistanceFromWheels);
  }

  if(should_print % 100 == 0) {
    Serial.print("estimated angle: ");
    Serial.println(track_follower_status.estimated_angle);
  }

  return track_follower_status;
}

SensorFilter::SensorFilter() {}

void SensorFilter::ProcessRawSensors(RawSensors raw_sensors) {
  track_follower_filter_.ProcessRawSensors(raw_sensors);
}

void SensorFilter::Zero() {
  track_follower_filter_.Zero();
}

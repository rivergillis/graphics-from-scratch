#ifndef PING_PONG_H_
#define PING_PONG_H_

// Returns a value that is goes back and forth between min and max,
// moving by speed every time we call next().

template <typename T>
struct PingPong {
  T value_;
  T min_;
  T max_;
  T speed_;

  PingPong(T min, T max, T speed = 1) : value_(min), min_(min), max_(max), speed_(speed) {}

  T Next() {
    value_ += speed_;
    if (value_ > max_) {
      value_ = max_;
      speed_ = -speed_;
    } else if (value_ < min_) {
      value_ = min_;
      speed_ = -speed_;
    }
    return value_;
  }
};

#endif  // PING_PONG_H_
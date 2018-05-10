#include "../io/io.h"

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

  State state_;

  bool running_;
};

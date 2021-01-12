#include "duration.h"

[[cpp11::register]]
cpp11::writable::list
sys_now_cpp() {
  using namespace std::chrono;

  typename system_clock::time_point tp = system_clock::now();

  // The precision of `system_clock::now()` is platform dependent.
  // We just cast them all to nanoseconds to at least return a consistent precision.
  // - MacOS = 1us
  // - Linux = 1ns
  // - Windows = 100ns
  // https://stackoverflow.com/questions/55120594/different-behaviour-of-system-clock-on-windows-and-linux
  nanoseconds d = duration_cast<nanoseconds>(tp.time_since_epoch());

  rclock::duration::nanoseconds out(1);
  out.assign(d, 0);

  return out.to_list();
}

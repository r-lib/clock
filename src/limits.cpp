#include "clock.h"

[[cpp11::register]]
int
clock_get_year_max() {
  return static_cast<int>(date::year::max());
}

[[cpp11::register]]
int
clock_get_year_min() {
  return static_cast<int>(date::year::min());
}

#include "clock.h"

[[cpp11::register]]
int
clock_get_calendar_year_maximum() {
  return static_cast<int>(date::year::max());
}

[[cpp11::register]]
int
clock_get_calendar_year_minimum() {
  return static_cast<int>(date::year::min());
}

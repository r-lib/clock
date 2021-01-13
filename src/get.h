#ifndef CLOCK_GET_H
#define CLOCK_GET_H

#include "clock.h"

namespace rclock {

namespace gregorian {

static
inline
cpp11::integers
get_year(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 1 ? fields[0] : cpp11::integers{};
}

static
inline
cpp11::integers
get_month(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 2 ? fields[1] : cpp11::integers{};
}

static
inline
cpp11::integers
get_day(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 3 ? fields[2] : cpp11::integers{};
}

static
inline
cpp11::integers
get_hour(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 4 ? fields[3] : cpp11::integers{};
}

static
inline
cpp11::integers
get_minute(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 5 ? fields[4] : cpp11::integers{};
}

static
inline
cpp11::integers
get_second(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 6 ? fields[5] : cpp11::integers{};
}

static
inline
cpp11::integers
get_subsecond(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 7 ? fields[6] : cpp11::integers{};
}

} // namespace gregorian

} // namespace rclock

// -----------------------------------------------------------------------------

namespace rclock {

namespace weekday {

static
inline
cpp11::integers
get_year(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 1 ? fields[0] : cpp11::integers{};
}

static
inline
cpp11::integers
get_month(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 2 ? fields[1] : cpp11::integers{};
}

static
inline
cpp11::integers
get_weekday(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 3 ? fields[2] : cpp11::integers{};
}

static
inline
cpp11::integers
get_weekday_index(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 4 ? fields[3] : cpp11::integers{};
}

static
inline
cpp11::integers
get_hour(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 5 ? fields[4] : cpp11::integers{};
}

static
inline
cpp11::integers
get_minute(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 6 ? fields[5] : cpp11::integers{};
}

static
inline
cpp11::integers
get_second(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 7 ? fields[6] : cpp11::integers{};
}

static
inline
cpp11::integers
get_subsecond(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 8 ? fields[7] : cpp11::integers{};
}

} // namespace weekday

} // namespace rclock

// -----------------------------------------------------------------------------

namespace rclock {

namespace rquarterly {

static
inline
cpp11::integers
get_year(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 1 ? fields[0] : cpp11::integers{};
}

static
inline
cpp11::integers
get_quarternum(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 2 ? fields[1] : cpp11::integers{};
}

static
inline
cpp11::integers
get_quarterday(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 3 ? fields[2] : cpp11::integers{};
}

static
inline
cpp11::integers
get_hour(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 4 ? fields[3] : cpp11::integers{};
}

static
inline
cpp11::integers
get_minute(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 5 ? fields[4] : cpp11::integers{};
}

static
inline
cpp11::integers
get_second(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 6 ? fields[5] : cpp11::integers{};
}

static
inline
cpp11::integers
get_subsecond(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 7 ? fields[6] : cpp11::integers{};
}

} // namespace rquarterly

} // namespace rclock

// -----------------------------------------------------------------------------

namespace rclock {

namespace duration {

static
inline
cpp11::integers
get_ticks(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 1 ? fields[0] : cpp11::integers{};
}

static
inline
cpp11::integers
get_ticks_of_day(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 2 ? fields[1] : cpp11::integers{};
}

static
inline
cpp11::integers
get_ticks_of_second(cpp11::list_of<cpp11::integers>& fields) {
  return fields.size() >= 3 ? fields[2] : cpp11::integers{};
}

} // namespace duration

} // namespace rclock

#endif

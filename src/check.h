#ifndef CLOCK_CHECK_H
#define CLOCK_CHECK_H

#include "clock.h"
#include "utils.h"

static inline void check_range_year(const int& value, const char* arg) {
  if (value > 9999 || value < 0) {
    clock_abort("`%s` must be within the range of [0, 9999], not %i.", arg, value);
  }
}
static inline void check_range_month(const int& value, const char* arg) {
  if (value > 12 || value < 1) {
    clock_abort("`%s` must be within the range of [1, 12], not %i.", arg, value);
  }
}
static inline void check_range_day(const int& value, const char* arg) {
  if (value > 31 || value < 1) {
    clock_abort("`%s` must be within the range of [1, 31], not %i.", arg, value);
  }
}
static inline void check_range_hour(const int& value, const char* arg) {
  if (value > 23 || value < 0) {
    clock_abort("`%s` must be within the range of [0, 23], not %i.", arg, value);
  }
}
static inline void check_range_minute(const int& value, const char* arg) {
  if (value > 59 || value < 0) {
    clock_abort("`%s` must be within the range of [0, 59], not %i.", arg, value);
  }
}
static inline void check_range_second(const int& value, const char* arg) {
  if (value > 59 || value < 0) {
    clock_abort("`%s` must be within the range of [0, 59], not %i.", arg, value);
  }
}
static inline void check_range_millisecond(const int& value, const char* arg) {
  if (value > 999 || value < 0) {
    clock_abort("`%s` must be within the range of [0, 999], not %i.", arg, value);
  }
}
static inline void check_range_microsecond(const int& value, const char* arg) {
  if (value > 999999 || value < 0) {
    clock_abort("`%s` must be within the range of [0, 999999], not %i.", arg, value);
  }
}
static inline void check_range_nanosecond(const int& value, const char* arg) {
  if (value > 999999999 || value < 0) {
    clock_abort("`%s` must be within the range of [0, 999999999], not %i.", arg, value);
  }
}

static inline void check_range_quarternum(const int& value, const char* arg) {
  if (value > 4 || value < 1) {
    clock_abort("`%s` must be within the range of [1, 4], not %i.", arg, value);
  }
}
static inline void check_range_quarterday(const int& value, const char* arg) {
  if (value > 92 || value < 1) {
    clock_abort("`%s` must be within the range of [1, 92], not %i.", arg, value);
  }
}

static inline void check_range_weeknum(const int& value, const char* arg) {
  if (value > 53 || value < 1) {
    clock_abort("`%s` must be within the range of [1, 53], not %i.", arg, value);
  }
}
static inline void check_range_weekday(const int& value, const char* arg) {
  if (value > 7 || value < 1) {
    clock_abort("`%s` must be within the range of [1, 7], not %i.", arg, value);
  }
}
static inline void check_range_index(const int& value, const char* arg) {
  if (value > 5 || value < 1) {
    clock_abort("`%s` must be within the range of [1, 5], not %i.", arg, value);
  }
}

// -----------------------------------------------------------------------------

template <typename Duration>
inline void check_range(const int& value, const char* arg) {
  clock_abort("Unimplemented range check");
}
template <>
inline void check_range<date::year>(const int& value, const char* arg) {
  check_range_year(value, arg);
}
template <>
inline void check_range<date::month>(const int& value, const char* arg) {
  check_range_month(value, arg);
}
template <>
inline void check_range<date::day>(const int& value, const char* arg) {
  check_range_day(value, arg);
}
template <>
inline void check_range<std::chrono::hours>(const int& value, const char* arg) {
  check_range_hour(value, arg);
}
template <>
inline void check_range<std::chrono::minutes>(const int& value, const char* arg) {
  check_range_minute(value, arg);
}
template <>
inline void check_range<std::chrono::seconds>(const int& value, const char* arg) {
  check_range_second(value, arg);
}
template <>
inline void check_range<std::chrono::milliseconds>(const int& value, const char* arg) {
  check_range_millisecond(value, arg);
}
template <>
inline void check_range<std::chrono::microseconds>(const int& value, const char* arg) {
  check_range_microsecond(value, arg);
}
template <>
inline void check_range<std::chrono::nanoseconds>(const int& value, const char* arg) {
  check_range_nanosecond(value, arg);
}

#endif

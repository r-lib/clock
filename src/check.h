#ifndef CLOCK_CHECK_H
#define CLOCK_CHECK_H

#include "clock.h"
#include "utils.h"
#include "enums.h"

// Gregorian / general
static inline void check_range_year(const int& value, const char* arg) {
  static const int max = static_cast<int>(date::year::max());
  static const int min = static_cast<int>(date::year::min());
  if (value > max || value < min) {
    clock_abort("`%s` must be within the range of [%i, %i], not %i.", arg, min, max, value);
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

// Quarterly
static inline void check_range_quarterly_quarter(const int& value, const char* arg) {
  if (value > 4 || value < 1) {
    clock_abort("`%s` must be within the range of [1, 4], not %i.", arg, value);
  }
}
static inline void check_range_quarterly_day(const int& value, const char* arg) {
  if (value > 92 || value < 1) {
    clock_abort("`%s` must be within the range of [1, 92], not %i.", arg, value);
  }
}

// Iso
static inline void check_range_iso_week(const int& value, const char* arg) {
  if (value > 53 || value < 1) {
    clock_abort("`%s` must be within the range of [1, 53], not %i.", arg, value);
  }
}
static inline void check_range_iso_day(const int& value, const char* arg) {
  if (value > 7 || value < 1) {
    clock_abort("`%s` must be within the range of [1, 7], not %i.", arg, value);
  }
}

// Weekday
static inline void check_range_weekday_day(const int& value, const char* arg) {
  if (value > 7 || value < 1) {
    clock_abort("`%s` must be within the range of [1, 7], not %i.", arg, value);
  }
}
static inline void check_range_weekday_index(const int& value, const char* arg) {
  if (value > 5 || value < 1) {
    clock_abort("`%s` must be within the range of [1, 5], not %i.", arg, value);
  }
}

// Ordinal
static inline void check_range_ordinal_day(const int& value, const char* arg) {
  if (value > 366 || value < 1) {
    clock_abort("`%s` must be within the range of [1, 366], not %i.", arg, value);
  }
}

#endif

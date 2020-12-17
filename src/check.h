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

#endif

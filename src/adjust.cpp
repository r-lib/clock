#include "r.h"
#include "utils.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "local.h"
#include "check.h"
#include <date/date.h>
#include <date/tz.h>

// -----------------------------------------------------------------------------

static sexp adjust_local_days(sexp x,
                              sexp value,
                              const enum day_nonexistent& day_nonexistent,
                              const r_ssize& size,
                              const enum adjuster& adjuster);

[[cpp11::register]]
SEXP adjust_local_days_cpp(SEXP x,
                           SEXP value,
                           SEXP day_nonexistent,
                           SEXP size,
                           SEXP adjuster) {
  enum day_nonexistent c_day_nonexistent = parse_day_nonexistent(day_nonexistent);
  r_ssize c_size = r_int_get(size, 0);
  enum adjuster c_adjuster = parse_adjuster(adjuster);

  return adjust_local_days(
    x,
    value,
    c_day_nonexistent,
    c_size,
    c_adjuster
  );
}

static inline
date::year_month_day
adjust_local_days_switch(const date::year_month_day& ymd,
                         const int& value,
                         const enum adjuster& adjuster);

static sexp adjust_local_days(sexp x,
                              sexp value,
                              const enum day_nonexistent& day_nonexistent,
                              const r_ssize& size,
                              const enum adjuster& adjuster) {
  x = PROTECT(local_maybe_clone(x));
  x = PROTECT(local_recycle(x, size));

  int* p_days = local_days_deref(x);
  int* p_time_of_day = local_time_of_day_deref(x);

  const bool recycle_value = r_is_scalar(value);
  const int* p_value = r_int_deref_const(value);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = p_days[i];
    int elt_value = recycle_value ? p_value[0] : p_value[i];

    if (elt_days == r_int_na) {
      continue;
    }
    if (elt_value == r_int_na) {
      local_assign_missing(i, p_days, p_time_of_day);
      continue;
    }

    date::local_days elt_lday{date::days{elt_days}};
    date::year_month_day elt_ymd{elt_lday};

    date::year_month_day out_ymd = adjust_local_days_switch(
      elt_ymd,
      elt_value,
      adjuster
    );

    convert_year_month_day_to_days_one(
      i,
      day_nonexistent,
      out_ymd,
      p_days,
      p_time_of_day
    );
  }

  UNPROTECT(2);
  return x;
}

// -----------------------------------------------------------------------------

static inline
date::year_month_day
adjust_local_days_year(const date::year_month_day& ymd, const int& value) {
  check_range_year(value, "value");
  return date::year{value} / ymd.month() / ymd.day();
}

static inline
date::year_month_day
adjust_local_days_month(const date::year_month_day& ymd, const int& value) {
  check_range_month(value, "value");
  unsigned int month = static_cast<unsigned int>(value);
  return ymd.year() / date::month{month} / ymd.day();
}

static inline
date::year_month_day
adjust_local_days_day(const date::year_month_day& ymd, const int& value) {
  check_range_day(value, "value");
  unsigned int day = static_cast<unsigned int>(value);
  return ymd.year() / ymd.month() / date::day{day};
}

static inline
date::year_month_day
adjust_local_days_last_day_of_month(const date::year_month_day& ymd) {
  return ymd.year() / ymd.month() / date::last;
}

static inline
date::year_month_day
adjust_local_days_switch(const date::year_month_day& ymd,
                         const int& value,
                         const enum adjuster& adjuster) {
  switch (adjuster) {
  case adjuster::year: {
    return adjust_local_days_year(ymd, value);
  }
  case adjuster::month: {
    return adjust_local_days_month(ymd, value);
  }
  case adjuster::day: {
    return adjust_local_days_day(ymd, value);
  }
  case adjuster::last_day_of_month: {
    return adjust_local_days_last_day_of_month(ymd);
  }
  default: {
    r_abort("Internal error: Unknown `adjuster` in `adjust_local_date_switch()`.");
  }
  }
}

// -----------------------------------------------------------------------------

static sexp adjust_local_time_of_day(sexp x,
                                     sexp value,
                                     const r_ssize& size,
                                     const enum adjuster& adjuster);

[[cpp11::register]]
SEXP adjust_local_time_of_day_cpp(SEXP x,
                                  SEXP value,
                                  SEXP size,
                                  SEXP adjuster) {
  r_ssize c_size = r_int_get(size, 0);
  enum adjuster c_adjuster = parse_adjuster(adjuster);

  return adjust_local_time_of_day(
    x,
    value,
    c_size,
    c_adjuster
  );
}

static inline
std::chrono::seconds
adjust_local_time_of_day_switch(const date::hh_mm_ss<std::chrono::seconds>& hms,
                                const int& value,
                                const enum adjuster& adjuster);

static sexp adjust_local_time_of_day(sexp x,
                                     sexp value,
                                     const r_ssize& size,
                                     const enum adjuster& adjuster) {
  x = PROTECT(local_maybe_clone(x));
  x = PROTECT(local_recycle(x, size));

  int* p_days = local_days_deref(x);
  int* p_time_of_day = local_time_of_day_deref(x);

  const bool recycle_value = r_is_scalar(value);
  const int* p_value = r_int_deref_const(value);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = p_days[i];
    int elt_time_of_day = p_time_of_day[i];
    int elt_value = recycle_value ? p_value[0] : p_value[i];

    if (elt_days == r_int_na) {
      continue;
    }
    if (elt_value == r_int_na) {
      local_assign_missing(i, p_days, p_time_of_day);
      continue;
    }

    date::local_days elt_lday{date::days{elt_days}};
    date::local_seconds elt_lsec_floor;

    std::chrono::seconds elt_tod{elt_time_of_day};
    date::hh_mm_ss<std::chrono::seconds> elt_hms{elt_tod};

    std::chrono::seconds out_tod = adjust_local_time_of_day_switch(
      elt_hms,
      elt_value,
      adjuster
    );

    p_time_of_day[i] = out_tod.count();
  }

  UNPROTECT(2);
  return x;
}

// -----------------------------------------------------------------------------

static inline
std::chrono::seconds
adjust_local_time_of_day_hour(const date::hh_mm_ss<std::chrono::seconds>& hms, const int& value) {
  check_range_hour(value, "value");
  return std::chrono::hours{value} + hms.minutes() + hms.seconds();
}

static inline
std::chrono::seconds
adjust_local_time_of_day_minute(const date::hh_mm_ss<std::chrono::seconds>& hms, const int& value) {
  check_range_minute(value, "value");
  return hms.hours() + std::chrono::minutes{value} + hms.seconds();
}

static inline
std::chrono::seconds
adjust_local_time_of_day_second(const date::hh_mm_ss<std::chrono::seconds>& hms, const int& value) {
  check_range_second(value, "value");
  return hms.hours() + hms.minutes() + std::chrono::seconds{value};
}

static inline
std::chrono::seconds
adjust_local_time_of_day_switch(const date::hh_mm_ss<std::chrono::seconds>& hms,
                                const int& value,
                                const enum adjuster& adjuster) {
  switch (adjuster) {
  case adjuster::hour: {
    return adjust_local_time_of_day_hour(hms, value);
  }
  case adjuster::minute: {
    return adjust_local_time_of_day_minute(hms, value);
  }
  case adjuster::second: {
    return adjust_local_time_of_day_second(hms, value);
  }
  default: {
    r_abort("Internal error: Unknown `adjuster` in `adjust_local_time_of_day_switch()`.");
  }
  }
}

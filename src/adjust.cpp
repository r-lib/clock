#include "civil.h"
#include "utils.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "civil-rcrd.h"
#include "check.h"

// -----------------------------------------------------------------------------

static civil_writable_rcrd adjust_local_days(const civil_rcrd& x,
                                             const cpp11::integers& value,
                                             const enum day_nonexistent& day_nonexistent,
                                             const r_ssize& size,
                                             const enum adjuster& adjuster);

[[cpp11::register]]
civil_writable_rcrd adjust_local_days_cpp(const civil_rcrd& x,
                                          const cpp11::integers& value,
                                          const cpp11::strings& day_nonexistent,
                                          const cpp11::integers& size,
                                          const cpp11::strings& adjuster) {
  return adjust_local_days(
    x,
    value,
    parse_day_nonexistent(day_nonexistent),
    size[0],
    parse_adjuster(adjuster)
  );
}

static inline
date::year_month_day
adjust_local_days_switch(const date::year_month_day& ymd,
                         const int& value,
                         const enum adjuster& adjuster);

static civil_writable_rcrd adjust_local_days(const civil_rcrd& x,
                                             const cpp11::integers& value,
                                             const enum day_nonexistent& day_nonexistent,
                                             const r_ssize& size,
                                             const enum adjuster& adjuster) {
  civil_writable_rcrd out = civil_rcrd_clone(x);
  civil_rcrd_recycle(out, size);

  int* p_days = civil_rcrd_days_deref(out);
  int* p_time_of_day = civil_rcrd_time_of_day_deref(out);
  int* p_nanos_of_second = civil_rcrd_nanos_of_second_deref(out);

  const bool recycle_value = civil_is_scalar(value);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = p_days[i];
    int elt_value = recycle_value ? value[0] : value[i];

    if (elt_days == r_int_na) {
      continue;
    }
    if (elt_value == r_int_na) {
      civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
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
      p_time_of_day,
      p_nanos_of_second
    );
  }

  return out;
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
    civil_abort("Internal error: Unknown `adjuster` in `adjust_local_date_switch()`.");
  }
  }
}

// -----------------------------------------------------------------------------

static civil_writable_rcrd adjust_local_time_of_day(const civil_rcrd& x,
                                                    const cpp11::integers& value,
                                                    const r_ssize& size,
                                                    const enum adjuster& adjuster);

[[cpp11::register]]
civil_writable_rcrd adjust_local_time_of_day_cpp(const civil_rcrd& x,
                                                 const cpp11::integers& value,
                                                 const cpp11::integers& size,
                                                 const cpp11::strings& adjuster) {
  r_ssize c_size = size[0];
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

static civil_writable_rcrd adjust_local_time_of_day(const civil_rcrd& x,
                                                    const cpp11::integers& value,
                                                    const r_ssize& size,
                                                    const enum adjuster& adjuster) {
  civil_writable_rcrd out = civil_rcrd_clone(x);
  civil_rcrd_recycle(out, size);

  int* p_days = civil_rcrd_days_deref(out);
  int* p_time_of_day = civil_rcrd_time_of_day_deref(out);
  int* p_nanos_of_second = civil_rcrd_nanos_of_second_deref(out);

  const bool recycle_value = civil_is_scalar(value);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_time_of_day = p_time_of_day[i];
    int elt_value = recycle_value ? value[0] : value[i];

    if (elt_time_of_day == r_int_na) {
      continue;
    }
    if (elt_value == r_int_na) {
      civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
      continue;
    }

    std::chrono::seconds elt_tod{elt_time_of_day};
    date::hh_mm_ss<std::chrono::seconds> elt_hms{elt_tod};

    std::chrono::seconds out_tod = adjust_local_time_of_day_switch(
      elt_hms,
      elt_value,
      adjuster
    );

    p_time_of_day[i] = out_tod.count();
  }

  return out;
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
    civil_abort("Internal error: Unknown `adjuster` in `adjust_local_time_of_day_switch()`.");
  }
  }
}

// -----------------------------------------------------------------------------

static civil_writable_rcrd adjust_local_nanos_of_second(const civil_rcrd& x,
                                                        const cpp11::integers& value,
                                                        const r_ssize& size,
                                                        const enum adjuster& adjuster);

[[cpp11::register]]
civil_writable_rcrd adjust_local_nanos_of_second_cpp(const civil_rcrd& x,
                                                     const cpp11::integers& value,
                                                     const cpp11::integers& size,
                                                     const cpp11::strings& adjuster) {
  r_ssize c_size = size[0];
  enum adjuster c_adjuster = parse_adjuster(adjuster);

  return adjust_local_nanos_of_second(
    x,
    value,
    c_size,
    c_adjuster
  );
}

static inline
std::chrono::nanoseconds
adjust_local_nanos_of_second_switch(const std::chrono::nanoseconds& x,
                                    const int& value,
                                    const enum adjuster& adjuster);

static civil_writable_rcrd adjust_local_nanos_of_second(const civil_rcrd& x,
                                                        const cpp11::integers& value,
                                                        const r_ssize& size,
                                                        const enum adjuster& adjuster) {
  civil_writable_rcrd out = civil_rcrd_clone(x);
  civil_rcrd_recycle(out, size);

  int* p_days = civil_rcrd_days_deref(out);
  int* p_time_of_day = civil_rcrd_time_of_day_deref(out);
  int* p_nanos_of_second = civil_rcrd_nanos_of_second_deref(out);

  const bool recycle_value = civil_is_scalar(value);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_nanos_of_second = p_nanos_of_second[i];
    int elt_value = recycle_value ? value[0] : value[i];

    if (elt_nanos_of_second == r_int_na) {
      continue;
    }
    if (elt_value == r_int_na) {
      civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
      continue;
    }

    std::chrono::nanoseconds elt_nanos{elt_nanos_of_second};

    std::chrono::nanoseconds out_nanos = adjust_local_nanos_of_second_switch(
      elt_nanos,
      elt_value,
      adjuster
    );

    p_nanos_of_second[i] = out_nanos.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

static inline
std::chrono::nanoseconds
adjust_local_nanos_of_second_nanosecond(const std::chrono::nanoseconds& nanos, const int& value) {
  check_range_nanos(value, "value");
  return std::chrono::nanoseconds{value};
}

static inline
std::chrono::nanoseconds
adjust_local_nanos_of_second_switch(const std::chrono::nanoseconds& nanos,
                                    const int& value,
                                    const enum adjuster& adjuster) {
  switch (adjuster) {
  case adjuster::nanosecond: {
    return adjust_local_nanos_of_second_nanosecond(nanos, value);
  }
  default: {
    civil_abort("Internal error: Unknown `adjuster` in `adjust_local_time_of_day_switch()`.");
  }
  }
}

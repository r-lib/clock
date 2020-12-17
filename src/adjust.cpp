#include "civil.h"
#include "utils.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "civil-rcrd.h"
#include "check.h"

// -----------------------------------------------------------------------------

static inline
date::year_month_day
adjust_naive_gregorian_switch(const date::year_month_day& ymd,
                              const int& value,
                              const enum adjuster& adjuster_val);

[[cpp11::register]]
cpp11::writable::list adjust_gregorian_calendar(const civil_field& calendar,
                                                const cpp11::integers& value,
                                                const cpp11::strings& day_nonexistent,
                                                const cpp11::strings& adjuster) {
  const r_ssize& size = calendar.size();

  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);
  enum adjuster adjuster_val = parse_adjuster(adjuster);

  civil_writable_field out_calendar{calendar};
  cpp11::writable::logicals ok(size);
  cpp11::writable::logicals any(1);
  any[0] = false;

  cpp11::writable::list out({out_calendar, ok, any});
  out.names() = {"calendar", "ok", "any"};

  for (r_ssize i = 0; i < size; ++i) {
    ok[i] = true;

    int elt_calendar = calendar[i];
    int elt_value = value[i];

    if (elt_calendar == r_int_na) {
      continue;
    }
    if (elt_value == r_int_na) {
      ok[i] = r_lgl_na;
      any[0] = true;
      out_calendar[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    date::year_month_day elt_ymd{elt_lday};

    date::year_month_day out_ymd = adjust_naive_gregorian_switch(
      elt_ymd,
      elt_value,
      adjuster_val
    );

    convert_ymd_to_calendar_one(i, day_nonexistent_val, out_ymd, out_calendar, ok, any);
  }

  return out;
}

// -----------------------------------------------------------------------------

static inline
date::year_month_day
adjust_naive_gregorian_year(const date::year_month_day& ymd, const int& value) {
  check_range_year(value, "value");
  return date::year{value} / ymd.month() / ymd.day();
}

static inline
date::year_month_day
adjust_naive_gregorian_month(const date::year_month_day& ymd, const int& value) {
  check_range_month(value, "value");
  unsigned int month = static_cast<unsigned int>(value);
  return ymd.year() / date::month{month} / ymd.day();
}

static inline
date::year_month_day
adjust_naive_gregorian_day(const date::year_month_day& ymd, const int& value) {
  check_range_day(value, "value");
  unsigned int day = static_cast<unsigned int>(value);
  return ymd.year() / ymd.month() / date::day{day};
}

static inline
date::year_month_day
adjust_naive_gregorian_last_day_of_month(const date::year_month_day& ymd) {
  return ymd.year() / ymd.month() / date::last;
}

static inline
date::year_month_day
adjust_naive_gregorian_switch(const date::year_month_day& ymd,
                              const int& value,
                              const enum adjuster& adjuster_val) {
  switch (adjuster_val) {
  case adjuster::year: {
    return adjust_naive_gregorian_year(ymd, value);
  }
  case adjuster::month: {
    return adjust_naive_gregorian_month(ymd, value);
  }
  case adjuster::day: {
    return adjust_naive_gregorian_day(ymd, value);
  }
  case adjuster::last_day_of_month: {
    return adjust_naive_gregorian_last_day_of_month(ymd);
  }
  default: {
    civil_abort("Internal error: Unknown `adjuster_val` in `adjust_naive_gregorian_switch()`.");
  }
  }
}

// -----------------------------------------------------------------------------

static inline
std::chrono::seconds
adjust_naive_time_point_seconds_of_day_switch(const date::hh_mm_ss<std::chrono::seconds>& hms,
                                              const int& value,
                                              const enum adjuster& adjuster_val);

[[cpp11::register]]
civil_writable_rcrd adjust_naive_time_point_seconds_of_day_cpp(const civil_rcrd& x,
                                                               const cpp11::integers& value,
                                                               const cpp11::strings& adjuster) {
  const enum adjuster adjuster_val = parse_adjuster(adjuster);

  civil_writable_rcrd out = civil_rcrd_clone(x);

  int* p_calendar = civil_rcrd_days_deref(out);
  int* p_seconds_of_day = civil_rcrd_time_of_day_deref(out);
  int* p_nanoseconds_of_second = civil_rcrd_nanos_of_second_deref(out);

  const r_ssize size = value.size();

  for (r_ssize i = 0; i < size; ++i) {
    int elt_seconds_of_day = p_seconds_of_day[i];
    int elt_value = value[i];

    if (elt_seconds_of_day == r_int_na) {
      continue;
    }
    if (elt_value == r_int_na) {
      civil_rcrd_assign_missing(i, p_calendar, p_seconds_of_day, p_nanoseconds_of_second);
      continue;
    }

    std::chrono::seconds elt_tod{elt_seconds_of_day};
    date::hh_mm_ss<std::chrono::seconds> elt_hms{elt_tod};

    std::chrono::seconds out_tod = adjust_naive_time_point_seconds_of_day_switch(
      elt_hms,
      elt_value,
      adjuster_val
    );

    p_seconds_of_day[i] = out_tod.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

static inline
std::chrono::seconds
adjust_naive_time_point_seconds_of_day_hour(const date::hh_mm_ss<std::chrono::seconds>& hms, const int& value) {
  check_range_hour(value, "value");
  return std::chrono::hours{value} + hms.minutes() + hms.seconds();
}

static inline
std::chrono::seconds
adjust_naive_time_point_seconds_of_day_minute(const date::hh_mm_ss<std::chrono::seconds>& hms, const int& value) {
  check_range_minute(value, "value");
  return hms.hours() + std::chrono::minutes{value} + hms.seconds();
}

static inline
std::chrono::seconds
adjust_naive_time_point_seconds_of_day_second(const date::hh_mm_ss<std::chrono::seconds>& hms, const int& value) {
  check_range_second(value, "value");
  return hms.hours() + hms.minutes() + std::chrono::seconds{value};
}

static inline
std::chrono::seconds
adjust_naive_time_point_seconds_of_day_switch(const date::hh_mm_ss<std::chrono::seconds>& hms,
                                              const int& value,
                                              const enum adjuster& adjuster_val) {
  switch (adjuster_val) {
  case adjuster::hour: {
    return adjust_naive_time_point_seconds_of_day_hour(hms, value);
  }
  case adjuster::minute: {
    return adjust_naive_time_point_seconds_of_day_minute(hms, value);
  }
  case adjuster::second: {
    return adjust_naive_time_point_seconds_of_day_second(hms, value);
  }
  default: {
    civil_abort("Internal error: Unknown `adjuster_val` in `adjust_naive_time_point_seconds_of_day_switch()`.");
  }
  }
}

// -----------------------------------------------------------------------------

static inline
std::chrono::nanoseconds
adjust_naive_time_point_nanoseconds_of_second_switch(const std::chrono::nanoseconds& x,
                                              const int& value,
                                              const enum adjuster& adjuster_val);

[[cpp11::register]]
civil_writable_rcrd adjust_naive_time_point_nanoseconds_of_second_cpp(const civil_rcrd& x,
                                                                      const cpp11::integers& value,
                                                                      const cpp11::strings& adjuster) {
  const enum adjuster adjuster_val = parse_adjuster(adjuster);
  const r_ssize size = value.size();

  civil_writable_rcrd out = civil_rcrd_clone(x);

  int* p_calendar = civil_rcrd_days_deref(out);
  int* p_seconds_of_day = civil_rcrd_time_of_day_deref(out);
  int* p_nanoseconds_of_second = civil_rcrd_nanos_of_second_deref(out);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_nanoseconds_of_second = p_nanoseconds_of_second[i];
    int elt_value = value[i];

    if (elt_nanoseconds_of_second == r_int_na) {
      continue;
    }
    if (elt_value == r_int_na) {
      civil_rcrd_assign_missing(i, p_calendar, p_seconds_of_day, p_nanoseconds_of_second);
      continue;
    }

    std::chrono::nanoseconds elt_nanos{elt_nanoseconds_of_second};

    std::chrono::nanoseconds out_nanos = adjust_naive_time_point_nanoseconds_of_second_switch(
      elt_nanos,
      elt_value,
      adjuster_val
    );

    p_nanoseconds_of_second[i] = out_nanos.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

static inline
std::chrono::nanoseconds
adjust_naive_time_point_nanoseconds_of_second_millisecond(const std::chrono::nanoseconds& nanos, const int& value) {
  check_range_millisecond(value, "value");
  return std::chrono::milliseconds{value};
}

static inline
std::chrono::nanoseconds
adjust_naive_time_point_nanoseconds_of_second_microsecond(const std::chrono::nanoseconds& nanos, const int& value) {
  check_range_microsecond(value, "value");
  return std::chrono::microseconds{value};
}

static inline
std::chrono::nanoseconds
adjust_naive_time_point_nanoseconds_of_second_nanosecond(const std::chrono::nanoseconds& nanos, const int& value) {
  check_range_nanosecond(value, "value");
  return std::chrono::nanoseconds{value};
}

static inline
std::chrono::nanoseconds
adjust_naive_time_point_nanoseconds_of_second_switch(const std::chrono::nanoseconds& nanos,
                                                     const int& value,
                                                     const enum adjuster& adjuster_val) {
  switch (adjuster_val) {
  case adjuster::millisecond: {
    return adjust_naive_time_point_nanoseconds_of_second_millisecond(nanos, value);
  }
  case adjuster::microsecond: {
    return adjust_naive_time_point_nanoseconds_of_second_microsecond(nanos, value);
  }
  case adjuster::nanosecond: {
    return adjust_naive_time_point_nanoseconds_of_second_nanosecond(nanos, value);
  }
  default: {
    civil_abort("Internal error: Unknown `adjuster_val` in `adjust_naive_time_point_nanoseconds_of_second_switch()`.");
  }
  }
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static cpp11::writable::list adjust_quarterly_calendar_impl(const civil_field& calendar,
                                                            const cpp11::integers& value,
                                                            const enum day_nonexistent& day_nonexistent_val,
                                                            const enum adjuster& adjuster_val);

[[cpp11::register]]
cpp11::writable::list adjust_quarterly_calendar(const civil_field& calendar,
                                                const cpp11::integers& value,
                                                const int& start,
                                                const cpp11::strings& day_nonexistent,
                                                const cpp11::strings& adjuster) {
  const enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);
  const enum adjuster adjuster_val = parse_adjuster(adjuster);

  if (start == 1) {
    return adjust_quarterly_calendar_impl<quarterly::start::january>(calendar, value, day_nonexistent_val, adjuster_val);
  } else if (start == 2) {
    return adjust_quarterly_calendar_impl<quarterly::start::february>(calendar, value, day_nonexistent_val, adjuster_val);
  } else if (start == 3) {
    return adjust_quarterly_calendar_impl<quarterly::start::march>(calendar, value, day_nonexistent_val, adjuster_val);
  } else if (start == 4) {
    return adjust_quarterly_calendar_impl<quarterly::start::april>(calendar, value, day_nonexistent_val, adjuster_val);
  } else if (start == 5) {
    return adjust_quarterly_calendar_impl<quarterly::start::may>(calendar, value, day_nonexistent_val, adjuster_val);
  } else if (start == 6) {
    return adjust_quarterly_calendar_impl<quarterly::start::june>(calendar, value, day_nonexistent_val, adjuster_val);
  } else if (start == 7) {
    return adjust_quarterly_calendar_impl<quarterly::start::july>(calendar, value, day_nonexistent_val, adjuster_val);
  } else if (start == 8) {
    return adjust_quarterly_calendar_impl<quarterly::start::august>(calendar, value, day_nonexistent_val, adjuster_val);
  } else if (start == 9) {
    return adjust_quarterly_calendar_impl<quarterly::start::september>(calendar, value, day_nonexistent_val, adjuster_val);
  } else if (start == 10) {
    return adjust_quarterly_calendar_impl<quarterly::start::october>(calendar, value, day_nonexistent_val, adjuster_val);
  } else if (start == 11) {
    return adjust_quarterly_calendar_impl<quarterly::start::november>(calendar, value, day_nonexistent_val, adjuster_val);
  } else if (start == 12) {
    return adjust_quarterly_calendar_impl<quarterly::start::december>(calendar, value, day_nonexistent_val, adjuster_val);
  }

  never_reached("adjust_quarterly_calendar");
}

template <quarterly::start S>
static
inline
quarterly::year_quarternum_quarterday<S>
adjust_naive_quarterly_switch(const quarterly::year_quarternum_quarterday<S>& yqnqd,
                              const int& value,
                              const enum adjuster& adjuster_val);

template <quarterly::start S>
static cpp11::writable::list adjust_quarterly_calendar_impl(const civil_field& calendar,
                                                            const cpp11::integers& value,
                                                            const enum day_nonexistent& day_nonexistent_val,
                                                            const enum adjuster& adjuster_val) {
  const r_ssize size = value.size();

  civil_writable_field out_calendar{calendar};
  cpp11::writable::logicals ok(size);
  cpp11::writable::logicals any(1);
  any[0] = false;

  cpp11::writable::list out({out_calendar, ok, any});
  out.names() = {"calendar", "ok", "any"};

  for (r_ssize i = 0; i < size; ++i) {
    ok[i] = true;

    int elt_calendar = calendar[i];
    int elt_value = value[i];

    if (elt_calendar == r_int_na) {
      continue;
    }
    if (elt_value == r_int_na) {
      ok[i] = false;
      any[0] = true;
      out_calendar[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    quarterly::year_quarternum_quarterday<S> elt_yqnqd{elt_lday};

    quarterly::year_quarternum_quarterday<S> out_yqnqd = adjust_naive_quarterly_switch(
      elt_yqnqd,
      elt_value,
      adjuster_val
    );

    convert_yqnqd_to_calendar_one(
      i,
      day_nonexistent_val,
      out_yqnqd,
      out_calendar,
      ok,
      any
    );
  }

  return out;
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static inline
quarterly::year_quarternum_quarterday<S>
adjust_naive_quarterly_year(const quarterly::year_quarternum_quarterday<S>& yqnqd, const int& value) {
  check_range_year(value, "value");
  return {quarterly::year<S>{value}, yqnqd.quarternum(), yqnqd.quarterday()};
}

template <quarterly::start S>
static inline
quarterly::year_quarternum_quarterday<S>
adjust_naive_quarterly_quarternum(const quarterly::year_quarternum_quarterday<S>& yqnqd, const int& value) {
  check_range_quarternum(value, "value");
  unsigned int quarternum = static_cast<unsigned int>(value);
  return {yqnqd.year(), quarterly::quarternum{quarternum}, yqnqd.quarterday()};
}

template <quarterly::start S>
static inline
quarterly::year_quarternum_quarterday<S>
adjust_naive_quarterly_quarterday(const quarterly::year_quarternum_quarterday<S>& yqnqd, const int& value) {
  check_range_quarterday(value, "value");
  unsigned int quarterday = static_cast<unsigned int>(value);
  return {yqnqd.year(), yqnqd.quarternum(), quarterly::quarterday{quarterday}};
}

template <quarterly::start S>
static inline
quarterly::year_quarternum_quarterday<S>
adjust_naive_quarterly_last_day_of_quarter(const quarterly::year_quarternum_quarterday<S>& yqnqd) {
  return quarterly::year_quarternum_quarterday_last<S>{yqnqd.year(), yqnqd.quarternum()};
}

template <quarterly::start S>
static inline
quarterly::year_quarternum_quarterday<S>
adjust_naive_quarterly_switch(const quarterly::year_quarternum_quarterday<S>& yqnqd,
                              const int& value,
                              const enum adjuster& adjuster_val) {
  switch (adjuster_val) {
  case adjuster::year: {
    return adjust_naive_quarterly_year(yqnqd, value);
  }
  case adjuster::quarternum: {
    return adjust_naive_quarterly_quarternum(yqnqd, value);
  }
  case adjuster::quarterday: {
    return adjust_naive_quarterly_quarterday(yqnqd, value);
  }
  case adjuster::last_day_of_quarter: {
    return adjust_naive_quarterly_last_day_of_quarter(yqnqd);
  }
  default: {
    civil_abort("Internal error: Unknown `adjuster_val` in `adjust_naive_quarterly_switch()`.");
  }
  }
}

// -----------------------------------------------------------------------------

static inline
iso_week::year_weeknum_weekday
adjust_naive_iso_switch(const iso_week::year_weeknum_weekday& yww,
                        const int& value,
                        const enum adjuster& adjuster_val);

[[cpp11::register]]
cpp11::writable::list adjust_iso_calendar(const civil_field& calendar,
                                          const cpp11::integers& value,
                                          const cpp11::strings& day_nonexistent,
                                          const cpp11::strings& adjuster) {
  const r_ssize& size = calendar.size();

  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);
  enum adjuster adjuster_val = parse_adjuster(adjuster);

  civil_writable_field out_calendar{calendar};
  cpp11::writable::logicals ok(size);
  cpp11::writable::logicals any(1);
  any[0] = false;

  cpp11::writable::list out({out_calendar, ok, any});
  out.names() = {"calendar", "ok", "any"};

  for (r_ssize i = 0; i < size; ++i) {
    ok[i] = true;

    int elt_calendar = calendar[i];
    int elt_value = value[i];

    if (elt_calendar == r_int_na) {
      continue;
    }
    if (elt_value == r_int_na) {
      ok[i] = r_lgl_na;
      any[0] = true;
      out_calendar[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    iso_week::year_weeknum_weekday elt_yww{elt_lday};

    iso_week::year_weeknum_weekday out_yww = adjust_naive_iso_switch(
      elt_yww,
      elt_value,
      adjuster_val
    );

    convert_iso_yww_to_calendar_one(
      i,
      day_nonexistent_val,
      out_yww,
      out_calendar,
      ok,
      any
    );
  }

  return out;
}

// -----------------------------------------------------------------------------

static inline
iso_week::year_weeknum_weekday
adjust_naive_iso_year(const iso_week::year_weeknum_weekday& yww, const int& value) {
  check_range_year(value, "value");
  return iso_week::year{value} / yww.weeknum() / yww.weekday();
}

static inline
iso_week::year_weeknum_weekday
adjust_naive_iso_weeknum(const iso_week::year_weeknum_weekday& yww, const int& value) {
  check_range_iso_weeknum(value, "value");
  unsigned int weeknum = static_cast<unsigned int>(value);
  return yww.year() / iso_week::weeknum{weeknum} / yww.weekday();
}

static inline
iso_week::year_weeknum_weekday
adjust_naive_iso_weekday(const iso_week::year_weeknum_weekday& yww, const int& value) {
  check_range_iso_weekday(value, "value");
  unsigned int weekday = static_cast<unsigned int>(value);
  return yww.year() / yww.weeknum() / iso_week::weekday{weekday};
}

static inline
iso_week::year_weeknum_weekday
adjust_naive_iso_last_weeknum_of_year(const iso_week::year_weeknum_weekday& yww) {
  return yww.year() / iso_week::last / yww.weekday();
}

static inline
iso_week::year_weeknum_weekday
adjust_naive_iso_switch(const iso_week::year_weeknum_weekday& yww,
                        const int& value,
                        const enum adjuster& adjuster_val) {
  switch (adjuster_val) {
  case adjuster::year: {
    return adjust_naive_iso_year(yww, value);
  }
  case adjuster::weeknum: {
    return adjust_naive_iso_weeknum(yww, value);
  }
  case adjuster::weekday: {
    return adjust_naive_iso_weekday(yww, value);
  }
  case adjuster::last_weeknum_of_year: {
    return adjust_naive_iso_last_weeknum_of_year(yww);
  }
  default: {
    civil_abort("Internal error: Unknown `adjuster_val` in `adjust_naive_iso_switch()`.");
  }
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_field downcast_nanoseconds_of_second_precision(const civil_field& nanoseconds_of_second,
                                                              const cpp11::strings& precision) {
  const enum precision precision_val = parse_precision(precision);
  const r_ssize size = nanoseconds_of_second.size();

  civil_writable_field out(size);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_nanoseconds_of_second = nanoseconds_of_second[i];

    if (elt_nanoseconds_of_second == r_int_na) {
      out[i] = r_int_na;
      continue;
    }

    std::chrono::nanoseconds elt_nanos{elt_nanoseconds_of_second};
    std::chrono::nanoseconds out_nanos;

    switch (precision_val) {
    case precision::nanosecond: {
      out_nanos = elt_nanos;
      break;
    }
    case precision::microsecond: {
      out_nanos = std::chrono::duration_cast<std::chrono::microseconds>(elt_nanos);
      break;
    }
    case precision::millisecond: {
      out_nanos = std::chrono::duration_cast<std::chrono::milliseconds>(elt_nanos);
      break;
    }
    case precision::second: {
      civil_abort("Internal error: Should never be called.");
    }
    }

    out[i] = out_nanos.count();
  }

  return out;
}


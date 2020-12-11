#include "civil.h"
#include "utils.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "civil-rcrd.h"
#include "check.h"

// -----------------------------------------------------------------------------

static civil_writable_rcrd adjust_naive_gregorian_days(const civil_rcrd& x,
                                                       const cpp11::integers& value,
                                                       const enum day_nonexistent& day_nonexistent_val,
                                                       const r_ssize& size,
                                                       const enum adjuster& adjuster_val);

[[cpp11::register]]
civil_writable_rcrd adjust_naive_gregorian_days_cpp(const civil_rcrd& x,
                                                    const cpp11::integers& value,
                                                    const cpp11::strings& day_nonexistent,
                                                    const cpp11::integers& size,
                                                    const cpp11::strings& adjuster) {
  return adjust_naive_gregorian_days(
    x,
    value,
    parse_day_nonexistent(day_nonexistent),
    size[0],
    parse_adjuster(adjuster)
  );
}

static inline
date::year_month_day
adjust_naive_gregorian_switch(const date::year_month_day& ymd,
                              const int& value,
                              const enum adjuster& adjuster_val);

static civil_writable_rcrd adjust_naive_gregorian_days(const civil_rcrd& x,
                                                       const cpp11::integers& value,
                                                       const enum day_nonexistent& day_nonexistent_val,
                                                       const r_ssize& size,
                                                       const enum adjuster& adjuster_val) {
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

    date::year_month_day out_ymd = adjust_naive_gregorian_switch(
      elt_ymd,
      elt_value,
      adjuster_val
    );

    convert_year_month_day_to_days_one(
      i,
      day_nonexistent_val,
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

static civil_writable_rcrd adjust_naive_gregorian_time_of_day(const civil_rcrd& x,
                                                              const cpp11::integers& value,
                                                              const r_ssize& size,
                                                              const enum adjuster& adjuster_val);

[[cpp11::register]]
civil_writable_rcrd adjust_naive_gregorian_time_of_day_cpp(const civil_rcrd& x,
                                                           const cpp11::integers& value,
                                                           const cpp11::integers& size,
                                                           const cpp11::strings& adjuster) {
  r_ssize c_size = size[0];
  enum adjuster adjuster_val = parse_adjuster(adjuster);

  return adjust_naive_gregorian_time_of_day(
    x,
    value,
    c_size,
    adjuster_val
  );
}

static inline
std::chrono::seconds
adjust_naive_gregorian_time_of_day_switch(const date::hh_mm_ss<std::chrono::seconds>& hms,
                                          const int& value,
                                          const enum adjuster& adjuster_val);

static civil_writable_rcrd adjust_naive_gregorian_time_of_day(const civil_rcrd& x,
                                                              const cpp11::integers& value,
                                                              const r_ssize& size,
                                                              const enum adjuster& adjuster_val) {
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

    std::chrono::seconds out_tod = adjust_naive_gregorian_time_of_day_switch(
      elt_hms,
      elt_value,
      adjuster_val
    );

    p_time_of_day[i] = out_tod.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

static inline
std::chrono::seconds
adjust_naive_gregorian_time_of_day_hour(const date::hh_mm_ss<std::chrono::seconds>& hms, const int& value) {
  check_range_hour(value, "value");
  return std::chrono::hours{value} + hms.minutes() + hms.seconds();
}

static inline
std::chrono::seconds
adjust_naive_gregorian_time_of_day_minute(const date::hh_mm_ss<std::chrono::seconds>& hms, const int& value) {
  check_range_minute(value, "value");
  return hms.hours() + std::chrono::minutes{value} + hms.seconds();
}

static inline
std::chrono::seconds
adjust_naive_gregorian_time_of_day_second(const date::hh_mm_ss<std::chrono::seconds>& hms, const int& value) {
  check_range_second(value, "value");
  return hms.hours() + hms.minutes() + std::chrono::seconds{value};
}

static inline
std::chrono::seconds
adjust_naive_gregorian_time_of_day_switch(const date::hh_mm_ss<std::chrono::seconds>& hms,
                                          const int& value,
                                          const enum adjuster& adjuster_val) {
  switch (adjuster_val) {
  case adjuster::hour: {
    return adjust_naive_gregorian_time_of_day_hour(hms, value);
  }
  case adjuster::minute: {
    return adjust_naive_gregorian_time_of_day_minute(hms, value);
  }
  case adjuster::second: {
    return adjust_naive_gregorian_time_of_day_second(hms, value);
  }
  default: {
    civil_abort("Internal error: Unknown `adjuster_val` in `adjust_naive_gregorian_time_of_day_switch()`.");
  }
  }
}

// -----------------------------------------------------------------------------

static civil_writable_rcrd adjust_naive_gregorian_nanos_of_second(const civil_rcrd& x,
                                                                  const cpp11::integers& value,
                                                                  const r_ssize& size,
                                                                  const enum adjuster& adjuster_val);

[[cpp11::register]]
civil_writable_rcrd adjust_naive_gregorian_nanos_of_second_cpp(const civil_rcrd& x,
                                                               const cpp11::integers& value,
                                                               const cpp11::integers& size,
                                                               const cpp11::strings& adjuster) {
  r_ssize c_size = size[0];
  enum adjuster adjuster_val = parse_adjuster(adjuster);

  return adjust_naive_gregorian_nanos_of_second(
    x,
    value,
    c_size,
    adjuster_val
  );
}

static inline
std::chrono::nanoseconds
adjust_naive_gregorian_nanos_of_second_switch(const std::chrono::nanoseconds& x,
                                              const int& value,
                                              const enum adjuster& adjuster_val);

static civil_writable_rcrd adjust_naive_gregorian_nanos_of_second(const civil_rcrd& x,
                                                                  const cpp11::integers& value,
                                                                  const r_ssize& size,
                                                                  const enum adjuster& adjuster_val) {
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

    std::chrono::nanoseconds out_nanos = adjust_naive_gregorian_nanos_of_second_switch(
      elt_nanos,
      elt_value,
      adjuster_val
    );

    p_nanos_of_second[i] = out_nanos.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

static inline
std::chrono::nanoseconds
adjust_naive_gregorian_nanos_of_second_nanosecond(const std::chrono::nanoseconds& nanos, const int& value) {
  check_range_nanos(value, "value");
  return std::chrono::nanoseconds{value};
}

static inline
std::chrono::nanoseconds
adjust_naive_gregorian_nanos_of_second_switch(const std::chrono::nanoseconds& nanos,
                                              const int& value,
                                              const enum adjuster& adjuster_val) {
  switch (adjuster_val) {
  case adjuster::nanosecond: {
    return adjust_naive_gregorian_nanos_of_second_nanosecond(nanos, value);
  }
  default: {
    civil_abort("Internal error: Unknown `adjuster_val` in `adjust_naive_gregorian_nanos_of_second_switch()`.");
  }
  }
}

// -----------------------------------------------------------------------------

template <fiscal_year::start S>
static civil_writable_rcrd adjust_naive_fiscal_days(const civil_rcrd& x,
                                                    const cpp11::integers& value,
                                                    const enum day_nonexistent& day_nonexistent_val,
                                                    const r_ssize& size,
                                                    const enum adjuster& adjuster_val);

[[cpp11::register]]
civil_writable_rcrd adjust_naive_fiscal_days_cpp(const civil_rcrd& x,
                                                 const cpp11::integers& value,
                                                 const int& fiscal_start,
                                                 const cpp11::strings& day_nonexistent,
                                                 const cpp11::integers& size,
                                                 const cpp11::strings& adjuster) {
  r_ssize c_size = size[0];
  const enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);
  const enum adjuster adjuster_val = parse_adjuster(adjuster);

  if (fiscal_start == 1) {
    return adjust_naive_fiscal_days<fiscal_year::start::january>(x, value, day_nonexistent_val, c_size, adjuster_val);
  } else if (fiscal_start == 2) {
    return adjust_naive_fiscal_days<fiscal_year::start::february>(x, value, day_nonexistent_val, c_size, adjuster_val);
  } else if (fiscal_start == 3) {
    return adjust_naive_fiscal_days<fiscal_year::start::march>(x, value, day_nonexistent_val, c_size, adjuster_val);
  } else if (fiscal_start == 4) {
    return adjust_naive_fiscal_days<fiscal_year::start::april>(x, value, day_nonexistent_val, c_size, adjuster_val);
  } else if (fiscal_start == 5) {
    return adjust_naive_fiscal_days<fiscal_year::start::may>(x, value, day_nonexistent_val, c_size, adjuster_val);
  } else if (fiscal_start == 6) {
    return adjust_naive_fiscal_days<fiscal_year::start::june>(x, value, day_nonexistent_val, c_size, adjuster_val);
  } else if (fiscal_start == 7) {
    return adjust_naive_fiscal_days<fiscal_year::start::july>(x, value, day_nonexistent_val, c_size, adjuster_val);
  } else if (fiscal_start == 8) {
    return adjust_naive_fiscal_days<fiscal_year::start::august>(x, value, day_nonexistent_val, c_size, adjuster_val);
  } else if (fiscal_start == 9) {
    return adjust_naive_fiscal_days<fiscal_year::start::september>(x, value, day_nonexistent_val, c_size, adjuster_val);
  } else if (fiscal_start == 10) {
    return adjust_naive_fiscal_days<fiscal_year::start::october>(x, value, day_nonexistent_val, c_size, adjuster_val);
  } else if (fiscal_start == 11) {
    return adjust_naive_fiscal_days<fiscal_year::start::november>(x, value, day_nonexistent_val, c_size, adjuster_val);
  } else if (fiscal_start == 12) {
    return adjust_naive_fiscal_days<fiscal_year::start::december>(x, value, day_nonexistent_val, c_size, adjuster_val);
  }

  never_reached("adjust_naive_fiscal_days_cpp");
}

template <fiscal_year::start S>
static
inline
fiscal_year::year_quarternum_quarterday<S>
adjust_naive_fiscal_switch(const fiscal_year::year_quarternum_quarterday<S>& yqnqd,
                           const int& value,
                           const enum adjuster& adjuster_val);

template <fiscal_year::start S>
static civil_writable_rcrd adjust_naive_fiscal_days(const civil_rcrd& x,
                                                    const cpp11::integers& value,
                                                    const enum day_nonexistent& day_nonexistent_val,
                                                    const r_ssize& size,
                                                    const enum adjuster& adjuster_val) {
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
    fiscal_year::year_quarternum_quarterday<S> elt_yqnqd{elt_lday};

    fiscal_year::year_quarternum_quarterday<S> out_yqnqd = adjust_naive_fiscal_switch(
      elt_yqnqd,
      elt_value,
      adjuster_val
    );

    convert_year_quarternum_quarterday_to_days_one(
      i,
      day_nonexistent_val,
      out_yqnqd,
      p_days,
      p_time_of_day,
      p_nanos_of_second
    );
  }

  return out;
}

// -----------------------------------------------------------------------------

template <fiscal_year::start S>
static inline
fiscal_year::year_quarternum_quarterday<S>
adjust_naive_fiscal_year(const fiscal_year::year_quarternum_quarterday<S>& yqnqd, const int& value) {
  check_range_year(value, "value");
  return {fiscal_year::year<S>{value}, yqnqd.quarternum(), yqnqd.quarterday()};
}

template <fiscal_year::start S>
static inline
fiscal_year::year_quarternum_quarterday<S>
adjust_naive_fiscal_quarternum(const fiscal_year::year_quarternum_quarterday<S>& yqnqd, const int& value) {
  check_range_quarternum(value, "value");
  unsigned int quarternum = static_cast<unsigned int>(value);
  return {yqnqd.year(), fiscal_year::quarternum{quarternum}, yqnqd.quarterday()};
}

template <fiscal_year::start S>
static inline
fiscal_year::year_quarternum_quarterday<S>
adjust_naive_fiscal_quarterday(const fiscal_year::year_quarternum_quarterday<S>& yqnqd, const int& value) {
  check_range_quarterday(value, "value");
  unsigned int quarterday = static_cast<unsigned int>(value);
  return {yqnqd.year(), yqnqd.quarternum(), fiscal_year::quarterday{quarterday}};
}

template <fiscal_year::start S>
static inline
fiscal_year::year_quarternum_quarterday<S>
adjust_naive_fiscal_last_day_of_quarter(const fiscal_year::year_quarternum_quarterday<S>& yqnqd) {
  return fiscal_year::year_quarternum_quarterday_last<S>{yqnqd.year(), yqnqd.quarternum()};
}

template <fiscal_year::start S>
static inline
fiscal_year::year_quarternum_quarterday<S>
adjust_naive_fiscal_switch(const fiscal_year::year_quarternum_quarterday<S>& yqnqd,
                           const int& value,
                           const enum adjuster& adjuster_val) {
  switch (adjuster_val) {
  case adjuster::year: {
    return adjust_naive_fiscal_year(yqnqd, value);
  }
  case adjuster::quarternum: {
    return adjust_naive_fiscal_quarternum(yqnqd, value);
  }
  case adjuster::quarterday: {
    return adjust_naive_fiscal_quarterday(yqnqd, value);
  }
  case adjuster::last_day_of_quarter: {
    return adjust_naive_fiscal_last_day_of_quarter(yqnqd);
  }
  default: {
    civil_abort("Internal error: Unknown `adjuster_val` in `adjust_naive_fiscal_switch()`.");
  }
  }
}

// -----------------------------------------------------------------------------

static civil_writable_rcrd adjust_naive_iso_days(const civil_rcrd& x,
                                                 const cpp11::integers& value,
                                                 const enum day_nonexistent& day_nonexistent_val,
                                                 const r_ssize& size,
                                                 const enum adjuster& adjuster_val);

[[cpp11::register]]
civil_writable_rcrd adjust_naive_iso_days_cpp(const civil_rcrd& x,
                                              const cpp11::integers& value,
                                              const cpp11::strings& day_nonexistent,
                                              const cpp11::integers& size,
                                              const cpp11::strings& adjuster) {
  const enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);
  const r_ssize size_val = size[0];
  const enum adjuster adjuster_val = parse_adjuster(adjuster);

  return adjust_naive_iso_days(
    x,
    value,
    day_nonexistent_val,
    size_val,
    adjuster_val
  );
}

static inline
iso_week::year_weeknum_weekday
adjust_naive_iso_switch(const iso_week::year_weeknum_weekday& yww,
                        const int& value,
                        const enum adjuster& adjuster_val);

static civil_writable_rcrd adjust_naive_iso_days(const civil_rcrd& x,
                                                 const cpp11::integers& value,
                                                 const enum day_nonexistent& day_nonexistent_val,
                                                 const r_ssize& size,
                                                 const enum adjuster& adjuster_val) {
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
    iso_week::year_weeknum_weekday elt_yww{elt_lday};

    iso_week::year_weeknum_weekday out_yww = adjust_naive_iso_switch(
      elt_yww,
      elt_value,
      adjuster_val
    );

    convert_year_weeknum_weekday_to_days_one(
      i,
      day_nonexistent_val,
      out_yww,
      p_days,
      p_time_of_day,
      p_nanos_of_second
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

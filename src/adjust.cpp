#include "clock.h"
#include "utils.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "clock-rcrd.h"
#include "check.h"

// -----------------------------------------------------------------------------

// static inline
// date::year_month_weekday
// adjust_gregorian_weekday_year(const date::year_month_weekday& ymw, const int& value) {
//   check_range_year(value, "value");
//   return date::year{value} / ymw.month() / ymw.weekday_indexed();
// }
//
// static inline
// date::year_month_weekday
// adjust_gregorian_weekday_month(const date::year_month_weekday& ymw, const int& value) {
//   check_range_month(value, "value");
//   unsigned int month = static_cast<unsigned int>(value);
//   return ymw.year() / date::month{month} / ymw.weekday_indexed();
// }
//
// static inline
// date::year_month_weekday
// adjust_gregorian_weekday_weekday(const date::year_month_weekday& ymw, const int& value) {
//   check_range_weekday(value, "value");
//   unsigned int weekday = static_cast<unsigned int>(value - 1);
//   return ymw.year() / ymw.month() / date::weekday{weekday}[ymw.index()];
// }
//
// static inline
// date::year_month_weekday
// adjust_gregorian_weekday_weekday_index(const date::year_month_weekday& ymw, const int& value) {
//   check_range_index(value, "value");
//   unsigned int index = static_cast<unsigned int>(value);
//   return ymw.year() / ymw.month() / ymw.weekday()[index];
// }
//
// static inline
// date::year_month_weekday
// adjust_gregorian_weekday_last_weekday_index_of_month(const date::year_month_weekday& ymw) {
//   return date::year_month_weekday{ymw.year() / ymw.month() / date::weekday_last{ymw.weekday()}};
// }


// -----------------------------------------------------------------------------

template <quarterly::start S>
static cpp11::writable::list adjust_quarterly_calendar_impl(const clock_field& calendar,
                                                            const cpp11::integers& value,
                                                            const enum day_nonexistent& day_nonexistent_val,
                                                            const enum adjuster& adjuster_val);

[[cpp11::register]]
cpp11::writable::list adjust_quarterly_calendar(const clock_field& calendar,
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
static cpp11::writable::list adjust_quarterly_calendar_impl(const clock_field& calendar,
                                                            const cpp11::integers& value,
                                                            const enum day_nonexistent& day_nonexistent_val,
                                                            const enum adjuster& adjuster_val) {
  const r_ssize size = value.size();

  clock_writable_field out_calendar{calendar};
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
    clock_abort("Internal error: Unknown `adjuster_val` in `adjust_naive_quarterly_switch()`.");
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
cpp11::writable::list adjust_iso_calendar(const clock_field& calendar,
                                          const cpp11::integers& value,
                                          const cpp11::strings& day_nonexistent,
                                          const cpp11::strings& adjuster) {
  const r_ssize& size = calendar.size();

  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);
  enum adjuster adjuster_val = parse_adjuster(adjuster);

  clock_writable_field out_calendar{calendar};
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
  check_range_weeknum(value, "value");
  unsigned int weeknum = static_cast<unsigned int>(value);
  return yww.year() / iso_week::weeknum{weeknum} / yww.weekday();
}

static inline
iso_week::year_weeknum_weekday
adjust_naive_iso_weekday(const iso_week::year_weeknum_weekday& yww, const int& value) {
  check_range_weekday(value, "value");
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
    clock_abort("Internal error: Unknown `adjuster_val` in `adjust_naive_iso_switch()`.");
  }
  }
}


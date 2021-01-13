#include "clock.h"
#include "utils.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "check.h"

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


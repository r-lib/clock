#include "clock.h"
#include "zone.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "clock-rcrd.h"

// -----------------------------------------------------------------------------

template <quarterly::start S>
static cpp11::writable::list add_quarterly_calendar_years_or_quarters_impl(const clock_field& calendar,
                                                                           const cpp11::integers& n,
                                                                           const cpp11::strings& day_nonexistent,
                                                                           const cpp11::strings& unit) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);
  enum unit unit_val = parse_unit(unit);

  const r_ssize size = calendar.size();

  clock_writable_field out_calendar{calendar};
  cpp11::writable::logicals ok(size);
  cpp11::writable::logicals any(1);
  any[0] = false;

  cpp11::writable::list out({out_calendar, ok, any});
  out.names() = {"calendar", "ok", "any"};

  for (r_ssize i = 0; i < size; ++i) {
    ok[i] = true;

    int elt_calendar = calendar[i];
    int elt_n = n[i];

    if (elt_calendar == r_int_na) {
      continue;
    }
    if (elt_n == r_int_na) {
      ok[i] = r_lgl_na;
      any[0] = true;
      out_calendar[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    quarterly::year_quarternum_quarterday<S> elt_yqnqd{elt_lday};

    quarterly::year_quarternum_quarterday<S> out_yqnqd;

    if (unit_val == unit::year) {
      out_yqnqd = elt_yqnqd + quarterly::years{elt_n};
    } else {
      out_yqnqd = elt_yqnqd + quarterly::quarters{elt_n};
    }

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


[[cpp11::register]]
cpp11::writable::list add_quarterly_calendar_years_or_quarters(const clock_field& calendar,
                                                               const cpp11::integers& n,
                                                               const int& start,
                                                               const cpp11::strings& day_nonexistent,
                                                               const cpp11::strings& unit) {
  if (start == 1) {
    return add_quarterly_calendar_years_or_quarters_impl<quarterly::start::january>(calendar, n, day_nonexistent, unit);
  } else if (start == 2) {
    return add_quarterly_calendar_years_or_quarters_impl<quarterly::start::february>(calendar, n, day_nonexistent, unit);
  } else if (start == 3) {
    return add_quarterly_calendar_years_or_quarters_impl<quarterly::start::march>(calendar, n, day_nonexistent, unit);
  } else if (start == 4) {
    return add_quarterly_calendar_years_or_quarters_impl<quarterly::start::april>(calendar, n, day_nonexistent, unit);
  } else if (start == 5) {
    return add_quarterly_calendar_years_or_quarters_impl<quarterly::start::may>(calendar, n, day_nonexistent, unit);
  } else if (start == 6) {
    return add_quarterly_calendar_years_or_quarters_impl<quarterly::start::june>(calendar, n, day_nonexistent, unit);
  } else if (start == 7) {
    return add_quarterly_calendar_years_or_quarters_impl<quarterly::start::july>(calendar, n, day_nonexistent, unit);
  } else if (start == 8) {
    return add_quarterly_calendar_years_or_quarters_impl<quarterly::start::august>(calendar, n, day_nonexistent, unit);
  } else if (start == 9) {
    return add_quarterly_calendar_years_or_quarters_impl<quarterly::start::september>(calendar, n, day_nonexistent, unit);
  } else if (start == 10) {
    return add_quarterly_calendar_years_or_quarters_impl<quarterly::start::october>(calendar, n, day_nonexistent, unit);
  } else if (start == 11) {
    return add_quarterly_calendar_years_or_quarters_impl<quarterly::start::november>(calendar, n, day_nonexistent, unit);
  } else if (start == 12) {
    return add_quarterly_calendar_years_or_quarters_impl<quarterly::start::december>(calendar, n, day_nonexistent, unit);
  }

  never_reached("add_quarterly_calendar_years_or_quarters");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list add_iso_calendar_years(const clock_field& calendar,
                                             const cpp11::integers& n,
                                             const cpp11::strings& day_nonexistent) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);

  const r_ssize size = calendar.size();

  clock_writable_field out_calendar{calendar};
  cpp11::writable::logicals ok(size);
  cpp11::writable::logicals any(1);
  any[0] = false;

  cpp11::writable::list out({out_calendar, ok, any});
  out.names() = {"calendar", "ok", "any"};

  for (r_ssize i = 0; i < size; ++i) {
    ok[i] = true;

    int elt_calendar = calendar[i];
    int elt_n = n[i];

    if (elt_calendar == r_int_na) {
      continue;
    }
    if (elt_n == r_int_na) {
      ok[i] = r_lgl_na;
      any[0] = true;
      out_calendar[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    iso_week::year_weeknum_weekday elt_yww{elt_lday};

    iso_week::year_weeknum_weekday out_yww = elt_yww + iso_week::years{elt_n};

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

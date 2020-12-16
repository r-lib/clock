#include "civil.h"
#include "zone.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "civil-rcrd.h"

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list add_gregorian_calendar_years_or_months(const civil_field& calendar,
                                                             const cpp11::integers& n,
                                                             const cpp11::strings& day_nonexistent,
                                                             const cpp11::strings& unit) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);
  enum unit unit_val = parse_unit(unit);

  const r_ssize size = calendar.size();

  civil_writable_field out_calendar{calendar};
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
    date::year_month_day elt_ymd{elt_lday};

    date::year_month_day out_ymd;

    if (unit_val == unit::year) {
      out_ymd = elt_ymd + date::years{elt_n};
    } else {
      out_ymd = elt_ymd + date::months{elt_n};
    }

    convert_ymd_to_calendar_one(i, day_nonexistent_val, out_ymd, out_calendar, ok, any);
  }

  return out;
}

// -----------------------------------------------------------------------------

static civil_writable_rcrd add_weeks_or_days(const civil_rcrd& x,
                                             const cpp11::integers& n,
                                             const enum unit& unit_val,
                                             const r_ssize& size) {
  civil_writable_rcrd out = civil_rcrd_clone(x);
  civil_rcrd_recycle(out, size);

  int* p_days = civil_rcrd_days_deref(out);
  int* p_time_of_day = civil_rcrd_time_of_day_deref(out);
  int* p_nanos_of_second = civil_rcrd_nanos_of_second_deref(out);

  const bool recycle_n = civil_is_scalar(n);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = p_days[i];
    int elt_n = recycle_n ? n[0] : n[i];

    if (elt_days == r_int_na) {
      continue;
    }
    if (elt_n == r_int_na) {
      civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
      continue;
    }

    // Handle weeks as a period of 7 days
    elt_n = (unit_val == unit::week) ? elt_n * 7 : elt_n;

    date::local_days elt_lday{date::days{elt_days}};
    date::local_days out_lday = elt_lday + date::days{elt_n};

    p_days[i] = out_lday.time_since_epoch().count();
  }

  return out;
}

[[cpp11::register]]
civil_writable_rcrd add_weeks_or_days_cpp(const civil_rcrd& x,
                                          const cpp11::integers& n,
                                          const cpp11::strings& unit,
                                          const cpp11::integers& size) {
  enum unit unit_val = parse_unit(unit);
  r_ssize c_size = size[0];

  return add_weeks_or_days(x, n, unit_val, c_size);
}

// -----------------------------------------------------------------------------

static civil_writable_rcrd add_hours_or_minutes_or_seconds(const civil_rcrd& x,
                                                           const cpp11::integers& n,
                                                           const enum unit& unit_val,
                                                           const r_ssize& size) {
  civil_writable_rcrd out = civil_rcrd_clone(x);
  civil_rcrd_recycle(out, size);

  int* p_days = civil_rcrd_days_deref(out);
  int* p_time_of_day = civil_rcrd_time_of_day_deref(out);
  int* p_nanos_of_second = civil_rcrd_nanos_of_second_deref(out);

  const bool recycle_n = civil_is_scalar(n);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = p_days[i];
    int elt_time_of_day = p_time_of_day[i];
    int elt_n = recycle_n ? n[0] : n[i];

    if (elt_days == r_int_na) {
      continue;
    }
    if (elt_n == r_int_na) {
      civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
      continue;
    }

    std::chrono::seconds elt_tod{elt_time_of_day};

    date::sys_days elt_sday{date::days{elt_days}};
    date::sys_seconds elt_ssec_floor{elt_sday};

    date::sys_seconds elt_ssec = elt_ssec_floor + elt_tod;

    std::chrono::seconds elt_chrono_n;

    if (unit_val == unit::hour) {
      elt_chrono_n = std::chrono::hours{elt_n};
    } else if (unit_val == unit::minute) {
      elt_chrono_n = std::chrono::minutes{elt_n};
    } else if (unit_val == unit::second) {
      elt_chrono_n = std::chrono::seconds{elt_n};
    }

    date::sys_seconds out_ssec = elt_ssec + elt_chrono_n;

    date::sys_days out_sday = date::floor<date::days>(out_ssec);
    date::sys_seconds out_ssec_floor{out_sday};

    std::chrono::seconds out_tod{out_ssec - out_ssec_floor};

    p_days[i] = out_sday.time_since_epoch().count();
    p_time_of_day[i] = out_tod.count();
  }

  return out;
}

[[cpp11::register]]
civil_writable_rcrd add_hours_or_minutes_or_seconds_cpp(const civil_rcrd& x,
                                                        const cpp11::integers& n,
                                                        const cpp11::strings& unit,
                                                        const cpp11::integers& size) {
  enum unit unit_val = parse_unit(unit);
  r_ssize c_size = size[0];

  return add_hours_or_minutes_or_seconds(x, n, unit_val, c_size);
}

// -----------------------------------------------------------------------------

/*
 * Handle sub-second arithmetic specially by avoiding conversion from
 * nano_datetime fields to std::chrono::nanosecond. We try to support
 * a wider range of years that would be supported by the int64_t that
 * generally backs a nanosecond.
 *
 * Arithmetic "overflow" into larger units generally follows the strategy
 * laid out by <date> with the year_month_day class
 * https://github.com/HowardHinnant/date/blob/97246a638a6d8f0269f4555c5e31106a86e3fd94/include/date/date.h#L2189-L2199
 */

struct fields_datetime {
  date::days days;
  std::chrono::seconds time_of_day;
};
struct fields_nano_datetime {
  date::days days;
  std::chrono::seconds time_of_day;
  std::chrono::nanoseconds nanos_of_second;
};

static
inline
date::days plus_days(const date::days& days,
                     const date::days& n) {
  return days + n;
}

static
inline
fields_datetime plus_time_of_day(const date::days& days,
                                 const std::chrono::seconds& time_of_day,
                                 const std::chrono::seconds& n) {
  static std::chrono::seconds::rep sec_in_day = 86400;
  std::chrono::seconds::rep count = time_of_day.count() + n.count();
  date::days overflow{(count >= 0 ? count : count - (sec_in_day - 1)) / sec_in_day};
  std::chrono::seconds out_time_of_day{count - overflow.count() * sec_in_day};
  date::days out_days = plus_days(days, overflow);
  return {out_days, out_time_of_day};
}

static
inline
fields_nano_datetime plus_nanos_of_second(const date::days& days,
                                          const std::chrono::seconds& time_of_day,
                                          const std::chrono::nanoseconds& nanos_of_second,
                                          const std::chrono::nanoseconds& n) {
  static std::chrono::nanoseconds::rep nanos_in_sec = 1000000000;
  std::chrono::nanoseconds::rep count = nanos_of_second.count() + n.count();
  std::chrono::seconds overflow{(count >= 0 ? count : count - (nanos_in_sec - 1)) / nanos_in_sec};
  std::chrono::nanoseconds out_nanos_of_second{count - overflow.count() * nanos_in_sec};
  fields_datetime fdt = plus_time_of_day(days, time_of_day, overflow);
  return {fdt.days, fdt.time_of_day, out_nanos_of_second};
}

static civil_writable_rcrd add_milliseconds_or_microseconds_or_nanoseconds(const civil_rcrd& x,
                                                                           const cpp11::integers& n,
                                                                           const enum unit& unit_val,
                                                                           const r_ssize& size) {
  civil_writable_rcrd out = civil_rcrd_clone(x);
  civil_rcrd_recycle(out, size);

  int* p_days = civil_rcrd_days_deref(out);
  int* p_time_of_day = civil_rcrd_time_of_day_deref(out);
  int* p_nanos_of_second = civil_rcrd_nanos_of_second_deref(out);

  const bool recycle_n = civil_is_scalar(n);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = p_days[i];
    int elt_time_of_day = p_time_of_day[i];
    int elt_nanos_of_second = p_nanos_of_second[i];
    int elt_n = recycle_n ? n[0] : n[i];

    if (elt_days == r_int_na) {
      continue;
    }
    if (elt_n == r_int_na) {
      civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
      continue;
    }

    std::chrono::nanoseconds elt_chrono_n;

    if (unit_val == unit::millisecond) {
      elt_chrono_n = std::chrono::milliseconds{elt_n};
    } else if (unit_val == unit::microsecond) {
      elt_chrono_n = std::chrono::microseconds{elt_n};
    } else if (unit_val == unit::nanosecond) {
      elt_chrono_n = std::chrono::nanoseconds{elt_n};
    }

    fields_nano_datetime fndt = plus_nanos_of_second(
      date::days{elt_days},
      std::chrono::seconds{elt_time_of_day},
      std::chrono::nanoseconds{elt_nanos_of_second},
      elt_chrono_n
    );

    p_days[i] = fndt.days.count();
    p_time_of_day[i] = fndt.time_of_day.count();
    p_nanos_of_second[i] = fndt.nanos_of_second.count();
  }

  return out;
}

[[cpp11::register]]
civil_writable_rcrd add_milliseconds_or_microseconds_or_nanoseconds_cpp(const civil_rcrd& x,
                                                                        const cpp11::integers& n,
                                                                        const cpp11::strings& unit,
                                                                        const cpp11::integers& size) {
  enum unit unit_val = parse_unit(unit);
  r_ssize c_size = size[0];

  return add_milliseconds_or_microseconds_or_nanoseconds(x, n, unit_val, c_size);
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static cpp11::writable::list add_quarterly_calendar_years_or_quarters_impl(const civil_field& calendar,
                                                                           const cpp11::integers& n,
                                                                           const cpp11::strings& day_nonexistent,
                                                                           const cpp11::strings& unit) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);
  enum unit unit_val = parse_unit(unit);

  const r_ssize size = calendar.size();

  civil_writable_field out_calendar{calendar};
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
cpp11::writable::list add_quarterly_calendar_years_or_quarters(const civil_field& calendar,
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
cpp11::writable::list add_iso_calendar_years(const civil_field& calendar,
                                             const cpp11::integers& n,
                                             const cpp11::strings& day_nonexistent) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);

  const r_ssize size = calendar.size();

  civil_writable_field out_calendar{calendar};
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

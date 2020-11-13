#include "r.h"
#include "utils.h"
#include "zone.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "civil-rcrd.h"
#include <date/date.h>
#include <date/tz.h>

// -----------------------------------------------------------------------------

static SEXP add_years_or_months_local(sexp x,
                                      sexp n,
                                      enum day_nonexistent day_nonexistent,
                                      enum unit unit,
                                      r_ssize size) {
  x = PROTECT(civil_rcrd_maybe_clone(x));
  x = PROTECT(civil_rcrd_recycle(x, size));

  int* p_days = civil_rcrd_days_deref(x);
  int* p_time_of_day = civil_rcrd_time_of_day_deref(x);
  int* p_nanos_of_second = civil_rcrd_nanos_of_second_deref(x);

  const bool recycle_n = r_is_scalar(n);
  const int* p_n = r_int_deref_const(n);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = p_days[i];
    int elt_n = recycle_n ? p_n[0] : p_n[i];

    if (elt_days == r_int_na) {
      continue;
    }
    if (elt_n == r_int_na) {
      civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
      continue;
    }

    date::local_days elt_lday{date::days{elt_days}};
    date::year_month_day elt_ymd{elt_lday};

    date::year_month_day out_ymd;

    if (unit == unit::year) {
      out_ymd = elt_ymd + date::years{elt_n};
    } else {
      out_ymd = elt_ymd + date::months{elt_n};
    }

    convert_year_month_day_to_days_one(
      i,
      day_nonexistent,
      out_ymd,
      p_days,
      p_time_of_day,
      p_nanos_of_second
    );
  }

  UNPROTECT(2);
  return x;
}

[[cpp11::register]]
SEXP add_years_or_months_local_cpp(SEXP x,
                                   SEXP n,
                                   SEXP day_nonexistent,
                                   SEXP unit,
                                   SEXP size) {
  enum day_nonexistent c_day_nonexistent = parse_day_nonexistent(day_nonexistent);
  enum unit c_unit = parse_unit(unit);
  r_ssize c_size = r_int_get(size, 0);

  return add_years_or_months_local(x, n, c_day_nonexistent, c_unit, c_size);
}

// -----------------------------------------------------------------------------

static SEXP add_weeks_or_days_local(sexp x,
                                    sexp n,
                                    enum unit unit,
                                    r_ssize size) {
  x = PROTECT(civil_rcrd_maybe_clone(x));
  x = PROTECT(civil_rcrd_recycle(x, size));

  int* p_days = civil_rcrd_days_deref(x);
  int* p_time_of_day = civil_rcrd_time_of_day_deref(x);
  int* p_nanos_of_second = civil_rcrd_nanos_of_second_deref(x);

  const bool recycle_n = r_is_scalar(n);
  const int* p_n = r_int_deref_const(n);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = p_days[i];
    int elt_n = recycle_n ? p_n[0] : p_n[i];

    if (elt_days == r_int_na) {
      continue;
    }
    if (elt_n == r_int_na) {
      civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
      continue;
    }

    // Handle weeks as a period of 7 days
    elt_n = (unit == unit::week) ? elt_n * 7 : elt_n;

    date::local_days elt_lday{date::days{elt_days}};
    date::local_days out_lday = elt_lday + date::days{elt_n};

    p_days[i] = out_lday.time_since_epoch().count();
  }

  UNPROTECT(2);
  return x;
}

[[cpp11::register]]
SEXP add_weeks_or_days_local_cpp(SEXP x,
                                 SEXP n,
                                 SEXP unit,
                                 SEXP size) {
  enum unit c_unit = parse_unit(unit);
  r_ssize c_size = r_int_get(size, 0);

  return add_weeks_or_days_local(x, n, c_unit, c_size);
}

// -----------------------------------------------------------------------------

static SEXP add_hours_or_minutes_or_seconds_local(sexp x,
                                                  sexp n,
                                                  enum unit unit,
                                                  r_ssize size) {
  x = PROTECT(civil_rcrd_maybe_clone(x));
  x = PROTECT(civil_rcrd_recycle(x, size));

  int* p_days = civil_rcrd_days_deref(x);
  int* p_time_of_day = civil_rcrd_time_of_day_deref(x);
  int* p_nanos_of_second = civil_rcrd_nanos_of_second_deref(x);

  const bool recycle_n = r_is_scalar(n);
  const int* p_n = r_int_deref_const(n);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = p_days[i];
    int elt_time_of_day = p_time_of_day[i];
    int elt_n = recycle_n ? p_n[0] : p_n[i];

    if (elt_days == r_int_na) {
      continue;
    }
    if (elt_n == r_int_na) {
      civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
      continue;
    }

    std::chrono::seconds elt_chrono_time_of_day{elt_time_of_day};

    date::local_days elt_lday{date::days{elt_days}};
    date::local_seconds elt_lsec_floor{elt_lday};
    date::local_seconds elt_lsec = elt_lsec_floor + elt_chrono_time_of_day;

    std::chrono::seconds elt_chrono_n;

    if (unit == unit::hour) {
      elt_chrono_n = std::chrono::hours{elt_n};
    } else if (unit == unit::minute) {
      elt_chrono_n = std::chrono::minutes{elt_n};
    } else if (unit == unit::second) {
      elt_chrono_n = std::chrono::seconds{elt_n};
    }

    date::local_seconds out_lsec = elt_lsec + elt_chrono_n;

    date::local_days out_lday = date::floor<date::days>(out_lsec);
    date::local_seconds out_lsec_floor{out_lday};

    std::chrono::seconds out_chrono_time_of_day = out_lsec - out_lsec_floor;

    p_days[i] = out_lday.time_since_epoch().count();
    p_time_of_day[i] = out_chrono_time_of_day.count();
  }

  UNPROTECT(2);
  return x;
}

[[cpp11::register]]
SEXP add_hours_or_minutes_or_seconds_local_cpp(SEXP x,
                                               SEXP n,
                                               SEXP unit,
                                               SEXP size) {
  enum unit c_unit = parse_unit(unit);
  r_ssize c_size = r_int_get(size, 0);

  return add_hours_or_minutes_or_seconds_local(x, n, c_unit, c_size);
}

// -----------------------------------------------------------------------------

static SEXP add_milliseconds_or_microseconds_or_nanoseconds_local(sexp x,
                                                                  sexp n,
                                                                  enum unit unit,
                                                                  r_ssize size) {
  x = PROTECT(civil_rcrd_maybe_clone(x));
  x = PROTECT(civil_rcrd_recycle(x, size));

  int* p_days = civil_rcrd_days_deref(x);
  int* p_time_of_day = civil_rcrd_time_of_day_deref(x);
  int* p_nanos_of_second = civil_rcrd_nanos_of_second_deref(x);

  const bool recycle_n = r_is_scalar(n);
  const int* p_n = r_int_deref_const(n);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = p_days[i];
    int elt_time_of_day = p_time_of_day[i];
    int elt_nanos_of_second = p_nanos_of_second[i];
    int elt_n = recycle_n ? p_n[0] : p_n[i];

    if (elt_days == r_int_na) {
      continue;
    }
    if (elt_n == r_int_na) {
      civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
      continue;
    }

    std::chrono::seconds elt_chrono_time_of_day{elt_time_of_day};
    std::chrono::nanoseconds elt_chrono_nanos_of_second{elt_nanos_of_second};

    date::local_days elt_lday{date::days{elt_days}};
    date::local_seconds elt_lsec_floor{elt_lday};

    local_nanoseconds elt_lnanosec =
      elt_lsec_floor +
      elt_chrono_time_of_day +
      elt_chrono_nanos_of_second;

    std::chrono::nanoseconds elt_chrono_n;

    if (unit == unit::millisecond) {
      elt_chrono_n = std::chrono::milliseconds{elt_n};
    } else if (unit == unit::microsecond) {
      elt_chrono_n = std::chrono::microseconds{elt_n};
    } else if (unit == unit::nanosecond) {
      elt_chrono_n = std::chrono::nanoseconds{elt_n};
    }

    local_nanoseconds out_lnanosec = elt_lnanosec + elt_chrono_n;

    date::local_seconds out_lsec = date::floor<std::chrono::seconds>(out_lnanosec);
    local_nanoseconds out_lnanosec_floor{out_lsec};

    date::local_days out_lday = date::floor<date::days>(out_lsec);
    date::local_seconds out_lsec_floor{out_lday};

    std::chrono::nanoseconds out_chrono_nanos_of_second{out_lnanosec - out_lnanosec_floor};
    std::chrono::seconds out_chrono_time_of_day{out_lsec - out_lsec_floor};

    p_days[i] = out_lday.time_since_epoch().count();
    p_time_of_day[i] = out_chrono_time_of_day.count();
    p_nanos_of_second[i] = out_chrono_nanos_of_second.count();
  }

  UNPROTECT(2);
  return x;
}

[[cpp11::register]]
SEXP add_milliseconds_or_microseconds_or_nanoseconds_local_cpp(SEXP x,
                                                               SEXP n,
                                                               SEXP unit,
                                                               SEXP size) {
  enum unit c_unit = parse_unit(unit);
  r_ssize c_size = r_int_get(size, 0);

  return add_milliseconds_or_microseconds_or_nanoseconds_local(x, n, c_unit, c_size);
}

// -----------------------------------------------------------------------------

static SEXP add_hours_or_minutes_or_seconds_zoned(sexp x,
                                                  sexp n,
                                                  enum unit unit,
                                                  r_ssize size) {
  x = PROTECT(civil_rcrd_maybe_clone(x));
  x = PROTECT(civil_rcrd_recycle(x, size));

  int* p_days = civil_rcrd_days_deref(x);
  int* p_time_of_day = civil_rcrd_time_of_day_deref(x);
  int* p_nanos_of_second = civil_rcrd_nanos_of_second_deref(x);

  const bool recycle_n = r_is_scalar(n);
  const int* p_n = r_int_deref_const(n);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = p_days[i];
    int elt_time_of_day = p_time_of_day[i];
    int elt_n = recycle_n ? p_n[0] : p_n[i];

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

    if (unit == unit::hour) {
      elt_chrono_n = std::chrono::hours{elt_n};
    } else if (unit == unit::minute) {
      elt_chrono_n = std::chrono::minutes{elt_n};
    } else if (unit == unit::second) {
      elt_chrono_n = std::chrono::seconds{elt_n};
    }

    date::sys_seconds out_ssec = elt_ssec + elt_chrono_n;

    date::sys_days out_sday = date::floor<date::days>(out_ssec);
    date::sys_seconds out_ssec_floor{out_sday};

    std::chrono::seconds out_tod{out_ssec - out_ssec_floor};

    p_days[i] = out_sday.time_since_epoch().count();
    p_time_of_day[i] = out_tod.count();
  }

  UNPROTECT(2);
  return x;
}

[[cpp11::register]]
SEXP add_hours_or_minutes_or_seconds_zoned_cpp(SEXP x,
                                               SEXP n,
                                               SEXP unit,
                                               SEXP size) {
  enum unit c_unit = parse_unit(unit);
  r_ssize c_size = r_int_get(size, 0);

  return add_hours_or_minutes_or_seconds_zoned(x, n, c_unit, c_size);
}

// -----------------------------------------------------------------------------

static SEXP add_milliseconds_or_microseconds_or_nanoseconds_zoned(sexp x,
                                                                  sexp n,
                                                                  enum unit unit,
                                                                  r_ssize size) {
  x = PROTECT(civil_rcrd_maybe_clone(x));
  x = PROTECT(civil_rcrd_recycle(x, size));

  int* p_days = civil_rcrd_days_deref(x);
  int* p_time_of_day = civil_rcrd_time_of_day_deref(x);
  int* p_nanos_of_second = civil_rcrd_nanos_of_second_deref(x);

  const bool recycle_n = r_is_scalar(n);
  const int* p_n = r_int_deref_const(n);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = p_days[i];
    int elt_time_of_day = p_time_of_day[i];
    int elt_nanos_of_second = p_nanos_of_second[i];
    int elt_n = recycle_n ? p_n[0] : p_n[i];

    if (elt_days == r_int_na) {
      continue;
    }
    if (elt_n == r_int_na) {
      civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
      continue;
    }

    std::chrono::seconds elt_tod{elt_time_of_day};
    std::chrono::nanoseconds elt_nanos{elt_nanos_of_second};

    date::sys_days elt_sday{date::days{elt_days}};
    date::sys_seconds elt_ssec_floor{elt_sday};

    sys_nanoseconds elt_snanosec =
      elt_ssec_floor +
      elt_tod +
      elt_nanos;

    std::chrono::nanoseconds elt_chrono_n;

    if (unit == unit::millisecond) {
      elt_chrono_n = std::chrono::milliseconds{elt_n};
    } else if (unit == unit::microsecond) {
      elt_chrono_n = std::chrono::microseconds{elt_n};
    } else if (unit == unit::nanosecond) {
      elt_chrono_n = std::chrono::nanoseconds{elt_n};
    }

    sys_nanoseconds out_snanosec = elt_snanosec + elt_chrono_n;

    date::sys_seconds out_ssec = date::floor<std::chrono::seconds>(out_snanosec);
    sys_nanoseconds out_snanosec_floor{out_ssec};

    date::sys_days out_sday = date::floor<date::days>(out_ssec);
    date::sys_seconds out_ssec_floor{out_sday};

    std::chrono::nanoseconds out_nanos{out_snanosec - out_snanosec_floor};
    std::chrono::seconds out_tod{out_ssec - out_ssec_floor};

    p_days[i] = out_sday.time_since_epoch().count();
    p_time_of_day[i] = out_tod.count();
    p_nanos_of_second[i] = out_nanos.count();
  }

  UNPROTECT(2);
  return x;
}

[[cpp11::register]]
SEXP add_milliseconds_or_microseconds_or_nanoseconds_zoned_cpp(SEXP x,
                                                               SEXP n,
                                                               SEXP unit,
                                                               SEXP size) {
  enum unit c_unit = parse_unit(unit);
  r_ssize c_size = r_int_get(size, 0);

  return add_milliseconds_or_microseconds_or_nanoseconds_zoned(x, n, c_unit, c_size);
}

#include "r.h"
#include "utils.h"
#include "zone.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "local.h"
#include <date/date.h>
#include <date/tz.h>

// -----------------------------------------------------------------------------

static date::local_seconds add_ym_to_local_one(const date::local_seconds& lsec,
                                               int n,
                                               r_ssize i,
                                               const enum day_nonexistent& day_nonexistent,
                                               const enum unit& unit,
                                               bool& na);

static inline date::local_seconds add_dhms_to_local_one(const date::local_seconds& lsec,
                                                        const int& n,
                                                        const enum unit& unit);

// -----------------------------------------------------------------------------

static sexp add_period_to_zoned(sexp x,
                                sexp n,
                                enum day_nonexistent day_nonexistent,
                                enum dst_nonexistent dst_nonexistent,
                                enum dst_ambiguous dst_ambiguous,
                                r_ssize size,
                                enum unit unit);

[[cpp11::register]]
SEXP add_period_to_zoned_cpp(SEXP x,
                             SEXP n,
                             SEXP day_nonexistent,
                             SEXP dst_nonexistent,
                             SEXP dst_ambiguous,
                             SEXP size,
                             SEXP unit) {
  enum day_nonexistent c_day_nonexistent = parse_day_nonexistent(day_nonexistent);
  enum dst_nonexistent c_dst_nonexistent = parse_dst_nonexistent_arithmetic(dst_nonexistent);
  enum dst_ambiguous c_dst_ambiguous = parse_dst_ambiguous_arithmetic(dst_ambiguous);

  r_ssize c_size = r_int_get(size, 0);

  enum unit c_unit = parse_unit(unit);

  return add_period_to_zoned(
    x,
    n,
    c_day_nonexistent,
    c_dst_nonexistent,
    c_dst_ambiguous,
    c_size,
    c_unit
  );
}

static sexp add_ym_to_zoned(sexp x,
                            sexp n,
                            enum day_nonexistent day_nonexistent,
                            enum dst_nonexistent dst_nonexistent,
                            enum dst_ambiguous dst_ambiguous,
                            r_ssize size,
                            enum unit unit);

static sexp add_d_to_zoned(sexp x,
                           sexp n,
                           enum dst_nonexistent dst_nonexistent,
                           enum dst_ambiguous dst_ambiguous,
                           r_ssize size,
                           enum unit unit);

static sexp add_period_to_zoned(sexp x,
                                sexp n,
                                enum day_nonexistent day_nonexistent,
                                enum dst_nonexistent dst_nonexistent,
                                enum dst_ambiguous dst_ambiguous,
                                r_ssize size,
                                enum unit unit) {
  switch (unit) {
  case unit::year:
  case unit::month: {
    return add_ym_to_zoned(
      x,
      n,
      day_nonexistent,
      dst_nonexistent,
      dst_ambiguous,
      size,
      unit
    );
  }
  case unit::week:
  case unit::day: {
    return add_d_to_zoned(
      x,
      n,
      dst_nonexistent,
      dst_ambiguous,
      size,
      unit
    );
  }
  case unit::hour:
  case unit::minute:
  case unit::second: {
    r_abort("Internal error: hour/minute/second addition should be handled as a duration.");
  }
  }
}

// -----------------------------------------------------------------------------

static sexp add_ym_to_zoned(sexp x,
                            sexp n,
                            enum day_nonexistent day_nonexistent,
                            enum dst_nonexistent dst_nonexistent,
                            enum dst_ambiguous dst_ambiguous,
                            r_ssize size,
                            enum unit unit) {
  sexp out = PROTECT(r_new_double(size));
  double* p_out = r_dbl_deref(out);

  sexp tzone = civil_get_tzone(x);
  tzone = PROTECT(zone_standardize(tzone));
  std::string zone_name = zone_unwrap(tzone);
  const date::time_zone* p_zone = zone_name_load(zone_name);

  r_poke_names(out, r_get_names(x));
  r_poke_class(out, civil_classes_posixct);
  civil_poke_tzone(out, tzone);

  const double* p_x = r_dbl_deref_const(x);
  const int* p_n = r_int_deref_const(n);

  const bool recycle_x = r_is_scalar(x);
  const bool recycle_n = r_is_scalar(n);

  for (r_ssize i = 0; i < size; ++i) {
    double elt_x = recycle_x ? p_x[0] : p_x[i];
    int elt_n = recycle_n ? p_n[0] : p_n[i];

    int64_t elt = as_int64(elt_x);

    if (elt == r_int64_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    if (elt_n == NA_INTEGER) {
      p_out[i] = r_dbl_na;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::sys_seconds elt_ssec{elt_sec};
    date::zoned_seconds elt_zsec = date::make_zoned(p_zone, elt_ssec);
    date::local_seconds elt_lsec = elt_zsec.get_local_time();

    bool na = false;

    date::local_seconds out_lsec = add_ym_to_local_one(
      elt_lsec,
      elt_n,
      i,
      day_nonexistent,
      unit,
      na
    );

    if (na) {
      p_out[i] = NA_REAL;
      continue;
    }

    const enum dst_direction dst_direction =
      elt_n >= 0 ?
      dst_direction::forward :
      dst_direction::backward;

    p_out[i] = convert_local_seconds_to_posixt(
      out_lsec,
      p_zone,
      i,
      dst_direction,
      dst_nonexistent,
      dst_ambiguous
    );
  }

  UNPROTECT(2);
  return out;
}

// -----------------------------------------------------------------------------

static sexp add_d_to_zoned(sexp x,
                           sexp n,
                           enum dst_nonexistent dst_nonexistent,
                           enum dst_ambiguous dst_ambiguous,
                           r_ssize size,
                           enum unit unit) {
  sexp out = PROTECT(r_new_double(size));
  double* p_out = r_dbl_deref(out);

  sexp tzone = civil_get_tzone(x);
  tzone = PROTECT(zone_standardize(tzone));
  std::string zone_name = zone_unwrap(tzone);
  const date::time_zone* p_zone = zone_name_load(zone_name);

  r_poke_names(out, r_get_names(x));
  r_poke_class(out, civil_classes_posixct);
  civil_poke_tzone(out, tzone);

  const double* p_x = r_dbl_deref_const(x);
  const int* p_n = r_int_deref_const(n);

  const bool recycle_x = r_is_scalar(x);
  const bool recycle_n = r_is_scalar(n);

  for (r_ssize i = 0; i < size; ++i) {
    double elt_x = recycle_x ? p_x[0] : p_x[i];
    int elt_n = recycle_n ? p_n[0] : p_n[i];

    int64_t elt = as_int64(elt_x);

    if (elt == r_int64_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    if (elt_n == NA_INTEGER) {
      p_out[i] = r_dbl_na;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::sys_seconds elt_ssec{elt_sec};
    date::zoned_seconds elt_zsec = date::make_zoned(p_zone, elt_ssec);
    date::local_seconds elt_lsec = elt_zsec.get_local_time();

    date::local_seconds out_lsec = add_dhms_to_local_one(elt_lsec, elt_n, unit);

    const enum dst_direction dst_direction =
      elt_n >= 0 ?
      dst_direction::forward :
      dst_direction::backward;

    p_out[i] = convert_local_seconds_to_posixt(
      out_lsec,
      p_zone,
      i,
      dst_direction,
      dst_nonexistent,
      dst_ambiguous
    );
  }

  UNPROTECT(2);
  return out;
}

// -----------------------------------------------------------------------------

static sexp add_duration_to_zoned(sexp x,
                                  sexp n,
                                  r_ssize size,
                                  enum unit unit);

[[cpp11::register]]
SEXP add_duration_to_zoned_cpp(SEXP x,
                               SEXP n,
                               SEXP size,
                               SEXP unit) {
  r_ssize c_size = r_int_get(size, 0);
  enum unit c_unit = parse_unit(unit);

  return add_duration_to_zoned(
    x,
    n,
    c_size,
    c_unit
  );
}

static sexp add_hms_to_zoned(sexp x,
                             sexp n,
                             r_ssize size,
                             enum unit unit);

static sexp add_duration_to_zoned(sexp x,
                                  sexp n,
                                  r_ssize size,
                                  enum unit unit) {
  switch (unit) {
  case unit::year:
  case unit::month:
  case unit::week:
  case unit::day: {
    r_abort("Internal error: year/month/week/day addition should be handled as a period.");
  }
  case unit::hour:
  case unit::minute:
  case unit::second: {
    return add_hms_to_zoned(
      x,
      n,
      size,
      unit
    );
  }
  }
}

// -----------------------------------------------------------------------------

static inline date::sys_seconds add_hms_to_sys_one(const date::sys_seconds& ssec,
                                                   const int& n,
                                                   const enum unit& unit);

static sexp add_hms_to_zoned(sexp x,
                             sexp n,
                             r_ssize size,
                             enum unit unit) {
  sexp out = PROTECT(r_new_double(size));
  double* p_out = r_dbl_deref(out);

  sexp tzone = civil_get_tzone(x);
  tzone = PROTECT(zone_standardize(tzone));

  r_poke_names(out, r_get_names(x));
  r_poke_class(out, civil_classes_posixct);
  civil_poke_tzone(out, tzone);

  const double* p_x = r_dbl_deref_const(x);
  const int* p_n = r_int_deref_const(n);

  const bool recycle_x = r_is_scalar(x);
  const bool recycle_n = r_is_scalar(n);

  for (r_ssize i = 0; i < size; ++i) {
    double elt_x = recycle_x ? p_x[0] : p_x[i];
    int elt_n = recycle_n ? p_n[0] : p_n[i];

    int64_t elt = as_int64(elt_x);

    if (elt == r_int64_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    if (elt_n == NA_INTEGER) {
      p_out[i] = r_dbl_na;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::sys_seconds elt_ssec{elt_sec};

    date::sys_seconds out_ssec = add_hms_to_sys_one(
      elt_ssec,
      elt_n,
      unit
    );

    p_out[i] = out_ssec.time_since_epoch().count();
  }

  UNPROTECT(2);
  return out;
}

// -----------------------------------------------------------------------------

static sexp add_period_to_local(sexp x,
                                sexp n,
                                enum day_nonexistent day_nonexistent,
                                r_ssize size,
                                enum unit unit);

[[cpp11::register]]
SEXP add_period_to_local_cpp(SEXP x,
                             SEXP n,
                             SEXP day_nonexistent,
                             SEXP size,
                             SEXP unit) {
  enum day_nonexistent c_day_nonexistent = parse_day_nonexistent(day_nonexistent);

  r_ssize c_size = r_int_get(size, 0);

  enum unit c_unit = parse_unit(unit);

  return add_period_to_local(
    x,
    n,
    c_day_nonexistent,
    c_size,
    c_unit
  );
}

static sexp add_ym_to_local(sexp x,
                            sexp n,
                            r_ssize size,
                            enum unit unit);

static sexp add_dhms_to_local(sexp x,
                              sexp n,
                              enum day_nonexistent day_nonexistent,
                              r_ssize size,
                              enum unit unit);

static sexp add_period_to_local(sexp x,
                                sexp n,
                                enum day_nonexistent day_nonexistent,
                                r_ssize size,
                                enum unit unit) {
  switch (unit) {
  case unit::year:
  case unit::month: {
    return add_ym_to_local(
      x,
      n,
      size,
      unit
    );
  }
  case unit::week:
  case unit::day:
  case unit::hour:
  case unit::minute:
  case unit::second: {
    return add_dhms_to_local(
      x,
      n,
      day_nonexistent,
      size,
      unit
    );
  }
  }
}

// -----------------------------------------------------------------------------

static sexp add_ym_to_local(sexp x,
                            sexp n,
                            r_ssize size,
                            enum unit unit) {
  x = PROTECT(r_maybe_clone(x));
  x = PROTECT(local_datetime_recycle(x, size));

  sexp year = r_maybe_clone(local_datetime_year(x));
  local_datetime_poke_year(x, year);

  sexp month = r_maybe_clone(local_datetime_month(x));
  local_datetime_poke_month(x, month);

  int* p_year = r_int_deref(year);
  int* p_month = r_int_deref(month);

  const bool recycle_n = r_is_scalar(n);

  const int* p_n = r_int_deref_const(n);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_year = p_year[i];
    int elt_month = p_month[i];
    int elt_n = recycle_n ? p_n[0] : p_n[i];

    if (elt_year == r_int_na) {
      continue;
    }
    if (elt_n == r_int_na) {
      p_year[i] = r_int_na;
      p_month[i] = r_int_na;
      continue;
    }

    unsigned int elt_date_month = static_cast<unsigned int>(elt_month);

    date::year_month_day elt_ymd{
      date::year{elt_year} / date::month{elt_date_month} / date::day{1}
    };

    date::year_month_day out_ymd;

    if (unit == unit::year) {
      out_ymd = elt_ymd + date::years{elt_n};
    } else {
      out_ymd = elt_ymd + date::months{elt_n};
    }

    p_year[i] = static_cast<int>(out_ymd.year());
    p_month[i] = static_cast<unsigned int>(out_ymd.month());
  }

  UNPROTECT(2);
  return x;
}

// -----------------------------------------------------------------------------

static sexp add_dhms_to_local(sexp x,
                              sexp n,
                              enum day_nonexistent day_nonexistent,
                              r_ssize size,
                              enum unit unit) {
  x = PROTECT(r_maybe_clone(x));
  x = PROTECT(local_datetime_recycle(x, size));

  sexp year = r_maybe_clone(local_datetime_year(x));
  local_datetime_poke_year(x, year);

  sexp month = r_maybe_clone(local_datetime_month(x));
  local_datetime_poke_month(x, month);

  sexp day = r_maybe_clone(local_datetime_day(x));
  local_datetime_poke_day(x, day);

  sexp hour = r_maybe_clone(local_datetime_hour(x));
  local_datetime_poke_hour(x, hour);

  sexp minute = r_maybe_clone(local_datetime_minute(x));
  local_datetime_poke_minute(x, minute);

  sexp second = r_maybe_clone(local_datetime_second(x));
  local_datetime_poke_second(x, second);

  int* p_year = r_int_deref(year);
  int* p_month = r_int_deref(month);
  int* p_day = r_int_deref(day);
  int* p_hour = r_int_deref(hour);
  int* p_minute = r_int_deref(minute);
  int* p_second = r_int_deref(second);

  const bool recycle_n = r_is_scalar(n);

  const int* p_n = r_int_deref_const(n);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_year = p_year[i];
    int elt_month = p_month[i];
    int elt_day = p_day[i];
    int elt_hour = p_hour[i];
    int elt_minute = p_minute[i];
    int elt_second = p_second[i];

    int elt_n = recycle_n ? p_n[0] : p_n[i];

    if (elt_year == r_int_na) {
      continue;
    }

    if (elt_n == r_int_na) {
      p_year[i] = r_int_na;
      p_month[i] = r_int_na;
      p_day[i] = r_int_na;
      p_hour[i] = r_int_na;
      p_minute[i] = r_int_na;
      p_second[i] = r_int_na;
      continue;
    }

    unsigned int elt_date_month = static_cast<unsigned int>(elt_month);
    unsigned int elt_date_day = static_cast<unsigned int>(elt_day);

    date::year_month_day elt_ymd{
      date::year{elt_year} / date::month{elt_date_month} / date::day{elt_date_day}
    };

    std::chrono::hours elt_chrono_hour{elt_hour};
    std::chrono::minutes elt_chrono_minute{elt_minute};
    std::chrono::seconds elt_chrono_second{elt_second};

    std::chrono::seconds elt_tod = elt_chrono_hour + elt_chrono_minute + elt_chrono_second;

    if (!elt_ymd.ok()) {
      bool na = false;

      resolve_day_nonexistent(i, day_nonexistent, elt_ymd, elt_tod, na);

      if (na) {
        p_year[i] = r_int_na;
        p_month[i] = r_int_na;
        p_day[i] = r_int_na;
        p_hour[i] = r_int_na;
        p_minute[i] = r_int_na;
        p_second[i] = r_int_na;
        continue;
      }
    }

    date::local_days elt_lday{elt_ymd};
    date::local_seconds elt_lsec_floor{elt_lday};
    date::local_seconds elt_lsec = elt_lsec_floor + elt_tod;

    date::local_seconds out_lsec = add_dhms_to_local_one(elt_lsec, elt_n, unit);

    date::local_days out_lday = date::floor<date::days>(out_lsec);
    date::local_seconds out_lsec_floor{out_lday};

    date::year_month_day out_ymd{out_lday};
    date::hh_mm_ss<std::chrono::seconds> out_tod = date::make_time(out_lsec - out_lsec_floor);

    p_year[i] = static_cast<int>(out_ymd.year());
    p_month[i] = static_cast<unsigned int>(out_ymd.month());
    p_day[i] = static_cast<unsigned int>(out_ymd.day());
    p_hour[i] = out_tod.hours().count();
    p_minute[i] = out_tod.minutes().count();
    p_second[i] = out_tod.seconds().count();
  }

  UNPROTECT(2);
  return x;
}

// -----------------------------------------------------------------------------

static date::local_seconds add_ym_to_local_one(const date::local_seconds& lsec,
                                               int n,
                                               r_ssize i,
                                               const enum day_nonexistent& day_nonexistent,
                                               const enum unit& unit,
                                               bool& na) {
  date::local_days lday = date::floor<date::days>(lsec);
  date::local_seconds lsec_floor = lday;

  std::chrono::seconds time_of_day = lsec - lsec_floor;

  date::year_month_day ymd{lday};

  date::year_month_day out_ymd;

  if (unit == unit::year) {
    out_ymd = ymd + date::years{n};
  } else {
    out_ymd = ymd + date::months{n};
  }

  if (!out_ymd.ok()) {
    bool na_resolve = false;

    resolve_day_nonexistent(i, day_nonexistent, out_ymd, time_of_day, na_resolve);

    if (na_resolve) {
      na = true;
      return lsec;
    }
  }

  date::local_days out_lday{out_ymd};
  date::local_seconds out_lsec_floor{out_lday};
  date::local_seconds out_lsec{out_lsec_floor + time_of_day};

  return out_lsec;
}

// -----------------------------------------------------------------------------

static inline date::local_seconds add_dhms_to_local_one(const date::local_seconds& lsec,
                                                        const int& n,
                                                        const enum unit& unit) {
  switch (unit) {
  case unit::week: {
    return lsec + date::days{n * 7};
  }
  case unit::day: {
    return lsec + date::days{n};
  }
  case unit::hour: {
    return lsec + std::chrono::hours{n};
  }
  case unit::minute: {
    return lsec + std::chrono::minutes{n};
  }
  case unit::second: {
    return lsec + std::chrono::seconds{n};
  }
  case unit::year:
  case unit::month: {
    r_abort("Internal error: year/month unit in day/hour/minute/second handler.");
  }
  }
}

// -----------------------------------------------------------------------------

static inline date::sys_seconds add_hms_to_sys_one(const date::sys_seconds& ssec,
                                                   const int& n,
                                                   const enum unit& unit) {
  switch (unit) {
  case unit::hour: {
    return ssec + std::chrono::hours{n};
  }
  case unit::minute: {
    return ssec + std::chrono::minutes{n};
  }
  case unit::second: {
    return ssec + std::chrono::seconds{n};
  }
  case unit::year:
  case unit::month:
  case unit::week:
  case unit::day: {
    r_abort("Internal error: year/month/week/day unit in hour/minute/second handler.");
  }
  }
}

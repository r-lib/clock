#include "civil.h"
#include "utils.h"
#include "civil-rcrd.h"
#include "zone.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "check.h"

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_rcrd convert_sys_seconds_to_naive_days_and_time_of_day(const cpp11::doubles& seconds,
                                                                      const cpp11::strings& zone) {
  r_ssize size = seconds.size();

  civil_writable_field days(size);
  civil_writable_field time_of_day(size);

  civil_writable_rcrd out = new_days_time_of_day_list(days, time_of_day);

  cpp11::writable::strings zone_standard = zone_standardize(zone);
  cpp11::r_string zone_name_r(zone_standard[0]);
  std::string zone_name(zone_name_r);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  for (r_ssize i = 0; i < size; ++i) {
    double elt_seconds = seconds[i];
    int64_t elt = as_int64(elt_seconds);

    if (elt == r_int64_na) {
      days[i] = r_int_na;
      time_of_day[i] = r_int_na;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::sys_seconds elt_ssec{elt_sec};
    date::zoned_seconds elt_zsec = date::make_zoned(p_time_zone, elt_ssec);
    date::local_seconds elt_lsec = elt_zsec.get_local_time();

    date::local_days elt_lday = date::floor<date::days>(elt_lsec);
    date::local_seconds elt_lsec_floor{elt_lday};

    std::chrono::seconds elt_tod_sec{elt_lsec - elt_lsec_floor};

    days[i] = elt_lday.time_since_epoch().count();
    time_of_day[i] = elt_tod_sec.count();
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::doubles convert_naive_days_and_time_of_day_to_sys_seconds_cpp(const civil_field& days,
                                                                               const civil_field& time_of_day,
                                                                               const cpp11::strings& zone,
                                                                               const cpp11::strings& dst_nonexistent,
                                                                               const cpp11::strings& dst_ambiguous,
                                                                               const cpp11::integers& size) {
  r_ssize c_size = size[0];

  cpp11::writable::doubles out(c_size);

  cpp11::writable::strings zone_standard = zone_standardize(zone);
  cpp11::r_string zone_name_r(zone_standard[0]);
  std::string zone_name(zone_name_r);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  bool recycle_days = civil_is_scalar(days);
  bool recycle_time_of_day = civil_is_scalar(time_of_day);
  bool recycle_dst_nonexistent = civil_is_scalar(dst_nonexistent);
  bool recycle_dst_ambiguous = civil_is_scalar(dst_ambiguous);

  enum dst_nonexistent dst_nonexistent_val;
  if (recycle_dst_nonexistent) {
    dst_nonexistent_val = parse_dst_nonexistent_one(dst_nonexistent[0]);
  }

  enum dst_ambiguous dst_ambiguous_val;
  if (recycle_dst_ambiguous) {
    dst_ambiguous_val = parse_dst_ambiguous_one(dst_ambiguous[0]);
  }

  for (r_ssize i = 0; i < c_size; ++i) {
    const int elt_days = recycle_days ? days[0] : days[i];
    const int elt_time_of_day = recycle_time_of_day ? time_of_day[0] : time_of_day[i];

    const enum dst_nonexistent elt_dst_nonexistent_val =
      recycle_dst_nonexistent ?
      dst_nonexistent_val :
      parse_dst_nonexistent_one(dst_nonexistent[i]);

    const enum dst_ambiguous elt_dst_ambiguous_val =
      recycle_dst_ambiguous ?
      dst_ambiguous_val :
      parse_dst_ambiguous_one(dst_ambiguous[i]);

    if (elt_days == r_int_na || elt_time_of_day == r_int_na) {
      out[i] = r_dbl_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_days}};
    date::local_seconds elt_lsec_floor{elt_lday};

    std::chrono::seconds elt_tod{elt_time_of_day};

    date::local_seconds elt_lsec = elt_lsec_floor + elt_tod;

    bool na = false;

    date::sys_seconds out_ssec = convert_local_to_sys(
      elt_lsec,
      p_time_zone,
      i,
      elt_dst_nonexistent_val,
      elt_dst_ambiguous_val,
      na
    );

    if (na) {
      out[i] = r_dbl_na;
    } else {
      out[i] = out_ssec.time_since_epoch().count();
    }
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_rcrd convert_year_month_day_to_naive_fields(const cpp11::integers& year,
                                                           const cpp11::integers& month,
                                                           const cpp11::integers& day,
                                                           const cpp11::strings& day_nonexistent) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);

  r_ssize size = year.size();

  civil_writable_field days(size);

  civil_writable_rcrd out = new_days_list(days);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_year = year[i];
    int elt_month = month[i];
    int elt_day = day[i];

    if (elt_year == r_int_na ||
        elt_month == r_int_na ||
        elt_day == r_int_na) {
      days[i] = r_int_na;
      continue;
    }

    check_range_year(elt_year, "year");
    check_range_month(elt_month, "month");
    check_range_day(elt_day, "day");

    unsigned int elt_date_month = static_cast<unsigned int>(elt_month);
    unsigned int elt_date_day = static_cast<unsigned int>(elt_day);

    date::year_month_day out_ymd{
      date::year{elt_year} / date::month{elt_date_month} / date::day{elt_date_day}
    };

    if (!out_ymd.ok()) {
      bool na = false;
      resolve_day_nonexistent_ymd(i, day_nonexistent_val, out_ymd, na);

      if (na) {
        days[i] = r_int_na;
        continue;
      }
    }

    date::local_days out_lday{out_ymd};

    days[i] = out_lday.time_since_epoch().count();
  }

  return out;
}

[[cpp11::register]]
civil_writable_rcrd convert_year_month_day_hour_minute_second_to_naive_fields_cpp(const cpp11::integers& year,
                                                                                  const cpp11::integers& month,
                                                                                  const cpp11::integers& day,
                                                                                  const cpp11::integers& hour,
                                                                                  const cpp11::integers& minute,
                                                                                  const cpp11::integers& second,
                                                                                  const cpp11::strings& day_nonexistent) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);

  r_ssize size = year.size();

  civil_writable_field days(size);
  civil_writable_field time_of_day(size);

  civil_writable_rcrd out = new_days_time_of_day_list(days, time_of_day);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_year = year[i];
    int elt_month = month[i];
    int elt_day = day[i];
    int elt_hour = hour[i];
    int elt_minute = minute[i];
    int elt_second = second[i];

    if (elt_year == r_int_na ||
        elt_month == r_int_na ||
        elt_day == r_int_na ||
        elt_hour == r_int_na ||
        elt_minute == r_int_na ||
        elt_second == r_int_na) {
      days[i] = r_int_na;
      time_of_day[i] = r_int_na;
      continue;
    }

    check_range_year(elt_year, "year");
    check_range_month(elt_month, "month");
    check_range_day(elt_day, "day");
    check_range_hour(elt_hour, "hour");
    check_range_minute(elt_minute, "minute");
    check_range_second(elt_second, "second");

    std::chrono::seconds out_tod =
      std::chrono::hours{elt_hour} +
      std::chrono::minutes{elt_minute} +
      std::chrono::seconds{elt_second};

    unsigned int elt_date_month = static_cast<unsigned int>(elt_month);
    unsigned int elt_date_day = static_cast<unsigned int>(elt_day);

    date::year_month_day out_ymd{
      date::year{elt_year} / date::month{elt_date_month} / date::day{elt_date_day}
    };

    if (!out_ymd.ok()) {
      bool na = false;
      resolve_day_nonexistent_ymd(i, day_nonexistent_val, out_ymd, na);
      resolve_day_nonexistent_tod(day_nonexistent_val, out_tod);

      if (na) {
        days[i] = r_int_na;
        time_of_day[i] = r_int_na;
        continue;
      }
    }

    date::local_days out_lday{out_ymd};

    days[i] = out_lday.time_since_epoch().count();
    time_of_day[i] = out_tod.count();
  }

  return out;
}

[[cpp11::register]]
civil_writable_rcrd convert_year_month_day_hour_minute_second_nanos_to_naive_fields_cpp(const cpp11::integers& year,
                                                                                        const cpp11::integers& month,
                                                                                        const cpp11::integers& day,
                                                                                        const cpp11::integers& hour,
                                                                                        const cpp11::integers& minute,
                                                                                        const cpp11::integers& second,
                                                                                        const cpp11::integers& nanos,
                                                                                        const cpp11::strings& day_nonexistent) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);

  r_ssize size = year.size();

  civil_writable_field days(size);
  civil_writable_field time_of_day(size);
  civil_writable_field nanos_of_second(size);

  civil_writable_rcrd out = new_days_time_of_day_nanos_of_second_list(days, time_of_day, nanos_of_second);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_year = year[i];
    int elt_month = month[i];
    int elt_day = day[i];
    int elt_hour = hour[i];
    int elt_minute = minute[i];
    int elt_second = second[i];
    int elt_nanos = nanos[i];

    if (elt_year == r_int_na ||
        elt_month == r_int_na ||
        elt_day == r_int_na ||
        elt_hour == r_int_na ||
        elt_minute == r_int_na ||
        elt_second == r_int_na ||
        elt_nanos == r_int_na) {
      days[i] = r_int_na;
      time_of_day[i] = r_int_na;
      nanos_of_second[i] = r_int_na;
      continue;
    }

    check_range_year(elt_year, "year");
    check_range_month(elt_month, "month");
    check_range_day(elt_day, "day");
    check_range_hour(elt_hour, "hour");
    check_range_minute(elt_minute, "minute");
    check_range_second(elt_second, "second");
    check_range_nanos(elt_nanos, "nanos");

    std::chrono::nanoseconds out_nanos_of_second{elt_nanos};

    std::chrono::seconds out_tod =
      std::chrono::hours{elt_hour} +
      std::chrono::minutes{elt_minute} +
      std::chrono::seconds{elt_second};

    unsigned int elt_date_month = static_cast<unsigned int>(elt_month);
    unsigned int elt_date_day = static_cast<unsigned int>(elt_day);

    date::year_month_day out_ymd{
      date::year{elt_year} / date::month{elt_date_month} / date::day{elt_date_day}
    };

    if (!out_ymd.ok()) {
      bool na = false;
      resolve_day_nonexistent_ymd(i, day_nonexistent_val, out_ymd, na);
      resolve_day_nonexistent_tod(day_nonexistent_val, out_tod);
      resolve_day_nonexistent_nanos_of_second(day_nonexistent_val, out_nanos_of_second);

      if (na) {
        days[i] = r_int_na;
        time_of_day[i] = r_int_na;
        nanos_of_second[i] = r_int_na;
        continue;
      }
    }

    date::local_days out_lday{out_ymd};

    days[i] = out_lday.time_since_epoch().count();
    time_of_day[i] = out_tod.count();
    nanos_of_second[i] = out_nanos_of_second.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_list_of_integers convert_naive_days_to_year_month_day_cpp(const civil_field& days) {
  r_ssize size = days.size();

  cpp11::writable::integers year(size);
  cpp11::writable::integers month(size);
  cpp11::writable::integers day(size);

  civil_writable_list_of_integers out({year, month, day});
  out.names() = {"year", "month", "day"};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = days[i];

    if (elt_days == r_int_na) {
      year[i] = r_int_na;
      month[i] = r_int_na;
      day[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_days}};
    date::year_month_day elt_ymd{elt_lday};

    year[i] = static_cast<int>(elt_ymd.year());
    month[i] = static_cast<unsigned int>(elt_ymd.month());
    day[i] = static_cast<unsigned int>(elt_ymd.day());
  }

  return out;
}

[[cpp11::register]]
civil_writable_list_of_integers convert_naive_time_of_day_to_hour_minute_second_cpp(const civil_field& time_of_day) {
  r_ssize size = time_of_day.size();

  cpp11::writable::integers hour(size);
  cpp11::writable::integers minute(size);
  cpp11::writable::integers second(size);

  civil_writable_list_of_integers out({hour, minute, second});
  out.names() = {"hour", "minute", "second"};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_time_of_day = time_of_day[i];

    if (elt_time_of_day == r_int_na) {
      hour[i] = r_int_na;
      minute[i] = r_int_na;
      second[i] = r_int_na;
      continue;
    }

    std::chrono::seconds elt_tod_sec{elt_time_of_day};

    date::hh_mm_ss<std::chrono::seconds> elt_hms = date::make_time(elt_tod_sec);

    hour[i] = elt_hms.hours().count();
    minute[i] = elt_hms.minutes().count();
    second[i] = elt_hms.seconds().count();
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_rcrd convert_datetime_fields_from_naive_to_zoned_cpp(const civil_field& days,
                                                                    const civil_field& time_of_day,
                                                                    const cpp11::strings& zone,
                                                                    const cpp11::strings& dst_nonexistent,
                                                                    const cpp11::strings& dst_ambiguous,
                                                                    const cpp11::integers& size) {
  r_ssize c_size = size[0];

  civil_writable_field out_days(c_size);
  civil_writable_field out_time_of_day(c_size);

  civil_writable_rcrd out = new_days_time_of_day_list(out_days, out_time_of_day);

  bool recycle_days = civil_is_scalar(days);
  bool recycle_time_of_day = civil_is_scalar(time_of_day);
  bool recycle_dst_nonexistent = civil_is_scalar(dst_nonexistent);
  bool recycle_dst_ambiguous = civil_is_scalar(dst_ambiguous);

  enum dst_nonexistent dst_nonexistent_val;
  if (recycle_dst_nonexistent) {
    dst_nonexistent_val = parse_dst_nonexistent_one(dst_nonexistent[0]);
  }

  enum dst_ambiguous dst_ambiguous_val;
  if (recycle_dst_ambiguous) {
    dst_ambiguous_val = parse_dst_ambiguous_one(dst_ambiguous[0]);
  }

  cpp11::writable::strings zone_standard = zone_standardize(zone);
  cpp11::r_string zone_name_r(zone_standard[0]);
  std::string zone_name(zone_name_r);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  for (r_ssize i = 0; i < c_size; ++i) {
    int elt_days = recycle_days ? days[0] : days[i];
    int elt_time_of_day = recycle_time_of_day ? time_of_day[0] : time_of_day[i];

    const enum dst_nonexistent elt_dst_nonexistent_val =
      recycle_dst_nonexistent ?
      dst_nonexistent_val :
      parse_dst_nonexistent_one(dst_nonexistent[i]);

    const enum dst_ambiguous elt_dst_ambiguous_val =
      recycle_dst_ambiguous ?
      dst_ambiguous_val :
      parse_dst_ambiguous_one(dst_ambiguous[i]);

    if (elt_days == r_int_na) {
      out_days[i] = r_int_na;
      out_time_of_day[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_days}};
    std::chrono::seconds elt_tod{elt_time_of_day};

    date::local_seconds elt_lsec_floor{elt_lday};
    date::local_seconds elt_lsec = elt_lsec_floor + elt_tod;

    bool na = false;

    date::sys_seconds out_ssec = convert_local_to_sys(
      elt_lsec,
      p_time_zone,
      i,
      elt_dst_nonexistent_val,
      elt_dst_ambiguous_val,
      na
    );

    if (na) {
      out_days[i] = r_int_na;
      out_time_of_day[i] = r_int_na;
      continue;
    }

    date::sys_days out_sday = date::floor<date::days>(out_ssec);
    date::sys_seconds out_ssec_floor{out_sday};

    std::chrono::seconds out_tod{out_ssec - out_ssec_floor};

    out_days[i] = out_sday.time_since_epoch().count();
    out_time_of_day[i] = out_tod.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_rcrd convert_nano_datetime_fields_from_naive_to_zoned_cpp(const civil_field& days,
                                                                         const civil_field& time_of_day,
                                                                         const civil_field& nanos_of_second,
                                                                         const cpp11::strings& zone,
                                                                         const cpp11::strings& dst_nonexistent,
                                                                         const cpp11::strings& dst_ambiguous,
                                                                         const cpp11::integers& size) {
  r_ssize c_size = size[0];

  civil_writable_field out_days(c_size);
  civil_writable_field out_time_of_day(c_size);
  civil_writable_field out_nanos_of_second(c_size);

  civil_writable_rcrd out = new_days_time_of_day_nanos_of_second_list(
    out_days,
    out_time_of_day,
    out_nanos_of_second
  );

  bool recycle_days = civil_is_scalar(days);
  bool recycle_time_of_day = civil_is_scalar(time_of_day);
  bool recycle_nanos_of_second = civil_is_scalar(nanos_of_second);
  bool recycle_dst_nonexistent = civil_is_scalar(dst_nonexistent);
  bool recycle_dst_ambiguous = civil_is_scalar(dst_ambiguous);

  enum dst_nonexistent dst_nonexistent_val;
  if (recycle_dst_nonexistent) {
    dst_nonexistent_val = parse_dst_nonexistent_one(dst_nonexistent[0]);
  }

  enum dst_ambiguous dst_ambiguous_val;
  if (recycle_dst_ambiguous) {
    dst_ambiguous_val = parse_dst_ambiguous_one(dst_ambiguous[0]);
  }

  cpp11::writable::strings zone_standard = zone_standardize(zone);
  cpp11::r_string zone_name_r(zone_standard[0]);
  std::string zone_name(zone_name_r);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  for (r_ssize i = 0; i < c_size; ++i) {
    int elt_days = recycle_days ? days[0] : days[i];
    int elt_time_of_day = recycle_time_of_day ? time_of_day[0] : time_of_day[i];
    int elt_nanos_of_second = recycle_nanos_of_second ? nanos_of_second[0] : nanos_of_second[i];

    const enum dst_nonexistent elt_dst_nonexistent_val =
      recycle_dst_nonexistent ?
      dst_nonexistent_val :
      parse_dst_nonexistent_one(dst_nonexistent[i]);

    const enum dst_ambiguous elt_dst_ambiguous_val =
      recycle_dst_ambiguous ?
      dst_ambiguous_val :
      parse_dst_ambiguous_one(dst_ambiguous[i]);

    if (elt_days == r_int_na) {
      out_days[i] = r_int_na;
      out_time_of_day[i] = r_int_na;
      out_nanos_of_second[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_days}};
    std::chrono::seconds elt_tod{elt_time_of_day};
    std::chrono::nanoseconds elt_nanos{elt_nanos_of_second};

    date::local_seconds elt_lsec_floor{elt_lday};
    date::local_seconds elt_lsec = elt_lsec_floor + elt_tod;

    bool na = false;

    date::sys_seconds out_ssec = convert_local_to_sys(
      elt_lsec,
      p_time_zone,
      i,
      elt_dst_nonexistent_val,
      elt_dst_ambiguous_val,
      na,
      elt_nanos
    );

    if (na) {
      out_days[i] = r_int_na;
      out_time_of_day[i] = r_int_na;
      out_nanos_of_second[i] = r_int_na;
      continue;
    }

    date::sys_days out_sday = date::floor<date::days>(out_ssec);
    date::sys_seconds out_ssec_floor{out_sday};

    std::chrono::seconds out_tod{out_ssec - out_ssec_floor};

    std::chrono::nanoseconds out_nanos{elt_nanos};

    out_days[i] = out_sday.time_since_epoch().count();
    out_time_of_day[i] = out_tod.count();
    out_nanos_of_second[i] = out_nanos.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

/*
 * Same for datetime and nano_datetime, since nanoseconds wont change when
 * going from zoned->naive. They are "beneath" time zone changes.
 */
[[cpp11::register]]
civil_writable_rcrd convert_datetime_fields_from_zoned_to_naive_cpp(const civil_field& days,
                                                                    const civil_field& time_of_day,
                                                                    const cpp11::strings& zone) {
  r_ssize size = days.size();

  civil_writable_field out_days(size);
  civil_writable_field out_time_of_day(size);

  civil_writable_rcrd out = new_days_time_of_day_list(
    out_days,
    out_time_of_day
  );

  cpp11::writable::strings zone_standard = zone_standardize(zone);
  cpp11::r_string zone_name_r(zone_standard[0]);
  std::string zone_name(zone_name_r);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = days[i];
    int elt_time_of_day = time_of_day[i];

    if (elt_days == r_int_na) {
      out_days[i] = r_int_na;
      out_time_of_day[i] = r_int_na;
      continue;
    }

    date::sys_days elt_sday{date::days{elt_days}};
    std::chrono::seconds elt_tod{elt_time_of_day};

    date::sys_seconds elt_ssec_floor{elt_sday};
    date::sys_seconds elt_ssec = elt_ssec_floor + elt_tod;

    date::zoned_seconds out_zsec = date::make_zoned(p_time_zone, elt_ssec);
    date::local_seconds out_lsec = out_zsec.get_local_time();

    date::local_days out_lday = date::floor<date::days>(out_lsec);
    date::local_seconds out_lsec_floor{out_lday};

    std::chrono::seconds out_tod{out_lsec - out_lsec_floor};

    out_days[i] = out_lday.time_since_epoch().count();
    out_time_of_day[i] = out_tod.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_rcrd convert_sys_seconds_to_sys_days_and_time_of_day_cpp(const cpp11::doubles& seconds) {
  r_ssize size = seconds.size();

  civil_writable_field out_days(size);
  civil_writable_field out_time_of_day(size);

  civil_writable_rcrd out = new_days_time_of_day_list(
    out_days,
    out_time_of_day
  );

  for (r_ssize i = 0; i < size; ++i) {
    double elt_seconds = seconds[i];
    int64_t elt = as_int64(elt_seconds);

    if (elt == r_int64_na) {
      out_days[i] = r_int_na;
      out_time_of_day[i] = r_int_na;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::sys_seconds elt_ssec{elt_sec};

    date::sys_days elt_sday = date::floor<date::days>(elt_ssec);
    date::sys_seconds elt_ssec_floor{elt_sday};

    std::chrono::seconds elt_tod{elt_ssec - elt_ssec_floor};

    out_days[i] = elt_sday.time_since_epoch().count();
    out_time_of_day[i] = elt_tod.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_field convert_fiscal_year_quarter_day_to_naive_days_cpp(const cpp11::integers& year,
                                                                       const cpp11::integers& quarter,
                                                                       const cpp11::integers& day,
                                                                       int fiscal_start,
                                                                       const cpp11::strings& day_nonexistent) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);

  r_ssize size = year.size();

  cpp11::writable::integers out(size);

  check_range_fiscal_start(fiscal_start, "fiscal_start");
  fiscal_year::fiscal_start start{static_cast<unsigned int>(fiscal_start)};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_year = year[i];
    int elt_quarter = quarter[i];
    int elt_day = day[i];

    if (elt_year == r_int_na || elt_quarter == r_int_na || elt_day == r_int_na) {
      out[i] = r_int_na;
      continue;
    }

    check_range_year(elt_year, "year");
    check_range_quarter(elt_quarter, "quarter");
    check_range_quarter_day(elt_day, "day");

    fiscal_year::year_quarter_day elt_yqd{
      fiscal_year::year{elt_year},
      fiscal_year::quarter{static_cast<unsigned>(elt_quarter)},
      fiscal_year::day{static_cast<unsigned>(elt_day)},
      start
    };

    if (!elt_yqd.ok()) {
      bool na = false;
      resolve_day_nonexistent_yqd(i, day_nonexistent_val, elt_yqd, na);

      if (na) {
        out[i] = r_int_na;
        continue;
      }
    }

    date::local_days elt_lday{elt_yqd};

    out[i] = elt_lday.time_since_epoch().count();
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::list_of<cpp11::writable::integers>
convert_naive_days_to_fiscal_year_quarter_day_cpp(const civil_field& days, int fiscal_start) {
  r_ssize size = days.size();

  cpp11::writable::integers out_year(size);
  cpp11::writable::integers out_quarter(size);
  cpp11::writable::integers out_day(size);

  cpp11::writable::list out{out_year, out_quarter, out_day};
  out.names() = {"year", "quarter", "day"};

  check_range_fiscal_start(fiscal_start, "fiscal_start");
  fiscal_year::fiscal_start fs{static_cast<unsigned int>(fiscal_start)};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = days[i];

    if (elt_days == r_int_na) {
      out_year[i] = r_int_na;
      out_quarter[i] = r_int_na;
      out_day[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_days}};
    fiscal_year::year_quarter_day elt_yqd(elt_lday, fs);

    out_year[i] = static_cast<int>(elt_yqd.year());
    out_quarter[i] = static_cast<unsigned int>(elt_yqd.quarter());
    out_day[i] = static_cast<unsigned int>(elt_yqd.day());
  }

  return out;
}

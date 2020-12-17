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
civil_writable_rcrd convert_zoned_seconds_to_naive_second_point_fields(const cpp11::doubles& seconds,
                                                                       const cpp11::strings& zone) {
  r_ssize size = seconds.size();

  civil_writable_field days(size);
  civil_writable_field seconds_of_day(size);

  civil_writable_rcrd out({days, seconds_of_day});
  out.names() = {"days", "seconds_of_day"};

  cpp11::writable::strings zone_standard = zone_standardize(zone);
  cpp11::r_string zone_name_r(zone_standard[0]);
  std::string zone_name(zone_name_r);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  for (r_ssize i = 0; i < size; ++i) {
    double elt_seconds = seconds[i];
    int64_t elt = as_int64(elt_seconds);

    if (elt == r_int64_na) {
      days[i] = r_int_na;
      seconds_of_day[i] = r_int_na;
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
    seconds_of_day[i] = elt_tod_sec.count();
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::doubles convert_naive_second_point_fields_to_zoned_seconds_cpp(const civil_field& calendar,
                                                                                const civil_field& seconds_of_day,
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

  bool recycle_calendar = civil_is_scalar(calendar);
  bool recycle_seconds_of_day = civil_is_scalar(seconds_of_day);
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
    const int elt_calendar = recycle_calendar ? calendar[0] : calendar[i];
    const int elt_seconds_of_day = recycle_seconds_of_day ? seconds_of_day[0] : seconds_of_day[i];

    const enum dst_nonexistent elt_dst_nonexistent_val =
      recycle_dst_nonexistent ?
      dst_nonexistent_val :
      parse_dst_nonexistent_one(dst_nonexistent[i]);

    const enum dst_ambiguous elt_dst_ambiguous_val =
      recycle_dst_ambiguous ?
      dst_ambiguous_val :
      parse_dst_ambiguous_one(dst_ambiguous[i]);

    if (elt_calendar == r_int_na || elt_seconds_of_day == r_int_na) {
      out[i] = r_dbl_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    date::local_seconds elt_lsec_floor{elt_lday};

    std::chrono::seconds elt_tod{elt_seconds_of_day};

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
civil_writable_rcrd convert_year_month_day_hour_minute_second_to_naive_second_point_fields(const cpp11::integers& year,
                                                                                           const cpp11::integers& month,
                                                                                           const cpp11::integers& day,
                                                                                           const cpp11::integers& hour,
                                                                                           const cpp11::integers& minute,
                                                                                           const cpp11::integers& second,
                                                                                           const cpp11::strings& day_nonexistent) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);

  r_ssize size = year.size();

  civil_writable_field days(size);
  civil_writable_field seconds_of_day(size);

  civil_writable_rcrd out({days, seconds_of_day});
  out.names() = {"days", "seconds_of_day"};

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
      seconds_of_day[i] = r_int_na;
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
        seconds_of_day[i] = r_int_na;
        continue;
      }
    }

    date::local_days out_lday{out_ymd};

    days[i] = out_lday.time_since_epoch().count();
    seconds_of_day[i] = out_tod.count();
  }

  return out;
}

[[cpp11::register]]
civil_writable_rcrd convert_year_month_day_hour_minute_second_nanosecond_to_naive_subsecond_point_fields(const cpp11::integers& year,
                                                                                                         const cpp11::integers& month,
                                                                                                         const cpp11::integers& day,
                                                                                                         const cpp11::integers& hour,
                                                                                                         const cpp11::integers& minute,
                                                                                                         const cpp11::integers& second,
                                                                                                         const cpp11::integers& nanosecond,
                                                                                                         const cpp11::strings& day_nonexistent) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);

  r_ssize size = year.size();

  civil_writable_field days(size);
  civil_writable_field seconds_of_day(size);
  civil_writable_field nanoseconds_of_second(size);

  civil_writable_rcrd out({days, seconds_of_day, nanoseconds_of_second});
  out.names() = {"days", "seconds_of_day", "nanoseconds_of_second"};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_year = year[i];
    int elt_month = month[i];
    int elt_day = day[i];
    int elt_hour = hour[i];
    int elt_minute = minute[i];
    int elt_second = second[i];
    int elt_nanosecond = nanosecond[i];

    if (elt_year == r_int_na ||
        elt_month == r_int_na ||
        elt_day == r_int_na ||
        elt_hour == r_int_na ||
        elt_minute == r_int_na ||
        elt_second == r_int_na ||
        elt_nanosecond == r_int_na) {
      days[i] = r_int_na;
      seconds_of_day[i] = r_int_na;
      nanoseconds_of_second[i] = r_int_na;
      continue;
    }

    check_range_year(elt_year, "year");
    check_range_month(elt_month, "month");
    check_range_day(elt_day, "day");
    check_range_hour(elt_hour, "hour");
    check_range_minute(elt_minute, "minute");
    check_range_second(elt_second, "second");
    check_range_nanosecond(elt_nanosecond, "nanosecond");

    std::chrono::nanoseconds out_nanos_of_second{elt_nanosecond};

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
        seconds_of_day[i] = r_int_na;
        nanoseconds_of_second[i] = r_int_na;
        continue;
      }
    }

    date::local_days out_lday{out_ymd};

    days[i] = out_lday.time_since_epoch().count();
    seconds_of_day[i] = out_tod.count();
    nanoseconds_of_second[i] = out_nanos_of_second.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_list_of_integers convert_calendar_days_to_year_month_day(const civil_field& calendar) {
  r_ssize size = calendar.size();

  cpp11::writable::integers year(size);
  cpp11::writable::integers month(size);
  cpp11::writable::integers day(size);

  civil_writable_list_of_integers out({year, month, day});
  out.names() = {"year", "month", "day"};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_calendar = calendar[i];

    if (elt_calendar == r_int_na) {
      year[i] = r_int_na;
      month[i] = r_int_na;
      day[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    date::year_month_day elt_ymd{elt_lday};

    year[i] = static_cast<int>(elt_ymd.year());
    month[i] = static_cast<unsigned int>(elt_ymd.month());
    day[i] = static_cast<unsigned int>(elt_ymd.day());
  }

  return out;
}

// -----------------------------------------------------------------------------

/*
 * Same for datetime and nano_datetime, since nanoseconds wont change when
 * going from zoned->naive. They are "beneath" time zone changes.
 */
[[cpp11::register]]
civil_writable_rcrd convert_second_point_fields_from_zoned_to_naive(const civil_field& calendar,
                                                                    const civil_field& seconds_of_day,
                                                                    const cpp11::strings& zone) {
  r_ssize size = calendar.size();

  civil_writable_field out_calendar{calendar};
  civil_writable_field out_seconds_of_day{seconds_of_day};

  civil_writable_rcrd out({out_calendar, out_seconds_of_day});
  out.names() = {"calendar", "seconds_of_day"};

  cpp11::writable::strings zone_standard = zone_standardize(zone);
  cpp11::r_string zone_name_r(zone_standard[0]);
  std::string zone_name(zone_name_r);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_calendar = calendar[i];
    int elt_seconds_of_day = seconds_of_day[i];

    if (elt_calendar == r_int_na) {
      out_calendar[i] = r_int_na;
      out_seconds_of_day[i] = r_int_na;
      continue;
    }

    date::sys_days elt_sday{date::days{elt_calendar}};
    std::chrono::seconds elt_tod{elt_seconds_of_day};

    date::sys_seconds elt_ssec_floor{elt_sday};
    date::sys_seconds elt_ssec = elt_ssec_floor + elt_tod;

    date::zoned_seconds out_zsec = date::make_zoned(p_time_zone, elt_ssec);
    date::local_seconds out_lsec = out_zsec.get_local_time();

    date::local_days out_lday = date::floor<date::days>(out_lsec);
    date::local_seconds out_lsec_floor{out_lday};

    std::chrono::seconds out_tod{out_lsec - out_lsec_floor};

    out_calendar[i] = out_lday.time_since_epoch().count();
    out_seconds_of_day[i] = out_tod.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_rcrd convert_zoned_seconds_to_zoned_second_point_fields(const cpp11::doubles& seconds) {
  r_ssize size = seconds.size();

  civil_writable_field out_days(size);
  civil_writable_field out_seconds_of_day(size);

  civil_writable_rcrd out({out_days, out_seconds_of_day});
  out.names() = {"days", "seconds_of_day"};

  for (r_ssize i = 0; i < size; ++i) {
    double elt_seconds = seconds[i];
    int64_t elt = as_int64(elt_seconds);

    if (elt == r_int64_na) {
      out_days[i] = r_int_na;
      out_seconds_of_day[i] = r_int_na;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::sys_seconds elt_ssec{elt_sec};

    date::sys_days elt_sday = date::floor<date::days>(elt_ssec);
    date::sys_seconds elt_ssec_floor{elt_sday};

    std::chrono::seconds elt_tod{elt_ssec - elt_ssec_floor};

    out_days[i] = elt_sday.time_since_epoch().count();
    out_seconds_of_day[i] = elt_tod.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static civil_writable_field convert_year_quarternum_quarterday_to_calendar_days_impl(const cpp11::integers& year,
                                                                                     const cpp11::integers& quarternum,
                                                                                     const cpp11::integers& quarterday,
                                                                                     const cpp11::strings& day_nonexistent) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);

  r_ssize size = year.size();

  cpp11::writable::integers out(size);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_year = year[i];
    int elt_quarternum = quarternum[i];
    int elt_quarterday = quarterday[i];

    if (elt_year == r_int_na || elt_quarternum == r_int_na || elt_quarterday == r_int_na) {
      out[i] = r_int_na;
      continue;
    }

    check_range_year(elt_year, "year");
    check_range_quarternum(elt_quarternum, "quarternum");
    check_range_quarterday(elt_quarterday, "quarterday");

    quarterly::year_quarternum_quarterday<S> elt_yqnqd{
      quarterly::year<S>{elt_year},
      quarterly::quarternum{static_cast<unsigned>(elt_quarternum)},
      quarterly::quarterday{static_cast<unsigned>(elt_quarterday)}
    };

    if (!elt_yqnqd.ok()) {
      bool na = false;
      resolve_day_nonexistent_yqnqd(i, day_nonexistent_val, elt_yqnqd, na);

      if (na) {
        out[i] = r_int_na;
        continue;
      }
    }

    date::local_days elt_lday{elt_yqnqd};

    out[i] = elt_lday.time_since_epoch().count();
  }

  return out;
}

[[cpp11::register]]
civil_writable_field convert_year_quarternum_quarterday_to_calendar_days(const cpp11::integers& year,
                                                                         const cpp11::integers& quarternum,
                                                                         const cpp11::integers& quarterday,
                                                                         int start,
                                                                         const cpp11::strings& day_nonexistent) {
  if (start == 1) {
    return convert_year_quarternum_quarterday_to_calendar_days_impl<quarterly::start::january>(year, quarternum, quarterday, day_nonexistent);
  } else if (start == 2) {
    return convert_year_quarternum_quarterday_to_calendar_days_impl<quarterly::start::february>(year, quarternum, quarterday, day_nonexistent);
  } else if (start == 3) {
    return convert_year_quarternum_quarterday_to_calendar_days_impl<quarterly::start::march>(year, quarternum, quarterday, day_nonexistent);
  } else if (start == 4) {
    return convert_year_quarternum_quarterday_to_calendar_days_impl<quarterly::start::april>(year, quarternum, quarterday, day_nonexistent);
  } else if (start == 5) {
    return convert_year_quarternum_quarterday_to_calendar_days_impl<quarterly::start::may>(year, quarternum, quarterday, day_nonexistent);
  } else if (start == 6) {
    return convert_year_quarternum_quarterday_to_calendar_days_impl<quarterly::start::june>(year, quarternum, quarterday, day_nonexistent);
  } else if (start == 7) {
    return convert_year_quarternum_quarterday_to_calendar_days_impl<quarterly::start::july>(year, quarternum, quarterday, day_nonexistent);
  } else if (start == 8) {
    return convert_year_quarternum_quarterday_to_calendar_days_impl<quarterly::start::august>(year, quarternum, quarterday, day_nonexistent);
  } else if (start == 9) {
    return convert_year_quarternum_quarterday_to_calendar_days_impl<quarterly::start::september>(year, quarternum, quarterday, day_nonexistent);
  } else if (start == 10) {
    return convert_year_quarternum_quarterday_to_calendar_days_impl<quarterly::start::october>(year, quarternum, quarterday, day_nonexistent);
  } else if (start == 11) {
    return convert_year_quarternum_quarterday_to_calendar_days_impl<quarterly::start::november>(year, quarternum, quarterday, day_nonexistent);
  } else if (start == 12) {
    return convert_year_quarternum_quarterday_to_calendar_days_impl<quarterly::start::december>(year, quarternum, quarterday, day_nonexistent);
  }

  never_reached("convert_year_quarternum_quarterday_to_calendar_days");
}

template <quarterly::start S>
static
cpp11::writable::list_of<cpp11::writable::integers>
convert_calendar_days_to_year_quarternum_quarterday_impl(const civil_field& calendar) {
  r_ssize size = calendar.size();

  cpp11::writable::integers out_year(size);
  cpp11::writable::integers out_quarternum(size);
  cpp11::writable::integers out_quarterday(size);

  cpp11::writable::list out{out_year, out_quarternum, out_quarterday};
  out.names() = {"year", "quarternum", "quarterday"};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_calendar = calendar[i];

    if (elt_calendar == r_int_na) {
      out_year[i] = r_int_na;
      out_quarternum[i] = r_int_na;
      out_quarterday[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    quarterly::year_quarternum_quarterday<S> elt_yqnqd(elt_lday);

    out_year[i] = static_cast<int>(elt_yqnqd.year());
    out_quarternum[i] = static_cast<unsigned int>(elt_yqnqd.quarternum());
    out_quarterday[i] = static_cast<unsigned int>(elt_yqnqd.quarterday());
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::list_of<cpp11::writable::integers>
convert_calendar_days_to_year_quarternum_quarterday(const civil_field& calendar, int start) {
  if (start == 1) {
    return convert_calendar_days_to_year_quarternum_quarterday_impl<quarterly::start::january>(calendar);
  } else if (start == 2) {
    return convert_calendar_days_to_year_quarternum_quarterday_impl<quarterly::start::february>(calendar);
  } else if (start == 3) {
    return convert_calendar_days_to_year_quarternum_quarterday_impl<quarterly::start::march>(calendar);
  } else if (start == 4) {
    return convert_calendar_days_to_year_quarternum_quarterday_impl<quarterly::start::april>(calendar);
  } else if (start == 5) {
    return convert_calendar_days_to_year_quarternum_quarterday_impl<quarterly::start::may>(calendar);
  } else if (start == 6) {
    return convert_calendar_days_to_year_quarternum_quarterday_impl<quarterly::start::june>(calendar);
  } else if (start == 7) {
    return convert_calendar_days_to_year_quarternum_quarterday_impl<quarterly::start::july>(calendar);
  } else if (start == 8) {
    return convert_calendar_days_to_year_quarternum_quarterday_impl<quarterly::start::august>(calendar);
  } else if (start == 9) {
    return convert_calendar_days_to_year_quarternum_quarterday_impl<quarterly::start::september>(calendar);
  } else if (start == 10) {
    return convert_calendar_days_to_year_quarternum_quarterday_impl<quarterly::start::october>(calendar);
  } else if (start == 11) {
    return convert_calendar_days_to_year_quarternum_quarterday_impl<quarterly::start::november>(calendar);
  } else if (start == 12) {
    return convert_calendar_days_to_year_quarternum_quarterday_impl<quarterly::start::december>(calendar);
  }

  never_reached("convert_calendar_days_to_year_quarternum_quarterday");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_field convert_iso_year_weeknum_weekday_to_calendar_days(const cpp11::integers& year,
                                                                       const cpp11::integers& weeknum,
                                                                       const cpp11::integers& weekday,
                                                                       const cpp11::strings& day_nonexistent) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);

  r_ssize size = year.size();

  cpp11::writable::integers out(size);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_year = year[i];
    int elt_weeknum = weeknum[i];
    int elt_weekday = weekday[i];

    if (elt_year == r_int_na || elt_weeknum == r_int_na || elt_weekday == r_int_na) {
      out[i] = r_int_na;
      continue;
    }

    check_range_year(elt_year, "year");
    check_range_iso_weeknum(elt_weeknum, "weeknum");
    check_range_iso_weekday(elt_weekday, "weekday");

    iso_week::year_weeknum_weekday elt_yww{
      iso_week::year{elt_year},
      iso_week::weeknum{static_cast<unsigned>(elt_weeknum)},
      iso_week::weekday{static_cast<unsigned>(elt_weekday)}
    };

    if (!elt_yww.ok()) {
      bool na = false;
      resolve_day_nonexistent_yww(i, day_nonexistent_val, elt_yww, na);

      if (na) {
        out[i] = r_int_na;
        continue;
      }
    }

    date::local_days elt_lday{elt_yww};

    out[i] = elt_lday.time_since_epoch().count();
  }

  return out;
}


[[cpp11::register]]
cpp11::writable::list_of<cpp11::writable::integers>
convert_calendar_days_to_iso_year_weeknum_weekday(const civil_field& calendar) {
  r_ssize size = calendar.size();

  cpp11::writable::integers out_year(size);
  cpp11::writable::integers out_weeknum(size);
  cpp11::writable::integers out_weekday(size);

  cpp11::writable::list out{out_year, out_weeknum, out_weekday};
  out.names() = {"year", "weeknum", "weekday"};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_calendar = calendar[i];

    if (elt_calendar == r_int_na) {
      out_year[i] = r_int_na;
      out_weeknum[i] = r_int_na;
      out_weekday[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    iso_week::year_weeknum_weekday elt_yww{elt_lday};

    out_year[i] = static_cast<int>(elt_yww.year());
    out_weeknum[i] = static_cast<unsigned int>(elt_yww.weeknum());
    out_weekday[i] = static_cast<unsigned int>(elt_yww.weekday());
  }

  return out;
}


// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_field convert_year_month_day_to_calendar_days(const cpp11::integers& year,
                                                             const cpp11::integers& month,
                                                             const cpp11::integers& day,
                                                             const cpp11::strings& day_nonexistent) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);

  r_ssize size = year.size();

  civil_writable_field days(size);

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

  return days;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_rcrd convert_calendar_days_hour_minute_second_to_naive_second_point_fields(const civil_field& calendar,
                                                                                          const cpp11::integers& hour,
                                                                                          const cpp11::integers& minute,
                                                                                          const cpp11::integers& second) {
  r_ssize size = calendar.size();

  civil_writable_field out_calendar{calendar};
  civil_writable_field out_seconds_of_day(size);

  civil_writable_rcrd out({out_calendar, out_seconds_of_day});
  out.names() = {"calendar", "seconds_of_day"};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_calendar = calendar[i];
    int elt_hour = hour[i];
    int elt_minute = minute[i];
    int elt_second = second[i];

    if (elt_calendar == r_int_na ||
        elt_hour == r_int_na ||
        elt_minute == r_int_na ||
        elt_second == r_int_na) {
      out_calendar[i] = r_int_na;
      out_seconds_of_day[i] = r_int_na;
      continue;
    }

    check_range_hour(elt_hour, "hour");
    check_range_minute(elt_minute, "minute");
    check_range_second(elt_second, "second");

    std::chrono::seconds elt_out_seconds_of_day =
      std::chrono::hours{elt_hour} +
      std::chrono::minutes{elt_minute} +
      std::chrono::seconds{elt_second};

    out_calendar[i] = elt_calendar;
    out_seconds_of_day[i] = elt_out_seconds_of_day.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_rcrd convert_calendar_days_hour_minute_second_subsecond_to_naive_subsecond_point_fields(const civil_field& calendar,
                                                                                                       const cpp11::integers& hour,
                                                                                                       const cpp11::integers& minute,
                                                                                                       const cpp11::integers& second,
                                                                                                       const cpp11::integers& subsecond,
                                                                                                       const cpp11::strings& precision) {
  enum precision precision_val = parse_precision(precision);

  r_ssize size = calendar.size();

  civil_writable_field out_calendar{calendar};
  civil_writable_field out_seconds_of_day(size);
  civil_writable_field out_nanoseconds_of_second(size);

  civil_writable_rcrd out({out_calendar, out_seconds_of_day, out_nanoseconds_of_second});
  out.names() = {"calendar", "seconds_of_day", "nanoseconds_of_second"};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_calendar = calendar[i];
    int elt_hour = hour[i];
    int elt_minute = minute[i];
    int elt_second = second[i];
    int elt_subsecond = subsecond[i];

    if (elt_calendar == r_int_na ||
        elt_hour == r_int_na ||
        elt_minute == r_int_na ||
        elt_second == r_int_na ||
        elt_subsecond == r_int_na) {
      out_calendar[i] = r_int_na;
      out_seconds_of_day[i] = r_int_na;
      out_nanoseconds_of_second[i] = r_int_na;
      continue;
    }

    check_range_hour(elt_hour, "hour");
    check_range_minute(elt_minute, "minute");
    check_range_second(elt_second, "second");

    std::chrono::nanoseconds elt_out_nanoseconds_of_second;

    switch (precision_val) {
    case precision::millisecond: {
      check_range_millisecond(elt_subsecond, "millisecond");
      elt_out_nanoseconds_of_second = std::chrono::milliseconds{elt_subsecond};
      break;
    }
    case precision::microsecond: {
      check_range_microsecond(elt_subsecond, "microsecond");
      elt_out_nanoseconds_of_second = std::chrono::microseconds{elt_subsecond};
      break;
    }
    case precision::nanosecond: {
      check_range_nanosecond(elt_subsecond, "nanosecond");
      elt_out_nanoseconds_of_second = std::chrono::nanoseconds{elt_subsecond};
      break;
    }
    case precision::second: {
      civil_abort("Internal error: Unexpected precision.");
    }
    }

    std::chrono::seconds elt_out_seconds_of_day =
      std::chrono::hours{elt_hour} +
      std::chrono::minutes{elt_minute} +
      std::chrono::seconds{elt_second};

    out_calendar[i] = elt_calendar;
    out_seconds_of_day[i] = elt_out_seconds_of_day.count();
    out_nanoseconds_of_second[i] = elt_out_nanoseconds_of_second.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_rcrd convert_second_point_fields_from_naive_to_zoned_cpp(const civil_field& calendar,
                                                                        const civil_field& seconds_of_day,
                                                                        const cpp11::strings& zone,
                                                                        const cpp11::strings& dst_nonexistent,
                                                                        const cpp11::strings& dst_ambiguous) {
  const r_ssize size = calendar.size();

  civil_writable_field out_calendar{calendar};
  civil_writable_field out_seconds_of_day{seconds_of_day};

  civil_writable_rcrd out({out_calendar, out_seconds_of_day});
  out.names() = {"calendar", "seconds_of_day"};

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

  for (r_ssize i = 0; i < size; ++i) {
    int elt_calendar = calendar[i];
    int elt_seconds_of_day = seconds_of_day[i];

    const enum dst_nonexistent elt_dst_nonexistent_val =
      recycle_dst_nonexistent ?
      dst_nonexistent_val :
      parse_dst_nonexistent_one(dst_nonexistent[i]);

    const enum dst_ambiguous elt_dst_ambiguous_val =
      recycle_dst_ambiguous ?
      dst_ambiguous_val :
      parse_dst_ambiguous_one(dst_ambiguous[i]);

    if (elt_calendar == r_int_na) {
      out_calendar[i] = r_int_na;
      out_seconds_of_day[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    std::chrono::seconds elt_tod{elt_seconds_of_day};

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
      out_calendar[i] = r_int_na;
      out_seconds_of_day[i] = r_int_na;
      continue;
    }

    date::sys_days out_sday = date::floor<date::days>(out_ssec);
    date::sys_seconds out_ssec_floor{out_sday};

    std::chrono::seconds out_tod{out_ssec - out_ssec_floor};

    out_calendar[i] = out_sday.time_since_epoch().count();
    out_seconds_of_day[i] = out_tod.count();
  }

  return out;
}

[[cpp11::register]]
civil_writable_rcrd convert_subsecond_point_fields_from_naive_to_zoned_cpp(const civil_field& calendar,
                                                                           const civil_field& seconds_of_day,
                                                                           const civil_field& nanoseconds_of_second,
                                                                           const cpp11::strings& precision,
                                                                           const cpp11::strings& zone,
                                                                           const cpp11::strings& dst_nonexistent,
                                                                           const cpp11::strings& dst_ambiguous) {
  const r_ssize size = calendar.size();

  const enum precision precision_val = parse_precision(precision);

  civil_writable_field out_calendar{calendar};
  civil_writable_field out_seconds_of_day{seconds_of_day};
  civil_writable_field out_nanoseconds_of_second{nanoseconds_of_second};

  civil_writable_rcrd out({out_calendar, out_seconds_of_day, out_nanoseconds_of_second});
  out.names() = {"calendar", "seconds_of_day", "nanoseconds_of_second"};

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

  for (r_ssize i = 0; i < size; ++i) {
    int elt_calendar = calendar[i];
    int elt_seconds_of_day = seconds_of_day[i];
    int elt_nanoseconds_of_second = nanoseconds_of_second[i];

    const enum dst_nonexistent elt_dst_nonexistent_val =
      recycle_dst_nonexistent ?
      dst_nonexistent_val :
      parse_dst_nonexistent_one(dst_nonexistent[i]);

    const enum dst_ambiguous elt_dst_ambiguous_val =
      recycle_dst_ambiguous ?
      dst_ambiguous_val :
      parse_dst_ambiguous_one(dst_ambiguous[i]);

    if (elt_calendar == r_int_na) {
      out_calendar[i] = r_int_na;
      out_seconds_of_day[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    std::chrono::seconds elt_tod{elt_seconds_of_day};
    std::chrono::nanoseconds elt_nanos{elt_nanoseconds_of_second};

    date::local_seconds elt_lsec_floor{elt_lday};
    date::local_seconds elt_lsec = elt_lsec_floor + elt_tod;

    bool na = false;

    date::sys_seconds out_ssec = convert_local_to_sys(
      elt_lsec,
      p_time_zone,
      i,
      elt_dst_nonexistent_val,
      elt_dst_ambiguous_val,
      precision_val,
      na,
      elt_nanos
    );

    if (na) {
      out_calendar[i] = r_int_na;
      out_seconds_of_day[i] = r_int_na;
      out_nanoseconds_of_second[i] = r_int_na;
      continue;
    }

    date::sys_days out_sday = date::floor<date::days>(out_ssec);
    date::sys_seconds out_ssec_floor{out_sday};

    std::chrono::seconds out_tod{out_ssec - out_ssec_floor};

    out_calendar[i] = out_sday.time_since_epoch().count();
    out_seconds_of_day[i] = out_tod.count();
    out_nanoseconds_of_second[i] = elt_nanos.count();
  }

  return out;
}

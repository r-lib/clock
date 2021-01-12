#include "clock.h"
#include "utils.h"
#include "clock-rcrd.h"
#include "zone.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "check.h"
#include "year-month-weekday.h"

// -----------------------------------------------------------------------------

[[cpp11::register]]
clock_writable_list_of_integers convert_seconds_of_day_to_hour_minute_second(const clock_field& seconds_of_day) {
  r_ssize size = seconds_of_day.size();

  cpp11::writable::integers hour(size);
  cpp11::writable::integers minute(size);
  cpp11::writable::integers second(size);

  clock_writable_list_of_integers out({hour, minute, second});
  out.names() = {"hour", "minute", "second"};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_seconds_of_day = seconds_of_day[i];

    if (elt_seconds_of_day == r_int_na) {
      hour[i] = r_int_na;
      minute[i] = r_int_na;
      second[i] = r_int_na;
      continue;
    }

    std::chrono::seconds elt_secs{elt_seconds_of_day};
    date::hh_mm_ss<std::chrono::seconds> elt_hms = date::make_time(elt_secs);

    hour[i] = elt_hms.hours().count();
    minute[i] = elt_hms.minutes().count();
    second[i] = elt_hms.seconds().count();
  }

  return out;
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static clock_writable_field convert_year_quarternum_quarterday_to_calendar_days_impl(const cpp11::integers& year,
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
clock_writable_field convert_year_quarternum_quarterday_to_calendar_days(const cpp11::integers& year,
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
convert_calendar_days_to_year_quarternum_quarterday_impl(const clock_field& calendar) {
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
convert_calendar_days_to_year_quarternum_quarterday(const clock_field& calendar, int start) {
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
clock_writable_field convert_iso_year_weeknum_weekday_to_calendar_days(const cpp11::integers& year,
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
    check_range_weeknum(elt_weeknum, "weeknum");
    check_range_weekday(elt_weekday, "weekday");

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
convert_calendar_days_to_iso_year_weeknum_weekday(const clock_field& calendar) {
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

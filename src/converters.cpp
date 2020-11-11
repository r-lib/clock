#include "local.h"
#include "utils.h"
#include "zone.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "check.h"
#include <date/date.h>
#include <date/tz.h>

static sexp new_local_datetime_fields(r_ssize size);

[[cpp11::register]]
SEXP convert_seconds_to_days_and_time_of_day_cpp(SEXP seconds, SEXP zone) {
  r_ssize size = r_length(seconds);

  sexp out = PROTECT(new_local_datetime_fields(size));

  sexp days = r_list_get(out, 0);
  sexp time_of_day = r_list_get(out, 1);

  int* p_days = r_int_deref(days);
  int* p_time_of_day = r_int_deref(time_of_day);

  zone = PROTECT(zone_standardize(zone));
  std::string zone_name = zone_unwrap(zone);
  const date::time_zone* p_zone = zone_name_load(zone_name);

  const double* p_seconds = r_dbl_deref_const(seconds);

  for (r_ssize i = 0; i < size; ++i) {
    double elt_seconds = p_seconds[i];
    int64_t elt = as_int64(elt_seconds);

    if (elt == r_int64_na) {
      p_days[i] = p_time_of_day[i] = r_int_na;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::sys_seconds elt_ssec{elt_sec};
    date::zoned_seconds elt_zsec = date::make_zoned(p_zone, elt_ssec);
    date::local_seconds elt_lsec = elt_zsec.get_local_time();

    date::local_days elt_lday = date::floor<date::days>(elt_lsec);
    date::local_seconds elt_lsec_floor{elt_lday};

    std::chrono::seconds elt_tod_sec{elt_lsec - elt_lsec_floor};

    p_days[i] = elt_lday.time_since_epoch().count();
    p_time_of_day[i] = elt_tod_sec.count();
  }

  UNPROTECT(2);
  return out;
}

static sexp new_local_datetime_fields(r_ssize size) {
  sexp out = PROTECT(r_new_list(2));

  r_list_poke(out, 0, r_new_integer(size));
  r_list_poke(out, 1, r_new_integer(size));

  sexp names = PROTECT(r_new_character(2));
  r_chr_poke(names, 0, r_new_string("days"));
  r_chr_poke(names, 1, r_new_string("time_of_day"));

  r_poke_names(out, names);

  UNPROTECT(2);
  return out;
}

[[cpp11::register]]
SEXP convert_days_and_time_of_day_to_seconds_cpp(SEXP days,
                                                 SEXP time_of_day,
                                                 SEXP zone,
                                                 SEXP dst_nonexistent,
                                                 SEXP dst_ambiguous,
                                                 SEXP size) {
  r_ssize c_size = r_int_get(size, 0);

  sexp out = PROTECT(r_new_double(c_size));
  double* p_out = r_dbl_deref(out);

  zone = PROTECT(zone_standardize(zone));
  std::string zone_name = zone_unwrap(zone);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  const int* p_days = r_int_deref_const(days);
  bool recycle_days = r_is_scalar(days);

  const int* p_time_of_day = r_int_deref_const(time_of_day);
  bool recycle_time_of_day = r_is_scalar(time_of_day);

  const sexp* p_dst_nonexistent = STRING_PTR_RO(dst_nonexistent);
  bool recycle_dst_nonexistent = r_is_scalar(dst_nonexistent);
  enum dst_nonexistent c_dst_nonexistent;
  if (recycle_dst_nonexistent) {
    c_dst_nonexistent = parse_dst_nonexistent_one(CHAR(p_dst_nonexistent[0]));
  }

  const sexp* p_dst_ambiguous = STRING_PTR_RO(dst_ambiguous);
  bool recycle_dst_ambiguous = r_is_scalar(dst_ambiguous);
  enum dst_ambiguous c_dst_ambiguous;
  if (recycle_dst_ambiguous) {
    c_dst_ambiguous = parse_dst_ambiguous_one(CHAR(p_dst_ambiguous[0]));
  }

  for (r_ssize i = 0; i < c_size; ++i) {
    const int elt_days =
      recycle_days ?
      p_days[0] :
      p_days[i];

    const int elt_time_of_day =
      recycle_time_of_day ?
      p_time_of_day[0] :
      p_time_of_day[i];

    const enum dst_nonexistent elt_dst_nonexistent =
      recycle_dst_nonexistent ?
      c_dst_nonexistent :
      parse_dst_nonexistent_one(CHAR(p_dst_nonexistent[i]));

    const enum dst_ambiguous elt_dst_ambiguous =
      recycle_dst_ambiguous ?
      c_dst_ambiguous :
      parse_dst_ambiguous_one(CHAR(p_dst_ambiguous[i]));

    if (elt_days == r_int_na || elt_time_of_day == r_int_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_days}};
    date::local_seconds elt_lsec_floor{elt_lday};

    std::chrono::seconds elt_tod{elt_time_of_day};

    date::local_seconds elt_lsec = elt_lsec_floor + elt_tod;

    p_out[i] = convert_local_seconds_to_posixt(
      elt_lsec,
      p_time_zone,
      i,
      elt_dst_nonexistent,
      elt_dst_ambiguous
    );
  }

  UNPROTECT(2);
  return out;
}


static sexp new_ymd(r_ssize size) {
  sexp out = PROTECT(r_new_list(3));

  r_list_poke(out, 0, r_new_integer(size));
  r_list_poke(out, 1, r_new_integer(size));
  r_list_poke(out, 2, r_new_integer(size));

  sexp names = PROTECT(r_new_character(3));
  r_chr_poke(names, 0, r_new_string("year"));
  r_chr_poke(names, 1, r_new_string("month"));
  r_chr_poke(names, 2, r_new_string("day"));

  r_poke_names(out, names);

  UNPROTECT(2);
  return out;
}

static inline sexp ymd_year(sexp x) {
  return r_list_get(x, 0);
}
static inline sexp ymd_month(sexp x) {
  return r_list_get(x, 1);
}
static inline sexp ymd_day(sexp x) {
  return r_list_get(x, 2);
}

[[cpp11::register]]
SEXP convert_days_to_year_month_day_cpp(SEXP days) {
  r_ssize size = r_length(days);

  sexp out = PROTECT(new_ymd(size));

  sexp year = ymd_year(out);
  sexp month = ymd_month(out);
  sexp day = ymd_day(out);

  int* p_year = r_int_deref(year);
  int* p_month = r_int_deref(month);
  int* p_day = r_int_deref(day);

  const int* p_days = r_int_deref_const(days);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = p_days[i];

    if (elt_days == r_int_na) {
      p_year[i] = p_month[i] = p_day[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_days}};
    date::year_month_day elt_ymd{elt_lday};

    p_year[i] = static_cast<int>(elt_ymd.year());
    p_month[i] = static_cast<unsigned int>(elt_ymd.month());
    p_day[i] = static_cast<unsigned int>(elt_ymd.day());
  }

  UNPROTECT(1);
  return out;
}

[[cpp11::register]]
SEXP convert_year_month_day_to_days_cpp(SEXP year,
                                        SEXP month,
                                        SEXP day,
                                        SEXP day_nonexistent) {
  enum day_nonexistent c_day_nonexistent = parse_day_nonexistent(day_nonexistent);

  r_ssize size = r_length(year);

  sexp days = PROTECT(r_new_integer(size));
  int* p_days = r_int_deref(days);

  int* p_time_of_day = NULL;
  int* p_nanos_of_second = NULL;

  const int* p_year = r_int_deref_const(year);
  const int* p_month = r_int_deref_const(month);
  const int* p_day = r_int_deref_const(day);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_year = p_year[i];
    int elt_month = p_month[i];
    int elt_day = p_day[i];

    if (elt_year == r_int_na || elt_month == r_int_na || elt_day == r_int_na) {
      p_days[i] = r_int_na;
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

    convert_year_month_day_to_days_one(
      i,
      c_day_nonexistent,
      out_ymd,
      p_days,
      p_time_of_day,
      p_nanos_of_second
    );
  }

  UNPROTECT(1);
  return days;
}

static sexp new_hms(r_ssize size) {
  sexp out = PROTECT(r_new_list(3));

  r_list_poke(out, 0, r_new_integer(size));
  r_list_poke(out, 1, r_new_integer(size));
  r_list_poke(out, 2, r_new_integer(size));

  sexp names = PROTECT(r_new_character(3));
  r_chr_poke(names, 0, r_new_string("hour"));
  r_chr_poke(names, 1, r_new_string("minute"));
  r_chr_poke(names, 2, r_new_string("second"));

  r_poke_names(out, names);

  UNPROTECT(2);
  return out;
}

static inline sexp hms_hour(sexp x) {
  return r_list_get(x, 0);
}
static inline sexp hms_minute(sexp x) {
  return r_list_get(x, 1);
}
static inline sexp hms_second(sexp x) {
  return r_list_get(x, 2);
}

[[cpp11::register]]
SEXP convert_time_of_day_to_hour_minute_second_cpp(SEXP time_of_day) {
  r_ssize size = r_length(time_of_day);

  sexp out = PROTECT(new_hms(size));

  sexp hour = hms_hour(out);
  sexp minute = hms_minute(out);
  sexp second = hms_second(out);

  int* p_hour = r_int_deref(hour);
  int* p_minute = r_int_deref(minute);
  int* p_second = r_int_deref(second);

  const int* p_time_of_day = r_int_deref_const(time_of_day);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_time_of_day = p_time_of_day[i];

    if (elt_time_of_day == r_int_na) {
      p_hour[i] = p_minute[i] = p_second[i] = r_int_na;
      continue;
    }

    std::chrono::seconds elt_tod_sec{elt_time_of_day};

    date::hh_mm_ss<std::chrono::seconds> elt_hms = date::make_time(elt_tod_sec);

    p_hour[i] = elt_hms.hours().count();
    p_minute[i] = elt_hms.minutes().count();
    p_second[i] = elt_hms.seconds().count();
  }

  UNPROTECT(1);
  return out;
}

[[cpp11::register]]
SEXP convert_hour_minute_second_to_time_of_day_cpp(SEXP hour,
                                                   SEXP minute,
                                                   SEXP second) {
  r_ssize size = r_length(hour);

  sexp out = PROTECT(r_new_integer(size));
  int* p_out = r_int_deref(out);

  const int* p_hour = r_int_deref_const(hour);
  const int* p_minute = r_int_deref_const(minute);
  const int* p_second = r_int_deref_const(second);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_hour = p_hour[i];
    int elt_minute = p_minute[i];
    int elt_second = p_second[i];

    if (elt_hour == r_int_na || elt_minute == r_int_na || elt_second == r_int_na) {
      p_out[i] = r_int_na;
      continue;
    }

    check_range_hour(elt_hour, "hour");
    check_range_minute(elt_minute, "minute");
    check_range_second(elt_second, "second");

    std::chrono::seconds elt_tod_sec =
      std::chrono::hours{elt_hour} +
      std::chrono::minutes{elt_minute} +
      std::chrono::seconds{elt_second};

    p_out[i] = elt_tod_sec.count();
  }

  UNPROTECT(1);
  return out;
}

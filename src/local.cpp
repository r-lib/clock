#include "local.h"
#include "utils.h"
#include "zone.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include <date/date.h>
#include <date/tz.h>

static sexp new_local_datetime_list();

[[cpp11::register]]
SEXP localize_posixct_cpp(SEXP x) {
  r_ssize size = r_length(x);

  sexp out = PROTECT(new_local_datetime_list());

  sexp year = PROTECT(r_new_integer(size));
  sexp month = PROTECT(r_new_integer(size));
  sexp day = PROTECT(r_new_integer(size));
  sexp hour = PROTECT(r_new_integer(size));
  sexp minute = PROTECT(r_new_integer(size));
  sexp second = PROTECT(r_new_integer(size));

  r_list_poke(out, 0, year);
  r_list_poke(out, 1, month);
  r_list_poke(out, 2, day);
  r_list_poke(out, 3, hour);
  r_list_poke(out, 4, minute);
  r_list_poke(out, 5, second);

  int* p_year = r_int_deref(year);
  int* p_month = r_int_deref(month);
  int* p_day = r_int_deref(day);
  int* p_hour = r_int_deref(hour);
  int* p_minute = r_int_deref(minute);
  int* p_second = r_int_deref(second);

  sexp tzone = civil_get_tzone(x);
  tzone = PROTECT(zone_standardize(tzone));
  std::string zone_name = zone_unwrap(tzone);
  const date::time_zone* p_zone = zone_name_load(zone_name);

  r_poke_names(year, r_get_names(x));

  const double* p_x = r_dbl_deref_const(x);

  for (r_ssize i = 0; i < size; ++i) {
    double x_elt = p_x[i];

    int64_t elt = as_int64(x_elt);

    if (elt == r_int64_na) {
      p_year[i] = r_int_na;
      p_month[i] = r_int_na;
      p_day[i] = r_int_na;
      p_hour[i] = r_int_na;
      p_minute[i] = r_int_na;
      p_second[i] = r_int_na;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::sys_seconds elt_ssec{elt_sec};
    date::zoned_seconds elt_zsec = date::make_zoned(p_zone, elt_ssec);
    date::local_seconds elt_lsec = elt_zsec.get_local_time();

    date::local_days elt_lday = date::floor<date::days>(elt_lsec);
    date::local_seconds elt_lsec_floor{elt_lday};

    date::year_month_day elt_ymd{elt_lday};
    date::hh_mm_ss<std::chrono::seconds> elt_hms = date::make_time(elt_lsec - elt_lsec_floor);

    p_year[i] = static_cast<int>(elt_ymd.year());
    p_month[i] = static_cast<unsigned int>(elt_ymd.month());
    p_day[i] = static_cast<unsigned int>(elt_ymd.day());
    p_hour[i] = elt_hms.hours().count();
    p_minute[i] = elt_hms.minutes().count();
    p_second[i] = elt_hms.seconds().count();
  }

  UNPROTECT(8);
  return out;
}

[[cpp11::register]]
SEXP unlocalize_date_cpp(SEXP x, SEXP day_nonexistent) {
  sexp year = local_datetime_year(x);
  sexp month = local_datetime_month(x);
  sexp day = local_datetime_day(x);

  const int* p_year = r_int_deref_const(year);
  const int* p_month = r_int_deref_const(month);
  const int* p_day = r_int_deref_const(day);

  r_ssize size = r_length(year);

  sexp out = PROTECT(r_new_double(size));
  double* p_out = r_dbl_deref(out);

  enum day_nonexistent c_day_nonexistent = parse_day_nonexistent(day_nonexistent);

  r_poke_names(out, local_names(x));
  r_poke_class(out, civil_classes_date);

  for (r_ssize i = 0; i < size; ++i) {
    const int elt_year = p_year[i];
    const int elt_month = p_month[i];
    const int elt_day = p_day[i];

    if (elt_year == r_int_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    const unsigned int elt_date_month = static_cast<unsigned int>(elt_month);
    const unsigned int elt_date_day = static_cast<unsigned int>(elt_day);

    date::year_month_day elt_ymd{
      date::year{elt_year} / date::month{elt_date_month} / date::day{elt_date_day}
    };

    std::chrono::seconds elt_tod{0};

    if (!elt_ymd.ok()) {
      bool na = false;

      resolve_day_nonexistent(i, c_day_nonexistent, elt_ymd, elt_tod, na);

      if (na) {
        p_out[i] = r_dbl_na;
        continue;
      }
    }

    date::local_days elt_lday{elt_ymd};

    p_out[i] = elt_lday.time_since_epoch().count();
  }

  UNPROTECT(1);
  return out;
}

[[cpp11::register]]
SEXP unlocalize_datetime_cpp(SEXP x,
                             SEXP zone,
                             SEXP day_nonexistent,
                             SEXP dst_nonexistent,
                             SEXP dst_ambiguous) {
  sexp year = local_datetime_year(x);
  sexp month = local_datetime_month(x);
  sexp day = local_datetime_day(x);
  sexp hour = local_datetime_hour(x);
  sexp minute = local_datetime_minute(x);
  sexp second = local_datetime_second(x);

  const int* p_year = r_int_deref_const(year);
  const int* p_month = r_int_deref_const(month);
  const int* p_day = r_int_deref_const(day);
  const int* p_hour = r_int_deref_const(hour);
  const int* p_minute = r_int_deref_const(minute);
  const int* p_second = r_int_deref_const(second);

  r_ssize size = r_length(year);

  sexp out = PROTECT(r_new_double(size));
  double* p_out = r_dbl_deref(out);

  zone = PROTECT(zone_standardize(zone));
  std::string zone_name = zone_unwrap(zone);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  enum dst_direction dst_direction = dst_direction::forward;
  enum day_nonexistent c_day_nonexistent = parse_day_nonexistent(day_nonexistent);
  enum dst_nonexistent c_dst_nonexistent = parse_dst_nonexistent(dst_nonexistent);
  enum dst_ambiguous c_dst_ambiguous = parse_dst_ambiguous(dst_ambiguous);

  r_poke_names(out, local_names(x));
  r_poke_class(out, civil_classes_posixct);
  civil_poke_tzone(out, zone);

  for (r_ssize i = 0; i < size; ++i) {
    const int elt_year = p_year[i];
    const int elt_month = p_month[i];
    const int elt_day = p_day[i];
    const int elt_hour = p_hour[i];
    const int elt_minute = p_minute[i];
    const int elt_second = p_second[i];

    if (elt_year == r_int_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    const unsigned int elt_date_month = static_cast<unsigned int>(elt_month);
    const unsigned int elt_date_day = static_cast<unsigned int>(elt_day);

    date::year_month_day elt_ymd{
      date::year{elt_year} / date::month{elt_date_month} / date::day{elt_date_day}
    };

    std::chrono::hours elt_chrono_hour{elt_hour};
    std::chrono::minutes elt_chrono_minute{elt_minute};
    std::chrono::seconds elt_chrono_second{elt_second};

    std::chrono::seconds elt_tod = elt_chrono_hour + elt_chrono_minute + elt_chrono_second;

    if (!elt_ymd.ok()) {
      bool na = false;

      resolve_day_nonexistent(i, c_day_nonexistent, elt_ymd, elt_tod, na);

      if (na) {
        p_out[i] = r_dbl_na;
        continue;
      }
    }

    date::local_days elt_lday{elt_ymd};
    date::local_seconds elt_lsec_floor{elt_lday};
    date::local_seconds elt_lsec = elt_lsec_floor + elt_tod;

    p_out[i] = convert_local_seconds_to_posixt(
      elt_lsec,
      p_time_zone,
      i,
      dst_direction,
      c_dst_nonexistent,
      c_dst_ambiguous
    );
  }

  UNPROTECT(2);
  return out;
}

static sexp new_local_datetime_list() {
  sexp out = PROTECT(r_new_list(6));

  sexp names = PROTECT(r_new_character(6));
  r_chr_poke(names, 0, r_new_string("year"));
  r_chr_poke(names, 1, r_new_string("month"));
  r_chr_poke(names, 2, r_new_string("day"));
  r_chr_poke(names, 3, r_new_string("hour"));
  r_chr_poke(names, 4, r_new_string("minute"));
  r_chr_poke(names, 5, r_new_string("second"));

  r_poke_names(out, names);

  UNPROTECT(2);
  return out;
}

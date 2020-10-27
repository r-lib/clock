#include "r.h"
#include "utils.h"
#include "zone.h"
#include "enums.h"
#include "conversion.h"
#include <date/tz.h>

static sexp civil_update_impl(sexp x,
                              sexp value,
                              enum update_unit unit,
                              enum day_nonexistant day_nonexistant,
                              enum dst_nonexistant dst_nonexistant,
                              enum dst_ambiguous dst_ambiguous);

[[cpp11::register]]
SEXP civil_update_cpp(SEXP x,
                      SEXP value,
                      SEXP unit,
                      SEXP day_nonexistant,
                      SEXP dst_nonexistant,
                      SEXP dst_ambiguous) {
  enum update_unit c_unit = parse_update_unit(unit);
  enum day_nonexistant c_day_nonexistant = parse_day_nonexistant(day_nonexistant);
  enum dst_nonexistant c_dst_nonexistant = parse_dst_nonexistant_no_directional(dst_nonexistant);
  enum dst_ambiguous c_dst_ambiguous = parse_dst_ambiguous_no_directional(dst_ambiguous);

  return civil_update_impl(
    x,
    value,
    c_unit,
    c_day_nonexistant,
    c_dst_nonexistant,
    c_dst_ambiguous
  );
}

// -----------------------------------------------------------------------------

static void civil_update_ymd(const double* p_x,
                             const int* p_value,
                             const date::time_zone* p_zone,
                             r_ssize size,
                             bool recycle_value,
                             enum update_unit unit,
                             enum day_nonexistant day_nonexistant,
                             enum dst_nonexistant dst_nonexistant,
                             enum dst_ambiguous dst_ambiguous,
                             double* p_out);

static void civil_update_hms(const double* p_x,
                             const int* p_value,
                             const date::time_zone* p_zone,
                             r_ssize size,
                             bool recycle_value,
                             enum update_unit unit,
                             enum day_nonexistant day_nonexistant,
                             enum dst_nonexistant dst_nonexistant,
                             enum dst_ambiguous dst_ambiguous,
                             double* p_out);

static void civil_update_yday(const double* p_x,
                              const int* p_value,
                              const date::time_zone* p_zone,
                              r_ssize size,
                              bool recycle_value,
                              enum update_unit unit,
                              enum day_nonexistant day_nonexistant,
                              enum dst_nonexistant dst_nonexistant,
                              enum dst_ambiguous dst_ambiguous,
                              double* p_out);

static sexp civil_update_impl(sexp x,
                              sexp value,
                              enum update_unit unit,
                              enum day_nonexistant day_nonexistant,
                              enum dst_nonexistant dst_nonexistant,
                              enum dst_ambiguous dst_ambiguous) {
  sexp tzone = civil_get_tzone(x);
  std::string zone_name = civil_zone_name_from_tzone(tzone);
  const date::time_zone* p_zone = civil_zone_name_load(zone_name);

  r_ssize size = r_length(x);
  const double* p_x = r_dbl_deref_const(x);

  const int* p_value = r_int_deref_const(value);
  bool recycle_value = r_is_scalar(value);

  sexp out = PROTECT(r_new_double(size));
  double* p_out = r_dbl_deref(out);

  r_poke_names(out, r_get_names(x));
  r_poke_class(out, civil_classes_posixct);
  civil_poke_tzone(out, tzone);

  switch (unit) {
  case update_unit::year:
  case update_unit::month:
  case update_unit::day: {
    civil_update_ymd(
      p_x,
      p_value,
      p_zone,
      size,
      recycle_value,
      unit,
      day_nonexistant,
      dst_nonexistant,
      dst_ambiguous,
      p_out
    );

    break;
  }
  case update_unit::hour:
  case update_unit::minute:
  case update_unit::second: {
    civil_update_hms(
      p_x,
      p_value,
      p_zone,
      size,
      recycle_value,
      unit,
      day_nonexistant,
      dst_nonexistant,
      dst_ambiguous,
      p_out
    );

    break;
  }
  case update_unit::yday: {
    civil_update_yday(
      p_x,
      p_value,
      p_zone,
      size,
      recycle_value,
      unit,
      day_nonexistant,
      dst_nonexistant,
      dst_ambiguous,
      p_out
    );

    break;
  }
  }

  UNPROTECT(1);
  return out;
}

static void civil_update_ymd(const double* p_x,
                             const int* p_value,
                             const date::time_zone* p_zone,
                             r_ssize size,
                             bool recycle_value,
                             enum update_unit unit,
                             enum day_nonexistant day_nonexistant,
                             enum dst_nonexistant dst_nonexistant,
                             enum dst_ambiguous dst_ambiguous,
                             double* p_out) {
  // Not used, but required as an argument
  const dst_direction dst_direction = dst_direction::positive;

  for (r_ssize i = 0; i < size; ++i) {
    const double x_elt = p_x[i];
    const int64_t elt = as_int64(x_elt);

    if (elt == r_int64_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    int elt_value = recycle_value ? p_value[0] : p_value[i];

    if (elt_value == NA_INTEGER) {
      p_out[i] = NA_REAL;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::sys_seconds elt_ssec{elt_sec};
    date::zoned_seconds elt_zsec = date::make_zoned(p_zone, elt_ssec);
    date::local_seconds elt_lsec = elt_zsec.get_local_time();

    date::local_days elt_lday = date::floor<date::days>(elt_lsec);
    date::local_seconds elt_lsec_floor = elt_lday;

    std::chrono::seconds elt_time_of_day = elt_lsec - elt_lsec_floor;

    date::year_month_day elt_ymd{elt_lday};

    date::year_month_day out_ymd;

    if (unit == update_unit::year) {
      if (elt_value > 9999 || elt_value < 0) {
        r_abort("Year based `value` must be within [0, 9999].");
      }

      out_ymd = date::year{elt_value} / elt_ymd.month() / elt_ymd.day();
    } else if (unit == update_unit::month) {
      if (elt_value > 12 || elt_value < 1) {
        r_abort("Month based `value` must be within [1, 12].");
      }

      // To appease date::month{}
      unsigned int month = static_cast<unsigned int>(elt_value);

      out_ymd = elt_ymd.year() / date::month{month} / elt_ymd.day();
    } else if (unit == update_unit::day) {
      if (elt_value > 31 || elt_value < 1) {
        r_abort("Day based `value` must be within [1, 31].");
      }

      // To appease date::day{}
      unsigned int day = static_cast<unsigned int>(elt_value);

      out_ymd = elt_ymd.year() / elt_ymd.month() / date::day{day};
    }

    if (!out_ymd.ok()) {
      switch (day_nonexistant) {
      case day_nonexistant::month_start: {
        // First day of next month - cancels out time of day
        out_ymd = out_ymd.year() / (out_ymd.month() + date::months(1)) / date::day(1);
        elt_time_of_day = std::chrono::seconds{0};
        break;
      }
      case day_nonexistant::month_end: {
        // Last day of current month, shifts time of day to last second
        out_ymd = out_ymd.year() / out_ymd.month() / date::last;
        elt_time_of_day = std::chrono::seconds{86400 - 1};
        break;
      }
      case day_nonexistant::na: {
        p_out[i] = NA_REAL;
        continue;
      }
      case day_nonexistant::error: {
        r_abort("Nonexistant day found at location %i.", (int) i + 1);
      }
      }
    }

    date::local_days out_lday{out_ymd};
    date::local_seconds out_lsec = out_lday;

    // Add time of day back on
    out_lsec += elt_time_of_day;

    p_out[i] = civil_local_seconds_to_posixt(
      out_lsec,
      p_zone,
      i,
      dst_direction,
      dst_nonexistant,
      dst_ambiguous
    );
  }
}

static void civil_update_hms(const double* p_x,
                             const int* p_value,
                             const date::time_zone* p_zone,
                             r_ssize size,
                             bool recycle_value,
                             enum update_unit unit,
                             enum day_nonexistant day_nonexistant,
                             enum dst_nonexistant dst_nonexistant,
                             enum dst_ambiguous dst_ambiguous,
                             double* p_out) {
  // Not used, but required as an argument
  const dst_direction dst_direction = dst_direction::positive;

  for (r_ssize i = 0; i < size; ++i) {
    const double x_elt = p_x[i];
    const int64_t elt = as_int64(x_elt);

    if (elt == r_int64_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    int elt_value = recycle_value ? p_value[0] : p_value[i];

    if (elt_value == NA_INTEGER) {
      p_out[i] = NA_REAL;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::sys_seconds elt_ssec{elt_sec};
    date::zoned_seconds elt_zsec = date::make_zoned(p_zone, elt_ssec);
    date::local_seconds elt_lsec = elt_zsec.get_local_time();

    date::local_days elt_lday = date::floor<date::days>(elt_lsec);
    date::local_seconds elt_lsec_floor = elt_lday;

    date::hh_mm_ss<std::chrono::seconds> elt_hms = date::make_time(elt_lsec - elt_lsec_floor);

    std::chrono::seconds out_time_of_day;

    if (unit == update_unit::hour) {
      if (elt_value > 23 || elt_value < 0) {
        r_abort("Hour based `value` must be within [0, 23].");
      }

      out_time_of_day =
        elt_hms.seconds() +
        elt_hms.minutes() +
        std::chrono::hours{elt_value};
    } else if (unit == update_unit::minute) {
      if (elt_value > 59 || elt_value < 0) {
        r_abort("Minute based `value` must be within [0, 59].");
      }

      out_time_of_day =
        elt_hms.seconds() +
        std::chrono::minutes{elt_value} +
        elt_hms.hours();
    } else if (unit == update_unit::second) {
      if (elt_value > 59 || elt_value < 0) {
        r_abort("Second based `value` must be within [0, 59].");
      }

      out_time_of_day =
        std::chrono::seconds{elt_value} +
        elt_hms.minutes() +
        elt_hms.hours();
    } else {
      r_abort("Internal error: Unexpected `unit`.");
    }

    date::local_seconds out_lsec = elt_lsec_floor + out_time_of_day;

    p_out[i] = civil_local_seconds_to_posixt(
      out_lsec,
      p_zone,
      i,
      dst_direction,
      dst_nonexistant,
      dst_ambiguous
    );
  }
}

static void yday_to_month_day(int yday,
                              bool is_leap,
                              unsigned int* p_day,
                              unsigned int* p_month);

static void civil_update_yday(const double* p_x,
                              const int* p_value,
                              const date::time_zone* p_zone,
                              r_ssize size,
                              bool recycle_value,
                              enum update_unit unit,
                              enum day_nonexistant day_nonexistant,
                              enum dst_nonexistant dst_nonexistant,
                              enum dst_ambiguous dst_ambiguous,
                              double* p_out) {
  // Not used, but required as an argument
  const dst_direction dst_direction = dst_direction::positive;

  for (r_ssize i = 0; i < size; ++i) {
    const double x_elt = p_x[i];
    const int64_t elt = as_int64(x_elt);

    if (elt == r_int64_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    int elt_value = recycle_value ? p_value[0] : p_value[i];

    if (elt_value == NA_INTEGER) {
      p_out[i] = NA_REAL;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::sys_seconds elt_ssec{elt_sec};
    date::zoned_seconds elt_zsec = date::make_zoned(p_zone, elt_ssec);
    date::local_seconds elt_lsec = elt_zsec.get_local_time();

    date::local_days elt_lday = date::floor<date::days>(elt_lsec);
    date::local_seconds elt_lsec_floor = elt_lday;

    std::chrono::seconds elt_time_of_day = elt_lsec - elt_lsec_floor;

    date::year_month_day elt_ymd{elt_lday};

    bool is_leap = elt_ymd.year().is_leap();

    date::year_month_day out_ymd;

    if (elt_value > 366 || elt_value < 0) {
      r_abort("Year day based `value` must be within [0, 366].");
    }

    if (elt_value == 366 && !is_leap) {
      // Implies !ok(), use `day_nonexistant`

      switch (day_nonexistant) {
      case day_nonexistant::month_start: {
        // First day of next year - cancels out time of day
        out_ymd = (elt_ymd.year() + date::years(1)) / date::month{1} / date::day{1};
        elt_time_of_day = std::chrono::seconds{0};
        break;
      }
      case day_nonexistant::month_end: {
        // Last day of current year, shifts time of day to last second
        out_ymd = elt_ymd.year() / date::month{12} / date::day{31};
        elt_time_of_day = std::chrono::seconds{86400 - 1};
        break;
      }
      case day_nonexistant::na: {
        p_out[i] = NA_REAL;
        continue;
      }
      case day_nonexistant::error: {
        r_abort("Nonexistant day found at location %i.", (int) i + 1);
      }
      }
    } else {
      unsigned int day;
      unsigned int month;
      yday_to_month_day(elt_value, is_leap, &day, &month);
      out_ymd = elt_ymd.year() / date::month{month} / date::day{day};
    }

    date::local_days out_lday{out_ymd};
    date::local_seconds out_lsec = out_lday;

    // Add time of day back on
    out_lsec += elt_time_of_day;

    p_out[i] = civil_local_seconds_to_posixt(
      out_lsec,
      p_zone,
      i,
      dst_direction,
      dst_nonexistant,
      dst_ambiguous
    );
  }
}


static const int DAYS_IN_MONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const int DAYS_UP_TO_MONTH[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

// yday should be 1 based [1, 366]
// p_day will get a 1 based day [1, 31]
// p_month will get a 1 based month [1, 12]
static void yday_to_month_day(int yday,
                              bool is_leap,
                              unsigned int* p_day,
                              unsigned int* p_month) {
  int day = yday - 1;
  int month = (day + 50) >> 5;

  // Number of days up to this month, computed using our `month` guess
  int preceding = DAYS_UP_TO_MONTH[month - 1] + (is_leap && month > 2);

  // If the number of `preceding` days is greater than the `n` yday
  // position in the year, then we obviously went too far. So subtract 1
  // month and recompute the number of days up to the (now correct) month.
  if (preceding > day) {
    --month;
    preceding -= DAYS_IN_MONTH[month - 1] + (is_leap && month == 2);
  }

  // Substract `position in year` - `days up to current month` = `day in month`
  // It will be 0-30 based already
  day -= preceding;

  // map to [1, 31]
  ++day;

  *p_day = static_cast<unsigned int>(day);
  *p_month = static_cast<unsigned int>(month);
}

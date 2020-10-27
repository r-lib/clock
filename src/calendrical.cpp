#include "r.h"
#include "utils.h"
#include "zone.h"
#include "enums.h"
#include "local.h"
#include "conversion.h"
#include <date/date.h>
#include <date/tz.h>

static sexp civil_add_calendrical_impl(sexp x,
                                       sexp n,
                                       enum unit unit,
                                       enum day_nonexistant day_nonexistant,
                                       enum dst_nonexistant dst_nonexistant,
                                       enum dst_ambiguous dst_ambiguous,
                                       r_ssize size);

[[cpp11::register]]
SEXP civil_add_calendrical_cpp(SEXP x,
                               SEXP n,
                               SEXP unit,
                               SEXP day_nonexistant,
                               SEXP dst_nonexistant,
                               SEXP dst_ambiguous,
                               SEXP size) {
  enum unit c_unit = parse_unit(unit);
  enum day_nonexistant c_day_nonexistant = parse_day_nonexistant(day_nonexistant);
  enum dst_nonexistant c_dst_nonexistant = parse_dst_nonexistant(dst_nonexistant);
  enum dst_ambiguous c_dst_ambiguous = parse_dst_ambiguous(dst_ambiguous);
  r_ssize c_size = r_int_get(size, 0);

  return civil_add_calendrical_impl(
    x,
    n,
    c_unit,
    c_day_nonexistant,
    c_dst_nonexistant,
    c_dst_ambiguous,
    c_size
  );
}


static sexp civil_add_calendrical_years(sexp x,
                                        sexp n,
                                        enum day_nonexistant day_nonexistant,
                                        enum dst_nonexistant dst_nonexistant,
                                        enum dst_ambiguous dst_ambiguous,
                                        r_ssize size);

static sexp civil_add_calendrical_months(sexp x,
                                         sexp n,
                                         enum day_nonexistant day_nonexistant,
                                         enum dst_nonexistant dst_nonexistant,
                                         enum dst_ambiguous dst_ambiguous,
                                         r_ssize size);

static sexp civil_add_calendrical_impl(sexp x,
                                       sexp n,
                                       enum unit unit,
                                       enum day_nonexistant day_nonexistant,
                                       enum dst_nonexistant dst_nonexistant,
                                       enum dst_ambiguous dst_ambiguous,
                                       r_ssize size) {
  if (unit == unit::year) {
    return civil_add_calendrical_years(
      x,
      n,
      day_nonexistant,
      dst_nonexistant,
      dst_ambiguous,
      size
    );
  }

  if (unit == unit::month) {
    return civil_add_calendrical_months(
      x,
      n,
      day_nonexistant,
      dst_nonexistant,
      dst_ambiguous,
      size
    );
  }

  sexp years = r_null;
  sexp months = r_null;
  sexp weeks = (unit == unit::week) ? n : r_null;
  sexp days = (unit == unit::day) ? n : r_null;
  sexp hours = (unit == unit::hour) ? n : r_null;
  sexp minutes = (unit == unit::minute) ? n : r_null;
  sexp seconds = (unit == unit::second) ? n : r_null;

  return civil_add_local_impl(
    x,
    years,
    months,
    weeks,
    days,
    hours,
    minutes,
    seconds,
    dst_nonexistant,
    dst_ambiguous,
    size
  );
}

// -----------------------------------------------------------------------------

static sexp civil_add_calendrical_years_or_months(sexp x,
                                                  sexp n,
                                                  enum day_nonexistant day_nonexistant,
                                                  enum dst_nonexistant dst_nonexistant,
                                                  enum dst_ambiguous dst_ambiguous,
                                                  r_ssize size,
                                                  bool years);

static sexp civil_add_calendrical_years(sexp x,
                                        sexp n,
                                        enum day_nonexistant day_nonexistant,
                                        enum dst_nonexistant dst_nonexistant,
                                        enum dst_ambiguous dst_ambiguous,
                                        r_ssize size) {
  return civil_add_calendrical_years_or_months(
    x,
    n,
    day_nonexistant,
    dst_nonexistant,
    dst_ambiguous,
    size,
    true
  );
}

static sexp civil_add_calendrical_months(sexp x,
                                         sexp n,
                                         enum day_nonexistant day_nonexistant,
                                         enum dst_nonexistant dst_nonexistant,
                                         enum dst_ambiguous dst_ambiguous,
                                         r_ssize size) {
  return civil_add_calendrical_years_or_months(
    x,
    n,
    day_nonexistant,
    dst_nonexistant,
    dst_ambiguous,
    size,
    false
  );
}

static sexp civil_add_calendrical_years_or_months(sexp x,
                                                  sexp n,
                                                  enum day_nonexistant day_nonexistant,
                                                  enum dst_nonexistant dst_nonexistant,
                                                  enum dst_ambiguous dst_ambiguous,
                                                  r_ssize size,
                                                  bool years) {
  sexp out = PROTECT(r_new_double(size));
  double* p_out = r_dbl_deref(out);

  sexp tzone = civil_get_tzone(x);
  std::string zone_name = civil_zone_name_from_tzone(tzone);
  const date::time_zone* p_zone = civil_zone_name_load(zone_name);

  r_poke_names(out, r_get_names(x));
  r_poke_class(out, civil_classes_posixct);
  civil_poke_tzone(out, tzone);

  const double* p_x = r_dbl_deref_const(x);
  const int* p_n = r_int_deref_const(n);

  const bool recycle_x = r_is_scalar(x);
  const bool recycle_n = r_is_scalar(n);

  for (r_ssize i = 0; i < size; ++i) {
    double x_elt = recycle_x ? p_x[0] : p_x[i];
    int n_elt = recycle_n ? p_n[0] : p_n[i];

    int64_t elt = as_int64(x_elt);

    if (elt == r_int64_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    if (n_elt == NA_INTEGER) {
      p_out[i] = r_dbl_na;
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

    if (years) {
      out_ymd = elt_ymd + date::years{n_elt};
    } else {
      out_ymd = elt_ymd + date::months{n_elt};
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
    date::local_seconds out_lsec_floor{out_lday};
    date::local_seconds out_lsec{out_lsec_floor + elt_time_of_day};

    const enum dst_direction dst_direction =
      n_elt >= 0 ?
      dst_direction::positive :
      dst_direction::negative;

    p_out[i] = civil_local_seconds_to_posixt(
      out_lsec,
      p_zone,
      i,
      dst_direction,
      dst_nonexistant,
      dst_ambiguous
    );
  }

  UNPROTECT(1);
  return out;
}

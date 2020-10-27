#include "local.h"
#include "utils.h"
#include "zone.h"
#include "conversion.h"
#include <date/date.h>
#include <date/tz.h>

[[cpp11::register]]
SEXP civil_add_local_cpp(SEXP x,
                         SEXP years,
                         SEXP months,
                         SEXP weeks,
                         SEXP days,
                         SEXP hours,
                         SEXP minutes,
                         SEXP seconds,
                         SEXP dst_nonexistant,
                         SEXP dst_ambiguous,
                         SEXP size) {
  enum dst_nonexistant c_dst_nonexistant = parse_dst_nonexistant(dst_nonexistant);
  enum dst_ambiguous c_dst_ambiguous = parse_dst_ambiguous(dst_ambiguous);
  r_ssize c_size = r_int_get(size, 0);

  return civil_add_local_impl(
    x,
    years,
    months,
    weeks,
    days,
    hours,
    minutes,
    seconds,
    c_dst_nonexistant,
    c_dst_ambiguous,
    c_size
  );
}


sexp civil_add_local_impl(sexp x,
                          sexp years,
                          sexp months,
                          sexp weeks,
                          sexp days,
                          sexp hours,
                          sexp minutes,
                          sexp seconds,
                          dst_nonexistant dst_nonexistant,
                          dst_ambiguous dst_ambiguous,
                          r_ssize size) {
  sexp out = PROTECT(r_new_double(size));
  double* p_out = r_dbl_deref(out);

  sexp tzone = civil_get_tzone(x);
  std::string zone_name = civil_zone_name_from_tzone(tzone);
  const date::time_zone* p_zone = civil_zone_name_load(zone_name);

  r_poke_names(out, r_get_names(x));
  r_poke_class(out, civil_classes_posixct);
  civil_poke_tzone(out, tzone);

  const bool do_years = !r_is_null(years);
  const bool do_months = !r_is_null(months);
  const bool do_weeks = !r_is_null(weeks);
  const bool do_days = !r_is_null(days);
  const bool do_hours = !r_is_null(hours);
  const bool do_minutes = !r_is_null(minutes);
  const bool do_seconds = !r_is_null(seconds);

  const double* p_x = r_dbl_deref_const(x);

  const int* p_years = do_years ? r_int_deref_const(years) : NULL;
  const int* p_months = do_months ? r_int_deref_const(months) : NULL;
  const int* p_weeks = do_weeks ? r_int_deref_const(weeks) : NULL;
  const int* p_days = do_days ? r_int_deref_const(days) : NULL;
  const int* p_hours = do_hours ? r_int_deref_const(hours) : NULL;
  const int* p_minutes = do_minutes ? r_int_deref_const(minutes) : NULL;
  const int* p_seconds = do_seconds ? r_int_deref_const(seconds) : NULL;

  const bool recycle_x = r_is_scalar(x);

  const bool recycle_years = do_years && r_is_scalar(years);
  const bool recycle_months = do_months && r_is_scalar(months);
  const bool recycle_weeks = do_weeks && r_is_scalar(weeks);
  const bool recycle_days = do_days && r_is_scalar(days);
  const bool recycle_hours = do_hours && r_is_scalar(hours);
  const bool recycle_minutes = do_minutes && r_is_scalar(minutes);
  const bool recycle_seconds = do_seconds && r_is_scalar(seconds);

  for (r_ssize i = 0; i < size; ++i) {
    const double x_elt = recycle_x ? p_x[0] : p_x[i];
    const int64_t elt = as_int64(x_elt);

    if (elt == r_int64_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    const std::chrono::seconds elt_sec{elt};
    const date::sys_seconds elt_ssec{elt_sec};
    const date::zoned_seconds elt_zsec = date::make_zoned(p_zone, elt_ssec);
    const date::local_seconds elt_lsec = elt_zsec.get_local_time();

    std::chrono::seconds duration{0};

    if (do_years) {
      int elt_years = recycle_years ? p_years[0] : p_years[i];

      if (elt_years == NA_INTEGER) {
        p_out[i] = r_dbl_na;
        continue;
      }

      duration += date::years{elt_years};
    }

    if (do_months) {
      int elt_months = recycle_months ? p_months[0] : p_months[i];

      if (elt_months == NA_INTEGER) {
        p_out[i] = r_dbl_na;
        continue;
      }

      duration += date::months{elt_months};
    }

    if (do_weeks) {
      int elt_weeks = recycle_weeks ? p_weeks[0] : p_weeks[i];

      if (elt_weeks == NA_INTEGER) {
        p_out[i] = r_dbl_na;
        continue;
      }

      elt_weeks *= 7;

      duration += date::days{elt_weeks};
    }

    if (do_days) {
      int elt_days = recycle_days ? p_days[0] : p_days[i];

      if (elt_days == NA_INTEGER) {
        p_out[i] = r_dbl_na;
        continue;
      }

      duration += date::days{elt_days};
    }

    if (do_hours) {
      int elt_hours = recycle_hours ? p_hours[0] : p_hours[i];

      if (elt_hours == NA_INTEGER) {
        p_out[i] = r_dbl_na;
        continue;
      }

      duration += std::chrono::hours{elt_hours};
    }

    if (do_minutes) {
      int elt_minutes = recycle_minutes ? p_minutes[0] : p_minutes[i];

      if (elt_minutes == NA_INTEGER) {
        p_out[i] = r_dbl_na;
        continue;
      }

      duration += std::chrono::minutes{elt_minutes};
    }

    if (do_seconds) {
      int elt_seconds = recycle_seconds ? p_seconds[0] : p_seconds[i];

      if (elt_seconds == NA_INTEGER) {
        p_out[i] = r_dbl_na;
        continue;
      }

      duration += std::chrono::seconds{elt_seconds};
    }

    const date::local_seconds out_lsec = elt_lsec + duration;

    const enum dst_direction dst_direction =
      duration.count() > 0 ?
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


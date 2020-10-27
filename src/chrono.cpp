#include "r.h"
#include "utils.h"
#include "zone.h"
#include <date/date.h>

[[cpp11::register]]
SEXP civil_add_chrono_cpp(SEXP x,
                          SEXP years,
                          SEXP months,
                          SEXP weeks,
                          SEXP days,
                          SEXP hours,
                          SEXP minutes,
                          SEXP seconds,
                          SEXP size) {
  r_ssize out_size = r_int_get(size, 0);

  sexp out = PROTECT(r_new_double(out_size));
  double* p_out = r_dbl_deref(out);

  r_poke_names(out, r_get_names(x));
  r_poke_class(out, civil_classes_posixct);
  civil_poke_tzone(out, civil_get_tzone(x));

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

  for (r_ssize i = 0; i < out_size; ++i) {
    const double x_elt = recycle_x ? p_x[0] : p_x[i];
    const int64_t elt = as_int64(x_elt);

    if (elt == r_int64_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    const std::chrono::seconds elt_sec{elt};
    const date::sys_seconds elt_ssec{elt_sec};

    std::chrono::seconds duration{0};

    if (do_years) {
      duration += date::years{recycle_years ? p_years[0] : p_years[i]};
    }
    if (do_months) {
      duration += date::months{recycle_months ? p_months[0] : p_months[i]};
    }
    if (do_weeks) {
      duration += date::days{(recycle_weeks ? p_weeks[0] : p_weeks[i]) * 7};
    }
    if (do_days) {
      duration += date::days{recycle_days ? p_days[0] : p_days[i]};
    }
    if (do_hours) {
      duration += std::chrono::hours{recycle_hours ? p_hours[0] : p_hours[i]};
    }
    if (do_minutes) {
      duration += std::chrono::minutes{recycle_minutes ? p_minutes[0] : p_minutes[i]};
    }
    if (do_seconds) {
      duration += std::chrono::seconds{recycle_seconds ? p_seconds[0] : p_seconds[i]};
    }

    const date::sys_seconds out_ssec = elt_ssec + duration;

    p_out[i] = out_ssec.time_since_epoch().count();
  }

  UNPROTECT(1);
  return out;
}


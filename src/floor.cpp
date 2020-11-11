#include "utils.h"
#include <date/date.h>

[[cpp11::register]]
SEXP floor_days_to_year_month_cpp(SEXP days) {
  r_ssize size = r_length(days);

  sexp out = PROTECT(r_new_integer(size));
  int* p_out = r_int_deref(out);

  const int* p_days = r_int_deref_const(days);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = p_days[i];

    if (elt_days == r_int_na) {
      p_out[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_days}};
    date::year_month_day elt_ymd{elt_lday};

    date::year_month_day out_ymd{
      elt_ymd.year() / elt_ymd.month() / date::day{1}
    };

    date::local_days out_lday{out_ymd};

    p_out[i] = out_lday.time_since_epoch().count();
  }

  UNPROTECT(1);
  return out;
}

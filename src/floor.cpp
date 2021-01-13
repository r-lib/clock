#include "clock.h"
#include "utils.h"

[[cpp11::register]]
clock_writable_field floor_calendar_days_to_iso_year_weeknum_precision(const clock_field& calendar) {
  r_ssize size = calendar.size();

  clock_writable_field out_calendar{calendar};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_calendar = calendar[i];

    if (elt_calendar == r_int_na) {
      out_calendar[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    iso_week::year_weeknum_weekday elt_yww{elt_lday};

    iso_week::year_weeknum_weekday out_yww{
      elt_yww.year() / elt_yww.weeknum() / iso_week::weekday{1u}
    };

    date::local_days out_lday{out_yww};

    out_calendar[i] = out_lday.time_since_epoch().count();
  }

  return out_calendar;
}

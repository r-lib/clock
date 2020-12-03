#include "civil.h"

[[cpp11::register]]
civil_writable_field floor_days_to_year_month_precision_cpp(const civil_field& days) {
  r_ssize size = days.size();

  civil_writable_field out_days(size);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = days[i];

    if (elt_days == r_int_na) {
      out_days[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_days}};
    date::year_month_day elt_ymd{elt_lday};

    date::year_month_day out_ymd{
      elt_ymd.year() / elt_ymd.month() / date::day{1}
    };

    date::local_days out_lday{out_ymd};

    out_days[i] = out_lday.time_since_epoch().count();
  }

  return out_days;
}

[[cpp11::register]]
civil_writable_field floor_days_to_year_quarter_precision_cpp(const civil_field& days,
                                                              int fiscal_start) {
  r_ssize size = days.size();

  civil_writable_field out_days(size);

  fiscal_year::fiscal_start start{static_cast<unsigned int>(fiscal_start)};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = days[i];

    if (elt_days == r_int_na) {
      out_days[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_days}};
    fiscal_year::year_quarter_day elt_yqd(elt_lday, start);

    fiscal_year::year_quarter_day out_yqd(
        elt_yqd.year(),
        elt_yqd.quarter(),
        fiscal_year::day{1u},
        start
    );

    date::local_days out_lday{out_yqd};

    out_days[i] = out_lday.time_since_epoch().count();
  }

  return out_days;
}

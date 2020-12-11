#include "civil.h"
#include "utils.h"

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

template <fiscal_year::start S>
civil_writable_field floor_days_to_year_quarternum_precision(const civil_field& days) {
  r_ssize size = days.size();

  civil_writable_field out_days(size);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = days[i];

    if (elt_days == r_int_na) {
      out_days[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_days}};
    fiscal_year::year_quarternum_quarterday<S> elt_yqnqd(elt_lday);

    fiscal_year::year_quarternum_quarterday<S> out_yqnqd(
        elt_yqnqd.year(),
        elt_yqnqd.quarternum(),
        fiscal_year::quarterday{1u}
    );

    date::local_days out_lday{out_yqnqd};

    out_days[i] = out_lday.time_since_epoch().count();
  }

  return out_days;
}

[[cpp11::register]]
civil_writable_field floor_days_to_year_quarternum_precision_cpp(const civil_field& days,
                                                                 int fiscal_start) {
  if (fiscal_start == 1) {
    return floor_days_to_year_quarternum_precision<fiscal_year::start::january>(days);
  } else if (fiscal_start == 2) {
    return floor_days_to_year_quarternum_precision<fiscal_year::start::february>(days);
  } else if (fiscal_start == 3) {
    return floor_days_to_year_quarternum_precision<fiscal_year::start::march>(days);
  } else if (fiscal_start == 4) {
    return floor_days_to_year_quarternum_precision<fiscal_year::start::april>(days);
  } else if (fiscal_start == 5) {
    return floor_days_to_year_quarternum_precision<fiscal_year::start::may>(days);
  } else if (fiscal_start == 6) {
    return floor_days_to_year_quarternum_precision<fiscal_year::start::june>(days);
  } else if (fiscal_start == 7) {
    return floor_days_to_year_quarternum_precision<fiscal_year::start::july>(days);
  } else if (fiscal_start == 8) {
    return floor_days_to_year_quarternum_precision<fiscal_year::start::august>(days);
  } else if (fiscal_start == 9) {
    return floor_days_to_year_quarternum_precision<fiscal_year::start::september>(days);
  } else if (fiscal_start == 10) {
    return floor_days_to_year_quarternum_precision<fiscal_year::start::october>(days);
  } else if (fiscal_start == 11) {
    return floor_days_to_year_quarternum_precision<fiscal_year::start::november>(days);
  } else if (fiscal_start == 12) {
    return floor_days_to_year_quarternum_precision<fiscal_year::start::december>(days);
  }

  never_reached("floor_days_to_year_quarternum_precision_cpp");
}

[[cpp11::register]]
civil_writable_field floor_days_to_iso_year_weeknum_precision_cpp(const civil_field& days) {
  r_ssize size = days.size();

  civil_writable_field out_days(size);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = days[i];

    if (elt_days == r_int_na) {
      out_days[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_days}};
    iso_week::year_weeknum_weekday elt_yww{elt_lday};

    iso_week::year_weeknum_weekday out_yww{
      elt_yww.year() / elt_yww.weeknum() / iso_week::weekday{1u}
    };

    date::local_days out_lday{out_yww};

    out_days[i] = out_lday.time_since_epoch().count();
  }

  return out_days;
}

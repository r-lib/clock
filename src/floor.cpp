#include "civil.h"
#include "utils.h"

[[cpp11::register]]
civil_writable_field floor_calendar_days_to_year_month_precision(const civil_field& calendar) {
  r_ssize size = calendar.size();

  civil_writable_field out_calendar{calendar};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_calendar = calendar[i];

    if (elt_calendar == r_int_na) {
      out_calendar[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    date::year_month_day elt_ymd{elt_lday};

    date::year_month_day out_ymd{
      elt_ymd.year() / elt_ymd.month() / date::day{1}
    };

    date::local_days out_lday{out_ymd};

    out_calendar[i] = out_lday.time_since_epoch().count();
  }

  return out_calendar;
}

template <quarterly::start S>
civil_writable_field floor_days_to_year_quarternum_precision_impl(const civil_field& calendar) {
  r_ssize size = calendar.size();

  civil_writable_field out_calendar{calendar};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_calendar = calendar[i];

    if (elt_calendar == r_int_na) {
      out_calendar[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    quarterly::year_quarternum_quarterday<S> elt_yqnqd(elt_lday);

    quarterly::year_quarternum_quarterday<S> out_yqnqd(
        elt_yqnqd.year(),
        elt_yqnqd.quarternum(),
        quarterly::quarterday{1u}
    );

    date::local_days out_lday{out_yqnqd};

    out_calendar[i] = out_lday.time_since_epoch().count();
  }

  return out_calendar;
}

[[cpp11::register]]
civil_writable_field floor_calendar_days_to_year_quarternum_precision(const civil_field& calendar,
                                                                      int start) {
  if (start == 1) {
    return floor_days_to_year_quarternum_precision_impl<quarterly::start::january>(calendar);
  } else if (start == 2) {
    return floor_days_to_year_quarternum_precision_impl<quarterly::start::february>(calendar);
  } else if (start == 3) {
    return floor_days_to_year_quarternum_precision_impl<quarterly::start::march>(calendar);
  } else if (start == 4) {
    return floor_days_to_year_quarternum_precision_impl<quarterly::start::april>(calendar);
  } else if (start == 5) {
    return floor_days_to_year_quarternum_precision_impl<quarterly::start::may>(calendar);
  } else if (start == 6) {
    return floor_days_to_year_quarternum_precision_impl<quarterly::start::june>(calendar);
  } else if (start == 7) {
    return floor_days_to_year_quarternum_precision_impl<quarterly::start::july>(calendar);
  } else if (start == 8) {
    return floor_days_to_year_quarternum_precision_impl<quarterly::start::august>(calendar);
  } else if (start == 9) {
    return floor_days_to_year_quarternum_precision_impl<quarterly::start::september>(calendar);
  } else if (start == 10) {
    return floor_days_to_year_quarternum_precision_impl<quarterly::start::october>(calendar);
  } else if (start == 11) {
    return floor_days_to_year_quarternum_precision_impl<quarterly::start::november>(calendar);
  } else if (start == 12) {
    return floor_days_to_year_quarternum_precision_impl<quarterly::start::december>(calendar);
  }

  never_reached("floor_calendar_days_to_year_quarternum_precision");
}

[[cpp11::register]]
civil_writable_field floor_calendar_days_to_iso_year_weeknum_precision(const civil_field& calendar) {
  r_ssize size = calendar.size();

  civil_writable_field out_calendar{calendar};

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

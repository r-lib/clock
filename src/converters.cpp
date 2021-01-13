#include "clock.h"
#include "utils.h"
#include "zone.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "check.h"

// -----------------------------------------------------------------------------

[[cpp11::register]]
clock_writable_field convert_iso_year_weeknum_weekday_to_calendar_days(const cpp11::integers& year,
                                                                       const cpp11::integers& weeknum,
                                                                       const cpp11::integers& weekday,
                                                                       const cpp11::strings& day_nonexistent) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);

  r_ssize size = year.size();

  cpp11::writable::integers out(size);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_year = year[i];
    int elt_weeknum = weeknum[i];
    int elt_weekday = weekday[i];

    if (elt_year == r_int_na || elt_weeknum == r_int_na || elt_weekday == r_int_na) {
      out[i] = r_int_na;
      continue;
    }

    check_range_year(elt_year, "year");
    check_range_weeknum(elt_weeknum, "weeknum");
    check_range_weekday(elt_weekday, "weekday");

    iso_week::year_weeknum_weekday elt_yww{
      iso_week::year{elt_year},
      iso_week::weeknum{static_cast<unsigned>(elt_weeknum)},
      iso_week::weekday{static_cast<unsigned>(elt_weekday)}
    };

    if (!elt_yww.ok()) {
      bool na = false;
      resolve_day_nonexistent_yww(i, day_nonexistent_val, elt_yww, na);

      if (na) {
        out[i] = r_int_na;
        continue;
      }
    }

    date::local_days elt_lday{elt_yww};

    out[i] = elt_lday.time_since_epoch().count();
  }

  return out;
}


[[cpp11::register]]
cpp11::writable::list_of<cpp11::writable::integers>
convert_calendar_days_to_iso_year_weeknum_weekday(const clock_field& calendar) {
  r_ssize size = calendar.size();

  cpp11::writable::integers out_year(size);
  cpp11::writable::integers out_weeknum(size);
  cpp11::writable::integers out_weekday(size);

  cpp11::writable::list out{out_year, out_weeknum, out_weekday};
  out.names() = {"year", "weeknum", "weekday"};

  for (r_ssize i = 0; i < size; ++i) {
    int elt_calendar = calendar[i];

    if (elt_calendar == r_int_na) {
      out_year[i] = r_int_na;
      out_weeknum[i] = r_int_na;
      out_weekday[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    iso_week::year_weeknum_weekday elt_yww{elt_lday};

    out_year[i] = static_cast<int>(elt_yww.year());
    out_weeknum[i] = static_cast<unsigned int>(elt_yww.weeknum());
    out_weekday[i] = static_cast<unsigned int>(elt_yww.weekday());
  }

  return out;
}

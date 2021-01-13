#include "clock.h"
#include "zone.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list add_iso_calendar_years(const clock_field& calendar,
                                             const cpp11::integers& n,
                                             const cpp11::strings& day_nonexistent) {
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);

  const r_ssize size = calendar.size();

  clock_writable_field out_calendar{calendar};
  cpp11::writable::logicals ok(size);
  cpp11::writable::logicals any(1);
  any[0] = false;

  cpp11::writable::list out({out_calendar, ok, any});
  out.names() = {"calendar", "ok", "any"};

  for (r_ssize i = 0; i < size; ++i) {
    ok[i] = true;

    int elt_calendar = calendar[i];
    int elt_n = n[i];

    if (elt_calendar == r_int_na) {
      continue;
    }
    if (elt_n == r_int_na) {
      ok[i] = r_lgl_na;
      any[0] = true;
      out_calendar[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{date::days{elt_calendar}};
    iso_week::year_weeknum_weekday elt_yww{elt_lday};

    iso_week::year_weeknum_weekday out_yww = elt_yww + iso_week::years{elt_n};

    convert_iso_yww_to_calendar_one(
      i,
      day_nonexistent_val,
      out_yww,
      out_calendar,
      ok,
      any
    );
  }

  return out;
}

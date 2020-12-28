#include "year-month-day.h"
#include "enums.h"
#include "resolve.h"

[[cpp11::register]]
cpp11::writable::strings
format_year_month_day(const cpp11::integers& year,
                      const cpp11::integers& month,
                      const cpp11::integers& day) {
  const r_ssize size = year.size();
  cpp11::writable::strings out(size);
  const rclock::year_month_day ymd(year, month, day);

  std::ostringstream stream;

  for (r_ssize i = 0; i < size; ++i) {
    if (ymd.is_na(i)) {
      SET_STRING_ELT(out, i, r_chr_na);
      continue;
    }

    stream.str(std::string());
    stream.clear();

    const date::year_month_day elt = ymd[i];

    // Don't use the << operator of year_month_day, as that prints out
    // "not a valid date" on invalid dates, which we print differently
    stream << elt.year() << '-';
    stream.fill('0');
    stream.flags(std::ios::dec | std::ios::right);
    stream.width(2);
    stream << static_cast<unsigned>(elt.month()) << '-';
    stream << elt.day();

    if (stream.fail()) {
      // Should never happen but...
      SET_STRING_ELT(out, i, r_chr_na);
      continue;
    }

    const std::string string = stream.str();
    const SEXP r_string = Rf_mkCharLenCE(string.c_str(), string.size(), CE_UTF8);
    SET_STRING_ELT(out, i, r_string);
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::logicals invalid_detect_year_month_day(const cpp11::integers& year,
                                                        const cpp11::integers& month,
                                                        const cpp11::integers& day) {
  const r_ssize size = year.size();
  cpp11::writable::logicals out(size);
  const rclock::year_month_day ymd(year, month, day);

  for (r_ssize i = 0; i < size; ++i) {
    if (ymd.is_na(i)) {
      out[i] = false;
    } else {
      out[i] = !ymd[i].ok();
    }
  }

  return out;
}

[[cpp11::register]]
int invalid_count_year_month_day(const cpp11::integers& year,
                                 const cpp11::integers& month,
                                 const cpp11::integers& day) {
  int count = 0;
  const r_ssize size = year.size();
  const rclock::year_month_day ymd(year, month, day);

  for (r_ssize i = 0; i < size; ++i) {
    if (ymd.is_na(i)) {
      continue;
    }
    if (!ymd[i].ok()) {
      ++count;
    }
  }

  return count;
}

[[cpp11::register]]
bool invalid_any_year_month_day(const cpp11::integers& year,
                                const cpp11::integers& month,
                                const cpp11::integers& day) {
  const r_ssize size = year.size();
  const rclock::year_month_day ymd(year, month, day);

  for (r_ssize i = 0; i < size; ++i) {
    if (ymd.is_na(i) || ymd[i].ok()) {
      continue;
    }
    return true;
  }

  return false;
}

[[cpp11::register]]
cpp11::writable::list_of<cpp11::writable::integers>
invalid_resolve_year_month_day(const cpp11::integers& year,
                               const cpp11::integers& month,
                               const cpp11::integers& day,
                               const cpp11::strings& invalid) {
  const enum invalid invalid_val = parse_invalid(invalid);
  const r_ssize size = year.size();
  const rclock::year_month_day ymd(year, month, day);
  rclock::writable::year_month_day out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (ymd.is_na(i)) {
      out.assign_na(i);
      continue;
    }

    date::year_month_day elt = ymd[i];

    if (elt.ok()) {
      out.assign(elt, i);
      continue;
    }

    switch (invalid_val) {
    case invalid::first_day:
    case invalid::first_time: {
      resolve_day_nonexistent_ymd_first_day(elt);
      out.assign(elt, i);
      continue;
    }
    case invalid::last_day:
    case invalid::last_time: {
      resolve_day_nonexistent_ymd_last_day(elt);
      out.assign(elt, i);
      continue;
    }
    case invalid::na: {
      out.assign_na(i);
      continue;
    }
    case invalid::error: {
      resolve_day_nonexistent_error(i);
    }
    }
  }

  return out.to_list();
}

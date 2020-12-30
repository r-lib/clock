#include "iso-year-weeknum.h"
#include "enums.h"
#include "resolve.h"
#include "check.h"

[[cpp11::register]]
cpp11::writable::strings
format_iso_year_weeknum(const cpp11::integers& year,
                        const cpp11::integers& weeknum) {
  const r_ssize size = year.size();
  cpp11::writable::strings out(size);
  const rclock::iso_year_weeknum x(year, weeknum);

  std::ostringstream stream;

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      SET_STRING_ELT(out, i, r_chr_na);
      continue;
    }

    stream.str(std::string());
    stream.clear();

    const iso_week::year_weeknum elt = x[i];

    stream << elt.year() << '-' << 'W';
    stream.fill('0');
    stream.flags(std::ios::dec | std::ios::right);
    stream.width(2);
    stream << static_cast<unsigned>(elt.weeknum());

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
cpp11::writable::logicals
invalid_detect_iso_year_weeknum(const cpp11::integers& year,
                                const cpp11::integers& weeknum) {
  const r_ssize size = year.size();
  cpp11::writable::logicals out(size);
  const rclock::iso_year_weeknum x(year, weeknum);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = false;
    } else {
      out[i] = !x[i].ok();
    }
  }

  return out;
}

[[cpp11::register]]
int
invalid_count_iso_year_weeknum(const cpp11::integers& year,
                               const cpp11::integers& weeknum) {
  int count = 0;
  const r_ssize size = year.size();
  const rclock::iso_year_weeknum x(year, weeknum);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      continue;
    }
    if (!x[i].ok()) {
      ++count;
    }
  }

  return count;
}

[[cpp11::register]]
bool
invalid_any_iso_year_weeknum(const cpp11::integers& year,
                             const cpp11::integers& weeknum) {
  const r_ssize size = year.size();
  const rclock::iso_year_weeknum x(year, weeknum);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i) || x[i].ok()) {
      continue;
    }
    return true;
  }

  return false;
}

[[cpp11::register]]
cpp11::writable::list_of<cpp11::writable::integers>
invalid_resolve_iso_year_weeknum(const cpp11::integers& year,
                                 const cpp11::integers& weeknum,
                                 const cpp11::strings& invalid) {
  const enum invalid invalid_val = parse_invalid(invalid);
  const r_ssize size = year.size();
  const rclock::iso_year_weeknum x(year, weeknum);
  rclock::writable::iso_year_weeknum out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out.assign_na(i);
      continue;
    }

    iso_week::year_weeknum elt = x[i];

    if (elt.ok()) {
      out.assign(elt, i);
      continue;
    }

    switch (invalid_val) {
    case invalid::first_day:
    case invalid::first_time: {
      resolve_day_nonexistent_yw_first_day(elt);
      out.assign(elt, i);
      continue;
    }
    case invalid::last_day:
    case invalid::last_time: {
      resolve_day_nonexistent_yw_last_day(elt);
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

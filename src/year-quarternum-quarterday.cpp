#include "year-quarternum-quarterday.h"
#include "enums.h"
#include "resolve.h"
#include "check.h"

template <quarterly::start S>
static
inline
cpp11::writable::strings
format_year_quarternum_quarterday_impl(const cpp11::integers& year,
                                       const cpp11::integers& quarternum,
                                       const cpp11::integers& quarterday) {
  const r_ssize size = year.size();
  cpp11::writable::strings out(size);
  const rclock::year_quarternum_quarterday<S> x(year, quarternum, quarterday);

  std::ostringstream stream;

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      SET_STRING_ELT(out, i, r_chr_na);
      continue;
    }

    stream.str(std::string());
    stream.clear();

    const quarterly::year_quarternum_quarterday<S> elt = x[i];

    stream << elt.year() << '-' << 'Q';
    stream.width(1);
    stream << static_cast<unsigned>(elt.quarternum()) << '-';
    stream.fill('0');
    stream.flags(std::ios::dec | std::ios::right);
    stream.width(2);
    stream << static_cast<unsigned>(elt.quarterday());

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
cpp11::writable::strings
format_year_quarternum_quarterday(const cpp11::integers& year,
                                  const cpp11::integers& quarternum,
                                  const cpp11::integers& quarterday,
                                  const cpp11::integers& start) {
  const int s = start[0];

  if (s == 1) return format_year_quarternum_quarterday_impl<quarterly::start::january>(year, quarternum, quarterday);
  else if (s == 2) return format_year_quarternum_quarterday_impl<quarterly::start::february>(year, quarternum, quarterday);
  else if (s == 3) return format_year_quarternum_quarterday_impl<quarterly::start::march>(year, quarternum, quarterday);
  else if (s == 4) return format_year_quarternum_quarterday_impl<quarterly::start::april>(year, quarternum, quarterday);
  else if (s == 5) return format_year_quarternum_quarterday_impl<quarterly::start::may>(year, quarternum, quarterday);
  else if (s == 6) return format_year_quarternum_quarterday_impl<quarterly::start::june>(year, quarternum, quarterday);
  else if (s == 7) return format_year_quarternum_quarterday_impl<quarterly::start::july>(year, quarternum, quarterday);
  else if (s == 8) return format_year_quarternum_quarterday_impl<quarterly::start::august>(year, quarternum, quarterday);
  else if (s == 9) return format_year_quarternum_quarterday_impl<quarterly::start::september>(year, quarternum, quarterday);
  else if (s == 10) return format_year_quarternum_quarterday_impl<quarterly::start::october>(year, quarternum, quarterday);
  else if (s == 11) return format_year_quarternum_quarterday_impl<quarterly::start::november>(year, quarternum, quarterday);
  else if (s == 12) return format_year_quarternum_quarterday_impl<quarterly::start::december>(year, quarternum, quarterday);

  never_reached("format_year_quarternum_quarterday");
}

template <quarterly::start S>
static
inline
cpp11::writable::logicals
invalid_detect_year_quarternum_quarterday_impl(const cpp11::integers& year,
                                               const cpp11::integers& quarternum,
                                               const cpp11::integers& quarterday) {
  const r_ssize size = year.size();
  cpp11::writable::logicals out(size);
  const rclock::year_quarternum_quarterday<S> x(year, quarternum, quarterday);

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
cpp11::writable::logicals
invalid_detect_year_quarternum_quarterday(const cpp11::integers& year,
                                          const cpp11::integers& quarternum,
                                          const cpp11::integers& quarterday,
                                          const cpp11::integers& start) {
  const int s = start[0];

  if (s == 1) return invalid_detect_year_quarternum_quarterday_impl<quarterly::start::january>(year, quarternum, quarterday);
  else if (s == 2) return invalid_detect_year_quarternum_quarterday_impl<quarterly::start::february>(year, quarternum, quarterday);
  else if (s == 3) return invalid_detect_year_quarternum_quarterday_impl<quarterly::start::march>(year, quarternum, quarterday);
  else if (s == 4) return invalid_detect_year_quarternum_quarterday_impl<quarterly::start::april>(year, quarternum, quarterday);
  else if (s == 5) return invalid_detect_year_quarternum_quarterday_impl<quarterly::start::may>(year, quarternum, quarterday);
  else if (s == 6) return invalid_detect_year_quarternum_quarterday_impl<quarterly::start::june>(year, quarternum, quarterday);
  else if (s == 7) return invalid_detect_year_quarternum_quarterday_impl<quarterly::start::july>(year, quarternum, quarterday);
  else if (s == 8) return invalid_detect_year_quarternum_quarterday_impl<quarterly::start::august>(year, quarternum, quarterday);
  else if (s == 9) return invalid_detect_year_quarternum_quarterday_impl<quarterly::start::september>(year, quarternum, quarterday);
  else if (s == 10) return invalid_detect_year_quarternum_quarterday_impl<quarterly::start::october>(year, quarternum, quarterday);
  else if (s == 11) return invalid_detect_year_quarternum_quarterday_impl<quarterly::start::november>(year, quarternum, quarterday);
  else if (s == 12) return invalid_detect_year_quarternum_quarterday_impl<quarterly::start::december>(year, quarternum, quarterday);

  never_reached("invalid_detect_year_quarternum_quarterday");
}

template <quarterly::start S>
static
inline
int
invalid_count_year_quarternum_quarterday_impl(const cpp11::integers& year,
                                              const cpp11::integers& quarternum,
                                              const cpp11::integers& quarterday) {
  int count = 0;
  const r_ssize size = year.size();
  const rclock::year_quarternum_quarterday<S> x(year, quarternum, quarterday);

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
int
invalid_count_year_quarternum_quarterday(const cpp11::integers& year,
                                         const cpp11::integers& quarternum,
                                         const cpp11::integers& quarterday,
                                         const cpp11::integers& start) {
  const int s = start[0];

  if (s == 1) return invalid_count_year_quarternum_quarterday_impl<quarterly::start::january>(year, quarternum, quarterday);
  else if (s == 2) return invalid_count_year_quarternum_quarterday_impl<quarterly::start::february>(year, quarternum, quarterday);
  else if (s == 3) return invalid_count_year_quarternum_quarterday_impl<quarterly::start::march>(year, quarternum, quarterday);
  else if (s == 4) return invalid_count_year_quarternum_quarterday_impl<quarterly::start::april>(year, quarternum, quarterday);
  else if (s == 5) return invalid_count_year_quarternum_quarterday_impl<quarterly::start::may>(year, quarternum, quarterday);
  else if (s == 6) return invalid_count_year_quarternum_quarterday_impl<quarterly::start::june>(year, quarternum, quarterday);
  else if (s == 7) return invalid_count_year_quarternum_quarterday_impl<quarterly::start::july>(year, quarternum, quarterday);
  else if (s == 8) return invalid_count_year_quarternum_quarterday_impl<quarterly::start::august>(year, quarternum, quarterday);
  else if (s == 9) return invalid_count_year_quarternum_quarterday_impl<quarterly::start::september>(year, quarternum, quarterday);
  else if (s == 10) return invalid_count_year_quarternum_quarterday_impl<quarterly::start::october>(year, quarternum, quarterday);
  else if (s == 11) return invalid_count_year_quarternum_quarterday_impl<quarterly::start::november>(year, quarternum, quarterday);
  else if (s == 12) return invalid_count_year_quarternum_quarterday_impl<quarterly::start::december>(year, quarternum, quarterday);

  never_reached("invalid_count_year_quarternum_quarterday");
}

template <quarterly::start S>
static
inline
bool
invalid_any_year_quarternum_quarterday_impl(const cpp11::integers& year,
                                            const cpp11::integers& quarternum,
                                            const cpp11::integers& quarterday) {
  const r_ssize size = year.size();
  const rclock::year_quarternum_quarterday<S> x(year, quarternum, quarterday);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i) || x[i].ok()) {
      continue;
    }
    return true;
  }

  return false;
}

[[cpp11::register]]
bool
invalid_any_year_quarternum_quarterday(const cpp11::integers& year,
                                       const cpp11::integers& quarternum,
                                       const cpp11::integers& quarterday,
                                       const cpp11::integers& start) {
  const int s = start[0];

  if (s == 1) return invalid_any_year_quarternum_quarterday_impl<quarterly::start::january>(year, quarternum, quarterday);
  else if (s == 2) return invalid_any_year_quarternum_quarterday_impl<quarterly::start::february>(year, quarternum, quarterday);
  else if (s == 3) return invalid_any_year_quarternum_quarterday_impl<quarterly::start::march>(year, quarternum, quarterday);
  else if (s == 4) return invalid_any_year_quarternum_quarterday_impl<quarterly::start::april>(year, quarternum, quarterday);
  else if (s == 5) return invalid_any_year_quarternum_quarterday_impl<quarterly::start::may>(year, quarternum, quarterday);
  else if (s == 6) return invalid_any_year_quarternum_quarterday_impl<quarterly::start::june>(year, quarternum, quarterday);
  else if (s == 7) return invalid_any_year_quarternum_quarterday_impl<quarterly::start::july>(year, quarternum, quarterday);
  else if (s == 8) return invalid_any_year_quarternum_quarterday_impl<quarterly::start::august>(year, quarternum, quarterday);
  else if (s == 9) return invalid_any_year_quarternum_quarterday_impl<quarterly::start::september>(year, quarternum, quarterday);
  else if (s == 10) return invalid_any_year_quarternum_quarterday_impl<quarterly::start::october>(year, quarternum, quarterday);
  else if (s == 11) return invalid_any_year_quarternum_quarterday_impl<quarterly::start::november>(year, quarternum, quarterday);
  else if (s == 12) return invalid_any_year_quarternum_quarterday_impl<quarterly::start::december>(year, quarternum, quarterday);

  never_reached("invalid_any_year_quarternum_quarterday");
}

template <quarterly::start S>
static
inline
cpp11::writable::list_of<cpp11::writable::integers>
invalid_resolve_year_quarternum_quarterday_impl(const cpp11::integers& year,
                                                const cpp11::integers& quarternum,
                                                const cpp11::integers& quarterday,
                                                const cpp11::strings& invalid) {
  const enum invalid invalid_val = parse_invalid(invalid);
  const r_ssize size = year.size();
  const rclock::year_quarternum_quarterday<S> x(year, quarternum, quarterday);
  rclock::writable::year_quarternum_quarterday<S> out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out.assign_na(i);
      continue;
    }

    quarterly::year_quarternum_quarterday<S> elt = x[i];

    if (elt.ok()) {
      out.assign(elt, i);
      continue;
    }

    switch (invalid_val) {
    case invalid::first_day:
    case invalid::first_time: {
      resolve_day_nonexistent_yqnqd_first_day(elt);
      out.assign(elt, i);
      continue;
    }
    case invalid::last_day:
    case invalid::last_time: {
      resolve_day_nonexistent_yqnqd_last_day(elt);
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

[[cpp11::register]]
cpp11::writable::list_of<cpp11::writable::integers>
invalid_resolve_year_quarternum_quarterday(const cpp11::integers& year,
                                           const cpp11::integers& quarternum,
                                           const cpp11::integers& quarterday,
                                           const cpp11::integers& start,
                                           const cpp11::strings& invalid) {
  const int s = start[0];

  if (s == 1) return invalid_resolve_year_quarternum_quarterday_impl<quarterly::start::january>(year, quarternum, quarterday, invalid);
  else if (s == 2) return invalid_resolve_year_quarternum_quarterday_impl<quarterly::start::february>(year, quarternum, quarterday, invalid);
  else if (s == 3) return invalid_resolve_year_quarternum_quarterday_impl<quarterly::start::march>(year, quarternum, quarterday, invalid);
  else if (s == 4) return invalid_resolve_year_quarternum_quarterday_impl<quarterly::start::april>(year, quarternum, quarterday, invalid);
  else if (s == 5) return invalid_resolve_year_quarternum_quarterday_impl<quarterly::start::may>(year, quarternum, quarterday, invalid);
  else if (s == 6) return invalid_resolve_year_quarternum_quarterday_impl<quarterly::start::june>(year, quarternum, quarterday, invalid);
  else if (s == 7) return invalid_resolve_year_quarternum_quarterday_impl<quarterly::start::july>(year, quarternum, quarterday, invalid);
  else if (s == 8) return invalid_resolve_year_quarternum_quarterday_impl<quarterly::start::august>(year, quarternum, quarterday, invalid);
  else if (s == 9) return invalid_resolve_year_quarternum_quarterday_impl<quarterly::start::september>(year, quarternum, quarterday, invalid);
  else if (s == 10) return invalid_resolve_year_quarternum_quarterday_impl<quarterly::start::october>(year, quarternum, quarterday, invalid);
  else if (s == 11) return invalid_resolve_year_quarternum_quarterday_impl<quarterly::start::november>(year, quarternum, quarterday, invalid);
  else if (s == 12) return invalid_resolve_year_quarternum_quarterday_impl<quarterly::start::december>(year, quarternum, quarterday, invalid);

  never_reached("invalid_resolve_year_quarternum_quarterday");
}

template <quarterly::start S>
static
inline
cpp11::writable::list_of<cpp11::writable::integers>
collect_year_quarternum_quarterday_fields_impl(const cpp11::integers& year,
                                               const cpp11::integers& quarternum,
                                               const cpp11::integers& quarterday,
                                               const bool& last) {
  r_ssize size = year.size();
  rclock::writable::year_quarternum_quarterday<S> out(size);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_year = year[i];
    int elt_quarternum = quarternum[i];
    int elt_quarterday = quarterday[i];

    if (elt_year == r_int_na ||
        elt_quarternum == r_int_na ||
        elt_quarterday == r_int_na) {
      out.assign_na(i);
      continue;
    }

    check_range_year(elt_year, "year");
    check_range_quarternum(elt_quarternum, "quarternum");
    unsigned elt_date_quarternum = static_cast<unsigned>(elt_quarternum);

    if (last) {
      out.assign(quarterly::year<S>{elt_year} / elt_date_quarternum / quarterly::last, i);
    } else {
      check_range_quarterday(elt_quarterday, "quarterday");
      unsigned elt_date_quarterday = static_cast<unsigned>(elt_quarterday);
      out.assign(quarterly::year<S>{elt_year} / elt_date_quarternum / elt_date_quarterday, i);
    }
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list_of<cpp11::writable::integers>
collect_year_quarternum_quarterday_fields(const cpp11::integers& year,
                                           const cpp11::integers& quarternum,
                                           const cpp11::integers& quarterday,
                                           const cpp11::integers& start,
                                           const bool& last) {
  const int s = start[0];

  if (s == 1) return collect_year_quarternum_quarterday_fields_impl<quarterly::start::january>(year, quarternum, quarterday, last);
  else if (s == 2) return collect_year_quarternum_quarterday_fields_impl<quarterly::start::february>(year, quarternum, quarterday, last);
  else if (s == 3) return collect_year_quarternum_quarterday_fields_impl<quarterly::start::march>(year, quarternum, quarterday, last);
  else if (s == 4) return collect_year_quarternum_quarterday_fields_impl<quarterly::start::april>(year, quarternum, quarterday, last);
  else if (s == 5) return collect_year_quarternum_quarterday_fields_impl<quarterly::start::may>(year, quarternum, quarterday, last);
  else if (s == 6) return collect_year_quarternum_quarterday_fields_impl<quarterly::start::june>(year, quarternum, quarterday, last);
  else if (s == 7) return collect_year_quarternum_quarterday_fields_impl<quarterly::start::july>(year, quarternum, quarterday, last);
  else if (s == 8) return collect_year_quarternum_quarterday_fields_impl<quarterly::start::august>(year, quarternum, quarterday, last);
  else if (s == 9) return collect_year_quarternum_quarterday_fields_impl<quarterly::start::september>(year, quarternum, quarterday, last);
  else if (s == 10) return collect_year_quarternum_quarterday_fields_impl<quarterly::start::october>(year, quarternum, quarterday, last);
  else if (s == 11) return collect_year_quarternum_quarterday_fields_impl<quarterly::start::november>(year, quarternum, quarterday, last);
  else if (s == 12) return collect_year_quarternum_quarterday_fields_impl<quarterly::start::december>(year, quarternum, quarterday, last);

  never_reached("collect_year_quarternum_quarterday_fields");
}

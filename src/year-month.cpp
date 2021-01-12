#include "clock.h"

[[cpp11::register]]
cpp11::writable::strings
format_year_month(const cpp11::integers& year,
                  const cpp11::integers& month) {
  const r_ssize size = year.size();
  cpp11::writable::strings out(size);

  std::ostringstream stream;

  for (r_ssize i = 0; i < size; ++i) {
    int elt_year = year[i];
    int elt_month = month[i];

    if (elt_year == r_int_na) {
      SET_STRING_ELT(out, i, r_chr_na);
      continue;
    }

    stream.str(std::string());
    stream.clear();

    const date::year elt_date_year{elt_year};
    stream << elt_date_year << '-';

    stream.fill('0');
    stream.flags(std::ios::dec | std::ios::right);
    stream.width(2);
    stream << elt_month;

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

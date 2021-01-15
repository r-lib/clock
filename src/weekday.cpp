#include "duration.h"
#include "get.h"

[[cpp11::register]]
cpp11::writable::integers
weekday_add_days_cpp(const cpp11::integers& x,
                     cpp11::list_of<cpp11::integers> n) {
  const r_ssize size = x.size();

  const cpp11::integers ticks = rclock::duration::get_ticks(n);
  const rclock::duration::days d{ticks};

  cpp11::writable::integers out(size);

  for (r_ssize i = 0; i < size; ++i) {
    const int elt = x[i];

    if (elt == r_int_na || d.is_na(i)) {
      out[i] = r_int_na;
      continue;
    }

    const unsigned weekday = static_cast<unsigned>(elt);
    const date::weekday out_elt = date::weekday{weekday} + d[i];
    out[i] = out_elt.c_encoding();
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::list
weekday_minus_weekday_cpp(const cpp11::integers& x, const cpp11::integers& y) {
  const r_ssize size = x.size();
  rclock::duration::days out(size);

  for (r_ssize i = 0; i < size; ++i) {
    const int x_elt = x[i];
    const int y_elt = y[i];

    if (x_elt == r_int_na || y_elt == r_int_na) {
      out.assign_na(i);
      continue;
    }

    const unsigned x_weekday = static_cast<unsigned>(x_elt);
    const unsigned y_weekday = static_cast<unsigned>(y_elt);

    date::days out_elt = date::weekday{x_weekday} - date::weekday{y_weekday};

    out.assign(out_elt, i);
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::integers
weekday_from_time_point_cpp(cpp11::list_of<cpp11::integers> x) {
  const cpp11::integers ticks = rclock::duration::get_ticks(x);
  const rclock::duration::days d{ticks};
  const r_ssize size = d.size();
  cpp11::writable::integers out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (d.is_na(i)) {
      out[i] = r_int_na;
      continue;
    }

    date::sys_days sd{d[i]};
    date::weekday weekday{sd};
    out[i] = weekday.c_encoding();
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::strings
format_weekday_cpp(const cpp11::integers& x, const cpp11::strings& day_ab) {
  const r_ssize size = x.size();

  const SEXP sun = day_ab[0];
  const SEXP mon = day_ab[1];
  const SEXP tue = day_ab[2];
  const SEXP wed = day_ab[3];
  const SEXP thu = day_ab[4];
  const SEXP fri = day_ab[5];
  const SEXP sat = day_ab[6];

  const std::vector<SEXP> abbreviations = {
    sun, mon, tue, wed, thu, fri, sat
  };

  cpp11::writable::strings out(size);

  for (r_ssize i = 0; i < size; ++i) {
    // Stored in C encoding [0, 6] => [Sun, Sat]
    const int elt = x[i];

    if (elt == r_int_na) {
      SET_STRING_ELT(out, i, r_chr_na);
      continue;
    }

    SET_STRING_ELT(out, i, abbreviations[elt]);
  }

  return out;
}

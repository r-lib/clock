#include "duration.h"
#include "get.h"

static
inline
unsigned
reencode_western_to_c(const unsigned& x) {
  return x - 1;
}

static
inline
unsigned
reencode_c_to_western(const unsigned& x) {
  return x + 1;
}

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

    const unsigned weekday = reencode_western_to_c(static_cast<unsigned>(elt));
    const date::weekday out_elt = date::weekday{weekday} + d[i];
    out[i] = static_cast<int>(reencode_c_to_western(out_elt.c_encoding()));
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

    const unsigned x_weekday = reencode_western_to_c(static_cast<unsigned>(x_elt));
    const unsigned y_weekday = reencode_western_to_c(static_cast<unsigned>(y_elt));

    const date::days out_elt = date::weekday{x_weekday} - date::weekday{y_weekday};

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

    const date::sys_days sd{d[i]};
    const date::weekday weekday{sd};
    out[i] = static_cast<int>(reencode_c_to_western(weekday.c_encoding()));
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::strings
format_weekday_cpp(const cpp11::integers& x, const cpp11::strings& labels) {
  const r_ssize size = x.size();

  const SEXP sun = labels[0];
  const SEXP mon = labels[1];
  const SEXP tue = labels[2];
  const SEXP wed = labels[3];
  const SEXP thu = labels[4];
  const SEXP fri = labels[5];
  const SEXP sat = labels[6];

  const std::vector<SEXP> abbreviations = {
    sun, mon, tue, wed, thu, fri, sat
  };

  cpp11::writable::strings out(size);

  for (r_ssize i = 0; i < size; ++i) {
    // Stored in Western encoding [1, 7] => [Sun, Sat]
    const int elt = x[i];

    if (elt == r_int_na) {
      SET_STRING_ELT(out, i, r_chr_na);
      continue;
    }

    SET_STRING_ELT(out, i, abbreviations[elt - 1]);
  }

  return out;
}

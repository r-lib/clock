#include "duration.h"
#include "get.h"
#include "zone.h"
#include "utils.h"

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::list
naive_time_info_impl(const ClockDuration& x, const cpp11::strings& zone) {
  const r_ssize size = x.size();
  using Duration = typename ClockDuration::duration;

  cpp11::writable::strings type(size);

  cpp11::r_string type_unique{"unique"};
  cpp11::r_string type_nonexistent{"nonexistent"};
  cpp11::r_string type_ambiguous{"ambiguous"};

  rclock::duration::seconds first_begin(size);
  rclock::duration::seconds first_end(size);
  rclock::duration::seconds first_offset(size);
  cpp11::writable::logicals first_dst(size);
  cpp11::writable::strings first_abbreviation(size);

  rclock::duration::seconds second_begin(size);
  rclock::duration::seconds second_end(size);
  rclock::duration::seconds second_offset(size);
  cpp11::writable::logicals second_dst(size);
  cpp11::writable::strings second_abbreviation(size);

  const std::chrono::minutes zero{0};

  const bool recycle_zone = zone.size() == 1;
  const date::time_zone* p_time_zone;
  if (recycle_zone) {
    const std::string zone_name = cpp11::r_string(zone[0]);
    p_time_zone = zone_name_load(zone_name);
  }

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      SET_STRING_ELT(type, i, r_chr_na);

      first_begin.assign_na(i);
      first_end.assign_na(i);
      first_offset.assign_na(i);
      first_dst[i] = r_lgl_na;
      SET_STRING_ELT(first_abbreviation, i, r_chr_na);

      second_begin.assign_na(i);
      second_end.assign_na(i);
      second_offset.assign_na(i);
      second_dst[i] = r_lgl_na;
      SET_STRING_ELT(second_abbreviation, i, r_chr_na);

      continue;
    }

    const date::time_zone* p_time_zone_elt;
    if (recycle_zone) {
      p_time_zone_elt = p_time_zone;
    } else {
      const std::string zone_name_elt = cpp11::r_string(zone[i]);
      p_time_zone_elt = zone_name_load(zone_name_elt);
    }

    const date::local_time<Duration> elt{x[i]};
    const date::local_info info = rclock::get_info(elt, p_time_zone_elt);

    const date::sys_info first = info.first;
    const date::sys_info second = info.second;

    switch (info.result) {
    case date::local_info::unique: SET_STRING_ELT(type, i, type_unique); break;
    case date::local_info::nonexistent: SET_STRING_ELT(type, i, type_nonexistent); break;
    case date::local_info::ambiguous: SET_STRING_ELT(type, i, type_ambiguous); break;
    default: never_reached("naive_time_info_impl");
    }

    first_begin.assign(first.begin.time_since_epoch(), i);
    first_end.assign(first.end.time_since_epoch(), i);
    first_offset.assign(first.offset, i);
    first_dst[i] = first.save != zero;
    SET_STRING_ELT(first_abbreviation, i, Rf_mkCharLenCE(first.abbrev.c_str(), first.abbrev.size(), CE_UTF8));

    if (info.result == date::local_info::unique) {
      // date zero initializes `info.second` since there is no applicable information
      second_begin.assign_na(i);
      second_end.assign_na(i);
      second_offset.assign_na(i);
      second_dst[i] = r_lgl_na;
      SET_STRING_ELT(second_abbreviation, i, r_chr_na);
    } else {
      second_begin.assign(second.begin.time_since_epoch(), i);
      second_end.assign(second.end.time_since_epoch(), i);
      second_offset.assign(second.offset, i);
      second_dst[i] = second.save != zero;
      SET_STRING_ELT(second_abbreviation, i, Rf_mkCharLenCE(second.abbrev.c_str(), second.abbrev.size(), CE_UTF8));
    }
  }

  cpp11::writable::list out_first_begin = first_begin.to_list();
  cpp11::writable::list out_first_end = first_end.to_list();
  cpp11::writable::list out_first_offset = first_offset.to_list();

  cpp11::writable::list out_second_begin = second_begin.to_list();
  cpp11::writable::list out_second_end = second_end.to_list();
  cpp11::writable::list out_second_offset = second_offset.to_list();

  cpp11::writable::list out_first = {
    out_first_begin,
    out_first_end,
    out_first_offset,
    first_dst,
    first_abbreviation
  };
  out_first.names() = {"begin", "end", "offset", "dst", "abbreviation"};

  cpp11::writable::list out_second = {
    out_second_begin,
    out_second_end,
    out_second_offset,
    second_dst,
    second_abbreviation
  };
  out_second.names() = {"begin", "end", "offset", "dst", "abbreviation"};

  cpp11::writable::list out = {
    type,
    out_first,
    out_second
  };
  out.names() = {"type", "first", "second"};

  return out;
}

[[cpp11::register]]
cpp11::writable::list
naive_time_info_cpp(cpp11::list_of<cpp11::integers> fields,
                    const cpp11::integers& precision_int,
                    const cpp11::strings& zone) {
  using namespace rclock;

  const cpp11::integers ticks = duration::get_ticks(fields);
  const cpp11::integers ticks_of_day = duration::get_ticks_of_day(fields);
  const cpp11::integers ticks_of_second = duration::get_ticks_of_second(fields);

  const duration::days dd{ticks};
  const duration::seconds ds{ticks, ticks_of_day};
  const duration::milliseconds dmilli{ticks, ticks_of_day, ticks_of_second};
  const duration::microseconds dmicro{ticks, ticks_of_day, ticks_of_second};
  const duration::nanoseconds dnano{ticks, ticks_of_day, ticks_of_second};

  switch (parse_precision(precision_int)) {
  case precision::day: return naive_time_info_impl(dd, zone);
  case precision::second: return naive_time_info_impl(ds, zone);
  case precision::millisecond: return naive_time_info_impl(dmilli, zone);
  case precision::microsecond: return naive_time_info_impl(dmicro, zone);
  case precision::nanosecond: return naive_time_info_impl(dnano, zone);
  default: clock_abort("Internal error: Should never be called.");
  }
}

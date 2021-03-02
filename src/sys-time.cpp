#include "duration.h"
#include "get.h"
#include "zone.h"

[[cpp11::register]]
cpp11::writable::list
sys_time_now_cpp() {
  using namespace std::chrono;

  typename system_clock::time_point tp = system_clock::now();

  // The precision of `system_clock::now()` is platform dependent.
  // We just cast them all to nanoseconds to at least return a consistent precision.
  // - MacOS = 1us
  // - Linux = 1ns
  // - Windows = 100ns
  // https://stackoverflow.com/questions/55120594/different-behaviour-of-system-clock-on-windows-and-linux
  nanoseconds d = duration_cast<nanoseconds>(tp.time_since_epoch());

  rclock::duration::nanoseconds out(1);
  out.assign(d, 0);

  return out.to_list();
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::list
sys_time_info_impl(const ClockDuration& x, const cpp11::strings& zone) {
  const r_ssize size = x.size();
  using Duration = typename ClockDuration::duration;

  rclock::duration::seconds begin(size);
  rclock::duration::seconds end(size);
  rclock::duration::seconds offset(size);
  cpp11::writable::logicals dst(size);
  cpp11::writable::strings abbreviation(size);

  const std::chrono::minutes zero{0};

  const bool recycle_zone = zone.size() == 1;
  const date::time_zone* p_time_zone;
  if (recycle_zone) {
    const std::string zone_name = cpp11::r_string(zone[0]);
    p_time_zone = zone_name_load(zone_name);
  }

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      begin.assign_na(i);
      end.assign_na(i);
      offset.assign_na(i);
      dst[i] = r_lgl_na;
      SET_STRING_ELT(abbreviation, i, r_chr_na);
      continue;
    }

    const date::time_zone* p_time_zone_elt;
    if (recycle_zone) {
      p_time_zone_elt = p_time_zone;
    } else {
      const std::string zone_name_elt = cpp11::r_string(zone[i]);
      p_time_zone_elt = zone_name_load(zone_name_elt);
    }

    const date::sys_time<Duration> elt{x[i]};
    const date::sys_info info = p_time_zone_elt->get_info(elt);

    begin.assign(info.begin.time_since_epoch(), i);
    end.assign(info.end.time_since_epoch(), i);
    offset.assign(info.offset, i);
    dst[i] = info.save != zero;
    const SEXP abbreviation_elt = Rf_mkCharLenCE(info.abbrev.c_str(), info.abbrev.size(), CE_UTF8);
    SET_STRING_ELT(abbreviation, i, abbreviation_elt);
  }

  cpp11::writable::list out_begin = begin.to_list();
  cpp11::writable::list out_end = end.to_list();
  cpp11::writable::list out_offset = offset.to_list();

  cpp11::writable::list out = { out_begin, out_end, out_offset, dst, abbreviation};
  out.names() = {"begin", "end", "offset", "dst", "abbreviation"};

  return out;
}

[[cpp11::register]]
cpp11::writable::list
sys_time_info_cpp(cpp11::list_of<cpp11::integers> fields,
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
  case precision::day: return sys_time_info_impl(dd, zone);
  case precision::second: return sys_time_info_impl(ds, zone);
  case precision::millisecond: return sys_time_info_impl(dmilli, zone);
  case precision::microsecond: return sys_time_info_impl(dmicro, zone);
  case precision::nanosecond: return sys_time_info_impl(dnano, zone);
  default: clock_abort("Internal error: Should never be called.");
  }
}

#include "duration.h"
#include "enums.h"
#include "utils.h"
#include "get.h"
#include "zone.h"

template <class ClockDuration>
static
inline
cpp11::writable::list
get_naive_time_impl(const ClockDuration& x,
                    const date::time_zone* p_time_zone) {
  using Duration = typename ClockDuration::duration;

  const r_ssize size = x.size();

  ClockDuration out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out.assign_na(i);
      continue;
    }

    Duration elt = x[i];
    date::sys_time<Duration> elt_st{elt};
    date::zoned_time<Duration> elt_zt = date::make_zoned(p_time_zone, elt_st);
    date::local_time<Duration> elt_lt = elt_zt.get_local_time();
    Duration elt_out = elt_lt.time_since_epoch();
    out.assign(elt_out, i);
  }

  return out.to_list();
}


[[cpp11::register]]
cpp11::writable::list
get_naive_time_cpp(cpp11::list_of<cpp11::integers> fields,
                   const cpp11::strings& precision,
                   const cpp11::strings& zone) {
  using namespace rclock;

  const cpp11::writable::strings zone_standard = zone_standardize(zone);
  const std::string zone_name = cpp11::r_string(zone_standard[0]);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  cpp11::integers ticks = get_ticks(fields);
  cpp11::integers ticks_of_day = get_ticks_of_day(fields);
  cpp11::integers ticks_of_second = get_ticks_of_second(fields);

  duration::seconds ds{ticks, ticks_of_day};
  duration::milliseconds dmilli{ticks, ticks_of_day, ticks_of_second};
  duration::microseconds dmicro{ticks, ticks_of_day, ticks_of_second};
  duration::nanoseconds dnano{ticks, ticks_of_day, ticks_of_second};

  switch (parse_precision2(precision)) {
  case precision2::second: return get_naive_time_impl(ds, p_time_zone);
  case precision2::millisecond: return get_naive_time_impl(dmilli, p_time_zone);
  case precision2::microsecond: return get_naive_time_impl(dmicro, p_time_zone);
  case precision2::nanosecond: return get_naive_time_impl(dnano, p_time_zone);
  default: clock_abort("Internal error: Should never be called.");
  }
}

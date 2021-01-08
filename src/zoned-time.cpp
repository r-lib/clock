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

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::list
as_zoned_sys_time_from_naive_time_impl(const ClockDuration& x,
                                       const date::time_zone* p_time_zone,
                                       const cpp11::strings& nonexistent,
                                       const cpp11::strings& ambiguous) {
  using Duration = typename ClockDuration::duration;

  const r_ssize size = x.size();
  ClockDuration out(size);

  const bool recycle_nonexistent = clock_is_scalar(nonexistent);
  const bool recycle_ambiguous = clock_is_scalar(ambiguous);

  enum nonexistent nonexistent_val;
  enum ambiguous ambiguous_val;

  if (recycle_nonexistent) {
    nonexistent_val = parse_nonexistent_one(nonexistent[0]);
  }
  if (recycle_ambiguous) {
    ambiguous_val = parse_ambiguous_one(ambiguous[0]);
  }

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out.assign_na(i);
      continue;
    }

    const enum nonexistent elt_nonexistent_val =
      recycle_nonexistent ?
      nonexistent_val :
      parse_nonexistent_one(nonexistent[i]);

    const enum ambiguous elt_ambiguous_val =
      recycle_ambiguous ?
      ambiguous_val :
      parse_ambiguous_one(ambiguous[i]);

    const Duration elt = x[i];
    const date::local_time<Duration> elt_lt{elt};

    out.convert_local_to_sys_and_assign(
      elt_lt,
      p_time_zone,
      elt_nonexistent_val,
      elt_ambiguous_val,
      i
    );
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
as_zoned_sys_time_from_naive_time_cpp(cpp11::list_of<cpp11::integers> fields,
                                      const cpp11::strings& precision,
                                      const cpp11::strings& zone,
                                      const cpp11::strings& nonexistent,
                                      const cpp11::strings& ambiguous) {
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
  case precision2::second: return as_zoned_sys_time_from_naive_time_impl(ds, p_time_zone, nonexistent, ambiguous);
  case precision2::millisecond: return as_zoned_sys_time_from_naive_time_impl(dmilli, p_time_zone, nonexistent, ambiguous);
  case precision2::microsecond: return as_zoned_sys_time_from_naive_time_impl(dmicro, p_time_zone, nonexistent, ambiguous);
  case precision2::nanosecond: return as_zoned_sys_time_from_naive_time_impl(dnano, p_time_zone, nonexistent, ambiguous);
  default: clock_abort("Internal error: Should never be called.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
to_sys_duration_fields_from_sys_seconds_cpp(const cpp11::doubles& seconds) {
  r_ssize size = seconds.size();
  rclock::duration::seconds out(size);

  for (r_ssize i = 0; i < size; ++i) {
    // Assume seconds precision!
    double elt_seconds = seconds[i];
    int64_t elt = as_int64(elt_seconds);

    if (elt == r_int64_na) {
      out.assign_na(i);
      continue;
    }

    std::chrono::seconds elt_sec{elt};

    out.assign(elt_sec, i);
  }

  return out.to_list();
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::doubles
to_sys_seconds_from_sys_duration_fields_cpp(cpp11::list_of<cpp11::integers> fields) {
  const cpp11::integers ticks = get_ticks(fields);
  const cpp11::integers ticks_of_day = get_ticks_of_day(fields);
  const rclock::duration::seconds x{ticks, ticks_of_day};

  r_ssize size = ticks.size();
  cpp11::writable::doubles out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = r_dbl_na;
      continue;
    }
    std::chrono::seconds elt = x[i];
    out[i] = static_cast<double>(elt.count());
  }

  return out;
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::integers
get_offset_impl(const ClockDuration& x,
                const date::time_zone* p_time_zone) {
  using Duration = typename ClockDuration::duration;

  const r_ssize size = x.size();
  cpp11::writable::integers out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = r_int_na;
      continue;
    }
    const date::sys_time<Duration> elt{x[i]};
    const date::sys_info info = p_time_zone->get_info(elt);
    out[i] = info.offset.count();
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::integers
get_offset_cpp(cpp11::list_of<cpp11::integers> fields,
               const cpp11::strings& precision,
               const cpp11::strings& zone) {
  using namespace rclock;

  const cpp11::writable::strings zone_standard = zone_standardize(zone);
  const std::string zone_name = cpp11::r_string(zone_standard[0]);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  const cpp11::integers ticks = get_ticks(fields);
  const cpp11::integers ticks_of_day = get_ticks_of_day(fields);
  const cpp11::integers ticks_of_second = get_ticks_of_second(fields);

  const duration::seconds ds{ticks, ticks_of_day};
  const duration::milliseconds dmilli{ticks, ticks_of_day, ticks_of_second};
  const duration::microseconds dmicro{ticks, ticks_of_day, ticks_of_second};
  const duration::nanoseconds dnano{ticks, ticks_of_day, ticks_of_second};

  switch (parse_precision2(precision)) {
  case precision2::second: return get_offset_impl(ds, p_time_zone);
  case precision2::millisecond: return get_offset_impl(dmilli, p_time_zone);
  case precision2::microsecond: return get_offset_impl(dmicro, p_time_zone);
  case precision2::nanosecond: return get_offset_impl(dnano, p_time_zone);
  default: clock_abort("Internal error: Should never be called.");
  }
}

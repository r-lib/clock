#include "conversion.h"
#include "utils.h"

// -----------------------------------------------------------------------------

static inline double info_unique(const date::local_info& info,
                                 const date::local_seconds& lsec) {
  std::chrono::seconds offset = info.first.offset;
  date::sys_seconds ssec = date::sys_seconds{lsec.time_since_epoch()} - offset;
  return ssec.time_since_epoch().count();
}

// -----------------------------------------------------------------------------

/*
 * I'm using `info.second.begin` here because that seems more intuitive, but
 * I think date uses `info.first.end`. As far as I can tell, these are identical.
 */
static inline double info_nonexistent_roll_forward(const date::local_info& info) {
  return info.second.begin.time_since_epoch().count();
}

static inline double info_nonexistent_roll_backward(const date::local_info& info) {
  return info_nonexistent_roll_forward(info) - 1;
}

static inline double info_nonexistent_shift_forward(const date::local_info& info,
                                                    const date::local_seconds& lsec) {
  std::chrono::seconds offset = info.second.offset;
  std::chrono::seconds gap = info.second.offset - info.first.offset;
  date::local_seconds lsec_shift = lsec + gap;
  date::sys_seconds out = date::sys_seconds{lsec_shift.time_since_epoch()} - offset;
  return out.time_since_epoch().count();
}

static inline double info_nonexistent_shift_backward(const date::local_info& info,
                                                     const date::local_seconds& lsec) {
  std::chrono::seconds offset = info.first.offset;
  std::chrono::seconds gap = info.second.offset - info.first.offset;
  date::local_seconds lsec_shift = lsec - gap;
  date::sys_seconds out = date::sys_seconds{lsec_shift.time_since_epoch()} - offset;
  return out.time_since_epoch().count();
}

static inline double info_nonexistent_na() {
  return NA_REAL;
}

static inline double info_nonexistent_error(r_ssize i) {
  r_abort("Nonexistent time due to daylight savings at location %i.", (int) i + 1);
}

// -----------------------------------------------------------------------------

static inline double info_ambiguous_latest(const date::local_info& info,
                                           const date::local_seconds& lsec) {
  std::chrono::seconds offset = info.second.offset;
  date::sys_seconds out = date::sys_seconds{lsec.time_since_epoch()} - offset;
  return out.time_since_epoch().count();
}

static inline double info_ambiguous_earliest(const date::local_info& info,
                                             const date::local_seconds& lsec) {
  std::chrono::seconds offset = info.first.offset;
  date::sys_seconds out = date::sys_seconds{lsec.time_since_epoch()} - offset;
  return out.time_since_epoch().count();
}

static inline double info_ambiguous_na() {
  return NA_REAL;
}

static inline double info_ambiguous_error(r_ssize i) {
  r_abort("Ambiguous time due to daylight savings at location %i.", (int) i + 1);
}

// -----------------------------------------------------------------------------

// [[ include("conversion.h") ]]
double convert_local_seconds_to_posixt(const date::local_seconds& lsec,
                                       const date::time_zone* p_zone,
                                       r_ssize i,
                                       const enum dst_nonexistent& dst_nonexistent,
                                       const enum dst_ambiguous& dst_ambiguous) {
  date::local_info info = p_zone->get_info(lsec);

  if (info.result == date::local_info::unique) {
    return info_unique(info, lsec);
  }

  if (info.result == date::local_info::nonexistent) {
    switch (dst_nonexistent) {
    case dst_nonexistent::roll_forward: {
      return info_nonexistent_roll_forward(info);
    }
    case dst_nonexistent::roll_backward: {
      return info_nonexistent_roll_backward(info);
    }
    case dst_nonexistent::shift_forward: {
      return info_nonexistent_shift_forward(info, lsec);
    }
    case dst_nonexistent::shift_backward: {
      return info_nonexistent_shift_backward(info, lsec);
    }
    case dst_nonexistent::na: {
      return info_nonexistent_na();
    }
    case dst_nonexistent::error: {
      return info_nonexistent_error(i);
    }
    }
  }

  if (info.result == date::local_info::ambiguous) {
    switch (dst_ambiguous) {
    case dst_ambiguous::latest: {
      return info_ambiguous_latest(info, lsec);
    }
    case dst_ambiguous::earliest: {
      return info_ambiguous_earliest(info, lsec);
    }
    case dst_ambiguous::na: {
      return info_ambiguous_na();
    }
    case dst_ambiguous::error: {
      return info_ambiguous_error(i);
    }
    }
  }

  never_reached("convert_local_seconds_to_posixt");
}

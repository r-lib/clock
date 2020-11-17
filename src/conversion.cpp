#include "conversion.h"
#include "utils.h"

// -----------------------------------------------------------------------------

static inline date::sys_seconds info_unique(const date::local_info& info,
                                            const date::local_seconds& lsec) {
  std::chrono::seconds offset = info.first.offset;
  return date::sys_seconds{lsec.time_since_epoch()} - offset;
}

// -----------------------------------------------------------------------------

/*
 * I'm using `info.second.begin` here because that seems more intuitive, but
 * I think date uses `info.first.end`. As far as I can tell, these are identical.
 */
static inline date::sys_seconds info_nonexistent_roll_forward(const date::local_info& info) {
  return info.second.begin;
}

static inline date::sys_seconds info_nonexistent_roll_backward(const date::local_info& info) {
  return info_nonexistent_roll_forward(info) - std::chrono::seconds{1};
}

static inline date::sys_seconds info_nonexistent_shift_forward(const date::local_info& info,
                                                               const date::local_seconds& lsec) {
  std::chrono::seconds offset = info.second.offset;
  std::chrono::seconds gap = info.second.offset - info.first.offset;
  date::local_seconds lsec_shift = lsec + gap;
  return date::sys_seconds{lsec_shift.time_since_epoch()} - offset;
}

static inline date::sys_seconds info_nonexistent_shift_backward(const date::local_info& info,
                                                                const date::local_seconds& lsec) {
  std::chrono::seconds offset = info.first.offset;
  std::chrono::seconds gap = info.second.offset - info.first.offset;
  date::local_seconds lsec_shift = lsec - gap;
  return date::sys_seconds{lsec_shift.time_since_epoch()} - offset;
}

static inline date::sys_seconds info_nonexistent_na(bool& na) {
  na = true;
  return date::sys_seconds::max();
}

static inline date::sys_seconds info_nonexistent_error(r_ssize i) {
  civil_abort("Nonexistent time due to daylight savings at location %i.", (int) i + 1);
}

// -----------------------------------------------------------------------------

static inline void info_nonexistent_roll_forward_nanos(std::chrono::nanoseconds& nanos) {
  nanos = std::chrono::nanoseconds{0};
}
static inline void info_nonexistent_roll_backward_nanos(std::chrono::nanoseconds& nanos) {
  nanos = std::chrono::nanoseconds{999999999};
}

// -----------------------------------------------------------------------------

static inline date::sys_seconds info_ambiguous_latest(const date::local_info& info,
                                                      const date::local_seconds& lsec) {
  std::chrono::seconds offset = info.second.offset;
  return date::sys_seconds{lsec.time_since_epoch()} - offset;
}

static inline date::sys_seconds info_ambiguous_earliest(const date::local_info& info,
                                                        const date::local_seconds& lsec) {
  std::chrono::seconds offset = info.first.offset;
  return date::sys_seconds{lsec.time_since_epoch()} - offset;
}

static inline date::sys_seconds info_ambiguous_na(bool& na) {
  na = true;
  return date::sys_seconds::max();
}

static inline date::sys_seconds info_ambiguous_error(r_ssize i) {
  civil_abort("Ambiguous time due to daylight savings at location %i.", (int) i + 1);
}

// -----------------------------------------------------------------------------

/*
 * Converts local_seconds to sys_seconds
 */
// [[ include("conversion.h") ]]
date::sys_seconds convert_local_to_sys(const date::local_seconds& lsec,
                                       const date::time_zone* p_zone,
                                       r_ssize i,
                                       const enum dst_nonexistent& dst_nonexistent,
                                       const enum dst_ambiguous& dst_ambiguous,
                                       bool& na) {
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
      return info_nonexistent_na(na);
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
      return info_ambiguous_na(na);
    }
    case dst_ambiguous::error: {
      return info_ambiguous_error(i);
    }
    }
  }

  never_reached("convert_local_to_sys");
}

// -----------------------------------------------------------------------------


/*
 * Converts local_seconds to sys_seconds
 * Also handles tweaking nanoseconds in place as required.
 *
 * `get_info()` floors to seconds, but that is appropriate because DST
 * handling is really only precise to the second level.
 */
// [[ include("conversion.h") ]]
date::sys_seconds convert_local_to_sys(const date::local_seconds& lsec,
                                       const date::time_zone* p_zone,
                                       r_ssize i,
                                       const enum dst_nonexistent& dst_nonexistent,
                                       const enum dst_ambiguous& dst_ambiguous,
                                       bool& na,
                                       std::chrono::nanoseconds& nanos) {
  date::local_info info = p_zone->get_info(lsec);

  if (info.result == date::local_info::unique) {
    return info_unique(info, lsec);
  }

  if (info.result == date::local_info::nonexistent) {
    switch (dst_nonexistent) {
    case dst_nonexistent::roll_forward: {
      info_nonexistent_roll_forward_nanos(nanos);
      return info_nonexistent_roll_forward(info);
    }
    case dst_nonexistent::roll_backward: {
      info_nonexistent_roll_backward_nanos(nanos);
      return info_nonexistent_roll_backward(info);
    }
    case dst_nonexistent::shift_forward: {
      return info_nonexistent_shift_forward(info, lsec);
    }
    case dst_nonexistent::shift_backward: {
      return info_nonexistent_shift_backward(info, lsec);
    }
    case dst_nonexistent::na: {
      return info_nonexistent_na(na);
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
      return info_ambiguous_na(na);
    }
    case dst_ambiguous::error: {
      return info_ambiguous_error(i);
    }
    }
  }

  never_reached("convert_local_to_sys");
}

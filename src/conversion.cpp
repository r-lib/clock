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

static inline double info_nonexistant_next(const date::local_info& info) {
  return info.first.end.time_since_epoch().count();
}

static inline double info_nonexistant_previous(const date::local_info& info) {
  return info_nonexistant_next(info) - 1;
}

static inline double info_nonexistant_directional(const date::local_info& info,
                                                  const std::chrono::seconds& duration) {
  if (duration.count() >= 0) {
    return info_nonexistant_next(info);
  } else {
    return info_nonexistant_previous(info);
  }
}

static inline double info_nonexistant_na() {
  return NA_REAL;
}

static inline double info_nonexistant_error(r_ssize i) {
  r_abort("Nonexistant time due to daylight savings at location %i.", (int) i + 1);
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

static inline double info_ambiguous_directional(const date::local_info& info,
                                                const date::local_seconds& lsec,
                                                const std::chrono::seconds& duration) {
  if (duration.count() >= 0) {
    return info_ambiguous_latest(info, lsec);
  } else {
    return info_ambiguous_earliest(info, lsec);
  }
}

static inline double info_ambiguous_na() {
  return NA_REAL;
}

static inline double info_ambiguous_error(r_ssize i) {
  r_abort("Ambiguous time due to daylight savings at location %i.", (int) i + 1);
}

// -----------------------------------------------------------------------------

// [[ include("conversion.h") ]]
double civil_local_seconds_to_posixt(const date::local_seconds& lsec,
                                     const date::time_zone* p_zone,
                                     const std::chrono::seconds& duration,
                                     r_ssize i,
                                     const enum dst_nonexistant& dst_nonexistant,
                                     const enum dst_ambiguous& dst_ambiguous) {
  date::local_info info = p_zone->get_info(lsec);

  if (info.result == date::local_info::unique) {
    return info_unique(info, lsec);
  }

  if (info.result == date::local_info::nonexistent) {
    switch (dst_nonexistant) {
    case dst_nonexistant::directional: {
      return info_nonexistant_directional(info, duration);
    }
    case dst_nonexistant::next: {
      return info_nonexistant_next(info);
    }
    case dst_nonexistant::previous: {
      return info_nonexistant_previous(info);
    }
    case dst_nonexistant::na: {
      return info_nonexistant_na();
    }
    case dst_nonexistant::error: {
      return info_nonexistant_error(i);
    }
    }
  }

  if (info.result == date::local_info::ambiguous) {
    switch (dst_ambiguous) {
    case dst_ambiguous::directional: {
      return info_ambiguous_directional(info, lsec, duration);
    }
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

  never_reached("civil_local_seconds_to_posixt");
}

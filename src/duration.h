#ifndef CLOCK_DURATION_H
#define CLOCK_DURATION_H

#include "clock.h"
#include "doubles.h"
#include "enums.h"
#include "utils.h"

#include <limits>

namespace rclock {

namespace duration {

template <typename Duration>
class duration
{
public:
  CONSTCD11 duration(cpp11::list_of<cpp11::doubles>& fields) NOEXCEPT;
  duration(r_ssize size);

  CONSTCD11 bool is_na(r_ssize i) const NOEXCEPT;
  CONSTCD11 r_ssize size() const NOEXCEPT;

  void assign_na(r_ssize i);
  void assign(const Duration& x, r_ssize i);

  CONSTCD14 Duration operator[](r_ssize i) const NOEXCEPT;

  cpp11::writable::list to_list() const;

  // Only used by `zoned-time.cpp`
  void convert_local_to_sys_and_assign(const date::local_time<Duration>& x,
                                       const date::local_info& info,
                                       const enum nonexistent& nonexistent_val,
                                       const enum ambiguous& ambiguous_val,
                                       const r_ssize& i,
                                       const cpp11::sexp& call);
  void convert_local_with_reference_to_sys_and_assign(const date::local_time<Duration>& x,
                                                      const date::local_info& info,
                                                      const enum nonexistent& nonexistent_val,
                                                      const enum ambiguous& ambiguous_val,
                                                      const date::sys_seconds& reference,
                                                      const date::time_zone* p_time_zone,
                                                      const r_ssize& i,
                                                      const cpp11::sexp& call);

  using chrono_duration = Duration;

protected:
  rclock::doubles lower_;
  rclock::doubles upper_;
};

using years = duration<date::years>;
using quarters = duration<quarterly::quarters>;
using months = duration<date::months>;
using weeks = duration<date::weeks>;
using days = duration<date::days>;
using hours = duration<std::chrono::hours>;
using minutes = duration<std::chrono::minutes>;
using seconds = duration<std::chrono::seconds>;
using milliseconds = duration<std::chrono::milliseconds>;
using microseconds = duration<std::chrono::microseconds>;
using nanoseconds = duration<std::chrono::nanoseconds>;

// Implementation

namespace detail {

static
inline
cpp11::doubles
get_lower(cpp11::list_of<cpp11::doubles>& fields)
{
  return fields[0];
}

static
inline
cpp11::doubles
get_upper(cpp11::list_of<cpp11::doubles>& fields)
{
  return fields[1];
}

}

template <typename Duration>
CONSTCD11
inline
duration<Duration>::duration(cpp11::list_of<cpp11::doubles>& fields) NOEXCEPT
  : lower_(detail::get_lower(fields))
  , upper_(detail::get_upper(fields))
  {}

template <typename Duration>
inline
duration<Duration>::duration(r_ssize size)
  : lower_(size)
  , upper_(size)
  {}

template <typename Duration>
CONSTCD11
inline
bool
duration<Duration>::is_na(r_ssize i) const NOEXCEPT
{
  return lower_.is_na(i);
}

template <typename Duration>
CONSTCD11
inline
r_ssize
duration<Duration>::size() const NOEXCEPT
{
  return lower_.size();
}

template <typename Duration>
inline
void
duration<Duration>::assign_na(r_ssize i)
{
  lower_.assign_na(i);
  upper_.assign_na(i);
}

namespace detail {

/*
 * This pair of functions facilitates:
 * - Splitting an `int64_t` into two `uint32_t` values, maintaining order
 * - Combining those two `uint32_t` values back into the original `int32_t`
 *
 * The two `uint32_t` values are stored in two doubles. This allows us to store
 * it in a two column data frame that vctrs knows how to work with, and we can
 * use the standard `NA_real_` as the missing value without fear of conflicting
 * with any other valid `int64_t` value.
 *
 * Unsigned 32-bit integers are used because bit shifting is undefined on signed
 * types.
 *
 * An arithmetic shift of `- std::numeric_limits<int64_t>::min()` is done to
 * remap the `int64_t` value into `uint64_t` space, while maintaining order.
 * This relies on unsigned arithmetic overflow behavior, which is well-defined.
 *
 * Taken from vctrs:
 * https://github.com/r-lib/vctrs/blob/c27b6988bd2f02aa970b6d14a640eccb299e03bb/src/type-integer64.c#L117-L156
 */

CONSTCD14
static
inline
std::pair<double, double>
int64_unpack(int64_t x)
{
  const uint64_t x_u64 = static_cast<uint64_t>(x) - std::numeric_limits<int64_t>::min();

  const uint32_t left_u32 = static_cast<uint32_t>(x_u64 >> 32);
  const uint32_t right_u32 = static_cast<uint32_t>(x_u64);

  const double left_out = static_cast<double>(left_u32);
  const double right_out = static_cast<double>(right_u32);

  return std::make_pair(left_out, right_out);
}

CONSTCD14
static
inline
int64_t int64_pack(double left, double right)
{
  const uint32_t left_u32 = static_cast<uint32_t>(left);
  const uint32_t right_u32 = static_cast<uint32_t>(right);

  const uint64_t out_u64 = static_cast<uint64_t>(left_u32) << 32 | right_u32;

  const int64_t out = static_cast<int64_t>(out_u64 + std::numeric_limits<int64_t>::min());

  return out;
}

} // namespace details

template <typename Duration>
inline
void
duration<Duration>::assign(const Duration& x, r_ssize i)
{
  const int64_t elt = static_cast<int64_t>(x.count());
  std::pair<double, double> unpacked = detail::int64_unpack(elt);
  lower_.assign(unpacked.first, i);
  upper_.assign(unpacked.second, i);
}

template <typename Duration>
CONSTCD14
inline
Duration
duration<Duration>::operator[](r_ssize i) const NOEXCEPT
{
  using rep = typename Duration::rep;
  const int64_t packed = detail::int64_pack(lower_[i], upper_[i]);
  const rep elt = static_cast<rep>(packed);
  return Duration{elt};
}

template <typename Duration>
inline
cpp11::writable::list
duration<Duration>::to_list() const
{
  cpp11::writable::list out({lower_.sexp(), upper_.sexp()});
  out.names() = {"lower", "upper"};
  return out;
}

namespace detail {

template <typename Duration>
inline
date::sys_time<Duration>
info_unique(const date::local_info& info, const date::local_time<Duration>& lt)
{
  std::chrono::seconds offset = info.first.offset;
  return date::sys_time<Duration>{lt.time_since_epoch()} - offset;
}

template <typename Duration>
inline
date::sys_time<Duration>
info_nonexistent_roll_forward(const date::local_info& info)
{
  return info.second.begin;
}

template <typename Duration>
inline
date::sys_time<Duration>
info_nonexistent_roll_backward(const date::local_info& info)
{
  return info_nonexistent_roll_forward<Duration>(info) - Duration{1};
}

template <typename Duration>
inline
date::sys_time<Duration>
info_nonexistent_shift_forward(const date::local_info& info, const date::local_time<Duration>& lt)
{
  std::chrono::seconds offset = info.second.offset;
  std::chrono::seconds gap = info.second.offset - info.first.offset;
  date::local_time<Duration> lt_shift = lt + gap;
  return date::sys_time<Duration>{lt_shift.time_since_epoch()} - offset;
}

template <typename Duration>
inline
date::sys_time<Duration>
info_nonexistent_shift_backward(const date::local_info& info, const date::local_time<Duration>& lt)
{
  std::chrono::seconds offset = info.first.offset;
  std::chrono::seconds gap = info.second.offset - info.first.offset;
  date::local_time<Duration> lt_shift = lt - gap;
  return date::sys_time<Duration>{lt_shift.time_since_epoch()} - offset;
}

inline
void
info_nonexistent_error(const r_ssize& i, const cpp11::sexp& call)
{
  cpp11::writable::integers arg(1);
  arg[0] = (int) i + 1;
  auto stop = cpp11::package("clock")["stop_clock_nonexistent_time"];
  stop(arg, call);
}

template <typename Duration>
inline
date::sys_time<Duration>
info_ambiguous_earliest(const date::local_info& info, const date::local_time<Duration>& lt)
{
  std::chrono::seconds offset = info.first.offset;
  return date::sys_time<Duration>{lt.time_since_epoch()} - offset;
}

template <typename Duration>
inline
date::sys_time<Duration>
info_ambiguous_latest(const date::local_info& info, const date::local_time<Duration>& lt)
{
  std::chrono::seconds offset = info.second.offset;
  return date::sys_time<Duration>{lt.time_since_epoch()} - offset;
}

inline
void
info_ambiguous_error(const r_ssize& i, const cpp11::sexp& call)
{
  cpp11::writable::integers arg(1);
  arg[0] = (int) i + 1;
  auto stop = cpp11::package("clock")["stop_clock_ambiguous_time"];
  stop(arg, call);
}

} // namespace detail

/*
 * Zoned times have at least seconds precision, so we expect that every
 * instantiation of this will have a `Duration` of at least second precision
 */
template <typename Duration>
inline
void
duration<Duration>::convert_local_to_sys_and_assign(const date::local_time<Duration>& x,
                                                    const date::local_info& info,
                                                    const enum nonexistent& nonexistent_val,
                                                    const enum ambiguous& ambiguous_val,
                                                    const r_ssize& i,
                                                    const cpp11::sexp& call)
{
  switch (info.result) {
  case date::local_info::unique: {
    date::sys_time<Duration> st = detail::info_unique(info, x);
    assign(st.time_since_epoch(), i);
    break;
  }
  case date::local_info::nonexistent: {
    switch (nonexistent_val) {
    case nonexistent::roll_forward: {
      date::sys_time<Duration> st = detail::info_nonexistent_roll_forward<Duration>(info);
      assign(st.time_since_epoch(), i);
      break;
    }
    case nonexistent::roll_backward: {
      date::sys_time<Duration> st = detail::info_nonexistent_roll_backward<Duration>(info);
      assign(st.time_since_epoch(), i);
      break;
    }
    case nonexistent::shift_forward: {
      date::sys_time<Duration> st = detail::info_nonexistent_shift_forward(info, x);
      assign(st.time_since_epoch(), i);
      break;
    }
    case nonexistent::shift_backward: {
      date::sys_time<Duration> st = detail::info_nonexistent_shift_backward(info, x);
      assign(st.time_since_epoch(), i);
      break;
    }
    case nonexistent::na: {
      assign_na(i);
      break;
    }
    case nonexistent::error: {
      detail::info_nonexistent_error(i, call);
    }
    }
    break;
  }
  case date::local_info::ambiguous: {
    switch (ambiguous_val) {
    case ambiguous::earliest: {
      date::sys_time<Duration> st = detail::info_ambiguous_earliest(info, x);
      assign(st.time_since_epoch(), i);
      break;
    }
    case ambiguous::latest: {
      date::sys_time<Duration> st = detail::info_ambiguous_latest(info, x);
      assign(st.time_since_epoch(), i);
      break;
    }
    case ambiguous::na: {
      assign_na(i);
      break;
    }
    case ambiguous::error: {
      detail::info_ambiguous_error(i, call);
    }
    }
    break;
  }
  }
}

template <typename Duration>
inline
void
duration<Duration>::convert_local_with_reference_to_sys_and_assign(const date::local_time<Duration>& x,
                                                                   const date::local_info& info,
                                                                   const enum nonexistent& nonexistent_val,
                                                                   const enum ambiguous& ambiguous_val,
                                                                   const date::sys_seconds& reference,
                                                                   const date::time_zone* p_time_zone,
                                                                   const r_ssize& i,
                                                                   const cpp11::sexp& call)
{
  if (info.result == date::local_info::unique ||
      info.result == date::local_info::nonexistent) {
    // For `unique` and `nonexistent`, nothing changes
    convert_local_to_sys_and_assign(x, info, nonexistent_val, ambiguous_val, i, call);
    return;
  }

  const date::local_seconds ref_lt = rclock::get_local_time(reference, p_time_zone);
  const date::local_info ref_info = rclock::get_info(ref_lt, p_time_zone);

  if (ref_info.result != date::local_info::ambiguous) {
    // If reference time is not ambiguous, we can't get any offset information
    // from it so fallback to using `ambiguous_val`
    convert_local_to_sys_and_assign(x, info, nonexistent_val, ambiguous_val, i, call);
    return;
  }
  if (ref_info.first.end != info.first.end) {
    // If reference time is ambiguous, but the transitions don't match,
    // we again can't get offset information from it
    convert_local_to_sys_and_assign(x, info, nonexistent_val, ambiguous_val, i, call);
    return;
  }

  const std::chrono::seconds offset =
    reference < ref_info.first.end ?
    ref_info.first.offset :
    ref_info.second.offset;

  const date::sys_time<Duration> st = date::sys_time<Duration>{x.time_since_epoch()} - offset;

  assign(st.time_since_epoch(), i);
}

} // namespace duration

} // namespace rclock

// `std::common_type()` specialization for `rclock::duration` types
namespace std {
  template<class Duration1, class Duration2>
  struct common_type<rclock::duration::duration<Duration1>, rclock::duration::duration<Duration2>> {
    using type = rclock::duration::duration<typename std::common_type<Duration1, Duration2>::type>;
  };
}

#endif

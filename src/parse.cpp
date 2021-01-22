#include "clock.h"
#include "utils.h"
#include "enums.h"
#include "zone.h"
#include "duration.h"
#include <sstream>
#include <locale>

// -----------------------------------------------------------------------------

// template <class ClockDuration>
// cpp11::writable::list
// parse_zoned_time_impl(const cpp11::strings& x,
//                       const cpp11::strings& format,
//                       const date::time_zone* p_time_zone,
//                       const cpp11::strings& nonexistent_string,
//                       const cpp11::strings& ambiguous_string) {
//   const r_ssize size = x.size();
//   ClockDuration out(size);
//   using Duration = typename ClockDuration::duration;
//
//   if (!r_is_string(format)) {
//     clock_abort("`format` must be a single string.");
//   }
//   const SEXP format_sexp = format[0];
//   const char* format_char = CHAR(format_sexp);
//
//   const bool recycle_nonexistent = clock_is_scalar(nonexistent_string);
//   const bool recycle_ambiguous = clock_is_scalar(ambiguous_string);
//
//   enum nonexistent nonexistent_val;
//   enum ambiguous ambiguous_val;
//
//   if (recycle_nonexistent) {
//     nonexistent_val = parse_nonexistent_one(nonexistent_string[0]);
//   }
//   if (recycle_ambiguous) {
//     ambiguous_val = parse_ambiguous_one(ambiguous_string[0]);
//   }
//
//   std::istringstream stream;
//
//   for (r_ssize i = 0; i < size; ++i) {
//     const SEXP elt = x[i];
//
//     if (elt == r_chr_na) {
//       out.assign_na(i);
//       continue;
//     }
//
//     const char* elt_char = CHAR(elt);
//
//     stream.clear();
//     stream.str(elt_char);
//
//     date::local_time<Duration> lt;
//
//     std::string elt_zone;
//     std::chrono::minutes elt_offset{0};
//
//     from_stream(stream, format_char, lt, &elt_zone, &elt_offset);
//
//     if (stream.fail()) {
//       out.assign_na(i);
//       continue;
//     }
//
//     // Default to supplied time zone
//     const date::time_zone* p_elt_time_zone = p_time_zone;
//
//     // Swap to element specific time zone if one exists and is valid
//     if (!elt_zone.empty()) {
//       try {
//         p_elt_time_zone = date::locate_zone(elt_zone);
//       } catch (const std::runtime_error& error) {
//         // If %Z isn't a real zone, `NA`
//         out.assign_na(i);
//         continue;
//       };
//     }
//
//     if (elt_offset != std::chrono::minutes{0}) {
//       // If an offset is present, treat the parsed time as an offset from UTC
//       out.assign(lt.time_since_epoch() - elt_offset, i);
//     } else {
//       // If no offset is present, treat the parsed time as a local time
//       // and convert to the zone that was either found in the string or
//       // supplied as `zone`
//       const enum nonexistent elt_nonexistent_val =
//         recycle_nonexistent ?
//         nonexistent_val :
//       parse_nonexistent_one(nonexistent_string[i]);
//
//       const enum ambiguous elt_ambiguous_val =
//         recycle_ambiguous ?
//         ambiguous_val :
//         parse_ambiguous_one(ambiguous_string[i]);
//
//       out.convert_local_to_sys_and_assign(
//         lt,
//         p_time_zone,
//         elt_nonexistent_val,
//         elt_ambiguous_val,
//         i
//       );
//     }
//   }
//
//   return out.to_list();
// }
//
// [[cpp11::register]]
// cpp11::writable::list
// parse_zoned_time_cpp(const cpp11::strings& x,
//                      const cpp11::strings& format,
//                      const cpp11::integers& precision_int,
//                      const cpp11::strings& zone,
//                      const cpp11::strings& nonexistent_string,
//                      const cpp11::strings& ambiguous_string) {
//   using namespace rclock;
//
//   const cpp11::writable::strings zone_standard = zone_standardize(zone);
//   const std::string zone_name = cpp11::r_string(zone_standard[0]);
//   const date::time_zone* p_time_zone = zone_name_load(zone_name);
//
//   switch (parse_precision(precision_int)) {
//   case precision::second: return parse_zoned_time_impl<duration::seconds>(x, format, p_time_zone, nonexistent_string, ambiguous_string);
//   case precision::millisecond: return parse_zoned_time_impl<duration::milliseconds>(x, format, p_time_zone, nonexistent_string, ambiguous_string);
//   case precision::microsecond: return parse_zoned_time_impl<duration::microseconds>(x, format, p_time_zone, nonexistent_string, ambiguous_string);
//   case precision::nanosecond: return parse_zoned_time_impl<duration::nanoseconds>(x, format, p_time_zone, nonexistent_string, ambiguous_string);
//   default: never_reached("parse_zoned_time_cpp");
//   }
// }

// -----------------------------------------------------------------------------

template <class ClockDuration, class Clock>
static
cpp11::writable::list
parse_time_point_impl(const cpp11::strings& x,
                      const cpp11::strings& format) {
  const r_ssize size = x.size();
  ClockDuration out(size);
  using Duration = typename ClockDuration::duration;

  if (!r_is_string(format)) {
    clock_abort("`format` must be a single string.");
  }
  const SEXP format_sexp = format[0];
  const char* format_char = CHAR(format_sexp);

  std::istringstream stream;

  for (r_ssize i = 0; i < size; ++i) {
    const SEXP elt = x[i];

    if (elt == r_chr_na) {
      out.assign_na(i);
      continue;
    }

    const char* elt_char = CHAR(elt);

    stream.clear();
    stream.str(elt_char);

    std::chrono::time_point<Clock, Duration> tp;

    date::from_stream(stream, format_char, tp);

    if (stream.fail()) {
      out.assign_na(i);
      continue;
    }

    out.assign(tp.time_since_epoch(), i);
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
parse_time_point_cpp(const cpp11::strings& x,
                     const cpp11::strings& format,
                     const cpp11::integers& precision_int,
                     const cpp11::integers& clock_int) {
  using namespace rclock;

  switch (parse_clock_name(clock_int)) {
  case clock_name::naive: {
    switch (parse_precision(precision_int)) {
    case precision::day: return parse_time_point_impl<duration::days, date::local_t>(x, format);
    case precision::hour: return parse_time_point_impl<duration::hours, date::local_t>(x, format);
    case precision::minute: return parse_time_point_impl<duration::minutes, date::local_t>(x, format);
    case precision::second: return parse_time_point_impl<duration::seconds, date::local_t>(x, format);
    case precision::millisecond: return parse_time_point_impl<duration::milliseconds, date::local_t>(x, format);
    case precision::microsecond: return parse_time_point_impl<duration::microseconds, date::local_t>(x, format);
    case precision::nanosecond: return parse_time_point_impl<duration::nanoseconds, date::local_t>(x, format);
    default: never_reached("parse_time_point_cpp");
    }
  }
  case clock_name::sys: {
    switch (parse_precision(precision_int)) {
    case precision::day: return parse_time_point_impl<duration::days, std::chrono::system_clock>(x, format);
    case precision::hour: return parse_time_point_impl<duration::hours, std::chrono::system_clock>(x, format);
    case precision::minute: return parse_time_point_impl<duration::minutes, std::chrono::system_clock>(x, format);
    case precision::second: return parse_time_point_impl<duration::seconds, std::chrono::system_clock>(x, format);
    case precision::millisecond: return parse_time_point_impl<duration::milliseconds, std::chrono::system_clock>(x, format);
    case precision::microsecond: return parse_time_point_impl<duration::microseconds, std::chrono::system_clock>(x, format);
    case precision::nanosecond: return parse_time_point_impl<duration::nanoseconds, std::chrono::system_clock>(x, format);
    default: never_reached("parse_time_point_cpp");
    }
  }
  default: never_reached("parse_time_point_cpp");
  }
}

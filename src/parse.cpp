#include "civil.h"
#include "utils.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "civil-rcrd.h"
#include "check.h"
#include "zone.h"
#include <sstream>
#include <locale>

// -----------------------------------------------------------------------------

/*
 * Adding another template variation of `from_stream()` that parses into a ymd
 * and tod directly. This way we keep the precision when parsing large dates
 * (i.e. with year past 1970 +/- 292) that have nanoseconds
 */

template <class Duration, class CharT, class Traits, class Alloc = std::allocator<CharT>>
std::basic_istream<CharT, Traits>&
from_stream(std::basic_istream<CharT, Traits>& is,
            const CharT* fmt,
            date::year_month_day& ymd,
            date::hh_mm_ss<Duration>& tod,
            std::basic_string<CharT, Traits, Alloc>* abbrev = nullptr,
            std::chrono::minutes* offset = nullptr)
{
  using CT = typename std::common_type<Duration, std::chrono::seconds>::type;
  std::chrono::minutes offset_local{};
  std::chrono::minutes* offptr = offset ? offset : &offset_local;
  date::fields<CT> fds{};
  fds.has_tod = true;
  from_stream(is, fmt, fds, abbrev, offptr);
  if (!fds.ymd.ok() || !fds.tod.in_conventional_range())
    is.setstate(std::ios::failbit);
  if (!is.fail()) {
    ymd = fds.ymd;
    tod = fds.tod;
  }
  return is;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_rcrd parse_zoned_datetime_cpp(const cpp11::strings& x,
                                             const cpp11::strings& format,
                                             const cpp11::strings& zone,
                                             const cpp11::strings& dst_nonexistent,
                                             const cpp11::strings& dst_ambiguous,
                                             const cpp11::integers& size) {
  r_ssize c_size = size[0];

  cpp11::writable::strings zone_standard = zone_standardize(zone);
  cpp11::r_string zone_name_r(zone_standard[0]);
  std::string zone_name(zone_name_r);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  if (format.size() != 1) {
    civil_abort("`format` must have size 1.");
  }

  std::string string_format(format[0]);
  const char* c_format = string_format.c_str();

  civil_writable_field days(c_size);
  civil_writable_field time_of_day(c_size);

  civil_writable_rcrd out = new_days_time_of_day_list(days, time_of_day);

  bool recycle_x = civil_is_scalar(x);
  bool recycle_dst_nonexistent = civil_is_scalar(dst_nonexistent);
  bool recycle_dst_ambiguous = civil_is_scalar(dst_ambiguous);

  enum dst_nonexistent dst_nonexistent_val;
  if (recycle_dst_nonexistent) {
    dst_nonexistent_val = parse_dst_nonexistent_one(dst_nonexistent[0]);
  }

  enum dst_ambiguous dst_ambiguous_val;
  if (recycle_dst_ambiguous) {
    dst_ambiguous_val = parse_dst_ambiguous_one(dst_ambiguous[0]);
  }

  std::istringstream stream;
  stream.imbue(std::locale::classic());

  for (r_ssize i = 0; i < c_size; ++i) {
    cpp11::r_string elt_x = recycle_x ? x[0] : x[i];

    if (elt_x == r_chr_na) {
      days[i] = r_int_na;
      time_of_day[i] = r_int_na;
      continue;
    }

    std::string elt_x_string(elt_x);
    stream.clear();
    stream.str(elt_x_string);

    date::year_month_day elt_ymd;
    date::hh_mm_ss<std::chrono::seconds> elt_tod;
    std::string elt_zone;
    std::chrono::minutes elt_offset{0};

    from_stream(stream, c_format, elt_ymd, elt_tod, &elt_zone, &elt_offset);

    if (stream.fail()) {
      days[i] = r_int_na;
      time_of_day[i] = r_int_na;
      continue;
    }

    // Default to supplied time zone
    const date::time_zone* p_elt_time_zone = p_time_zone;

    // Swap to string specific time zone if one exists and is valid
    if (!elt_zone.empty()) {
      try {
        p_elt_time_zone = date::locate_zone(elt_zone);
      } catch (const std::runtime_error& error) {
        // Ignore runtime error if %Z isn't a real zone
      };
    }

    const enum dst_nonexistent elt_dst_nonexistent_val =
      recycle_dst_nonexistent ?
      dst_nonexistent_val :
      parse_dst_nonexistent_one(dst_nonexistent[i]);

    const enum dst_ambiguous elt_dst_ambiguous_val =
      recycle_dst_ambiguous ?
      dst_ambiguous_val :
      parse_dst_ambiguous_one(dst_ambiguous[i]);

    date::local_days elt_lday{elt_ymd};
    date::local_seconds elt_lsec_floor{elt_lday};

    date::local_seconds elt_lsec =
      elt_lsec_floor +
      elt_tod.hours() +
      elt_tod.minutes() +
      elt_tod.seconds();

    date::sys_seconds out_ssec;

    if (elt_offset != std::chrono::minutes{0}) {
      // If an offset is present, treat as offset from UTC and return sys seconds
      date::sys_seconds elt_ssec{elt_lsec.time_since_epoch()};
      out_ssec = elt_ssec - elt_offset;
    } else {
      // If no offset is present, treat as a local time and convert to the
      // zone either implicit in the string or supplied as `zone`.
      bool na = false;

      out_ssec = convert_local_to_sys(
        elt_lsec,
        p_elt_time_zone,
        i,
        elt_dst_nonexistent_val,
        elt_dst_ambiguous_val,
        na
      );

      if (na) {
        days[i] = r_int_na;
        time_of_day[i] = r_int_na;
        continue;
      }
    }

    date::sys_days out_sday = date::floor<date::days>(out_ssec);
    date::sys_seconds out_ssec_floor{out_sday};

    std::chrono::seconds out_tod{out_ssec - out_ssec_floor};

    days[i] = out_sday.time_since_epoch().count();
    time_of_day[i] = out_tod.count();
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
civil_writable_rcrd parse_naive_datetime_cpp(const cpp11::strings& x,
                                             const cpp11::strings& format) {
  r_ssize c_size = x.size();

  if (format.size() != 1) {
    civil_abort("`format` must have size 1.");
  }

  std::string string_format(format[0]);
  const char* c_format = string_format.c_str();

  civil_writable_field days(c_size);
  civil_writable_field time_of_day(c_size);

  civil_writable_rcrd out = new_days_time_of_day_list(days, time_of_day);

  std::istringstream stream;
  stream.imbue(std::locale::classic());

  for (r_ssize i = 0; i < c_size; ++i) {
    cpp11::r_string elt_x = x[i];

    if (elt_x == r_chr_na) {
      days[i] = r_int_na;
      time_of_day[i] = r_int_na;
      continue;
    }

    std::string elt_x_string(elt_x);
    stream.clear();
    stream.str(elt_x_string);

    date::year_month_day elt_ymd;
    date::hh_mm_ss<std::chrono::seconds> elt_tod;

    from_stream(stream, c_format, elt_ymd, elt_tod);

    if (stream.fail()) {
      days[i] = r_int_na;
      time_of_day[i] = r_int_na;
      continue;
    }

    date::local_days elt_lday{elt_ymd};
    date::local_seconds elt_lsec_floor{elt_lday};

    date::local_seconds out_lsec =
      elt_lsec_floor +
      elt_tod.hours() +
      elt_tod.minutes() +
      elt_tod.seconds();

    date::local_days out_lday = date::floor<date::days>(out_lsec);
    date::local_seconds out_lsec_floor{out_lday};

    std::chrono::seconds out_tod{out_lsec - out_lsec_floor};

    days[i] = out_lday.time_since_epoch().count();
    time_of_day[i] = out_tod.count();
  }

  return out;
}

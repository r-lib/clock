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

/*
 * Reference for format tokens
 * https://howardhinnant.github.io/date/date.html#to_stream_formatting
 */

// -----------------------------------------------------------------------------

/*
 * Adding another template variation of `to_stream()` that parses from a ymd
 * and tod directly. This way we keep the precision when parsing large dates
 * (i.e. with year past 1970 +/- 292) that have nanoseconds
 */

template <class CharT, class Traits, class Duration>
inline
std::basic_ostream<CharT, Traits>&
to_stream(std::basic_ostream<CharT, Traits>& os,
          const CharT* fmt,
          const date::year_month_day& ymd,
          const date::hh_mm_ss<Duration>& hms,
          const std::string* abbrev = nullptr,
          const std::chrono::seconds* offset_sec = nullptr)
{
  date::fields<Duration> fds{ymd, hms};
  return to_stream(os, fmt, fds, abbrev, offset_sec);
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::strings format_civil_rcrd_cpp(const civil_field& days,
                                               const civil_field& time_of_day,
                                               const civil_field& nanos_of_second,
                                               const cpp11::strings& zone,
                                               const cpp11::strings& format,
                                               const bool& naive,
                                               const bool& nano,
                                               const bool& abbreviate_zone) {
  r_ssize size = days.size();

  cpp11::writable::strings out(size);

  cpp11::writable::strings zone_standard = zone_standardize(zone);
  std::string zone_name = cpp11::r_string(zone_standard[0]);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  if (format.size() != 1) {
    civil_abort("`format` must have size 1.");
  }

  std::string string_format(format[0]);
  const char* c_format = string_format.c_str();

  std::basic_ostringstream<char> stream;
  stream.imbue(std::locale::classic());

  std::string zone_name_print;
  std::string* p_zone_name_print = nullptr;

  if (!naive) {
    // If zoned, default printable zone name to full provided zone name
    zone_name_print = (zone_name.size() == 0) ? zone_name_current() : zone_name;
    p_zone_name_print = &zone_name_print;
  }

  // Default to no offset, which might change if formatting a zoned datetime
  std::chrono::seconds offset;
  std::chrono::seconds* p_offset = nullptr;

  for (r_ssize i = 0; i < size; ++i) {
    int elt_days = days[i];
    int elt_time_of_day = time_of_day[i];

    if (elt_days == r_int_na) {
      out[i] = r_chr_na;
      continue;
    }

    date::local_days elt_lday;
    std::chrono::seconds elt_ltod;

    if (naive) {
      elt_lday = date::local_days{date::days{elt_days}};
      elt_ltod = std::chrono::seconds{elt_time_of_day};
    } else {
      date::sys_days elt_sday{date::days{elt_days}};
      std::chrono::seconds elt_stod{elt_time_of_day};

      date::sys_seconds elt_ssec_floor{elt_sday};
      date::sys_seconds elt_ssec{elt_ssec_floor + elt_stod};

      date::sys_info info = p_time_zone->get_info(elt_ssec);

      offset = info.offset;
      p_offset = &offset;

      if (abbreviate_zone) {
        zone_name_print = info.abbrev;
        p_zone_name_print = &zone_name_print;
      }

      date::local_seconds elt_lsec{(elt_ssec + info.offset).time_since_epoch()};

      elt_lday = date::floor<date::days>(elt_lsec);
      date::local_seconds elt_lsec_floor{elt_lday};

      elt_ltod = elt_lsec - elt_lsec_floor;
    }

    // Reset stream
    stream.str(std::string());
    // Reset flags
    stream.clear();

    date::year_month_day elt_ymd{elt_lday};

    if (nano) {
      int elt_nanos_of_second = nanos_of_second[i];
      std::chrono::nanoseconds elt_nanos{elt_nanos_of_second};
      date::hh_mm_ss<std::chrono::nanoseconds> elt_hms{elt_ltod + elt_nanos};
      to_stream(stream, c_format, elt_ymd, elt_hms, p_zone_name_print, p_offset);
    } else {
      date::hh_mm_ss<std::chrono::seconds> elt_hms{elt_ltod};
      to_stream(stream, c_format, elt_ymd, elt_hms, p_zone_name_print, p_offset);
    }

    if (stream.fail()) {
      out[i] = r_chr_na;
      continue;
    }

    out[i] = stream.str();
  }

  return out;
}


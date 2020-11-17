#include "r.h"
#include "utils.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "civil-rcrd.h"
#include "check.h"
#include "zone.h"
#include <date/date.h>
#include <date/tz.h>
#include <sstream>

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
SEXP parse_zoned_datetime_cpp(SEXP x,
                              SEXP format,
                              SEXP zone,
                              SEXP locale,
                              SEXP dst_nonexistent,
                              SEXP dst_ambiguous,
                              SEXP size) {
  r_ssize c_size = r_int_get(size, 0);

  zone = PROTECT(zone_standardize(zone));
  std::string zone_name = zone_unwrap(zone);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  const char* c_format = CHAR(r_chr_get(format, 0));

  const char* c_locale = CHAR(r_chr_get(locale, 0));
  std::locale cpp_locale;

  try {
    cpp_locale = std::locale{c_locale};
  }
  catch (const std::runtime_error& error) {
    r_abort("Failed to load locale.");
  }

  SEXP days = PROTECT(r_new_integer(c_size));
  int* p_days = r_int_deref(days);

  SEXP time_of_day = PROTECT(r_new_integer(c_size));
  int* p_time_of_day = r_int_deref(time_of_day);

  int* p_nanos_of_second = NULL;

  SEXP out = PROTECT(new_days_time_of_day_list(days, time_of_day));

  const SEXP* p_dst_nonexistent = STRING_PTR_RO(dst_nonexistent);
  bool recycle_dst_nonexistent = r_is_scalar(dst_nonexistent);
  enum dst_nonexistent c_dst_nonexistent;
  if (recycle_dst_nonexistent) {
    c_dst_nonexistent = parse_dst_nonexistent_one(CHAR(p_dst_nonexistent[0]));
  }

  const SEXP* p_dst_ambiguous = STRING_PTR_RO(dst_ambiguous);
  bool recycle_dst_ambiguous = r_is_scalar(dst_ambiguous);
  enum dst_ambiguous c_dst_ambiguous;
  if (recycle_dst_ambiguous) {
    c_dst_ambiguous = parse_dst_ambiguous_one(CHAR(p_dst_ambiguous[0]));
  }

  const SEXP* p_x = STRING_PTR_RO(x);

  std::istringstream elt_stream;
  elt_stream.imbue(cpp_locale);

  for (r_ssize i = 0; i < c_size; ++i) {
    SEXP elt_x = p_x[i];

    if (elt_x == NA_STRING) {
      civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
      continue;
    }

    const char* elt_x_char = Rf_translateCharUTF8(elt_x);
    elt_stream.clear();
    elt_stream.str(elt_x_char);

    date::year_month_day elt_ymd;
    date::hh_mm_ss<std::chrono::seconds> elt_tod;
    std::string elt_zone;
    std::chrono::minutes elt_offset{0};

    from_stream(elt_stream, c_format, elt_ymd, elt_tod, &elt_zone, &elt_offset);

    if (elt_stream.fail()) {
      civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
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

    const enum dst_nonexistent elt_dst_nonexistent =
      recycle_dst_nonexistent ?
      c_dst_nonexistent :
      parse_dst_nonexistent_one(CHAR(p_dst_nonexistent[i]));

    const enum dst_ambiguous elt_dst_ambiguous =
      recycle_dst_ambiguous ?
      c_dst_ambiguous :
      parse_dst_ambiguous_one(CHAR(p_dst_ambiguous[i]));

    date::local_days elt_lday{elt_ymd};
    date::local_seconds elt_lsec_floor{elt_lday};

    date::local_seconds elt_lsec =
      elt_lsec_floor +
      elt_tod.hours() +
      elt_tod.minutes() +
      elt_tod.seconds();

    date::sys_seconds out_ssec;

    if (elt_offset != std::chrono::minutes{0}) {
      date::sys_seconds elt_ssec{elt_lsec.time_since_epoch()};
      out_ssec = elt_ssec - elt_offset;
    } else {
      bool na = false;

      out_ssec = convert_local_to_sys(
        elt_lsec,
        p_elt_time_zone,
        i,
        elt_dst_nonexistent,
        elt_dst_ambiguous,
        na
      );

      if (na) {
        civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
        continue;
      }
    }

    date::sys_days out_sday = date::floor<date::days>(out_ssec);
    date::sys_seconds out_ssec_floor{out_sday};

    std::chrono::seconds out_tod{out_ssec - out_ssec_floor};

    p_days[i] = out_sday.time_since_epoch().count();
    p_time_of_day[i] = out_tod.count();
  }

  UNPROTECT(4);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP parse_local_datetime_cpp(SEXP x,
                              SEXP format,
                              SEXP locale) {
  r_ssize c_size = r_length(x);

  const char* c_format = CHAR(r_chr_get(format, 0));

  const char* c_locale = CHAR(r_chr_get(locale, 0));
  std::locale cpp_locale;

  try {
    cpp_locale = std::locale{c_locale};
  }
  catch (const std::runtime_error& error) {
    r_abort("Failed to load locale.");
  }

  SEXP days = PROTECT(r_new_integer(c_size));
  int* p_days = r_int_deref(days);

  SEXP time_of_day = PROTECT(r_new_integer(c_size));
  int* p_time_of_day = r_int_deref(time_of_day);

  int* p_nanos_of_second = NULL;

  SEXP out = PROTECT(new_days_time_of_day_list(days, time_of_day));

  const SEXP* p_x = STRING_PTR_RO(x);

  std::istringstream elt_stream;
  elt_stream.imbue(cpp_locale);

  for (r_ssize i = 0; i < c_size; ++i) {
    SEXP elt_x = p_x[i];

    if (elt_x == NA_STRING) {
      civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
      continue;
    }

    const char* elt_x_char = Rf_translateCharUTF8(elt_x);
    elt_stream.clear();
    elt_stream.str(elt_x_char);

    date::year_month_day elt_ymd;
    date::hh_mm_ss<std::chrono::seconds> elt_tod;

    from_stream(elt_stream, c_format, elt_ymd, elt_tod);

    if (elt_stream.fail()) {
      civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
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

    p_days[i] = out_lday.time_since_epoch().count();
    p_time_of_day[i] = out_tod.count();
  }

  UNPROTECT(3);
  return out;
}

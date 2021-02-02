#include "duration.h"
#include "enums.h"
#include "utils.h"
#include "get.h"
#include "rcrd.h"
#include "zone.h"
#include "parse.h"
#include "locale.h"

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
new_zoned_time_from_fields(SEXP fields,
                           const cpp11::integers& precision_int,
                           const cpp11::strings& zone,
                           SEXP names) {
  const enum precision precision_val = parse_precision(precision_int);

  const r_ssize n_fields = Rf_xlength(fields);

  switch (precision_val) {
  case precision::year:
  case precision::quarter:
  case precision::month:
  case precision::week:
  case precision::day:
  case precision::hour:
  case precision::minute: {
    clock_abort("`precision` must be at least 'second' precision.");
  }
  case precision::second: {
    if (n_fields != 2) {
      clock_abort("`fields` must have 2 fields for second precision.");
    }
    break;
  }
  case precision::millisecond:
  case precision::microsecond:
  case precision::nanosecond: {
    if (n_fields != 3) {
      clock_abort("`fields` must have 3 fields for [millisecond, nanosecond] precision.");
    }
    break;
  }
  default: {
    never_reached("new_zoned_time_from_fields");
  }
  }

  if (!r_is_string(zone)) {
    clock_abort("`zone` must be a string.");
  }

  SEXP out = PROTECT(new_clock_rcrd_from_fields(fields, names, classes_zoned_time));

  Rf_setAttrib(out, syms_precision, precision_int);
  Rf_setAttrib(out, syms_zone, zone);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
zoned_time_restore(SEXP x, SEXP to) {
  SEXP zone = Rf_getAttrib(to, syms_zone);
  SEXP precision = Rf_getAttrib(to, syms_precision);

  SEXP out = PROTECT(clock_rcrd_restore(x, to, classes_zoned_time));

  Rf_setAttrib(out, syms_zone, zone);
  Rf_setAttrib(out, syms_precision, precision);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

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
                   const cpp11::integers& precision_int,
                   const cpp11::strings& zone) {
  using namespace rclock;

  const cpp11::writable::strings zone_standard = zone_standardize(zone);
  const std::string zone_name = cpp11::r_string(zone_standard[0]);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  cpp11::integers ticks = duration::get_ticks(fields);
  cpp11::integers ticks_of_day = duration::get_ticks_of_day(fields);
  cpp11::integers ticks_of_second = duration::get_ticks_of_second(fields);

  duration::seconds ds{ticks, ticks_of_day};
  duration::milliseconds dmilli{ticks, ticks_of_day, ticks_of_second};
  duration::microseconds dmicro{ticks, ticks_of_day, ticks_of_second};
  duration::nanoseconds dnano{ticks, ticks_of_day, ticks_of_second};

  switch (parse_precision(precision_int)) {
  case precision::second: return get_naive_time_impl(ds, p_time_zone);
  case precision::millisecond: return get_naive_time_impl(dmilli, p_time_zone);
  case precision::microsecond: return get_naive_time_impl(dmicro, p_time_zone);
  case precision::nanosecond: return get_naive_time_impl(dnano, p_time_zone);
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
                                       const cpp11::strings& nonexistent_string,
                                       const cpp11::strings& ambiguous_string) {
  using Duration = typename ClockDuration::duration;

  const r_ssize size = x.size();
  ClockDuration out(size);

  const bool recycle_nonexistent = clock_is_scalar(nonexistent_string);
  const bool recycle_ambiguous = clock_is_scalar(ambiguous_string);

  enum nonexistent nonexistent_val;
  enum ambiguous ambiguous_val;

  if (recycle_nonexistent) {
    nonexistent_val = parse_nonexistent_one(nonexistent_string[0]);
  }
  if (recycle_ambiguous) {
    ambiguous_val = parse_ambiguous_one(ambiguous_string[0]);
  }

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out.assign_na(i);
      continue;
    }

    const enum nonexistent elt_nonexistent_val =
      recycle_nonexistent ?
      nonexistent_val :
      parse_nonexistent_one(nonexistent_string[i]);

    const enum ambiguous elt_ambiguous_val =
      recycle_ambiguous ?
      ambiguous_val :
      parse_ambiguous_one(ambiguous_string[i]);

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
                                      const cpp11::integers& precision_int,
                                      const cpp11::strings& zone,
                                      const cpp11::strings& nonexistent_string,
                                      const cpp11::strings& ambiguous_string) {
  using namespace rclock;

  const cpp11::writable::strings zone_standard = zone_standardize(zone);
  const std::string zone_name = cpp11::r_string(zone_standard[0]);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  cpp11::integers ticks = duration::get_ticks(fields);
  cpp11::integers ticks_of_day = duration::get_ticks_of_day(fields);
  cpp11::integers ticks_of_second = duration::get_ticks_of_second(fields);

  duration::seconds ds{ticks, ticks_of_day};
  duration::milliseconds dmilli{ticks, ticks_of_day, ticks_of_second};
  duration::microseconds dmicro{ticks, ticks_of_day, ticks_of_second};
  duration::nanoseconds dnano{ticks, ticks_of_day, ticks_of_second};

  switch (parse_precision(precision_int)) {
  case precision::second: return as_zoned_sys_time_from_naive_time_impl(ds, p_time_zone, nonexistent_string, ambiguous_string);
  case precision::millisecond: return as_zoned_sys_time_from_naive_time_impl(dmilli, p_time_zone, nonexistent_string, ambiguous_string);
  case precision::microsecond: return as_zoned_sys_time_from_naive_time_impl(dmicro, p_time_zone, nonexistent_string, ambiguous_string);
  case precision::nanosecond: return as_zoned_sys_time_from_naive_time_impl(dnano, p_time_zone, nonexistent_string, ambiguous_string);
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
  const cpp11::integers ticks = rclock::duration::get_ticks(fields);
  const cpp11::integers ticks_of_day = rclock::duration::get_ticks_of_day(fields);
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
cpp11::writable::list
zoned_info_impl(const ClockDuration& x,
                const date::time_zone* p_time_zone) {
  const r_ssize size = x.size();
  using Duration = typename ClockDuration::duration;

  rclock::duration::seconds begin(size);
  rclock::duration::seconds end(size);
  rclock::duration::seconds offset(size);
  cpp11::writable::logicals dst(size);
  cpp11::writable::strings abbreviation(size);

  const std::chrono::minutes zero{0};

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      begin.assign_na(i);
      end.assign_na(i);
      offset.assign_na(i);
      dst[i] = r_lgl_na;
      SET_STRING_ELT(abbreviation, i, r_chr_na);
      continue;
    }

    const date::sys_time<Duration> elt{x[i]};
    const date::sys_info info = p_time_zone->get_info(elt);

    begin.assign(info.begin.time_since_epoch(), i);
    end.assign(info.end.time_since_epoch(), i);

    offset.assign(info.offset, i);

    dst[i] = info.save != zero;

    const SEXP abbreviation_elt = Rf_mkCharLenCE(info.abbrev.c_str(), info.abbrev.size(), CE_UTF8);
    SET_STRING_ELT(abbreviation, i, abbreviation_elt);
  }

  cpp11::writable::list out_begin = begin.to_list();
  cpp11::writable::list out_end = end.to_list();
  cpp11::writable::list out_offset = offset.to_list();

  cpp11::writable::list out = { out_begin, out_end, out_offset, dst, abbreviation};
  out.names() = {"begin", "end", "offset", "dst", "abbreviation"};

  return out;
}

[[cpp11::register]]
cpp11::writable::list
zoned_info_cpp(cpp11::list_of<cpp11::integers> fields,
               const cpp11::integers& precision_int,
               const cpp11::strings& zone) {
  using namespace rclock;

  const cpp11::writable::strings zone_standard = zone_standardize(zone);
  const std::string zone_name = cpp11::r_string(zone_standard[0]);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  const cpp11::integers ticks = duration::get_ticks(fields);
  const cpp11::integers ticks_of_day = duration::get_ticks_of_day(fields);
  const cpp11::integers ticks_of_second = duration::get_ticks_of_second(fields);

  const duration::seconds ds{ticks, ticks_of_day};
  const duration::milliseconds dmilli{ticks, ticks_of_day, ticks_of_second};
  const duration::microseconds dmicro{ticks, ticks_of_day, ticks_of_second};
  const duration::nanoseconds dnano{ticks, ticks_of_day, ticks_of_second};

  switch (parse_precision(precision_int)) {
  case precision::second: return zoned_info_impl(ds, p_time_zone);
  case precision::millisecond: return zoned_info_impl(dmilli, p_time_zone);
  case precision::microsecond: return zoned_info_impl(dmicro, p_time_zone);
  case precision::nanosecond: return zoned_info_impl(dnano, p_time_zone);
  default: clock_abort("Internal error: Should never be called.");
  }
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
cpp11::writable::list
parse_zoned_time_impl(const cpp11::strings& x,
                      const cpp11::strings& format,
                      const cpp11::strings& month,
                      const cpp11::strings& month_abbrev,
                      const cpp11::strings& day,
                      const cpp11::strings& day_ab,
                      const cpp11::strings& am_pm,
                      const cpp11::strings& mark) {
  const r_ssize size = x.size();
  ClockDuration fields(size);
  using Duration = typename ClockDuration::duration;

  if (!r_is_string(format)) {
    clock_abort("`format` must be a single string.");
  }
  const SEXP format_sexp = format[0];
  const char* format_char = CHAR(format_sexp);

  char dmark;
  switch (parse_decimal_mark(mark)) {
  case decimal_mark::comma: dmark = ','; break;
  case decimal_mark::period: dmark = '.'; break;
  default: clock_abort("Internal error: Unknown decimal mark.");
  }

  std::string month_names[24];
  const std::pair<const std::string*, const std::string*>& month_names_pair = fill_month_names(
    month,
    month_abbrev,
    month_names
  );

  std::string weekday_names[14];
  const std::pair<const std::string*, const std::string*>& weekday_names_pair = fill_weekday_names(
    day,
    day_ab,
    weekday_names
  );

  std::string ampm_names[2];
  const std::pair<const std::string*, const std::string*>& ampm_names_pair = fill_ampm_names(
    am_pm,
    ampm_names
  );

  std::string zone;

  std::istringstream stream;

  for (r_ssize i = 0; i < size; ++i) {
    const SEXP elt = x[i];

    if (elt == r_chr_na) {
      fields.assign_na(i);
      continue;
    }

    const char* elt_char = CHAR(elt);

    stream.clear();
    stream.str(elt_char);

    date::local_time<Duration> elt_lt;
    std::string elt_zone;
    std::chrono::minutes elt_offset{std::chrono::minutes::min()};

    rclock::from_stream(
      stream,
      format_char,
      month_names_pair,
      weekday_names_pair,
      ampm_names_pair,
      dmark,
      elt_lt,
      &elt_zone,
      &elt_offset
    );

    if (stream.fail()) {
      fields.assign_na(i);
      continue;
    }

    if (zone.empty()) {
      // First time, load zone
      try {
        (void) date::locate_zone(elt_zone);
        zone = elt_zone;
      } catch (const std::runtime_error& error) {
        std::string message =
          "`%%Z` must be used, and must result in a valid time zone name, not '" + elt_zone + "'.";
        clock_abort(message.c_str());
      };
    } else if (elt_zone != zone) {
      std::string message =
        std::string{"All elements of `x` must have the same time zone name. "} +
        "Found different zone names of: '" +
        zone + "' and '" + elt_zone +
        "'.";

      clock_abort(message.c_str());
    }

    if (elt_offset == std::chrono::minutes::min()) {
      clock_abort("`%%z` must be used, and must result in a valid offset from UTC.");
    }

    fields.assign(elt_lt.time_since_epoch() - elt_offset, i);
  }

  cpp11::writable::strings out_zone(1);
  out_zone[0] = zone;

  cpp11::writable::list out_fields = fields.to_list();

  cpp11::writable::list out = {out_fields, out_zone};
  out.names() = {"fields", "zone"};

  return out;
}

[[cpp11::register]]
cpp11::writable::list
parse_zoned_time_cpp(const cpp11::strings& x,
                     const cpp11::strings& format,
                     const cpp11::integers& precision_int,
                     const cpp11::strings& month,
                     const cpp11::strings& month_abbrev,
                     const cpp11::strings& day,
                     const cpp11::strings& day_ab,
                     const cpp11::strings& am_pm,
                     const cpp11::strings& mark) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::second: return parse_zoned_time_impl<duration::seconds>(x, format, month, month_abbrev, day, day_ab, am_pm, mark);
  case precision::millisecond: return parse_zoned_time_impl<duration::milliseconds>(x, format, month, month_abbrev, day, day_ab, am_pm, mark);
  case precision::microsecond: return parse_zoned_time_impl<duration::microseconds>(x, format, month, month_abbrev, day, day_ab, am_pm, mark);
  case precision::nanosecond: return parse_zoned_time_impl<duration::nanoseconds>(x, format, month, month_abbrev, day, day_ab, am_pm, mark);
  default: never_reached("parse_zoned_time_cpp");
  }
}

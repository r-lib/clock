#include "duration.h"
#include "enums.h"
#include "utils.h"
#include "get.h"
#include "rcrd.h"
#include "zone.h"
#include "parse.h"
#include "failure.h"
#include "fill.h"

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
new_zoned_time_from_fields(SEXP fields,
                           const cpp11::integers& precision_int,
                           const cpp11::strings& zone,
                           SEXP names) {
  const enum precision precision_val = parse_precision(precision_int);

  const r_ssize n_fields = Rf_xlength(fields);
  if (n_fields != 2) {
    clock_abort("`fields` must be length 2.");
  }

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
  case precision::second:
  case precision::millisecond:
  case precision::microsecond:
  case precision::nanosecond: {
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
get_naive_time_impl(cpp11::list_of<cpp11::doubles>& fields,
                    const date::time_zone* p_time_zone) {
  using Duration = typename ClockDuration::chrono_duration;

  const ClockDuration x{fields};
  const r_ssize size = x.size();

  ClockDuration out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out.assign_na(i);
      continue;
    }

    const Duration elt = x[i];
    const date::sys_time<Duration> elt_st{elt};
    const date::local_time<Duration> elt_lt = rclock::get_local_time(elt_st, p_time_zone);
    const Duration elt_out = elt_lt.time_since_epoch();
    out.assign(elt_out, i);
  }

  return out.to_list();
}


[[cpp11::register]]
cpp11::writable::list
get_naive_time_cpp(cpp11::list_of<cpp11::doubles> fields,
                   const cpp11::integers& precision_int,
                   const cpp11::strings& zone) {
  using namespace rclock;

  zone_size_validate(zone);
  const std::string zone_name = cpp11::r_string(zone[0]);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  switch (parse_precision(precision_int)) {
  case precision::second: return get_naive_time_impl<duration::seconds>(fields, p_time_zone);
  case precision::millisecond: return get_naive_time_impl<duration::milliseconds>(fields, p_time_zone);
  case precision::microsecond: return get_naive_time_impl<duration::microseconds>(fields, p_time_zone);
  case precision::nanosecond: return get_naive_time_impl<duration::nanoseconds>(fields, p_time_zone);
  default: clock_abort("Internal error: Should never be called.");
  }
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::list
as_zoned_sys_time_from_naive_time_impl(cpp11::list_of<cpp11::doubles>& fields,
                                       const date::time_zone* p_time_zone,
                                       const cpp11::strings& nonexistent_string,
                                       const cpp11::strings& ambiguous_string,
                                       const cpp11::sexp& call) {
  using Duration = typename ClockDuration::chrono_duration;

  const ClockDuration x{fields};
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
    const date::local_info elt_info = rclock::get_info(elt_lt, p_time_zone);

    out.convert_local_to_sys_and_assign(
      elt_lt,
      elt_info,
      elt_nonexistent_val,
      elt_ambiguous_val,
      i,
      call
    );
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
as_zoned_sys_time_from_naive_time_cpp(cpp11::list_of<cpp11::doubles> fields,
                                      const cpp11::integers& precision_int,
                                      const cpp11::strings& zone,
                                      const cpp11::strings& nonexistent_string,
                                      const cpp11::strings& ambiguous_string,
                                      const cpp11::sexp& call) {
  using namespace rclock;

  zone_size_validate(zone);
  const std::string zone_name = cpp11::r_string(zone[0]);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  switch (parse_precision(precision_int)) {
  case precision::second: return as_zoned_sys_time_from_naive_time_impl<duration::seconds>(fields, p_time_zone, nonexistent_string, ambiguous_string, call);
  case precision::millisecond: return as_zoned_sys_time_from_naive_time_impl<duration::milliseconds>(fields, p_time_zone, nonexistent_string, ambiguous_string, call);
  case precision::microsecond: return as_zoned_sys_time_from_naive_time_impl<duration::microseconds>(fields, p_time_zone, nonexistent_string, ambiguous_string, call);
  case precision::nanosecond: return as_zoned_sys_time_from_naive_time_impl<duration::nanoseconds>(fields, p_time_zone, nonexistent_string, ambiguous_string, call);
  default: clock_abort("Internal error: Should never be called.");
  }
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::list
as_zoned_sys_time_from_naive_time_with_reference_impl(cpp11::list_of<cpp11::doubles>& fields,
                                                      const date::time_zone* p_time_zone,
                                                      const cpp11::strings& nonexistent_string,
                                                      const cpp11::strings& ambiguous_string,
                                                      const rclock::duration::seconds& reference,
                                                      const cpp11::sexp& call) {
  using Duration = typename ClockDuration::chrono_duration;

  const ClockDuration x{fields};
  const r_ssize size = x.size();
  ClockDuration out(size);

  const bool recycle_nonexistent = clock_is_scalar(nonexistent_string);
  const bool recycle_ambiguous = clock_is_scalar(ambiguous_string);
  const bool recycle_reference = reference.size() == 1;

  enum nonexistent nonexistent_val;
  enum ambiguous ambiguous_val;
  date::sys_seconds reference_val;

  if (recycle_nonexistent) {
    nonexistent_val = parse_nonexistent_one(nonexistent_string[0]);
  }
  if (recycle_ambiguous) {
    ambiguous_val = parse_ambiguous_one(ambiguous_string[0]);
  }
  if (recycle_reference) {
    reference_val = date::sys_seconds{reference[0]};
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

    const date::sys_seconds elt_reference_val =
      recycle_reference ?
      reference_val :
      date::sys_seconds{reference[i]};

    const Duration elt = x[i];
    const date::local_time<Duration> elt_lt{elt};
    const date::local_info elt_info = rclock::get_info(elt_lt, p_time_zone);

    out.convert_local_with_reference_to_sys_and_assign(
      elt_lt,
      elt_info,
      elt_nonexistent_val,
      elt_ambiguous_val,
      elt_reference_val,
      p_time_zone,
      i,
      call
    );
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
as_zoned_sys_time_from_naive_time_with_reference_cpp(cpp11::list_of<cpp11::doubles> fields,
                                                     const cpp11::integers& precision_int,
                                                     const cpp11::strings& zone,
                                                     const cpp11::strings& nonexistent_string,
                                                     const cpp11::strings& ambiguous_string,
                                                     cpp11::list_of<cpp11::doubles> reference_fields,
                                                     const cpp11::sexp& call) {
  using namespace rclock;

  zone_size_validate(zone);
  const std::string zone_name = cpp11::r_string(zone[0]);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  const duration::seconds reference{reference_fields};

  switch (parse_precision(precision_int)) {
  case precision::second: return as_zoned_sys_time_from_naive_time_with_reference_impl<duration::seconds>(fields, p_time_zone, nonexistent_string, ambiguous_string, reference, call);
  case precision::millisecond: return as_zoned_sys_time_from_naive_time_with_reference_impl<duration::milliseconds>(fields, p_time_zone, nonexistent_string, ambiguous_string, reference, call);
  case precision::microsecond: return as_zoned_sys_time_from_naive_time_with_reference_impl<duration::microseconds>(fields, p_time_zone, nonexistent_string, ambiguous_string, reference, call);
  case precision::nanosecond: return as_zoned_sys_time_from_naive_time_with_reference_impl<duration::nanoseconds>(fields, p_time_zone, nonexistent_string, ambiguous_string, reference, call);
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

    if (r_dbl_is_missing(elt_seconds)) {
      out.assign_na(i);
      continue;
    }
    if (clock_dbl_is_oob_for_int64(elt_seconds)) {
      out.assign_na(i);
      continue;
    }

    const int64_t elt = clock_dbl_as_int64(elt_seconds);
    const std::chrono::seconds elt_sec{elt};

    out.assign(elt_sec, i);
  }

  return out.to_list();
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::doubles
to_sys_seconds_from_sys_duration_fields_cpp(cpp11::list_of<cpp11::doubles> fields) {
  const rclock::duration::seconds x{fields};

  const r_ssize size = x.size();
  cpp11::writable::doubles out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = r_dbl_na;
      continue;
    }
    const std::chrono::seconds elt = x[i];
    out[i] = static_cast<double>(elt.count());
  }

  return out;
}

// -----------------------------------------------------------------------------

static
inline
void
finalize_parse_zone(const std::string& candidate,
                    std::string& zone,
                    const date::time_zone*& p_time_zone) {
  if (!tzdb::locate_zone(candidate, p_time_zone)) {
    std::string message{
      "`%%Z` must be used, and must result in a valid time zone name, "
      "not '" + candidate + "'."
    };

    clock_abort(message.c_str());
  }

  zone = candidate;
}

static
inline
void
stop_heterogeneous_zones(const std::string& old_zone, const std::string& new_zone) {
  std::string message{
    "All elements of `x` must have the same time zone name. "
    "Found different zone names of: '" + old_zone + "' and '" + new_zone + "'."
  };
  clock_abort(message.c_str());
}

template <class ClockDuration>
static
inline
void
zoned_time_parse_complete_one(std::istringstream& stream,
                              const std::vector<std::string>& fmts,
                              const std::pair<const std::string*, const std::string*>& month_names_pair,
                              const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                              const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                              const char& dmark,
                              const r_ssize& i,
                              rclock::failures& fail,
                              std::string& zone,
                              const date::time_zone*& p_time_zone,
                              ClockDuration& fields) {
  using Duration = typename ClockDuration::chrono_duration;
  static const std::chrono::minutes not_an_offset = std::chrono::minutes::min();

  const r_ssize size = fmts.size();

  for (r_ssize j = 0; j < size; ++j) {
    stream.clear();
    stream.seekg(0);

    const char* fmt = fmts[j].c_str();

    date::local_time<Duration> lt;
    std::string new_zone;
    std::chrono::minutes offset{not_an_offset};

    rclock::from_stream(
      stream,
      fmt,
      month_names_pair,
      weekday_names_pair,
      ampm_names_pair,
      dmark,
      lt,
      &new_zone,
      &offset
    );

    if (stream.fail()) {
      continue;
    }

    if (p_time_zone == NULL) {
      finalize_parse_zone(new_zone, zone, p_time_zone);
    } else if (new_zone != zone) {
      stop_heterogeneous_zones(zone, new_zone);
    }

    if (offset == not_an_offset) {
      clock_abort("`%%z` must be used, and must result in a valid offset from UTC.");
    }

    const date::local_info info = rclock::get_info(lt, p_time_zone);

    switch (info.result) {
    case date::local_info::nonexistent: {
      continue;
    }
    case date::local_info::unique: {
      if (offset == info.first.offset) {
        break;
      } else {
        continue;
      }
    }
    case date::local_info::ambiguous: {
      if (offset == info.first.offset || offset == info.second.offset) {
        break;
      } else {
        continue;
      }
    }
    default: {
      never_reached("zoned_time_parse_complete_one");
    }
    }

    fields.assign(lt.time_since_epoch() - offset, i);
    return;
  }

  fail.write(i);
  fields.assign_na(i);
}

template <class ClockDuration>
cpp11::writable::list
zoned_time_parse_complete_impl(const cpp11::strings& x,
                               const cpp11::strings& format,
                               const cpp11::strings& month,
                               const cpp11::strings& month_abbrev,
                               const cpp11::strings& weekday,
                               const cpp11::strings& weekday_abbrev,
                               const cpp11::strings& am_pm,
                               const cpp11::strings& mark) {
  const r_ssize size = x.size();
  ClockDuration fields(size);

  std::vector<std::string> fmts(format.size());
  rclock::fill_formats(format, fmts);

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
    weekday,
    weekday_abbrev,
    weekday_names
  );

  std::string ampm_names[2];
  const std::pair<const std::string*, const std::string*>& ampm_names_pair = fill_ampm_names(
    am_pm,
    ampm_names
  );

  rclock::failures fail{};

  std::string zone;
  const date::time_zone* p_time_zone = NULL;

  std::istringstream stream;

  void* vmax = vmaxget();

  for (r_ssize i = 0; i < size; ++i) {
    const SEXP elt = x[i];

    if (elt == r_chr_na) {
      fields.assign_na(i);
      continue;
    }

    const char* p_elt = Rf_translateCharUTF8(elt);

    stream.str(p_elt);

    zoned_time_parse_complete_one(
      stream,
      fmts,
      month_names_pair,
      weekday_names_pair,
      ampm_names_pair,
      dmark,
      i,
      fail,
      zone,
      p_time_zone,
      fields
    );
  }

  vmaxset(vmax);

  if (fail.any_failures()) {
    fail.warn_parse();
  }

  if (zone.empty()) {
    // In the case of all failures, all NAs, or empty input, there will
    // be no way to determine a time zone.
    // In those cases, we default to UTC.
    zone = "UTC";
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
zoned_time_parse_complete_cpp(const cpp11::strings& x,
                              const cpp11::strings& format,
                              const cpp11::integers& precision_int,
                              const cpp11::strings& month,
                              const cpp11::strings& month_abbrev,
                              const cpp11::strings& weekday,
                              const cpp11::strings& weekday_abbrev,
                              const cpp11::strings& am_pm,
                              const cpp11::strings& mark) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::second: return zoned_time_parse_complete_impl<duration::seconds>(x, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  case precision::millisecond: return zoned_time_parse_complete_impl<duration::milliseconds>(x, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  case precision::microsecond: return zoned_time_parse_complete_impl<duration::microseconds>(x, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  case precision::nanosecond: return zoned_time_parse_complete_impl<duration::nanoseconds>(x, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  default: never_reached("zoned_time_parse_complete_cpp");
  }
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
void
zoned_time_parse_abbrev_one(std::istringstream& stream,
                            const std::vector<std::string>& fmts,
                            const std::pair<const std::string*, const std::string*>& month_names_pair,
                            const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                            const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                            const char& dmark,
                            const r_ssize& i,
                            rclock::failures& fail,
                            const date::time_zone*& p_time_zone,
                            ClockDuration& fields) {
  using Duration = typename ClockDuration::chrono_duration;

  const r_ssize size = fmts.size();

  for (r_ssize j = 0; j < size; ++j) {
    stream.clear();
    stream.seekg(0);

    const char* fmt = fmts[j].c_str();

    date::local_time<Duration> lt;
    std::string parsed_abbrev;

    // Parsed, but ignored
    std::chrono::minutes parsed_offset{};

    rclock::from_stream(
      stream,
      fmt,
      month_names_pair,
      weekday_names_pair,
      ampm_names_pair,
      dmark,
      lt,
      &parsed_abbrev,
      &parsed_offset
    );

    if (stream.fail()) {
      continue;
    }

    if (parsed_abbrev.empty()) {
      clock_abort("`%%Z` must be used and must result in a time zone abbreviation.");
    }

    const date::local_info info = rclock::get_info(lt, p_time_zone);
    std::chrono::seconds offset{};

    switch (info.result) {
    case date::local_info::nonexistent: {
      continue;
    }
    case date::local_info::unique: {
      if (parsed_abbrev == info.first.abbrev) {
        offset = info.first.offset;
        break;
      } else {
        continue;
      }
    }
    case date::local_info::ambiguous: {
      if (parsed_abbrev == info.first.abbrev) {
        offset = info.first.offset;
        break;
      } else if (parsed_abbrev == info.second.abbrev) {
        offset = info.second.offset;
        break;
      } else {
        continue;
      }
    }
    default: {
      never_reached("zoned_time_parse_abbrev_one");
    }
    }

    fields.assign(lt.time_since_epoch() - offset, i);
    return;
  }

  fail.write(i);
  fields.assign_na(i);
}

template <class ClockDuration>
cpp11::writable::list
zoned_time_parse_abbrev_impl(const cpp11::strings& x,
                             const date::time_zone*& p_time_zone,
                             const cpp11::strings& format,
                             const cpp11::strings& month,
                             const cpp11::strings& month_abbrev,
                             const cpp11::strings& weekday,
                             const cpp11::strings& weekday_abbrev,
                             const cpp11::strings& am_pm,
                             const cpp11::strings& mark) {
  const r_ssize size = x.size();
  ClockDuration fields(size);

  std::vector<std::string> fmts(format.size());
  rclock::fill_formats(format, fmts);

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
    weekday,
    weekday_abbrev,
    weekday_names
  );

  std::string ampm_names[2];
  const std::pair<const std::string*, const std::string*>& ampm_names_pair = fill_ampm_names(
    am_pm,
    ampm_names
  );

  rclock::failures fail{};

  std::istringstream stream;

  void* vmax = vmaxget();

  for (r_ssize i = 0; i < size; ++i) {
    const SEXP elt = x[i];

    if (elt == r_chr_na) {
      fields.assign_na(i);
      continue;
    }

    const char* p_elt = Rf_translateCharUTF8(elt);

    stream.str(p_elt);

    zoned_time_parse_abbrev_one(
      stream,
      fmts,
      month_names_pair,
      weekday_names_pair,
      ampm_names_pair,
      dmark,
      i,
      fail,
      p_time_zone,
      fields
    );
  }

  vmaxset(vmax);

  if (fail.any_failures()) {
    fail.warn_parse();
  }

  return fields.to_list();
}

[[cpp11::register]]
cpp11::writable::list
zoned_time_parse_abbrev_cpp(const cpp11::strings& x,
                            const cpp11::strings& zone,
                            const cpp11::strings& format,
                            const cpp11::integers& precision_int,
                            const cpp11::strings& month,
                            const cpp11::strings& month_abbrev,
                            const cpp11::strings& weekday,
                            const cpp11::strings& weekday_abbrev,
                            const cpp11::strings& am_pm,
                            const cpp11::strings& mark) {
  using namespace rclock;

  zone_size_validate(zone);
  const std::string zone_name = cpp11::r_string(zone[0]);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  switch (parse_precision(precision_int)) {
  case precision::second: return zoned_time_parse_abbrev_impl<duration::seconds>(x, p_time_zone, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  case precision::millisecond: return zoned_time_parse_abbrev_impl<duration::milliseconds>(x, p_time_zone, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  case precision::microsecond: return zoned_time_parse_abbrev_impl<duration::microseconds>(x, p_time_zone, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  case precision::nanosecond: return zoned_time_parse_abbrev_impl<duration::nanoseconds>(x, p_time_zone, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  default: never_reached("zoned_time_parse_abbrev_cpp");
  }
}

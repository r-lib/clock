#include "r.h"
#include "utils.h"
#include "zone.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include <date/date.h>
#include <date/tz.h>

// -----------------------------------------------------------------------------

static date::local_seconds adjust_ymd_based(const date::local_seconds& lsec,
                                            int value,
                                            r_ssize i,
                                            const enum day_nonexistant& day_nonexistant,
                                            const enum adjuster& adjuster,
                                            bool& na);

// -----------------------------------------------------------------------------

static sexp adjust_posixct(sexp x,
                           sexp value,
                           enum day_nonexistant day_nonexistant,
                           enum dst_nonexistant dst_nonexistant,
                           enum dst_ambiguous dst_ambiguous,
                           r_ssize size,
                           enum adjuster adjuster);

[[cpp11::register]]
SEXP adjust_posixct_cpp(SEXP x,
                        SEXP value,
                        SEXP day_resolver,
                        SEXP dst_resolver,
                        SEXP size,
                        SEXP adjuster) {
  sexp day_nonexistant = r_list_get(day_resolver, 0);
  sexp dst_nonexistant = r_list_get(dst_resolver, 0);
  sexp dst_ambiguous = r_list_get(dst_resolver, 1);

  enum day_nonexistant c_day_nonexistant = parse_day_nonexistant(day_nonexistant);
  enum dst_nonexistant c_dst_nonexistant = parse_dst_nonexistant_arithmetic(dst_nonexistant);
  enum dst_ambiguous c_dst_ambiguous = parse_dst_ambiguous_arithmetic(dst_ambiguous);

  r_ssize c_size = r_int_get(size, 0);

  enum adjuster c_adjuster = parse_adjuster(adjuster);

  return adjust_posixct(
    x,
    value,
    c_day_nonexistant,
    c_dst_nonexistant,
    c_dst_ambiguous,
    c_size,
    c_adjuster
  );
}

static sexp adjust_posixct(sexp x,
                           sexp value,
                           enum day_nonexistant day_nonexistant,
                           enum dst_nonexistant dst_nonexistant,
                           enum dst_ambiguous dst_ambiguous,
                           r_ssize size,
                           enum adjuster adjuster) {
  sexp out = PROTECT(r_new_double(size));
  double* p_out = r_dbl_deref(out);

  sexp tzone = civil_get_tzone(x);
  tzone = PROTECT(zone_standardize(tzone));
  std::string zone_name = zone_unwrap(tzone);
  const date::time_zone* p_zone = zone_name_load(zone_name);

  r_poke_names(out, r_get_names(x));
  r_poke_class(out, civil_classes_posixct);
  civil_poke_tzone(out, tzone);

  const double* p_x = r_dbl_deref_const(x);
  const int* p_value = r_int_deref_const(value);

  const bool recycle_x = r_is_scalar(x);
  const bool recycle_value = r_is_scalar(value);

  const enum dst_direction dst_direction = dst_direction::positive;

  for (r_ssize i = 0; i < size; ++i) {
    double elt_x = recycle_x ? p_x[0] : p_x[i];
    int elt_value = recycle_value ? p_value[0] : p_value[i];

    int64_t elt = as_int64(elt_x);

    if (elt == r_int64_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    if (elt_value == NA_INTEGER) {
      p_out[i] = r_dbl_na;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::sys_seconds elt_ssec{elt_sec};
    date::zoned_seconds elt_zsec = date::make_zoned(p_zone, elt_ssec);
    date::local_seconds elt_lsec = elt_zsec.get_local_time();

    bool na = false;

    date::local_seconds out_lsec = adjust_ymd_based(
      elt_lsec,
      elt_value,
      i,
      day_nonexistant,
      adjuster,
      na
    );

    if (na) {
      p_out[i] = NA_REAL;
      continue;
    }

    p_out[i] = convert_local_seconds_to_posixt(
      out_lsec,
      p_zone,
      i,
      dst_direction,
      dst_nonexistant,
      dst_ambiguous
    );
  }

  UNPROTECT(2);
  return out;
}

// -----------------------------------------------------------------------------

static sexp adjust_local(sexp x,
                         sexp value,
                         enum day_nonexistant day_nonexistant,
                         r_ssize size,
                         enum adjuster adjuster);

[[cpp11::register]]
SEXP adjust_local_cpp(SEXP x,
                      SEXP value,
                      SEXP day_resolver,
                      SEXP size,
                      SEXP adjuster) {
  sexp day_nonexistant = r_list_get(day_resolver, 0);
  enum day_nonexistant c_day_nonexistant = parse_day_nonexistant(day_nonexistant);

  r_ssize c_size = r_int_get(size, 0);

  enum adjuster c_adjuster = parse_adjuster(adjuster);

  return adjust_local(
    x,
    value,
    c_day_nonexistant,
    c_size,
    c_adjuster
  );
}

static sexp adjust_local(sexp x,
                           sexp value,
                           enum day_nonexistant day_nonexistant,
                           r_ssize size,
                           enum adjuster adjuster) {
  sexp out = PROTECT(r_new_double(size));
  double* p_out = r_dbl_deref(out);

  sexp tzone = civil_get_tzone(x);
  tzone = PROTECT(zone_standardize(tzone));

  r_poke_names(out, r_get_names(x));

  const double* p_x = r_dbl_deref_const(x);
  const int* p_value = r_int_deref_const(value);

  const bool recycle_x = r_is_scalar(x);
  const bool recycle_value = r_is_scalar(value);

  for (r_ssize i = 0; i < size; ++i) {
    double elt_x = recycle_x ? p_x[0] : p_x[i];
    int elt_value = recycle_value ? p_value[0] : p_value[i];

    int64_t elt = as_int64(elt_x);

    if (elt == r_int64_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    if (elt_value == NA_INTEGER) {
      p_out[i] = r_dbl_na;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::local_seconds elt_lsec{elt_sec};

    bool na = false;

    date::local_seconds out_lsec = adjust_ymd_based(
      elt_lsec,
      elt_value,
      i,
      day_nonexistant,
      adjuster,
      na
    );

    if (na) {
      p_out[i] = NA_REAL;
    } else {
      p_out[i] = out_lsec.time_since_epoch().count();
    }
  }

  UNPROTECT(2);
  return out;
}

// -----------------------------------------------------------------------------

static inline date::year_month_day adjust_day_of_month(const date::year_month_day& ymd, int value);
static inline date::year_month_day adjust_last_day_of_month(const date::year_month_day& ymd);

static date::local_seconds adjust_ymd_based(const date::local_seconds& lsec,
                                            int value,
                                            r_ssize i,
                                            const enum day_nonexistant& day_nonexistant,
                                            const enum adjuster& adjuster,
                                            bool& na) {
  date::local_days lday = date::floor<date::days>(lsec);
  date::local_seconds lsec_floor = lday;

  std::chrono::seconds time_of_day = lsec - lsec_floor;

  date::year_month_day ymd{lday};

  date::year_month_day out_ymd;

  switch (adjuster) {
  case adjuster::day_of_month: {
    out_ymd = adjust_day_of_month(ymd, value);
    break;
  }
  case adjuster::last_day_of_month: {
    out_ymd = adjust_last_day_of_month(ymd);
    break;
  }
  }

  if (!out_ymd.ok()) {
    bool na_resolve = false;

    resolve_day_nonexistant(i, day_nonexistant, out_ymd, time_of_day, na_resolve);

    if (na_resolve) {
      na = true;
      return lsec;
    }
  }

  date::local_days out_lday{out_ymd};
  date::local_seconds out_lsec_floor{out_lday};
  date::local_seconds out_lsec{out_lsec_floor + time_of_day};

  return out_lsec;
}


// -----------------------------------------------------------------------------

static inline date::year_month_day adjust_day_of_month(const date::year_month_day& ymd, int value) {
  if (value > 31 || value < 1) {
    r_abort("`value` must be within the range of [1, 31], not %i.", value);
  }

  unsigned int day = static_cast<unsigned int>(value);

  return ymd.year() / ymd.month() / date::day{day};
}

static inline date::year_month_day adjust_last_day_of_month(const date::year_month_day& ymd) {
  return ymd.year() / ymd.month() / date::last;
}

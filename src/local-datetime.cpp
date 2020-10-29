#include "r.h"
#include "utils.h"
#include "zone.h"
#include "enums.h"
#include "local.h"
#include "conversion.h"
#include <date/date.h>
#include <date/tz.h>

[[cpp11::register]]
SEXP localize_posixct_cpp(SEXP x) {
  r_ssize size = r_length(x);

  sexp out = PROTECT(r_new_double(size));
  double* p_out = r_dbl_deref(out);

  sexp tzone = civil_get_tzone(x);
  tzone = PROTECT(zone_standardize(tzone));
  std::string zone_name = zone_unwrap(tzone);
  const date::time_zone* p_zone = zone_name_load(zone_name);

  r_poke_names(out, r_get_names(x));

  const double* p_x = r_dbl_deref_const(x);

  for (r_ssize i = 0; i < size; ++i) {
    double x_elt = p_x[i];

    int64_t elt = as_int64(x_elt);

    if (elt == r_int64_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::sys_seconds elt_ssec{elt_sec};
    date::zoned_seconds elt_zsec = date::make_zoned(p_zone, elt_ssec);
    date::local_seconds elt_lsec = elt_zsec.get_local_time();

    p_out[i] = elt_lsec.time_since_epoch().count();
  }

  UNPROTECT(2);
  return out;
}

[[cpp11::register]]
SEXP unlocalize_cpp(SEXP x, SEXP zone, SEXP dst_resolver) {
  r_ssize size = r_length(x);

  sexp out = PROTECT(r_new_double(size));
  double* p_out = r_dbl_deref(out);

  zone = PROTECT(zone_standardize(zone));
  std::string zone_name = zone_unwrap(zone);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  // TODO: Fix these parses to new API
  enum dst_direction dst_direction = dst_direction::positive;
  enum dst_nonexistant dst_nonexistant = parse_dst_nonexistant_no_directional(r_list_get(dst_resolver, 0));
  enum dst_ambiguous dst_ambiguous = parse_dst_ambiguous_no_directional(r_list_get(dst_resolver, 1));

  r_poke_names(out, r_get_names(x));
  r_poke_class(out, civil_classes_posixct);
  civil_poke_tzone(out, zone);

  const double* p_x = r_dbl_deref_const(x);

  for (r_ssize i = 0; i < size; ++i) {
    double x_elt = p_x[i];

    int64_t elt = as_int64(x_elt);

    if (elt == r_int64_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::local_seconds elt_lsec{elt_sec};

    p_out[i] = civil_local_seconds_to_posixt(
      elt_lsec,
      p_time_zone,
      i,
      dst_direction,
      dst_nonexistant,
      dst_ambiguous
    );
  }

  UNPROTECT(2);
  return out;
}

#include "r.h"
#include "utils.h"
#include "zone.h"
#include "enums.h"
#include "conversion.h"
#include <date/tz.h>

static sexp civil_force_zone_impl(sexp x,
                                  sexp tzone,
                                  enum dst_nonexistant dst_nonexistant,
                                  enum dst_ambiguous dst_ambiguous);

[[cpp11::register]]
SEXP civil_force_zone_cpp(SEXP x,
                          SEXP tzone,
                          SEXP dst_nonexistant,
                          SEXP dst_ambiguous) {
  enum dst_nonexistant c_dst_nonexistant = parse_dst_nonexistant_no_directional(dst_nonexistant);
  enum dst_ambiguous c_dst_ambiguous = parse_dst_ambiguous_no_directional(dst_ambiguous);

  return civil_force_zone_impl(
    x,
    tzone,
    c_dst_nonexistant,
    c_dst_ambiguous
  );
}

static sexp civil_force_zone_impl(sexp x,
                                  sexp tzone,
                                  enum dst_nonexistant dst_nonexistant,
                                  enum dst_ambiguous dst_ambiguous) {
  sexp x_tzone = civil_get_tzone(x);

  x_tzone = PROTECT(zone_standardize(x_tzone));
  tzone = PROTECT(zone_standardize(tzone));

  std::string old_zone_name = zone_unwrap(x_tzone);
  std::string new_zone_name = zone_unwrap(tzone);

  if (old_zone_name == new_zone_name) {
    UNPROTECT(2);
    return x;
  }

  const date::time_zone* p_old_zone = zone_name_load(old_zone_name);
  const date::time_zone* p_new_zone = zone_name_load(new_zone_name);

  r_ssize size = r_length(x);
  const double* p_x = r_dbl_deref_const(x);

  sexp out = PROTECT(r_new_double(size));
  double* p_out = r_dbl_deref(out);

  r_poke_names(out, r_get_names(x));
  r_poke_class(out, civil_classes_posixct);
  civil_poke_tzone(out, tzone);

  // Not used, but required as an argument
  const dst_direction dst_direction = dst_direction::positive;

  for (r_ssize i = 0; i < size; ++i) {
    const double x_elt = p_x[i];
    const int64_t elt = as_int64(x_elt);

    if (elt == r_int64_na) {
      p_out[i] = r_dbl_na;
      continue;
    }

    std::chrono::seconds elt_sec{elt};
    date::sys_seconds elt_ssec{elt_sec};
    date::zoned_seconds elt_zsec = date::make_zoned(p_old_zone, elt_ssec);
    date::local_seconds elt_lsec = elt_zsec.get_local_time();

    p_out[i] = civil_local_seconds_to_posixt(
      elt_lsec,
      p_new_zone,
      i,
      dst_direction,
      dst_nonexistant,
      dst_ambiguous
    );
  }

  UNPROTECT(3);
  return out;
}

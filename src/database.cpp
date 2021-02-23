#include "clock.h"
#include <vector>
#include <algorithm>

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::strings
zone_database_version_cpp() {
  const date::tzdb& db = date::get_tzdb();
  cpp11::writable::strings out{db.version};
  return out;
}

// -----------------------------------------------------------------------------

#if USE_OS_TZDB

/*
 * All zone names are present in `db.zones`, even links from historic zone
 * names to their present counterparts
 */
static
cpp11::writable::strings
zone_database_names_impl() {
  const date::tzdb& db = date::get_tzdb();
  const r_ssize n_zones = static_cast<r_ssize>(db.zones.size());

  cpp11::writable::strings zones(n_zones);

  for (r_ssize i = 0; i < n_zones; ++i) {
    const std::string& name = db.zones[i].name();
    SEXP elt = Rf_mkCharLenCE(name.c_str(), name.size(), CE_UTF8);
    SET_STRING_ELT(zones, i, elt);
  }

  return zones;
}

#else // !USE_OS_TZDB

/*
 * `db.zones` contains the currently used zone names
 * `db.links` contains the rest
 * Results need to be sorted in C locale to match when `USE_OS_TZDB` is used
 */
static
cpp11::writable::strings
zone_database_names_impl() {
  const date::tzdb& db = date::get_tzdb();

  const r_ssize n_current_zones = static_cast<r_ssize>(db.zones.size());
  const r_ssize n_historic_zones = static_cast<r_ssize>(db.links.size());
  const r_ssize n_zones = n_current_zones + n_historic_zones;

  r_ssize loc = 0;
  std::vector<std::string> zones(n_zones);

  for (r_ssize i = 0; i < n_current_zones; ++i, ++loc) {
    zones[loc] = db.zones[i].name();
  }
  for (r_ssize i = 0; i < n_historic_zones; ++i, ++loc) {
    zones[loc] = db.links[i].name();
  }

  // Should use C locale to sort with, giving same ordering as binary tzdb
  std::sort(zones.begin(), zones.end());

  cpp11::writable::strings out(n_zones);

  for (r_ssize i = 0; i < n_zones; ++i) {
    const std::string name = zones[i];
    SEXP elt = Rf_mkCharLenCE(name.c_str(), name.size(), CE_UTF8);
    SET_STRING_ELT(out, i, elt);
  }

  return out;
}

#endif // USE_OS_TZDB

[[cpp11::register]]
cpp11::writable::strings
zone_database_names_cpp() {
  return zone_database_names_impl();
}

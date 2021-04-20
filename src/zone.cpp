#include "zone.h"
#include "utils.h"
#include <string>

/*
 * Brought over from lubridate/timechange
 * https://github.com/vspinu/timechange/blob/9c1f769a6c85899665af8b86e43115d392c09989/src/tzone.cpp#L4
 */

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::logicals zone_is_valid(const cpp11::strings& zone) {
  zone_size_validate(zone);

  cpp11::r_string zone_name_r(zone[0]);
  std::string zone_name(zone_name_r);

  // Local time
  if (zone_name.size() == 0) {
    return cpp11::writable::logicals({cpp11::r_bool(true)});
  }

  const date::time_zone* p_time_zone;

  if (tzdb::locate_zone(zone_name, p_time_zone)) {
    return cpp11::writable::logicals({cpp11::r_bool(true)});
  } else {
    return cpp11::writable::logicals({cpp11::r_bool(false)});
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::strings zone_current() {
  return cpp11::writable::strings({zone_name_current()});
}

// -----------------------------------------------------------------------------


const date::time_zone* zone_name_load_try(const std::string& zone_name);

// [[ include("zone.h") ]]
const date::time_zone* zone_name_load(const std::string& zone_name) {
  if (zone_name.size() == 0) {
    // We look up the local time zone using R's `Sys.timezone()`
    // or the `TZ` envvar when an empty string is the input.
    // This is consistent with lubridate.
    std::string current_zone_name = zone_name_current();
    return zone_name_load_try(current_zone_name);
  } else {
    return zone_name_load_try(zone_name);
  }
}

const date::time_zone* zone_name_load_try(const std::string& zone_name) {
  const date::time_zone* p_time_zone;

  if (!tzdb::locate_zone(zone_name, p_time_zone)) {
    clock_abort("'%s' not found in the timezone database.", zone_name.c_str());
  }

  return p_time_zone;
}

static std::string zone_name_system();

// [[ include("zone.h") ]]
std::string zone_name_current() {
  const char* tz_env = std::getenv("TZ");

  if (tz_env == NULL) {
    // Unset, use system tz
    return zone_name_system();
  }

  if (strlen(tz_env) == 0) {
    // If set but empty, R behaves in a system specific way and there is no way
    // to infer local time zone.
    cpp11::warning("Environment variable `TZ` is set to \"\". Using system time zone.");
    return zone_name_system();
  }

  std::string out(tz_env);

  return out;
}

static std::string zone_name_system_get();

static std::string zone_name_system() {
  // Once per session
  static std::string ZONE_NAME_SYSTEM = zone_name_system_get();
  return ZONE_NAME_SYSTEM;
}

static std::string zone_name_system_get() {
  auto sys_timezone = cpp11::package("base")["Sys.timezone"];
  cpp11::sexp result = sys_timezone();

  if (!clock_is_string(result)) {
    cpp11::warning("Unexpected result from `Sys.timezone()`, returning 'UTC'.");
    return "UTC";
  }

  cpp11::strings timezone = cpp11::as_cpp<cpp11::strings>(result);
  cpp11::r_string tz = timezone[0];
  SEXP tz_char(tz);

  if (tz_char == r_chr_na || strlen(CHAR(tz_char)) == 0) {
    cpp11::warning(
      "System timezone name is unknown. "
      "Please set the environment variable `TZ`. "
      "Defaulting to 'UTC'."
    );

    return "UTC";
  }

  std::string out(tz);

  return out;
}

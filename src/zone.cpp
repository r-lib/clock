#include "zone.h"
#include "utils.h"

/*
 * Brought over from lubridate/timechange
 * https://github.com/vspinu/timechange/blob/9c1f769a6c85899665af8b86e43115d392c09989/src/tzone.cpp#L4
 */

// -----------------------------------------------------------------------------

/*
 * Extract a string time zone from an R `tzone` attribute
 *
 * This is slightly different from the lubridate version. For POSIXlt objects,
 * the time zone attribute might be a character vector of length 3.
 * If the first element is `""` (which happens on a Mac with
 * `as.POSIXlt(Sys.time())`), then lubridate will look to the second element
 * and will use that as the time zone. I think this is incorrect, because those
 * are always time zone abbreviations, and will fail to load because they
 * aren't true time zone names. I think that is the reason Vitalie opened
 * this issue, and the reason for the time zone map in lubridate. This function
 * works more like `lubridate:::tz.POSIXt()` which just takes the first element
 * of the tzone attribute.
 * https://github.com/google/cctz/issues/46
 * https://github.com/tidyverse/lubridate/blob/b9025e6d5152f9da3857d7ef18f2571d3d861bae/src/update.cpp#L49
 */
// [[ include("zone.h") ]]
std::string civil_zone_name_from_tzone(sexp tzone) {
  if (r_is_null(tzone)) {
    return "";
  }

  if (r_typeof(tzone) != r_type_character) {
    r_abort("`tzone` must be a character vector or `NULL`.");
  }

  r_ssize size = r_length(tzone);

  // Assume `character()` tzone is also local time
  if (size == 0) {
    return "";
  }

  const char* char_out = CHAR(STRING_ELT(tzone, 0));
  std::string out = char_out;

  return out;
}

// [[ include("zone.h") ]]
std::string civil_zone_name_from_posixt(sexp x) {
  sexp tzone = civil_get_tzone(x);
  return civil_zone_name_from_tzone(tzone);
}

// -----------------------------------------------------------------------------

const char* zone_name_local();
const date::time_zone* zone_name_load_try(const std::string& zone_name);

// [[ include("zone.h") ]]
const date::time_zone* civil_zone_name_load(const std::string& zone_name) {
  if (zone_name.size() == 0) {
    // We look up the local time zone using R's `Sys.timezone()`
    // or the `TZ` envvar when an empty string is the input.
    // This is consistent with lubridate.
    std::string local_zone_name = zone_name_local();
    return zone_name_load_try(local_zone_name);
  } else {
    return zone_name_load_try(zone_name);
  }
}

const date::time_zone* zone_name_load_try(const std::string& zone_name) {
  try {
    return date::locate_zone(zone_name);
  } catch (const std::runtime_error& error) {
    r_abort("'%s' not found in the timezone database.", zone_name.c_str());
  };
}

const char* zone_name_system();

const char* zone_name_local() {
  const char* tz_env = std::getenv("TZ");

  if (tz_env == NULL) {
    // Unset, use system tz
    return zone_name_system();
  }

  if (strlen(tz_env) == 0) {
    // If set but empty, R behaves in a system specific way and there is no way
    // to infer local time zone.
    Rf_warningcall(r_null, "Environment variable `TZ` is set to \"\". Using system time zone.");
    return zone_name_system();
  }

  return tz_env;
}

static const char* zone_name_system_get();

const char* zone_name_system() {
  // Once per session
  static const char* ZONE_NAME_SYSTEM = NULL;

  if (ZONE_NAME_SYSTEM == NULL) {
    ZONE_NAME_SYSTEM = strdup(zone_name_system_get());
  }

  return ZONE_NAME_SYSTEM;
}

static const char* zone_name_system_get() {
  SEXP fn = Rf_findFun(Rf_install("Sys.timezone"), R_BaseEnv);
  SEXP call = PROTECT(Rf_lang1(fn));
  SEXP timezone = PROTECT(Rf_eval(call, R_GlobalEnv));

  if (!r_is_string(timezone)) {
    Rf_warningcall(r_null, "Unexpected result from `Sys.timezone()`, returning 'UTC'.");
    UNPROTECT(2);
    return "UTC";
  }

  SEXP tz = STRING_ELT(timezone, 0);

  if (tz == NA_STRING || strlen(CHAR(tz)) == 0) {
    Rf_warningcall(
      r_null,
      "System timezone name is unknown. "
      "Please set the environment variable `TZ`. "
      "Defaulting to 'UTC'."
    );

    UNPROTECT(2);
    return "UTC";
  }

  UNPROTECT(2);
  return CHAR(tz);
}

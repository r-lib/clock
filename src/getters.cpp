#include "civil.h"
#include "zone.h"

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::integers get_offset_cpp(const civil_field& calendar,
                                         const civil_field& seconds_of_day,
                                         const cpp11::strings& zone) {
  r_ssize size = calendar.size();

  cpp11::writable::strings zone_standard = zone_standardize(zone);
  std::string zone_name = cpp11::r_string(zone_standard[0]);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  cpp11::writable::integers out(size);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_calendar = calendar[i];
    int elt_seconds_of_day = seconds_of_day[i];

    if (elt_calendar == r_int_na) {
      out[i] = r_int_na;
      continue;
    }

    date::sys_days elt_sday{date::days{elt_calendar}};
    std::chrono::seconds elt_tod{elt_seconds_of_day};

    date::sys_seconds elt_ssec_floor{elt_sday};
    date::sys_seconds elt_ssec = elt_ssec_floor + elt_tod;

    date::sys_info info = p_time_zone->get_info(elt_ssec);

    out[i] = info.offset.count();
  }

  return out;
}
